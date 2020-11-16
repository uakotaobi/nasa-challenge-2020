#include "matrix.h"

Matrix::Matrix() {

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
        // "i" is the current row of m1.
        for (int i = 0; i < 4; i += 1) {
            // "j" is both the current row of m2 and current column of m1
            // The current column of m2 is k MOD 4.
            for (int j = 0; j < 4; j += 1) {
                result.data[k] += m1.data[4 * i + j] * m2.data[4 * j + (k % 4)];
            }
        }
    }  
    return result; 
}

std::ostream& operator<<(std::ostream& s, Matrix m) {
    for (int i = 0; i < 16; i += 1) {
        s << m.data[i] << ", ";        
    }
    return s;
}     
    
    