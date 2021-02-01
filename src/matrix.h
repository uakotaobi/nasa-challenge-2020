#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include <array>
#include <ostream>

#include "SDL.h"

#include "vector.h"
#include "point.h"
#include "basis.h"

class Matrix {
    public:
        Matrix();
        Matrix(double a11, double a12, double a13, double a14,
               double a21, double a22, double a23, double a24,
               double a31, double a32, double a33, double a34,
               double a41, double a42, double a43, double a44);

        friend Matrix operator*(Matrix m1, Matrix m2);
        friend Vector operator*(Matrix m, Vector v);
        friend Point operator*(Matrix m, Point p);
        friend std::ostream& operator<<(std::ostream&, Matrix m);
    private:
        std::array<double, 16> data;
};

Matrix identityMatrix();

Matrix translationMatrix(Vector v);

Matrix scalingMatrix(double xFactor, double yFactor, double zFactor);

Matrix scalingMatrix(double scaleFactor);

Matrix xRotate(double thetaDeg);
Matrix yRotate(double thetaDeg);
Matrix zRotate(double thetaDeg);

// Returns a rotation matrix around a line that starts at the origin and points in the given direction.
Matrix rotationMatrix(Vector axis, double thetaDeg);

// Returns a rotation matrix around an arbitrary line.
Matrix rotationMatrix(Point a, Point b, double thetaDeg);

// This function returns the rotation matrix that rotates the given number of degrees around the given Basis's axes.
//
// Yaw is the angle in degrees around b.axisY (think of it like a door opening).
// Pitch is the angle in degrees around b.axisX (think of it like a pitcher pouring).
// Roll is the angle in degrees around b.axisZ (think of it like a cat
// rolling).
//
// TODO: Euler angles seem to be based on a fixed coordinate system, rendering
// the Basis argument meaningless (the basis for all of these rotations is
// ijk-origin.)  Get rid of the Basis arg -- we're not using it anyway.
Matrix eulerRotationMatrix(Basis b, double yawDeg, double pitchDeg, double rollDeg);

class SDL_Rect;
Matrix projectionMatrix(double focalDistance, SDL_Rect screenRect, SDL_Rect viewPortRect);

Matrix cameraTransform(Vector X, Vector Y, Vector Z, Point p);


#endif // MATRIX_H_INCLUDED
