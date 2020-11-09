#include "point.h"
#include <cmath>

Point::Point() : x (0), y (0), z (0) { }
Point::Point(double x_, double y_, double z_) {

}

double distance(Point a, Point b) {
    return std::hypot(b.x-a.x, b.y-a.y, b.z-a.z);
}
