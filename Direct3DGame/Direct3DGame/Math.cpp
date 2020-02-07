#include "Math.h"
#include <iostream>
#include <algorithm>

using namespace std;

float Sin(float degree)
{
	return sin(degree * PI / 180);
}

float Cos(float degree)
{
	return cos(degree * PI / 180);
}

float Tan(float degree)
{
	return tan(degree * PI / 180);
}

AColor operator*(const AColor &color, const Vector3 &v)
{
	return AColor(color.a_ * 1,
		min(255, int(color.r_ * v.x_)),
		min(255, int(color.g_ * v.y_)),
		min(255, int(color.b_ * v.z_)));
}

AColor operator*(const Vector3 &v, const AColor &color)
{
	return AColor(color.a_ * 1,
		min(255, int(color.r_ * v.x_)),
		min(255, int(color.g_ * v.y_)),
		min(255, int(color.b_ * v.z_)));
}