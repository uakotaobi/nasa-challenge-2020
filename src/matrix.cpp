#include <iostream>
#include <cmath>

#include "matrix.h"
#include "common.h"

using std::sin;
using std::cos;
using std::tan;

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

        // "i" is the current position in the row and column we are multiplying
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
    // 0 at end because vectors can't be translated, they don't have a location, just a direction
    double x = m.data[0] * v.x + m.data[1] * v.y + m.data[2] * v.z + m.data[3] * 0;
    double y = m.data[4] * v.x + m.data[5] * v.y + m.data[6] * v.z + m.data[7] * 0;
    double z = m.data[8] * v.x + m.data[9] * v.y + m.data[10] * v.z + m.data[11] * 0;
    double w = m.data[12] * v.x + m.data[13] * v.y  + m.data[14] * v.z + m.data[15] * 0;
    if (abs(w) >= epsilon) {
        x /= w;
        y /= w;
        z /= w;
    }
    Vector result = Vector(x, y, z);
    return result;
}

Point operator*(Matrix m, Point p) {
    double x = m.data[0] * p.x + m.data[1] * p.y + m.data[2] * p.z + m.data[3] * 1;
    double y = m.data[4] * p.x + m.data[5] * p.y + m.data[6] * p.z + m.data[7] * 1;
    double z = m.data[8] * p.x + m.data[9] * p.y + m.data[10] * p.z + m.data[11] * 1;
    double w = m.data[12] * p.x + m.data[13] * p.y  + m.data[14] * p.z + m.data[15] * 1;
    if (abs(w) >= epsilon) {
        x /= w;
        y /= w;
        z /= w;
    }
    Point result = Point(x, y, z);
    return result;
}

Matrix identityMatrix() {
    return Matrix(1, 0, 0, 0,
                  0, 1, 0, 0,
                  0, 0, 1, 0,
                  0, 0, 0, 1);
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

Matrix rotationMatrix(Vector axis, double thetaDeg) {

    double thetaRad = thetaDeg * deg_to_rad;
    double cosTheta = cos(thetaRad);
    double sinTheta = sin(thetaRad);
    axis = normalize(axis);
    double u = axis.x;
    double v = axis.y;
    double w = axis.z;

    return Matrix(   cosTheta + u*u*(1-cosTheta), -w*sinTheta + u*v*(1-cosTheta),  v*sinTheta + u*w*(1-cosTheta), 0,
                   w*sinTheta + v*u*(1-cosTheta),    cosTheta + v*v*(1-cosTheta), -u*sinTheta + v*w*(1-cosTheta), 0,
                  -v*sinTheta + w*u*(1-cosTheta),  u*sinTheta + w*v*(1-cosTheta),    cosTheta + w*w*(1-cosTheta), 0,
                  0, 0, 0, 1);
}

Matrix rotationMatrix(Point a, Point b, double thetaDeg) {
    // Axis is a vector that point from a to b.
    // Rotates clockwise, not counterclockwise like the right hand rule
    Vector axis = -(b - a);
    Matrix translateToOrigin = translationMatrix(-Vector(a));
    Matrix translateFromOrigin = translationMatrix(Vector(a));

    return translateFromOrigin * rotationMatrix(axis, thetaDeg) * translateToOrigin;
}

Matrix eulerRotationMatrix(Basis b, double yawDeg, double pitchDeg, double rollDeg) {
    Matrix translateToOrigin = translationMatrix(-Vector(b.center));

    // Transforms the basis so its axes are mapped to i, j, k.
    /* Matrix rigidBodyTransformation(b.axisX.x, b.axisY.x, b.axisZ.x, 0,
                                   b.axisX.y, b.axisY.y, b.axisZ.y, 0,
                                   b.axisX.z, b.axisY.z, b.axisZ.z, 0,
                                   0, 0, 0, 1); */

    // Start with Z (roll), then Y (yaw), then X (pitch), for the ZYX Tait-Bryan angle Matrix .
    // Order is Important with a capital I.
    double c1 = cos(rollDeg * deg_to_rad);
    double c2 = cos(yawDeg * deg_to_rad);
    double c3 = cos(pitchDeg * deg_to_rad);
    double s1 = sin(rollDeg * deg_to_rad);
    double s2 = sin(yawDeg * deg_to_rad);
    double s3 = sin(pitchDeg * deg_to_rad);

    Matrix taitBryanZYX (
        c1 * c2, c1 * s2 * s3 - c3 * s1, s1 * s3 + c1 * c3 * s2, 0,
        c2 * s1, c1 * c3 + s1 * s2 * s3, c3 * s1 * s2 - c1 * s3, 0,
        -s2    , c2 * s3               , c2 * c3               , 0,
        0      , 0                     , 0                     , 1);

    /* Matrix inverseRigidBodyTransformation(b.axisX.x, b.axisX.y, b.axisX.z, 0,
                                          b.axisY.x, b.axisY.y, b.axisY.z, 0,
                                          b.axisZ.x, b.axisZ.y, b.axisZ.z, 0,
                                          0, 0, 0, 1); */

    Matrix translateFromOrigin = translationMatrix(Vector(b.center));
    return translateFromOrigin */* inverseRigidBodyTransformation */ taitBryanZYX */* rigidBodyTransformation */ translateToOrigin;
}

Matrix projectionMatrix(double focalDistance, SDL_Rect screenRect, SDL_Rect viewPortRect) {
    // World Space to World Space's XY plane
    Matrix p(1, 0, 0, 0,
             0, -1, 0, 0,
             0, 0, 1/focalDistance, 0,
             0, 0, 1/focalDistance, 1);

    // Rectangle on XY plane, shrinks everything in screenRect to between 0-1 on XY
    Matrix s(1.0/screenRect.w, 0, 0, double(-screenRect.x)/screenRect.w,
             0, 1.0/screenRect.h,  0, double(-screenRect.y)/screenRect.h,
             0, 0, 1, 0,
             0, 0, 0, 1);

    // Scaling out from 0-1 to screen space's viewPortRect
    Matrix v(viewPortRect.w, 0, 0, viewPortRect.x,
             0, viewPortRect.h, 0, viewPortRect.y,
             0, 0, 1, 0,
             0, 0, 0, 1);

    // v = identityMatrix();
    // s = identityMatrix();

    // Multiply matrices in reverse order that you want the operations to occur (p, then s, then v)
    return v * s * p;
}

Matrix cameraTransform(Vector X, Vector Y, Vector Z, Point p) {
    Matrix p_to_origin(translationMatrix(-Vector(p)));
    X = normalize(X);
    Y = normalize(Y);
    Z = normalize(Z);
    // Transforms a basis so its axes are mapped to i, j, k.
    Matrix rigidBodyTransformation(X.x, X.y, X.z, 0,
                                   Y.x, Y.y, Y.z, 0,
                                   Z.x, Z.y, Z.z, 0,
                                   0, 0, 0, 1);
    return rigidBodyTransformation * p_to_origin;
}
