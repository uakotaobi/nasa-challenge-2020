#ifndef GRID_H_INCLUDED
#define GRID_H_INCLUDED

#include <vector>
#include <functional>

#include "SDL.h"
#include "point.h"
#include "vector.h"
#include "basis.h"
#include "plane.h"

struct GridPoint : public Point {
    // We don't need elevation variable, we have this->y
    SDL_Color color;
    double temperatureKelvin;
    double slopeDeg;
    double height;

    GridPoint();
    GridPoint(Point p, SDL_Color color_, double temperatureKelvin_, double slopeDeg_, double height_);
};

class Grid {
    public:
        Grid();

        // Construct a grid with the given dimensions.
        //
        // - rows: the depth of the grid
        // - columns: the width of the grid
        // - cellSize: the space between grid cells according to the grid's own basis
        Grid(int rows, int columns, double cellSize = 1.0 );

        // Have 3D grid points displayed in 2D
        void render(SDL_Surface* canvas, SDL_Rect viewPortRect, Basis camera);


        // Return the center plane of the grid (grid points can be located above or BELOW the plane).
        Plane gridPlane() const;

        // Returns a plane that is on the left side of the bounding box.
        // Similar to a square room, the bounding box sides are defined by which way the basis axes are oriented and the corners.
        Plane leftPlane() const;

        // Returns a plane that is on the right side of the bounding box.
        Plane rightPlane() const;

        // Returns a plane that is on the back side of the bounding box.
        Plane backPlane() const;

        // Returns a plane that is on the forward side of the bounding box.
        Plane forwardPlane() const;

        void setHeightByFunction(std::function<double(double, double)> zCoordinateFunc,
                                 std::function<SDL_Color(double, double)> colorFunc = [] (double x, double y) {
                                     return SDL_Color{255, 255, 255, 255};
                                 });
    private:
        std::vector<GridPoint> lattice;
        Basis system;
        int rows, columns;
        double cellSize;
        void setLatticePoints();



};






#endif // GRID_H_INCLUDED
