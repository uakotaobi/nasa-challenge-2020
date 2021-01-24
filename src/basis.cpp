#include "basis.h"
#include "matrix.h"

Basis::Basis() : center(0, 0, 0), axisX(1, 0, 0), axisY(0, 1, 0), axisZ(0, 0, 1) {

}

Basis::Basis(Point center_, Vector axisX_, Vector axisY_, Vector axisZ_)
    : center(center_), axisX(axisX_), axisY(axisY_), axisZ(axisZ_){

}

void Basis::apply(const Matrix& transformationMatrix) {
    center = transformationMatrix * center;
    axisX = transformationMatrix * axisX;
    axisY = transformationMatrix * axisY;
    axisZ = transformationMatrix * axisZ;
}

std::ostream& operator<<(std::ostream& s, Basis b) {
    s << "{ center: " << b.center << " X: " << b.axisX << " Y: " << b.axisY << " Z: " << b.axisZ << " }";
    return s;
}
