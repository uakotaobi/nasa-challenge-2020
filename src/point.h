#ifndef POINT_H_INCLUDED
#define POINT_H_INCLUDED

struct Point {
    double x;
    double y;
    double z;

    Point();
    Point(double x_, double y_, double z_);

    friend double distance(Point a, Point b);
};






#endif // POINT_H_INCLUDED
