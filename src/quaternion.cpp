#include "quaternion.h"
#include "common.h"

using std::ostream;

Quaternion::Quaternion() :
    realPart(0), iCoefficient(0), jCoefficient(0), kCoefficient(0) { }

Quaternion::Quaternion(double A, double B, double C, double D) :
    realPart(A), iCoefficient(B), jCoefficient(C), kCoefficient(D) { }


Quaternion operator*(Quaternion q, double f) {
    return Quaternion(q.realPart * f, q.iCoefficient * f, q.jCoefficient * f, q.kCoefficient * f);
}

Quaternion operator*(double f, Quaternion q) {
    return Quaternion(q.realPart * f, q.iCoefficient * f, q.jCoefficient * f, q.kCoefficient * f);
}

Quaternion operator/(Quaternion q, double f) {
    return Quaternion(q.realPart / f, q.iCoefficient / f, q.jCoefficient / f, q.kCoefficient / f);
}

Quaternion operator/(double f, Quaternion q) {
    return Quaternion(q.realPart / f, q.iCoefficient / f, q.jCoefficient / f, q.kCoefficient / f);
}

Quaternion operator*(Quaternion p, Quaternion q) {
    const double a = p.realPart, b = p.iCoefficient, c = p.jCoefficient, d = p.kCoefficient;
    const double e = q.realPart, f = q.iCoefficient, g = q.jCoefficient, h = q.kCoefficient;

    // Where the heck do these numbers come from?
    //
    // Well, specifically, they come from
    // https://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation#Quaternions.
    // But *conceptually*, these numbers calculate
    //
    //   VW = V × W - V ⋅ W
    //      = (st - v⋅w) + (sw + tv + v×w)
    //
    // Where:
    //   V is the first quaternion, with:
    //     - Real part s, and
    //     - Imaginary VECTOR part v;
    //   W is the second quaternion, with:
    //     - Real part t, and
    //     - Imaginary VECTOR part w;
    //   "×" denotes the usual vector cross product, and
    //   "⋅" denotes the usual vector dot product.
    //
    // Multiply all that out and you get the formula below.
    return Quaternion(a*e - b*f - c*g - d*h,
                      a*f + b*e + c*h - d*g,
                      a*g - b*h + c*e + d*f,
                      a*h + b*g - c*f + d*e);
}

Quaternion Quaternion::conjugate() const {
    return Quaternion(realPart,
                      -iCoefficient,
                      -jCoefficient,
                      -kCoefficient);
}

double Quaternion::norm() const {
    return realPart * realPart +
        iCoefficient * iCoefficient +
        jCoefficient * jCoefficient +
        kCoefficient * kCoefficient;
}

Quaternion Quaternion::versor() const {
    return *this / norm();
}

Quaternion Quaternion::inverse() const {
    const double n = norm();
    return conjugate() / (n * n);
}

// The most important function in this file.  Sticks a rotation into a
// quaternion and returns it.
Quaternion rotationQuaternion(Vector axis, double thetaDeg) {
    const Vector u = normalize(axis);
    const double theta = thetaDeg * deg_to_rad;

    Quaternion q = Quaternion(cos(theta / 2),
                              u.x * sin(theta / 2),
                              u.y * sin(theta / 2),
                              u.z * sin(theta / 2));
    return q;
}

Point rotate(Point p, Quaternion r) {
    Quaternion P(0, p.x, p.y, p.z);
    Quaternion P_prime = r * P * r.inverse();
    return Point(P_prime.iCoefficient, P_prime.jCoefficient, P_prime.kCoefficient);
}

Vector rotate(Vector p, Quaternion r) {
    Quaternion P(0, p.x, p.y, p.z);
    Quaternion P_prime = r * P * r.inverse();
    return Vector(P_prime.iCoefficient, P_prime.jCoefficient, P_prime.kCoefficient);
}

// Rotation matrix cribbed from
// https://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation#Quaternion-derived_rotation_matrix.
Matrix rotationMatrix(Quaternion r) {
    const double s = 1 / (r.norm() * r.norm());  // Note that if r is a unit quaternion, s == 1.

    const double rr = r.realPart;
    const double ri = r.iCoefficient;
    const double rj = r.jCoefficient;
    const double rk = r.kCoefficient;

    return Matrix(1 - 2*s * (rj*rj + rk*rk),     2*s * (ri*rj - rk*rr),     2*s * (ri*rk + rj*rr), 0,
                      2*s * (ri*rj + rk*rr), 1 - 2*s * (ri*ri + rk*rk),     2*s * (rj*rk - ri*rr), 0,
                      2*s * (ri*rk - rj*rr),     2*s * (rj*rk + ri*rr), 1 - 2*s * (ri*ri + rj*rj), 0,
                  0,                         0,                         0,                         1);
}

ostream& operator<<(ostream& out, Quaternion q) {
    return out << "(" << q.realPart << " + "
               << q.iCoefficient << "i + "
               << q.jCoefficient << "j + "
               << q.kCoefficient << "k)";
}
