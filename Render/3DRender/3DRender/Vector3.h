#ifndef VECTOR3H
#define VECTOR3H

#include "Math.h"

class Vector3
{
public:
	Vector3(float x_, float y_, float z_, float w_=1):x(x_), y(y_), z(z_), w(w_){}
	Vector3():x(), y(), z(), w(1){}
	Vector3(const Vector3& v):x(v.x), y(v.y), z(v.z), w(v.w){}

	inline float length(){ return Sqrt(x*x + y*y + z*z);}
	inline void normalize()
	{
		float factor = 0;
		float magSq = x * x + y * y + z * z;
		if (magSq > 0) {
			factor = 1.0f / sqrt(magSq);
			x *= factor;
			y *= factor;
			z *= factor;
		}
	}

	inline Vector3 operator+(const Vector3 &v)const { return Vector3(x+v.x ,  y+v.y ,  z+v.z);}
	inline Vector3 operator-(const Vector3 &v)const { return Vector3(x-v.x ,  y-v.y ,  z-v.z);}
	inline Vector3& operator+=(const Vector3 &v)
	{ 
		x += v.x;
		y += v.y;
		z += v.z;
		return  * this;
	}
	inline Vector3& operator-=(const Vector3 &v)
	{ 
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return  * this;
	}
	inline Vector3 interp(const Vector3 &v, float factor)
	{
		float x_ = x + (v.x - x) * factor;
		float y_ = y + (v.y - y) * factor;
		float z_ = z + (v.z - z) * factor;
		float w_ = w + (v.w - w) * factor;
		return Vector3(x_, y_, z_, w_);
	}

	float x;
	float y;
	float z;
	float w;
};

inline float operator * (const Vector3 &v, const Vector3 &v1)
{ 
	return v1.x * v.x+ v1.y * v.y + v1.z * v.z  ;
}

inline Vector3 operator * (float f, const Vector3 &v)
{ 
	return Vector3(f * v.x ,  f * v.y ,  f * v.z); 
}

inline Vector3 operator * (const Vector3 &v, float f)
{ 
	return Vector3(f * v.x ,  f * v.y ,  f * v.z); 
}

inline Vector3 operator / (const Vector3 &v, float f)
{
	return v * (1 / f);
}

inline Vector3 crossproduct(const Vector3 &v1, const Vector3 &v2)
{
	return Vector3(v1.y * v2.z - v1.z * v2.y,  v1.z * v2.x - v1.x * v2.z ,  v1.x * v2.y-v1.y * v2.x);
}


#endif