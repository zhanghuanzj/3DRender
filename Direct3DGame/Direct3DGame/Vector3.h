#ifndef VECTOR3_H_
#define VECTOR3_H_
#include <cmath>
class Vector3
{
public:
	Vector3(float x,float y,float z):x_(x),y_(y),z_(z){}
	Vector3():x_(),y_(),z_(){}

	float length(){ return sqrt(x_*x_ +y_*y_+z_*z_);}
	void normalize();
	Vector3 operator+(const Vector3& v){ return Vector3(x_+v.x_ , y_+v.y_ , z_+v.z_);}

	float x_;
	float y_;
	float z_;
};

inline float operator*(const Vector3 &v,const Vector3 &v1)
{ 
	return v1.x_*v.x_+ v1.y_*v.y_ + v1.z_*v.z_  ;
}

inline Vector3 operator*(float f,const Vector3 &v)
{ 
	return Vector3(f*v.x_ , f*v.y_ , f*v.z_); 
}

inline Vector3 operator*(const Vector3 &v,float f)
{ 
	return Vector3(f*v.x_ , f*v.y_ , f*v.z_); 
}


#endif