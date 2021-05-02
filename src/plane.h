#ifndef PLANE_H_INCLUDED
#define PLANE_H_INCLUDED

#include "vector.h"
#include "point.h"

#include <optional>

// A plane is the locus of points that solve the equation Ax + By + Cz + D = 0
// A plane is flat, 2d, and infinite. Planes define a vector called a normal.
// Normals are perpendicular to planes, and they point away from them.

struct Plane {
    double A;
    double B;
    double C;
    double D;

    Plane();
    Plane(Point center, Vector normal);
    Plane(Point a, Point b, Point c);
    
    // Define the plane Ax + By + Cz + D = 0 using the coefficients A, B, C, and D. 
    Plane(double A_, double B_, double C_, double D_);
    
    // Normal is always perpendicular to the plane
    Vector normalVector() const;

    // 0- point is on this plane
    // Positive- point is above the plane
    // Negative- point below the plane
    double whichSide(Point p) const;

    // Returns a point that will be on the plane
    Point pointOnPlane() const;

    // Returns a reflection of an incoming vector.
    Vector reflection(Vector incomingVector) const;

    // Returns the point of intersection between the given line and this plane.
    // Returns nullopt if line is parallel to the plane.
    std::optional<Point> pointOfIntersection(Point p1, Point p2) const;

    // Returns the projection of v onto this plane, in other words it returns
    // the component of v which is parallel to this plane.
    Vector projection(Vector v) const;

    // Returns the smallest distance between "p" and the Plane.
    double distance(Point p) const;

};

#endif // PLANE_H_INCLUDED
