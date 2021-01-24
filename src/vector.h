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
    friend Vector operator*(Vector v, double f);
    friend Vector operator*(double f, Vector v);
    friend Vector operator/(Vector v, double f);
    friend std::ostream& operator<<(std::ostream&, Vector v);
    friend double dotProduct(Vector v1, Vector v2);
    friend Vector crossProduct(Vector v1, Vector v2);
    friend Vector normalize(Vector v);
};

#endif
