#ifndef GRID_H_INCLUDED
#define GRID_H_INCLUDED

#include <vector>
#include "SDL.h"
#include "point.h"
#include "vector.h"

struct GridPoint : public Point {
    // We don't need elevation variable, we have this->y
    SDL_Color color;
    double temperatureKelvin;
    double slopeDeg;

    GridPoint();
    GridPoint(Point p, SDL_Color color_, double temperatureKelvin_, double slopeDeg_);
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
    private:
        std::vector<GridPoint> lattice;
        Point center;
        Vector axisX, axisY, axisZ;
        int rows, columns;
        double cellSize;
        
        
};






#endif // GRID_H_INCLUDED
