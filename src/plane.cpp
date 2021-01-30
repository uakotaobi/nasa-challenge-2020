#include "plane.h"
#include <cmath>
#include "common.h"

Plane::Plane() : A (0), B (0), C (1), D (0) {};
Plane::Plane(double A_, double B_, double C_, double D_) : A (A_), B (B_), C (C_), D (D_) {};
Plane::Plane(Point center, Vector normal) : A (normal.x), B (normal.y), C (normal.z), D (-(normal.x * center.x  + normal.y * center.y + normal.z * center.z)) {};

Vector Plane::normalVector() const {
    return Vector(A, B, C);
}
double Plane::whichSide(Point p) const {
    return A*p.x + B*p.y + C*p.z + D;
}
Point Plane::pointOnPlane() const {
    Point p;
    // If A != 0: return (-D/A, 0, 0)
    // Else If B != 0: return (0, -D/B, 0)
    // Else If C != 0: return (0, 0, -D/C)
    // Else: return (0, 0, 0)
    if (abs(A) > epsilon) {
        return Point(-D/A, 0, 0);
    } else if (abs(B) > epsilon) {
        return Point(0, -D/B, 0);
    } else if (abs(C) > epsilon) {
        return Point(0, 0, -D/C);
    } else {
        return Point(0, 0, 0);
    }
}
