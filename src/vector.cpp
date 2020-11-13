#include "vector.h"
#include <cmath>
#include <string>

const double epsilon = 0.00001;

Vector::Vector(): x(0), y(0), z(0) { }

Vector::Vector(double x_, double y_, double z_): x(x_), y(y_), z(z_) { }

double Vector::magnitude() const {
    return std::hypot(x, y, z);
}

Point operator+(Point p, Vector v) {
    return Point(p.x+v.x, p.y+v.y, p.z+v.z);
}

Point operator+(Vector v, Point p) {
    return Point(p.x+v.x, p.y+v.y, p.z+v.z);
}

Vector operator*(Vector v, double f) {
    return Vector(v.x*f, v.y*f, v.z*f);
}

Vector operator*(double f, Vector v) {
    return Vector(v.x*f, v.y*f, v.z*f);
}

Vector operator/(Vector v, double f) {
    return Vector(v.x/f, v.y/f, v.z/f);
}

std::ostream& operator<<(std::ostream& s, Vector v) {
    s << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return s;
}

double dotProduct(Vector v1, Vector v2) {
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

// Understand the wizardry at work: https://www.mathsisfun.com/algebra/vectors-cross-product.html
Vector crossProduct(Vector v1, Vector v2) {
    // Cross product formula finds a vector perpendicular to the other 2 vectors
    // cx = aybz − azby
    // cy = azbx − axbz
    // cz = axby − aybx
    return Vector(v1.y*v2.z - v1.z*v2.y,
                  v1.z*v2.x - v1.x*v2.z,
                  v1.x*v2.y - v1.y*v2.x);
}


Vector normalize (Vector v) {
    if (v.magnitude()<-epsilon || v.magnitude()>epsilon){
        return v / v.magnitude();
    } else {
        throw std::runtime_error("Attempted to normalize a vector of length " + std::to_string(v.magnitude()));
    }
}
