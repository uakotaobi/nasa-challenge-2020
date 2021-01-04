#ifndef PLANE_H_INCLUDED
#define PLANE_H_INCLUDED

#include "vector.h"
#include "point.h"

struct Plane {
    double A;
    double B;
    double C;
    double D;

    Plane();
    Plane(double A_, double B_, double C_, double D_);
    Plane(Point center, Vector normal);

    // Normal is always perpendicular to the plane
    Vector normalVector() const;
    
    // 0- point is on this plane
    // Positive- point is above the plane
    // Negative- point below the plane
    double whichSide(Point p) const;
    
    // Returns a point that will be on the plane 
    Point pointOnPlane() const;

};

#endif // PLANE_H_INCLUDED
