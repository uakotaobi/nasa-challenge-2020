#include "plane.h"
#include <cmath>
#include "common.h"

Plane::Plane() : A (0), B (0), C (1), D (0) {};
Plane::Plane(double A_, double B_, double C_, double D_) : A (A_), B (B_), C (C_), D (D_) {};
Plane::Plane(Point center, Vector normal) : A (normal.x), B (normal.y), C (normal.z), D (-(normal.x * center.x  + normal.y * center.y + normal.z * center.z)) {};

Plane::Plane(Point a, Point b, Point c) {
    A = (b.y - a.y) * (c.z - a.z) - (c.y - a.y) * (b.z - a.z);
    B = (b.z - a.z) * (c.x - a.x) - (c.z - a.z) * (b.x - a.x);
    C = (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
    D = -(A * a.x + B * a.y + C * a.z);
}

Vector Plane::normalVector() const {
    return Vector(A, B, C);
}
double Plane::whichSide(Point p) const {
    return A*p.x + B*p.y + C*p.z + D;
}
Vector Plane::projection(Vector v) const{
    Vector n = normalize(normalVector());
    Vector p = n * dotProduct(v, n);
    return v - p;
}
double Plane::distance(Point p) const{
    return whichSide(p) / normalVector().magnitude();
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

Vector Plane::reflection(Vector incomingVector) const {
    Vector n = normalize(Vector(A, B, C));
    // Here is a link to the reflection function: https://math.stackexchange.com/questions/13261/how-to-get-a-reflection-vector.
    return incomingVector - 2 * dotProduct(incomingVector, n) * n;
}

// function was implemented with the following source:
// https://www.geomalgorithms.com/a05-_intersect-1.html#Line-Plane-Intersection
std::optional<Point> Plane::pointOfIntersection(Point p1, Point p2) const {
    Vector u = p2 - p1;
    Vector n = normalVector();
    if (abs(dotProduct(u, n)) < epsilon) {
        // They are parallel.
        return std::nullopt;
    } else {
        // They are not parallel so they intersect at p1 + s*(p2 - p1).
        double s = -(A*p1.x + B*p1.y + C*p1.z + D) / dotProduct(u, n);
        if (s >= 0 && s <= 1) {
            // The line segment intersects the plane.
            return std::optional<Point>(p1 + s*(p2 - p1));
        } else {
            // The intersection is outside the line segment
            return std::nullopt;
        }
    }
}
