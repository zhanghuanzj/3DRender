#ifndef RENDER_VCOLOR_H_
#define RENDER_VCOLOR_H_

#include "Vector3.h"

struct VColor
{
	VColor(float a_, float r_, float g_, float b_):a(a_), r(r_), g(g_), b(b_){}
	VColor(const VColor &color) :a(color.a), r(color.r), g(color.g), b(color.b) {}
	VColor():a(1.0f), r(0.0f), g(0.0f), b(0.0f){}
	inline VColor interp(const VColor &c, float factor) const
	{
		float na = a + (c.a-a) * factor;
		float nr = r + (c.r-r) * factor;
		float ng = g + (c.g-g) * factor;
		float nb = b + (c.b-b) * factor;
		return VColor(na, nr, ng, nb);
	}

	inline VColor& operator+=(const VColor &v)
	{
		a += v.a;
		r += v.r;
		g += v.g;
		b += v.b;
		return  * this;
	}

	inline VColor& operator-=(const VColor &v)
	{
		a -= v.a;
		r -= v.r;
		g -= v.g;
		b -= v.b;
		return  * this;
	}

	inline VColor& operator=(const VColor &v)
	{
		a = v.a;
		r = v.r;
		g = v.g;
		b = v.b;
		return  *this;
	}

	float a, r, g, b;
};

inline VColor operator / (const VColor &color, float v)
{
	return VColor(
		color.a  /  v, 
		color.r  /  v, 
		color.g  /  v, 
		color.b  /  v);
}

inline VColor operator * (const VColor &color, float v)
{
	return VColor(
		color.a  *  v, 
		color.r  *  v, 
		color.g  *  v, 
		color.b  *  v);
}

inline VColor operator * (float v, const VColor &color)
{
	return VColor(
		color.a  *  v, 
		color.r  *  v, 
		color.g  *  v, 
		color.b  *  v);
}

inline VColor operator * (const VColor &color1, const VColor &color2)
{
	return VColor(
		color1.a  *  color2.a, 
		color1.r  *  color2.r, 
		color1.g  *  color2.g, 
		color1.b  *  color2.b);
}

inline VColor operator+(const VColor &color1, const VColor &color2)
{
	return VColor(
		color1.a + color2.a, 
		color1.r + color2.r, 
		color1.g + color2.g, 
		color1.b + color2.b);
}

inline VColor operator-(const VColor &color1, const VColor &color2)
{
	return VColor(
		color1.a - color2.a, 
		color1.r - color2.r, 
		color1.g - color2.g, 
		color1.b - color2.b);
}

inline VColor operator * (const VColor &color, const Vector3 &v)
{
	return VColor(
		color.a  *  1, 
		color.r  *  v.x, 
		color.g  *  v.y, 
		color.b  *  v.z);
}

inline VColor operator * (const Vector3 &v, const VColor &color)
{
	return VColor(
		color.a  *  1, 
		color.r  *  v.x, 
		color.g  *  v.y, 
		color.b  *  v.z);
}

#endif