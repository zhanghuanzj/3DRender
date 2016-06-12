#ifndef MATH_H_
#define MATH_H_
#include <math.h>

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
	Vector2(double nx,double ny,Color nc):x(nx),y(ny),color(nc){}
	bool operator==(const Vector2 &v)const{return x==v.x&&y==v.y;}

	Color color;
	double x;
	double y;
};

float Sin(float degree);
float Cos(float degree);
float Tan(float defree);

#endif