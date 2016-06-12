#include "Matrix.h"

void Matrix::identify()
{
	m11=1 , m12=0 , m13=0 ;
	m21=0 , m22=1 , m23=0 ;
	m31=0 , m32=0 , m33=1 ;
	tx=0  , ty=0  , tz=0  ;
}

void Matrix::setTranslation(const Vector3& d)
{
	tx = d.x_ , ty = d.y_ , tz = d.z_;
}

void Matrix::zeroTranslation()
{
	tx = ty = tz =0;
}

void Matrix::setRotate(int axis,float theta)
{
	float s = Sin(theta);
	float c = Cos(theta);

	switch(axis)
	{
	case 1: //X-axis
		m11=1 , m12=0 , m13=0 ;
		m21=0 , m22=c , m23=s ;
		m31=0 , m32=-s , m33=c ;
		break;

	case 2: //Y-axis
		m11=c , m12=0 , m13=-s ;
		m21=0 , m22=1 , m23=0 ;
		m31=s , m32=0 , m33=c ;
		break;

	case 3: //Z-axis
		m11=c , m12=s , m13=0 ;
		m21=-s , m22=c , m23=0 ;
		m31=0 , m32=0 , m33=1 ;
		break;

	default:
		assert(false);
	}
	tx = ty = tz =0;
}

void Matrix::setRotate(const Vector3 &axis, float theta) {

	// Quick sanity check to make sure they passed in a unit vector
	// to specify the axis
	assert(fabs(axis*axis - 1.0f) < .01f);

	// Get sin and cosine of rotation angle

	float	s =Sin(theta), c=Cos(theta);

	// Compute 1 - cos(theta) and some common subexpressions

	float	a = 1.0f - c;
	float	ax = a * axis.x_;
	float	ay = a * axis.y_;
	float	az = a * axis.z_;

	// Set the matrix elements.  There is still a little more
	// opportunity for optimization due to the many common
	// subexpressions.  We'll let the compiler handle that...

	m11 = ax*axis.x_ + c;
	m12 = ax*axis.y_ + axis.z_*s;
	m13 = ax*axis.z_ - axis.y_*s;

	m21 = ay*axis.x_ - axis.z_*s;
	m22 = ay*axis.y_ + c;
	m23 = ay*axis.z_ + axis.x_*s;

	m31 = az*axis.x_ + axis.y_*s;
	m32 = az*axis.y_ - axis.x_*s;
	m33 = az*axis.z_ + c;

	// Reset the translation portion

	tx = ty = tz = 0.0f;
}

void Matrix::setScale(const Vector3 &s)
{
	m11=s.x_ , m12=0 , m13=0 ;
	m21=0 , m22=s.y_ , m23=0 ;
	m31=0 , m32=0 , m33=s.z_ ;
	tx=0  , ty=0  , tz=0  ;
}

Vector3	operator*(const Vector3 &p, const Matrix &m) {

	// Grind through the linear algebra.

	return Vector3(
		p.x_*m.m11 + p.y_*m.m21 + p.z_*m.m31 + m.tx,
		p.x_*m.m12 + p.y_*m.m22 + p.z_*m.m32 + m.ty,
		p.x_*m.m13 + p.y_*m.m23 + p.z_*m.m33 + m.tz
		);
}

Vector3 &operator*=(Vector3 &p, const Matrix &m) {
	p = p * m;
	return p;
}

Matrix operator*(const Matrix &a, const Matrix &b) {

	Matrix r;

	// Compute the upper 3x3 (linear transformation) portion

	r.m11 = a.m11*b.m11 + a.m12*b.m21 + a.m13*b.m31;
	r.m12 = a.m11*b.m12 + a.m12*b.m22 + a.m13*b.m32;
	r.m13 = a.m11*b.m13 + a.m12*b.m23 + a.m13*b.m33;

	r.m21 = a.m21*b.m11 + a.m22*b.m21 + a.m23*b.m31;
	r.m22 = a.m21*b.m12 + a.m22*b.m22 + a.m23*b.m32;
	r.m23 = a.m21*b.m13 + a.m22*b.m23 + a.m23*b.m33;

	r.m31 = a.m31*b.m11 + a.m32*b.m21 + a.m33*b.m31;
	r.m32 = a.m31*b.m12 + a.m32*b.m22 + a.m33*b.m32;
	r.m33 = a.m31*b.m13 + a.m32*b.m23 + a.m33*b.m33;

	// Compute the translation portion

	r.tx = a.tx*b.m11 + a.ty*b.m21 + a.tz*b.m31 + b.tx;
	r.ty = a.tx*b.m12 + a.ty*b.m22 + a.tz*b.m32 + b.ty;
	r.tz = a.tx*b.m13 + a.ty*b.m23 + a.tz*b.m33 + b.tz;

	// Return it.  Ouch - involves a copy constructor call.  If speed
	// is critical, we may need a seperate function which places the
	// result where we want it...

	return r;
}

Matrix &operator*=(Matrix &a, const Matrix &b) {
	a = a * b;
	return a;
}