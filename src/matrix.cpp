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
    std::cout << result;
    // FOR k from 0 to 15
    //     m1_row = FLOOR(k/4)
    //     m2_column = k % 4
    //     FOR i from 0 to 3
    //         result[k] += m1[m1_row * 4 + i] * m2[m2_column + 4 * i]
    //     NEXT i
    // NEXT k

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
