#ifndef GRID_H_INCLUDED
#define GRID_H_INCLUDED

#include <vector>
#include "SDL.h"
#include "point.h"

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
        Grid(int rows, int columns, double defaultDistance);
    private:
        std::vector<GridPoint> lattice;
};






#endif // GRID_H_INCLUDED
