#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include <array>
#include "vector.h"
#include "point.h"
#include <ostream>

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

Matrix translationMatrix(Vector v);

Matrix scalingMatrix(double xFactor, double yFactor, double zFactor);

Matrix scalingMatrix(double scaleFactor);

Matrix xRotate(double thetaDeg);
Matrix yRotate(double thetaDeg);
Matrix zRotate(double thetaDeg);

Matrix projectionMatrix(double focalDistance);

#endif // MATRIX_H_INCLUDED
