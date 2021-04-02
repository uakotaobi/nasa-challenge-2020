#ifndef QUATERNION_H_INCLUDED
#define QUATERNION_H_INCLUDED

#include <iostream>
#include "matrix.h"

// What in the world is a quaternion?
//
// You may think of them as a sort of 3D version of complex numbers.  Instead
// of existing in a 2-space (ℂ, the complex plane) with a real axis and an
// imaginary axis, they exist in a four-dimensional space called ℍ with one
// real axis and three imaginary axes: 𝐢, 𝐣, and 𝐤.  These unit vectors have the
// unusual property 𝐢² = 𝐣² = 𝐤² = 𝐢𝐣𝐤 = -1.
//
// There are all sorts of things you can do with these, but we only use them
// to represent 3D rotations around vectors.  Suppose I have a basis with
// three axes (axisX, axisY, and axisZ), and I wish to rotate a point p by
// yaw° around axisY AND pitch° around axisX.  I can accomplish this as
// follows:
//
//   p -= Vector(basis.center);                              // Translate to origin
//   Quaternion q1 = rotationQuaternion(basis.axisY, yaw);   // Azimuth rotation
//   Quaternion q2 = rotationQuaternion(basis.axisX, pitch); // Elevation rotation
//   p = rotate(p, q2 * q1);                                 // Composite rotation: azimuth, THEN elevation
//   p += Vector(basis.center);                              // Translate from origin
//
// Should you wish to repeat the rotations across a large number of points,
// you can also extract matrices from the rotation quaternion:
//
//   Matrix toOrigin = translationMatrix(-Vector(basis.center));
//   Quaternion q1 = rotationQuaternion(basis.axisY, yaw);
//   Quaternion q2 = rotationQuaternion(basis.axisX, pitch);
//   Matrix fromOrigin = translationMatrix(Vector(basis.center));
//   Matrix composite = fromOrigin * rotationMatrix(q2 * q1) * toOrigin;
//   p = composite * p;
//
// Unlike Euler angles, these rotations are numerically stable, no matter how
// many are applied and no matter which order.  Quaternion rotations are not
// subject to gimbal lock.
struct Quaternion {

        double realPart;
        double iCoefficient;
        double jCoefficient;
        double kCoefficient;

        // Constructs the 0-quaternion.
        Quaternion();

        // Constructs the quaternion (a + bi + cj + dk).
        //
        // The real part of the quaternion is a, and behaves like a scalar.
        //
        // The imaginary part of the quaternion is bi + cj + dk, and it
        // behaves like a vector (b, c, d) in 3-space.  (This is important for
        // rotation.)
        Quaternion(double a, double b, double c, double d);

        // Scales the given quaternion by a constant.
        friend Quaternion operator*(Quaternion q, double f);
        friend Quaternion operator*(double f, Quaternion q);
        friend Quaternion operator/(Quaternion q, double f);
        friend Quaternion operator/(double f, Quaternion q);

        // Returns the Hamilton product p * q of of two quaternions.
        //
        // Warning: Quaternion multiplication is _not_ commutative; order matters!
        friend Quaternion operator*(Quaternion p, Quaternion q);

        // Returns the conjugate of the given quaternion.
        // If a quaternion is defined as a + bi + cj + dk, then its conjugate,
        // q*, is:
        //
        //   q* = a - bi - cj - dk
        //
        // Another way to express this solely in terms of q itself is:
        //
        //   q* = -(q + i*q*i + j*q*j + k*q*k)/2
        //
        // Other neat facts about conjugates:
        //
        // - The scalar (real) part of a quaternion q is (q + q*)/2.
        // - The vector (imaginary) part of a quaternion q is (q - q*)/2.
        Quaternion conjugate() const;

        // The norm of a quaternion q = a + bi + cj + dk, denoted ||q||, can
        // be defined as either:
        //
        // - sqrt(q * q.conjugate()); or
        // - sqrt(a^2 + b^2 + c^2 + d^2).
        //
        // The distance between two quaternions p and q is just ||p - q||.
        double norm() const;

        // Returns this quaternion divided by its norm -- that is, a
        // quaternion of norm 1.
        //
        // Note that for any given versor r, the quaternion
        //
        //   cos(a) + r * sin(a)
        //
        // where a is an angle, with 0 <= a <= π, represents a rotation by
        // the angle 2a around the axis vector represented by r's imaginary
        // part.
        //
        // When a versor's angle is π/2, the versor is a right versor, and has
        // no scalar part (meaning it represents a vector of length 1 in
        // 3-space.)
        //
        // A versor represents an arc on the unit sphere, and the product of
        // two versors represents a traversal of one arc followed by the
        // other.
        Quaternion versor() const;

        // Returns the multiplicative inverse of q.
        Quaternion inverse() const;

        // Returns the unit quaternion representation of a rotation, around
        // the origin, by thetaDeg degrees around the given axis (which must
        // not be a zero vector.)  The rotation is clockwise when viewed in
        // the direction of the axis.
        //
        // Multiplying two such quaternions together composes the rotations
        // _without_ inducing gimbal lock.  Like matrix multiplication, such
        // quaternions must be multiplied in the REVERSE order of the desired
        // rotation (i.e., rotating by q * p rotates by p, then q.)
        friend Quaternion rotationQuaternion(Vector axis, double thetaDeg);

        // Conjugates the quaternion with a real part of 0 and an imaginary
        // part of p by the given unit quaternion q.  In short, calculates
        // r*p*r⁻¹.
        //
        // This has the effect of rotating p around the rotation quaternion's
        // axis.
        //
        // The same effect can be achieved by calling rotationMatrix(r) * p,
        // but calling rotate() instead has the advantage of being faster
        // in the long run (fewer arithmetic operations).
        friend Point rotate(Point p, Quaternion r);

        // Same as the above, but for Vectors.
        friend Vector rotate(Vector v, Quaternion r);

        // Converts the given unit quaternion into a rotation matrix.
        friend Matrix rotationMatrix(Quaternion r);

        // Prints a quaternion.
        friend std::ostream& operator<<(std::ostream& out, Quaternion q);
};

// It may be a friend, but it still has to be declared extern separately.
extern Quaternion rotationQuaternion(Vector axis, double thetaDeg);


#endif // (#ifndef QUATERNION_H_INCLUDED)
