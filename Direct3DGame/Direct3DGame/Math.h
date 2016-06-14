#ifndef MATH_H_
#define MATH_H_
#include <cmath>

#define PI 3.141592654

class Color
{
public:
	Color(int a,int r,int g,int b):a_(a),r_(r),g_(g),b_(b){};

	int a_;
	int r_;
	int g_;
	int b_;
};

inline Color operator*(const Color &color,double v)
{
	return Color(color.a_ * v,
				 color.r_ * v,
				 color.g_ * v,
				 color.b_ * v);
}

inline Color operator*(double v,const Color &color)
{
	return Color(color.a_ * v,
				 color.r_ * v,
				 color.g_ * v,
				 color.b_ * v);
}

inline Color operator/(const Color &color,double v)
{
	return Color(color.a_ / v,
				 color.r_ / v,
				 color.g_ / v,
				 color.b_ / v);
}

inline Color operator/(double v,const Color &color)
{
	return Color(color.a_ / v,
				 color.r_ / v,
				 color.g_ / v,
				 color.b_ / v);
}

inline Color operator+(const Color &color1,const Color &color2)
{
	return Color(color1.a_ + color2.a_,
				 color1.r_ + color2.r_,
				 color1.g_ + color2.g_,
				 color1.b_ + color2.b_);
}

inline Color operator-(const Color &color1,const Color &color2)
{
	return Color(color1.a_ - color2.a_,
				 color1.r_ - color2.r_,
				 color1.g_ - color2.g_,
				 color1.b_ - color2.b_);
}


class Vector2
{
public:
	Vector2(float nx,float ny,Color nc):x(nx),y(ny),color(nc){}
	bool operator==(const Vector2 &v)const{return x==v.x&&y==v.y;}
	Vector2 operator-(const Vector2 &v)const{return Vector2(x-v.x,y-v.y,color-v.color);}
	Vector2 operator+(const Vector2 &v)const{return Vector2(x+v.x,y+v.y,color+v.color);}
	Color color;
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