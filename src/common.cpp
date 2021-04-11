#include "common.h"
#include "SDL.h"
#include <cmath>
#include <iostream>
#include <iomanip>

const double epsilon = 1e-9;
const double deg_to_rad = M_PI/180;
const double rad_to_deg = 180/M_PI;

std::ostream& operator<<(std::ostream& out, const SDL_Color& c) {

    using std::hex;
    using std::dec;
    using std::setw;

    out << "#";

    char oldfill = out.fill('0');
    int oldwidth = out.width(2);

    out << hex
        << setw(2) << static_cast<int>(c.r)
        << setw(2) << static_cast<int>(c.g)
        << setw(2) << static_cast<int>(c.b)
        << setw(2) << static_cast<int>(c.a);

    out.fill(oldfill);
    out.width(oldwidth);
    return out << dec;
}
