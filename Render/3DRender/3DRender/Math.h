#ifndef MATH_H_
#define MATH_H_
#include <iostream>
#include <cmath>
#include "Vector3.h"

#define PI 3.141592654
using namespace std;

inline float CMID(float x, float min, float max) 
{ 
	return (x < min)? min : ((x > max)? max : x); 
}

class AColor
{
public:
	AColor(int a,int r,int g,int b):a_(a),r_(r),g_(g),b_(b){};
	AColor():a_(0),r_(0),g_(0),b_(0){};
	void normalize()
	{
		a_ = CMID(a_,0,255),
		r_ = CMID(r_,0,255),
		g_ = CMID(g_,0,255),
		b_ = CMID(b_,0,255);
	}

	int a_;
	int r_;
	int g_;
	int b_;
};

AColor operator*(const AColor &color,const Vector3 &v);
AColor operator*(const Vector3 &v,const AColor &color);


float Sin(float degree);
float Cos(float degree);
float Tan(float degree);
float Cot(float degree);

#endif