#include "vector.h"
#include <cmath>

Vector::Vector(): x(0), y(0), z(0) { }

Vector::Vector(double x_, double y_, double z_): x(x_), y(y_), z(z_) { }

double Vector::magnitude() const {
  return std::hypot(x, y, z); 
}