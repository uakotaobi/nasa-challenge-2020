#include "common.h"
#include <cmath>
#include <iomanip>

const double epsilon = 1e-9;
const double deg_to_rad = M_PI/180;
const double rad_to_deg = 180/M_PI;

using namespace std;

ostream& operator<<(ostream& out, SDL_Color color) {
    out.fill('0');
    out << "#" << hex << setw(2) << (int)color.r << setw(2) 
        << (int)color.g << setw(2) << (int)color.b << setw(2) << (int)color.a << dec;
    out.fill(' ');
    return out;

}