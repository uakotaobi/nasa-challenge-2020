#include "grid.h"

GridPoint::GridPoint() : Point(0, 0, 0), color(SDL_Color{0, 0, 0}), temperatureKelvin(0), slopeDeg(0), height(0) {}
GridPoint::GridPoint(Point p, SDL_Color color_, double temperatureKelvin_, double slopeDeg_, double height_)
    : Point(p), color(color_), temperatureKelvin(temperatureKelvin_), slopeDeg(slopeDeg_), height(height_) {}

Grid::Grid() : lattice((49 + 1) * (49 + 1)),
               center(0, 0, 0),
               axisX(1, 0, 0),
               axisY(0, 1, 0),
               axisZ(0, 0, 1),
               rows(49),
               columns(49),
               cellSize(1) {
                   setLatticePoints();
}

Grid::Grid(int rows_, int columns_, double cellSize_)
    : lattice((rows_ + 1) * (columns_ + 1)),
      center(0, 0, 0),
      axisX(1, 0, 0),
      axisY(0, 1, 0),
      axisZ(0, 0, 1),
      rows(rows_),
      columns(columns_),
      cellSize(cellSize_) {
    setLatticePoints();
}

void Grid::setLatticePoints() {
    Point displacedCenter = center;
    displacedCenter.x = center.x + ((columns + 1) * cellSize);
    displacedCenter.z = center.z - ((rows + 1) * cellSize);
    for (int row = 0; row <= rows; row += 1) {
        for (int column = 0; column <= columns; column += 1) {
            int index = (row * columns) + column;
            GridPoint& gridPoint = lattice[index];
            Point actualLocation = displacedCenter + (column * cellSize * axisX) + (row * cellSize * axisZ) + (gridPoint.height * axisY);
            gridPoint.x = actualLocation.x;
            gridPoint.y = actualLocation.y;
            gridPoint.z = actualLocation.z;
        }
    }

}
