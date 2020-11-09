#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED 

struct Vector {
	double x;
	double y;
	double z;
	
	Vector();
	Vector(double x_, double y_, double z_);
	
	double magnitude() const;
};
	
#endif