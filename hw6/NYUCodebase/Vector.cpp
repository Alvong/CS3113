

#include "Vector.h"
#include <cmath>
Vector::Vector():x(0),y(0),z(0),f(1){};
Vector::Vector(float x, float y, float z,float f=1):x(x),y(y),z(0),f(f){};
float Vector::length()
{
    return sqrt(x*x+y*y+z*z);
}
void Vector::normalize()
{
    float length=Vector::length();
    x=x/length;
    y=y/length;
    z=z/length;
    
}
