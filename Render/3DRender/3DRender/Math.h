#ifndef MATH_H_
#define MATH_H_
#include <iostream>
#include <cmath>
#include "Vector3.h"

#define PI 3.141592654
using namespace std;

class AColor
{
public:
	AColor(int a,int r,int g,int b):a_(a),r_(r),g_(g),b_(b){};
	AColor():a_(0),r_(0),g_(0),b_(0){};

	int a_;
	int r_;
	int g_;
	int b_;
};




float Sin(float degree);
float Cos(float degree);
float Tan(float degree);
float Cot(float degree);

#endif