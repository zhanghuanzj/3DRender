#ifndef MATH_H_
#define MATH_H_
#include <iostream>
#include <cmath>

#define PI 3.141592654
using namespace std;


inline float Sin(float degree)
{
	return sin(degree * PI / 180.0f);
}

inline float Cos(float degree)
{
	return cos(degree * PI / 180.0f);
}

inline float Tan(float degree)
{
	return tan(degree * PI / 180.0f);
}

inline float Cot(float degree)
{
	return 1.0f / tan(degree * PI / 180.0f);
}

inline float Lerp(float v1, float v2, float factor)
{
	return v1 * (1 - factor) + factor * v2;
}

inline float Sqrt(float x)
{
	float xhalf = 0.5f*x;
	int i = *(int*)&x; // get bits for floating VALUE 
	i = 0x5f3759df - (i >> 1); // gives initial guess y0
	x = *(float*)&i; // convert bits BACK to float
	x = x * (1.5f - xhalf * x*x); // Newton step, repeating increases accuracy
	x = x * (1.5f - xhalf * x*x); // Newton step, repeating increases accuracy
	x = x * (1.5f - xhalf * x*x); // Newton step, repeating increases accuracy

	return 1 / x;
}

inline float Clamp(float x, float min, float max)
{
	return (x < min) ? min : ((x > max) ? max : x);
}


#endif