#include "matrix.h"
#include <iostream>

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
    double x = m.data[0] * v.x + m.data[1] * v.y + m.data[2] * v.z + m.data[3] * 1;
    double y = m.data[4] * v.x + m.data[5] * v.y + m.data[6] * v.z + m.data[7] * 1;
    double z = m.data[8] * v.x + m.data[9] * v.y + m.data[10] * v.z + m.data[11] * 1;
    Vector result = Vector(x, y, z);
    return result;
}

Point operator*(Matrix m, Point p) {
    double x = m.data[0] * p.x + m.data[1] * p.y + m.data[2] * p.z + m.data[3] * 1;
    double y = m.data[4] * p.x + m.data[5] * p.y + m.data[6] * p.z + m.data[7] * 1;
    double z = m.data[8] * p.x + m.data[9] * p.y + m.data[10] * p.z + m.data[11] * 1;
    Point result = Point(x, y, z);
    return result;
}