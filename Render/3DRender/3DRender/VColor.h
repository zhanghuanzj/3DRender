#ifndef RENDER_VCOLOR_H_
#define RENDER_VCOLOR_H_

#include "Math.h"
struct VColor
{
	VColor(float a_,float r_,float g_,float b_):a(a_),r(r_),g(g_),b(b_){}
	VColor():a(0),r(0),g(0),b(0){}
	VColor interp(const VColor &c,float factor)
	{
		float na = a + (c.a-a)*factor;
		float nr = r + (c.r-r)*factor;
		float ng = g + (c.g-g)*factor;
		float nb = b + (c.b-b)*factor;
		return VColor(na,nr,ng,nb);
	}
	VColor& operator+=(const VColor &v)
	{
		a += v.a;
		r += v.r;
		g += v.g;
		b += v.b;
		return *this;
	}
	VColor& operator-=(const VColor &v)
	{
		a -= v.a;
		r -= v.r;
		g -= v.g;
		b -= v.b;
		return *this;
	}
	AColor to_AColor()
	{
		return AColor(a*255,r*255,g*255,b*255);
	}
	float a,r,g,b;
};

inline VColor operator/(const VColor &color,float v)
{
	return VColor(
		color.a / v,
		color.r / v,
		color.g / v,
		color.b / v);
}

inline VColor operator*(const VColor &color,float v)
{
	return VColor(
		color.a * v,
		color.r * v,
		color.g * v,
		color.b * v);
}

inline VColor operator*(float v,const VColor &color)
{
	return VColor(
		color.a * v,
		color.r * v,
		color.g * v,
		color.b * v);
}

inline VColor operator*(const VColor &color1,const VColor &color2)
{
	return VColor(
		color1.a * color2.a,
		color1.r * color2.r,
		color1.g * color2.g,
		color1.b * color2.b);
}



inline VColor operator+(const VColor &color1,const VColor &color2)
{
	return VColor(
		color1.a + color2.a,
		color1.r + color2.r,
		color1.g + color2.g,
		color1.b + color2.b);
}

inline VColor operator-(const VColor &color1,const VColor &color2)
{
	return VColor(
		color1.a - color2.a,
		color1.r - color2.r,
		color1.g - color2.g,
		color1.b - color2.b);
}

inline VColor operator*(const VColor &color,const Vector3 &v)
{
	return VColor(
		color.a * 1,
		color.r * v.x,
		color.g * v.y,
		color.b * v.z);
}

inline VColor operator*(const Vector3 &v,const VColor &color)
{
	return VColor(
		color.a * 1,
		color.r * v.x,
		color.g * v.y,
		color.b * v.z);
}

#endif