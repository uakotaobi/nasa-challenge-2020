#include "grid.h"
#include "plane.h"
#include <iostream>
#include "matrix.h"

GridPoint::GridPoint() : Point(0, 0, 0), color(SDL_Color{255, 255, 255, 255}), temperatureKelvin(0), slopeDeg(0), height(0) {}
GridPoint::GridPoint(Point p, SDL_Color color_, double temperatureKelvin_, double slopeDeg_, double height_)
    : Point(p), color(color_), temperatureKelvin(temperatureKelvin_), slopeDeg(slopeDeg_), height(height_) {}

Grid::Grid() : lattice((49 + 1) * (49 + 1)),
               system_(),
               rows_(49),
               columns_(49),
               cellSize_(1) {
                   setLatticePoints();
}

Grid::Grid(int rows_, int columns_, double cellSize_)
    : lattice((rows_ + 1) * (columns_ + 1)),
      system_(),
      rows_(rows_),
      columns_(columns_),
      cellSize_(cellSize_) {
    setLatticePoints();
}

void Grid::setLatticePoints() {
    Point displacedCenter = system_.center;
    displacedCenter.x = system_.center.x - (columns_ / 2.0 * cellSize_);
    displacedCenter.z = system_.center.z - (rows_ / 2.0 * cellSize_);

    for (int row = 0; row <= rows_; row += 1) {
        for (int column = 0; column <= columns_; column += 1) {
            int index = (columns_ + 1) * row + column;
            GridPoint& gridPoint = lattice[index];
            Point actualLocation = displacedCenter + (column * cellSize_ * system_.axisX) + (row * cellSize_ * system_.axisZ) + (gridPoint.height * system_.axisY);
            gridPoint.x = actualLocation.x;
            gridPoint.y = actualLocation.y;
            gridPoint.z = actualLocation.z;
        }
    }
}

void Grid::setHeightByFunction(std::function<double(double,double)> zCoordinateFunc,
                               std::function<SDL_Color(double, double)> colorFunc) {
    // zCoordinateFunc takes normalized coordinates
    setLatticePoints();
    int index = 0;
    for (double y = 0; y <= 1; y += 1.0/(rows_)) {
        for (double x = 0; x <= 1; x += 1.0/(columns_)) {
            double z = zCoordinateFunc(x, y);
            SDL_Color a = colorFunc(x, y);
            GridPoint& gridPoint = lattice.at(index);
            gridPoint.y = z;
            gridPoint.color = a;
            index++;
        }
    }
}

Basis Grid::system() const {
    return system_;
}

double Grid::rows() const {
    return rows_;
}

double Grid::columns() const {
    return columns_;
}

double Grid::cellSize() const {
    return cellSize_;
}

Plane Grid::gridPlane() const {
    return Plane(system_.center, system_.axisY);
}

Plane Grid::leftPlane() const {
    // Normal for the leftPlane is -system.axisX.
    // Grid's center + the leftPlane's normal times 1/2 of the width.
    return Plane(system_.center + -system_.axisX * cellSize_/2 * columns_, -system_.axisX);
}

Plane Grid::rightPlane() const {
    // Normal for the rightPlane is system.axisX.
    // Grid's center + the rightPlane's normal times 1/2 of the width.
    return Plane(system_.center + system_.axisX * cellSize_/2 * columns_, system_.axisX);
}

Plane Grid::forwardPlane() const {
    // Normal for the forwardPlane is system.axisZ.
    // Grid's center + the forwardPlane's normal times 1/2 of the height.
    return Plane(system_.center + system_.axisZ * cellSize_/2 * rows_, system_.axisZ);
}

Plane Grid::backPlane() const {
    // Normal for the backPlane is -system.axisZ.
    // Grid's center + the backPlane's normal times 1/2 of the height.
    return Plane(system_.center + -system_.axisZ * cellSize_/2 * rows_, -system_.axisZ);
}

void Grid::render(SDL_Surface* canvas, SDL_Rect viewPortRect, Basis camera) {
    const Matrix cameraMatrix = cameraTransform(camera.axisX, camera.axisY, camera.axisZ, camera.center);
    const double focalDistance = 120;
    // Screen rect is the rectangle in the camera space that represents what the camera currently sees.
    // Growing this rectangle zooms the camera out.
    const SDL_Rect screenRect = {-63, -63, 125, 125};

    const Matrix projectionMatrix = ::projectionMatrix(focalDistance, screenRect, viewPortRect);

    uint32_t* const pixels = static_cast<uint32_t*>(canvas->pixels);

    for (const GridPoint& currentPoint: lattice) {
        Point p = currentPoint;
        // std::cout << p << '\n';

        p = cameraMatrix * p;
        if (p.z < 0) {
            continue;
        }
        p = projectionMatrix * p;

        // Any point out of bounds of the view rectangle is skipped.
        if (p.x < viewPortRect.x ||
            p.y < viewPortRect.y ||
            p.x >= viewPortRect.x + viewPortRect.w ||
            p.y >= viewPortRect.y + viewPortRect.h) {
                continue;
        }

        // Render points that weren't skipped.
        // Offset formula:
        // width*y+x
        unsigned int offset = canvas->w * static_cast<unsigned int>(p.y) + static_cast<unsigned int>(p.x);
        pixels[offset] = SDL_MapRGBA(canvas->format, currentPoint.color.r, currentPoint.color.g, currentPoint.color.b, currentPoint.color.a);

        /* // Temporary: make the pixels much bigger to see better.
        const int pixelSize = 7;
        SDL_Rect pixelRect = {static_cast<int>(p.x) - pixelSize/2,
                              static_cast<int>(p.y) - pixelSize/2,
                              pixelSize,
                              pixelSize};
        SDL_Rect intersection;
        if (SDL_IntersectRect(&viewPortRect, &pixelRect, &intersection)) {
            SDL_FillRect(canvas,
                         &pixelRect,
                         SDL_MapRGBA(canvas->format,
                                     currentPoint.color.r,
                                     currentPoint.color.g,
                                     currentPoint.color.b,
                                     currentPoint.color.a));
           //std::cout << p << '\n';

        } */

    }

}

std::tuple<double, double, double> Grid::gridLocation(Point p) const {
    Plane P = gridPlane();
    // Guarantees that the ray will hit the grid, no matter how far you are from it;
    double rayDistance = (distance(system_.center, p) * 2);
    Point pointPastPlane = p + -P.whichSide(p) * P.normalVector() * rayDistance;
    auto op = P.pointOfIntersection(p, pointPastPlane);
    if (!op) {
        throw std::runtime_error("Internal error: line segment didn't hit plane.");
    }

    // Calculate the height of p from the gridPlane (P).
    double h = distance(p, *op);

    // Use the cameraTransform function to transform the grid's basis to i, j, k origin.
    // This will give us a matrix that will transform p into a new point, p'.
    // p' will be used for the parameters of interpolation.
    Point pPrime = cameraTransform(system_.axisX, system_.axisY, system_.axisZ, system_.center) * p;
    double v = pPrime.x / (cellSize_ * (columns_ + 1));
    double u = pPrime.z / (cellSize_ * (rows_ + 1));
    return std::make_tuple(u, v, h);

}

Point Grid::findFloor(double u, double v) const {
    if (u < 0) {
        u = 0;
    }
    if (v < 0) {
        v = 0;
    }
        
    if (u > 1) {
        u = 1;
    }
    if (v > 1) {
        v = 1;
    }
    
    double row = u * (rows_ + 1);
    double column = v * (columns_ + 1);
    
    auto index = [this] (int x, int y) {
        return (this->columns_ + 1) * y + x;
    };
    GridPoint ul = lattice.at(index(floor(column), floor(row)));
    GridPoint ur = lattice.at(index(ceil(column), floor(row))); 
    GridPoint ll = lattice.at(index(floor(column), ceil(row)));
    GridPoint lr = lattice.at(index(ceil(column), ceil(row)));
    
    Point resultPoint ((ul.x + ur.x + ll.x + lr.x)/4, 
                       (ul.y + ur.y + ll.y + lr.y)/4,
                       (ul.z + ur.z + ll.z + lr.z)/4);
    return resultPoint;                   
    
}
    