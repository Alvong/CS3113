
#include <stdio.h>

#ifndef Vector_h
#define Vector_h
class Vector {
public:
    Vector();
    Vector(float x, float y, float z,float f);
    float length();
    void normalize();
    float x;
    float y;
    float z;
    float f;
};


#endif /* defined(__NYUCodebase__Vector__) */
