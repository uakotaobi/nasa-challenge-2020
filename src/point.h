#ifndef POINT_H_INCLUDED
#define POINT_H_INCLUDED

// Forward declaration
class Vector;

struct Point {
    double x;
    double y;
    double z;

    Point();
    Point(double x_, double y_, double z_);

    friend double distance(Point a, Point b);

    friend Vector operator-(Point p1, Point p2);

    
};

#endif // POINT_H_INCLUDED
