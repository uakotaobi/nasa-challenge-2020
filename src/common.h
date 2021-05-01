#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <iostream>
#include "SDL.h"

// Epsilon is used when doing comparisons with zero, but accounting for
// inconsistencies caused by data limits.
extern const double epsilon;
// Converts degrees to radians when multiplied
extern const double deg_to_rad;
// Converts radians to degrees when multiplied
extern const double rad_to_deg;


// Print an SDL color to the given output stream.
std::ostream& operator<<(std::ostream& out, SDL_Color color);

#endif // COMMON_H_INCLUDED
