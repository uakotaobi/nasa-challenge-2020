#include "grid.h"

GridPoint::GridPoint() : Point(0, 0, 0), color(SDL_Color{0, 0, 0}), temperatureKelvin(0), slopeDeg(0) {}
GridPoint::GridPoint(Point p, SDL_Color color_, double temperatureKelvin_, double slopeDeg_)
    : Point(p), color(color_), temperatureKelvin(temperatureKelvin_), slopeDeg(slopeDeg_) {}
