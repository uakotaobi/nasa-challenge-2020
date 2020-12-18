#ifndef GRID_H_INCLUDED
#define GRID_H_INCLUDED

#include <vector>
#include "SDL.h"
#include "point.h"
#include "vector.h"
#include "basis.h"

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
        void render(SDL_Surface* canvas, SDL_Rect viewPortRect, Basis camera);

    private:
        std::vector<GridPoint> lattice;
        Basis system;
        int rows, columns;
        double cellSize;
        void setLatticePoints();


};






#endif // GRID_H_INCLUDED
