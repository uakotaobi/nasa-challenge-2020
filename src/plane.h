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

    Vector normalVector() const;
    double whichSide(Point p) const;
    Point pointOnPlane() const;

};

#endif // PLANE_H_INCLUDED
