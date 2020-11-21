#include "matrix.h"
#include <iostream>
#include <cmath>
#include <SDL.h>

using std::sin;
using std::cos;
using std::abs;

const double deg_to_rad = M_PI/180;
const double epsilon = 1.0e-9;

Matrix::Matrix() {
    data = {0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0};
}



Matrix::Matrix(double a11, double a12, double a13, double a14,
               double a21, double a22, double a23, double a24,
               double a31, double a32, double a33, double a34,
               double a41, double a42, double a43, double a44) {
    data = {
        a11, a12, a13, a14,
        a21, a22, a23, a24,
        a31, a32, a33, a34,
        a41, a42, a43, a44
    };

}

Matrix operator*(Matrix m1, Matrix m2) {
    Matrix result;

    // "k" is the current position in the result array.
    for (int k = 0; k < 16; k += 1) {
        int m1_row = int(k/4);
        int m2_column = k%4;

        // "i" is the current position in the row and colunm we are multiplying
        for (int i = 0; i < 4; i += 1) {
            int m1_data_index = m1_row * 4 + i;
            int m2_data_index = m2_column + 4 * i;
            result.data[k] += m1.data[m1_data_index] * m2.data[m2_data_index];
        }
    }
    return result;
}

std::ostream& operator<<(std::ostream& s, Matrix m) {
    for (int i = 0; i < 16; i += 1) {
        if ((i + 1) % 4 == 0) {
            s << m.data[i] << ", \n";
        } else {
            s << m.data[i] << ", ";
        }
    }
    return s;
}

Vector operator*(Matrix m, Vector v) {
    double x = m.data[0]  * v.x + m.data[1]  * v.y + m.data[2]  * v.z + m.data[3]  * 1;
    double y = m.data[4]  * v.x + m.data[5]  * v.y + m.data[6]  * v.z + m.data[7]  * 1;
    double z = m.data[8]  * v.x + m.data[9]  * v.y + m.data[10] * v.z + m.data[11] * 1;
    double w = m.data[12] * v.x + m.data[13] * v.y + m.data[14] * v.z + m.data[15] * 1;
    if (abs(w) > epsilon) {
        // Normalize.
        x /= w;
        y /= w;
        z /= w;
    }
    Vector result = Vector(x, y, z);
    return result;
}

Point operator*(Matrix m, Point p) {
    double x = m.data[0]  * p.x + m.data[1]  * p.y + m.data[2]  * p.z + m.data[3]  * 1;
    double y = m.data[4]  * p.x + m.data[5]  * p.y + m.data[6]  * p.z + m.data[7]  * 1;
    double z = m.data[8]  * p.x + m.data[9]  * p.y + m.data[10] * p.z + m.data[11] * 1;
    double w = m.data[12] * p.x + m.data[13] * p.y + m.data[14] * p.z + m.data[15] * 1;
    if (abs(w) > epsilon) {
        // Normalize.
        x /= w;
        y /= w;
        z /= w;
    }
    Point result = Point(x, y, z);
    return result;
}

Matrix translationMatrix(Vector v) {
    Matrix matrix(1, 0, 0, v.x,
                  0, 1, 0, v.y,
                  0, 0, 1, v.z,
                  0, 0, 0, 1);
    return matrix;
}

Matrix scalingMatrix(double xFactor, double yFactor, double zFactor) {
    Matrix matrix(xFactor, 0, 0, 0,
                  0, yFactor, 0, 0,
                  0, 0, zFactor, 0,
                  0, 0, 0, 1);
    return matrix;
}

Matrix scalingMatrix(double scaleFactor) {
    return scalingMatrix(scaleFactor, scaleFactor, scaleFactor);
}

Matrix xRotate(double thetaDeg) {
    double thetaRad = thetaDeg * deg_to_rad;
    return Matrix(1, 0, 0, 0,
                  0, cos(thetaRad), -sin(thetaRad), 0,
                  0, sin(thetaRad), cos(thetaRad), 0,
                  0, 0, 0, 1);

}

Matrix yRotate(double thetaDeg) {
    double thetaRad = thetaDeg * deg_to_rad;
    return Matrix(cos(thetaRad), 0, sin(thetaRad), 0,
                  0, 1, 0, 0,
                  -sin(thetaRad), 0, cos(thetaRad), 0,
                  0, 0, 0, 1);

}

Matrix zRotate(double thetaDeg) {
    double thetaRad = thetaDeg * deg_to_rad;
    return Matrix(cos(thetaRad), -sin(thetaRad), 0, 0,
                  sin(thetaRad), cos(thetaRad), 0, 0,
                  0, 0, 1, 0,
                  0, 0, 0, 1);

}

Matrix identity() {
    return Matrix(1, 0, 0, 0,
                  0, 1, 0, 0,
                  0, 0, 1, 0,
                  0, 0, 0, 1);
}
// Produces a matrix that transforms points from world space to screen space,
// and then from screen space to viewport space.
//
// World space is the original location of the point in the virtual world.
//
// Screen space is a compressed space -- a box.  The front of this box lies on
// the world space's XY plane, with an upper left corner of:
//   (x=screenSpaceRect.x, y=screenSpaceRect.y, z=0)
// and a lower right corner of:
//   (x=screenSpaceRect.x + screenSpaceRect.w,
//    y=screenSpaceRect.y + screenSpaceRect.h,
//    z=0)
// The back of the screen space box lies at infinity, but Z-coordinates are
// still compressed: large Z coordinates in world space have smaller Z
// coordinates in screen space.
//
// Another way to think of the screenSpaceRect is as the field of view for the
// 3D camera.
//
// Viewport space is a scaled transformation of world space meant to represent
// where you will be drawing on the actual screen.  The front of its box is
// also on the XY plane, but this time between (viewportRect.x,
// viewportRect.y) and (viewportRect.x + viewportRect.w, viewportRect.y +
// viewportRect.h).  The Z coordinates are the same as they were in screen
// space.  The front upper-left corner of screen space should project to the
// front upper-left corner of view space, and the line z=0 in world space
// should project to the center of view space.
//
// d is the putative distance from your eyeballs to the XY plane in screen
// space, and it must be positive.
Matrix projectionMatrix(double d, SDL_Rect screenSpaceRect, SDL_Rect viewportRect) {

    // The projection formula is:
    //
    //   x' = dx / (d + z)
    //   y' = dy / (d + z)
    //
    // So how does this matrix help?  Well, it maps (x, y, z, 1) (as a column
    // matrix) to (x, y, z/d, z/d + 1).  Pay close attention to that last
    // number: the final component is *not equal to 1*, even though it was
    // supposed to be.  That means we need to normalize by *DIVIDING* by z/d +
    // 1!
    //
    //       x / (z/d + 1) =     x / ((z + d) / d) = dx / (z + d)
    //       y / (z/d + 1) =     y / ((z + d) / d) = dy / (z + d)
    //   (z/d) / (z/d + 1) = (z/d) / ((z + d) / d) =  z / (z + d)
    //
    // So this really _is_ the projection formula, provided matrix
    // multiplication normalizes when it's done.
    //
    // Along the way, we flip the sign of y, since positive Y points in the
    // downward direction in screen screen space.

    Matrix p(1,  0, 0,     0,
             0, -1, 0,     0,
             0,  0, 1.0/d, 0,
             0,  0, 1.0/d, 1);

    // To translate the X and Y coordinates from world space to screen space,
    // we use linear interpolation:
    //
    //   u = (x' - screenSpaceRect.x) / (screenSpaceRect.w)
    //   v = (y' - screenSpaceRect.y) / (screenSpaceRect.h)
    //
    // Note that 0 <= u <= 1 and 0 <= v <= 1 if a point is on the screen.

    Matrix s(1.0 / screenSpaceRect.w, 0,                       0, double(-screenSpaceRect.x) / screenSpaceRect.w,
             0,                       1.0 / screenSpaceRect.h, 0, double(-screenSpaceRect.y) / screenSpaceRect.h,
             0,                       0,                       1, 0,
             0,                       0,                       0, 1);
    std::cout << s;

    // Since u and v are normalized, scaling them to view space is simple:
    //
    //   final x = viewportRect.x + u * viewportRect.w
    //   final y = viewportRect.y + v * viewportRect.h

    Matrix v(viewportRect.w, 0,              0, viewportRect.x,
             0,              viewportRect.h, 0, viewportRect.y,
             0,              0,              1, 0,
             0,              0,              0, 1);

    // Apply the transformations in reverse.
    return v * s * p;
}
