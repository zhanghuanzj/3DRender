#ifndef MATH_H_
#define MATH_H_
#include "Vector3.h"
#define PI 3.141592654

class AColor
{
public:
	AColor(int a,int r,int g,int b):a_(a),r_(r),g_(g),b_(b){};
	AColor interp(const AColor &c,float factor)
	{
		int a = a_ + (c.a_-a_)*factor;
		int r = r_ + (c.r_-r_)*factor;
		int g = g_ + (c.g_-g_)*factor;
		int b = b_ + (c.b_-b_)*factor;
		return AColor(a,r,g,b);
	}
	int a_;
	int r_;
	int g_;
	int b_;
};

inline AColor operator*(const AColor &color,double v)
{
	return AColor(color.a_ * v,
				 color.r_ * v,
				 color.g_ * v,
				 color.b_ * v);
}

inline AColor operator*(double v,const AColor &color)
{
	return AColor(color.a_ * v,
				 color.r_ * v,
				 color.g_ * v,
				 color.b_ * v);
}

inline AColor operator/(const AColor &color,double v)
{
	return AColor(color.a_ / v,
				 color.r_ / v,
				 color.g_ / v,
				 color.b_ / v);
}

inline AColor operator/(double v,const AColor &color)
{
	return AColor(color.a_ / v,
				 color.r_ / v,
				 color.g_ / v,
				 color.b_ / v);
}

inline AColor operator+(const AColor &color1,const AColor &color2)
{
	return AColor(color1.a_ + color2.a_,
				 color1.r_ + color2.r_,
				 color1.g_ + color2.g_,
				 color1.b_ + color2.b_);
}

inline AColor operator-(const AColor &color1,const AColor &color2)
{
	return AColor(color1.a_ - color2.a_,
				 color1.r_ - color2.r_,
				 color1.g_ - color2.g_,
				 color1.b_ - color2.b_);
}

AColor operator*(const AColor &color,const Vector3 &v);
AColor operator*(const Vector3 &v,const AColor &color);


class Vector2
{
public:
	Vector2(float nx,float ny,AColor nc):x(nx),y(ny),color(nc){}
	bool operator==(const Vector2 &v)const{return x==v.x&&y==v.y;}
	Vector2 operator-(const Vector2 &v)const{return Vector2(x-v.x,y-v.y,color-v.color);}
	Vector2 operator+(const Vector2 &v)const{return Vector2(x+v.x,y+v.y,color+v.color);}
	AColor color;
	float x;
	float y;
};
inline Vector2 operator*(const Vector2 &v,float f)
{
	return Vector2(v.x*f,v.y*f,v.color);
}

inline Vector2 operator*(float f,const Vector2 &v)
{
	return Vector2(v.x*f,v.y*f,v.color);
}

float Sin(float degree);
float Cos(float degree);
float Tan(float defree);

#endif