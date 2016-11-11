#include "Math.h"

float Sin(float degree)
{
	return sin(degree*PI/180.0f);
}

float Cos(float degree)
{
	return cos(degree*PI/180.0f);
}

float Tan(float degree)
{
	return tan(degree*PI/180.0f);
}

float Cot(float degree)
{
	return 1.0f/tan(degree*PI/180.0f);
}

AColor operator*(const AColor &color,const Vector3 &v)
{
	return AColor(color.a_ * 1,
		min(255,int(color.r_ * v.x)),
		min(255,int(color.g_ * v.y)),
		min(255,int(color.b_ * v.z)));
}

AColor operator*(const Vector3 &v,const AColor &color)
{
	return AColor(color.a_ * 1,
		min(255,int(color.r_ * v.x)),
		min(255,int(color.g_ * v.y)),
		min(255,int(color.b_ * v.z)));
}