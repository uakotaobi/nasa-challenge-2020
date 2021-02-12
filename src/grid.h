#ifndef GRID_H_INCLUDED
#define GRID_H_INCLUDED

#include <vector>
#include <functional>
#include <tuple>

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


        Basis system() const;

        // Gets the number of divsisions of the grid in the system.axisZ direction.
        double rows() const;

        // Gets the number of divisions of the grid in the system.axisX direction.
        double columns() const;

        // Gets the distance between adjacent gridpoints.
        double cellSize() const;


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

        // Takes a function f(x,y) that returns a height z and applies that height to each of the respective points in the grid.
        // Also applies colors to each respective point in the grid.
        void setHeightByFunction(std::function<double(double, double)> zCoordinateFunc,
                                 std::function<SDL_Color(double, double)> colorFunc = [] (double x, double y) {
                                     return SDL_Color{255, 255, 255, 255};
                                 });

        // Gets the parameters of surface interpolation for the given point
        //
        // Returns a triplet of floating point numbers:
        // - u, which is the parameter of interpolation for the row.
        // - v, which is the parameter of interpolation for the columns.
        // - h, which is the minimum distance of the point from the gridPlane().
        //
        // If the point is at the upper left, it is at (u = 0, v = 0) (elevation doesn't matter)
        // Lower right = (1, 1)
        // Upper right = (0, 1)
        // Lower left = (1, 0)
        //
        // Can return valid data for points outside the grid.
        std::tuple<double, double, double> gridLocation(Point p) const;

        // Uses averages of the nearest grid points to u and v, and determines
        // where the "floor" should be, even when u and v don't directly
        // correspond to a grid point boundary.
        //
        // 0 <= v <= 1
        // 0 <= u <= 1
        Point findFloor(double u, double v) const;
    private:
        std::vector<GridPoint> lattice;
        Basis system_;
        int rows_, columns_;
        double cellSize_;
        void setLatticePoints();



};






#endif // GRID_H_INCLUDED
