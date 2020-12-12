#include "grid.h"

GridPoint::GridPoint() : Point(0, 0, 0), color(SDL_Color{0, 0, 0}), temperatureKelvin(0), slopeDeg(0) {}
GridPoint::GridPoint(Point p, SDL_Color color_, double temperatureKelvin_, double slopeDeg_)
    : Point(p), color(color_), temperatureKelvin(temperatureKelvin_), slopeDeg(slopeDeg_) {}


Grid::Grid() : lattice((49 + 1) * (49 + 1)), 
               center(0, 0, 0), 
               axisX(1, 0, 0), 
               axisY(0, 1, 0),
               axisZ(0, 0, 1),
               rows(49),
               columns(49),
               cellSize(1) {}
               