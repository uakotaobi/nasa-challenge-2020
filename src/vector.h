#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

#include "point.h"
#include <ostream>

struct Vector {
	double x;
	double y;
	double z;

	Vector();
	Vector(double x_, double y_, double z_);
    Vector(Point p);

	double magnitude() const;

    Vector& operator*=(double f);
    Vector& operator+=(Vector v);
    Vector& operator-=(Vector v);

    friend Point operator+(Point p, Vector v);
    friend Point operator+(Vector v, Point p);
    friend Vector operator+(Vector v1, Vector v2);
    friend Vector operator-(Vector v);
    friend Point operator-(Point p, Vector v);
    friend Vector operator-(Vector v1, Vector v2);
    friend Vector operator*(Vector v, double f);
    friend Vector operator*(double f, Vector v);
    friend Vector operator/(Vector v, double f);
    friend std::ostream& operator<<(std::ostream&, Vector v);
    friend double dotProduct(Vector v1, Vector v2);
    // Returns a vector that is perpendicular to the given two vectors and has a
    // length of a.magnitude() * b.magnitude() * sin(angle between a & b)
    friend Vector crossProduct(Vector v1, Vector v2);
    // Nominally, its crossProduct(p2-p1, p3-p1), but if the length is zero, it does crossProduct(p1-p2, p3-p2) instead.
    // The surface will always be non-zero unless the triangle is degenerate.
    friend Vector surfaceNormal(Point p1, Point p2, Point p3);
    friend Vector normalize(Vector v);
};

extern Vector surfaceNormal(Point p1, Point p2, Point p3);

#endif
