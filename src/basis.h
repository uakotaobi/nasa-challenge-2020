#ifndef BASIS_H_INCLUDED
#define BASIS_H_INCLUDED

#include "matrix.h"
#include "point.h"
#include "vector.h"

// A basis is a point and 3 vectors defining local coordinate systems (camera)
struct Basis {
    Point center;
    Vector axisX;
    Vector axisY;
    Vector axisZ;

    Basis();
    Basis(Point center_, Vector axisX_, Vector axisY_, Vector axisZ_);

    // Applies matrix to center point and 3 axes
    void apply(Matrix transformationMatrix);
};

#endif // BASIS_H_INCLUDED
