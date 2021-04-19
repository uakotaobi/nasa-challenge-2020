#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <iosfwd>
struct SDL_Color;

// Epsilon is used when doing comparisons with zero, but accounting for
// inconsistencies caused by data limits.
extern const double epsilon;
// Converts degrees to radians when multiplied
extern const double deg_to_rad;
// Converts radians to degrees when multiplied
extern const double rad_to_deg;

// Returns -1.0 if x < 0, +1.0 if x > 0, and 0.0 otherwise.
template <typename T>
inline T sgn(T x) { return x < 0 ? -1.0 : x > 0 ? 1.0 : 0.0; }

// Prints a color.
std::ostream& operator<<(std::ostream& out, const SDL_Color& c);

#endif // COMMON_H_INCLUDED
