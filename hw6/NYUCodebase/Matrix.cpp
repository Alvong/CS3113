

#include "Matrix.h"
#include <math.h>
Matrix::Matrix()
{
    m[0][0] =  0.0;
    m[0][1] =  0.0;
    m[0][2] =  0.0;
    m[0][3] =  0.0;
    m[1][0] =  0.0;
    m[1][1] =  0.0;
    m[1][2] =  0.0;
    m[1][3] =  0.0;
    m[2][0] =  0.0;
    m[2][1] =  0.0;
    m[2][2] =  0.0;
    m[2][3] =  0.0;
    m[3][0] =  0.0;
    m[3][1] =  0.0;
    m[3][2] =  0.0;
    m[3][3] =  0.0;
   
}

void Matrix::identity()
{
    Matrix temp;
    temp.m[0][0] =  1.0;
    temp.m[1][1] =  1.0;
    temp.m[2][2] =  1.0;
    temp.m[3][3] =  1.0;
    
}
Matrix Matrix::translatef(float x, float y,float z)
{
    Matrix temp;
    temp.m[0][0] =  1.0;
    temp.m[1][1] =  1.0;
    temp.m[2][2] =  1.0;
    temp.m[3][3] =  1.0;
    temp.m[0][3] =  x;
    temp.m[1][3] =  y;
    temp.m[2][3] =  0;
    return temp;
    
}
Matrix Matrix::scale(float x,float y)
{
    Matrix temp;
    temp.m[0][0] =  x;
    temp.m[1][1] =  y;
    temp.m[2][2]=1.0;
    temp.m[3][3]=1.0;
    return temp;

}
Matrix Matrix::roate(float angle)
{
    Matrix temp;
    temp.m[0][0] =  cos(angle);
    temp.m[0][1] =  -sin(angle);
    temp.m[1][0]=sin(angle);
    temp.m[1][1]=cos(angle);
    temp.m[2][2]=1;
    temp.m[3][3]=1;
    return temp;

}
Vector Matrix::operator * (const Vector &v2)
{
    Vector temp;
    
    temp.x=v2.x*m[0][0]+v2.y*m[0][1]+v2.z*m[0][2]+v2.f*m[0][3];
    temp.y=v2.x*m[1][0]+v2.y*m[1][1]+v2.z*m[1][2]+v2.f*m[1][3];
    temp.z=v2.x*m[2][0]+v2.y*m[2][1]+v2.z*m[2][2]+v2.f*m[2][3];
    return temp;
}
Matrix Matrix::operator * (const Matrix &m2)
{
    Matrix temp;
    temp.m[0][0]=m2.m[0][0]*m[0][0]+m2.m[1][0]*m[0][1]+m2.m[2][0]*m[0][2]+m2.m[3][0]*m[0][3];
    
    temp.m[0][1]=m2.m[1][0]*m[0][0]+m2.m[1][1]*m[0][1]+m2.m[2][1]*m[0][2]+m2.m[3][1]*m[0][3];
    
    temp.m[0][2]=m2.m[0][2]*m[0][0]+m2.m[1][2]*m[0][1]+m2.m[2][2]*m[0][2]+m2.m[3][2]*m[0][3];
    
    temp.m[0][3]=m2.m[0][3]*m[0][0]+m2.m[1][3]*m[0][1]+m2.m[2][3]*m[0][2]+m2.m[3][3]*m[0][3];
    /////////////
    
    temp.m[1][0]=m2.m[0][0]*m[1][0]+m2.m[1][0]*m[1][1]+m2.m[2][0]*m[1][2]+m2.m[3][0]*m[1][3];
    
    temp.m[1][1]=m2.m[0][1]*m[1][0]+m2.m[1][1]*m[1][1]+m2.m[2][1]*m[1][2]+m2.m[3][1]*m[1][3];
    
    temp.m[1][2]=m2.m[0][2]*m[1][0]+m2.m[1][2]*m[1][1]+m2.m[2][2]*m[1][2]+m2.m[3][2]*m[1][3];
    
    temp.m[1][3]=m2.m[0][3]*m[1][0]+m2.m[1][3]*m[1][1]+m2.m[2][3]*m[1][2]+m2.m[3][3]*m[1][3];
    /////////////
    
    temp.m[2][0]=m2.m[0][0]*m[2][0]+m2.m[1][0]*m[2][1]+m2.m[2][2]*m[2][0]+m2.m[2][3]*m[3][0];
    
    temp.m[2][1]=m2.m[0][1]*m[2][0]+m2.m[1][1]*m[2][1]+m2.m[2][2]*m[2][1]+m2.m[2][3]*m[3][1];
    
    temp.m[2][2]=m2.m[0][2]*m[2][0]+m2.m[1][2]*m[2][1]+m2.m[2][2]*m[2][2]+m2.m[3][2]*m[2][3];
    
    temp.m[2][3]=m2.m[0][3]*m[2][0]+m2.m[1][3]*m[2][1]+m2.m[2][3]*m[2][2]+m2.m[3][3]*m[2][3];
    //////////////
    
    temp.m[3][0]=m2.m[0][0]*m[3][0]+m2.m[1][0]*m[3][1]+m2.m[2][0]*m[3][2]+m2.m[3][0]*m[3][3];
    
    temp.m[3][1]=m2.m[0][1]*m[3][0]+m2.m[1][1]*m[3][1]+m2.m[3][2]*m[2][1]+m2.m[3][3]*m[3][1];
    
    temp.m[3][2]=m2.m[0][2]*m[3][0]+m2.m[1][2]*m[3][1]+m2.m[2][2]*m[3][2]+m2.m[3][2]*m[3][3];
    
    temp.m[3][3]=m2.m[0][3]*m[3][0]+m2.m[1][3]*m[3][1]+m2.m[2][3]*m[3][2]+m2.m[3][3]*m[3][3];
    return temp;
}