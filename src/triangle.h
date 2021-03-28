#ifndef TRIANGLE_H_INCLUDED
#define TRIANGLE_H_INCLUDED

#include <array.h>
#include "point.h"

struct Triangle {
    std::array<Point, 3> vertices;
    std::array<SDL_Color, 3> colors;
    Triangle(Point a, Point b, Point c, SDL_Color x, SDL_Color y, SDL_Color z);
    Triangle();
    
    // For blackface calling. 
    void flip();
};

#endif // TRIANGLE_H_INCLUDED