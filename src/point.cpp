#include "point.h"
#include <cmath>
#include "vector.h"

Point::Point() : x (0), y (0), z (0) { }
Point::Point(double x_, double y_, double z_) : x (x_), y (y_), z (z_) { }

double distance(Point a, Point b) {
    return std::hypot(b.x-a.x, b.y-a.y, b.z-a.z);
}

Vector operator-(Point p1, Point p2) {
    return Vector(p1.x-p2.x, p1.y-p2.y, p1.z-p2.z);
}

Point& Point::operator-=(Vector v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

std::ostream& operator<<(std::ostream& s, Point p) {
    s << "(" << p.x << ", " << p.y << ", " << p.z << ")";
    return s;
}
