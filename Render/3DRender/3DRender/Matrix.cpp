#include "Matrix.h"

void Matrix::identify()
{
	m11=1 ,  m12=0 ,  m13=0, m14=0;
	m21=0 ,  m22=1 ,  m23=0, m24 = 0;
	m31=0 ,  m32=0 ,  m33=1, m34 = 0;
	tx=0  ,  ty=0  ,  tz=0, m44 = 1;
}

void Matrix::setTranslation(const Vector3& d)
{
	identify();
	tx = d.x ,  ty = d.y ,  tz = d.z;
}

void Matrix::zeroTranslation()
{
	identify();
	tx = ty = tz =0;
}

void Matrix::setRotate(int axis, float theta)
{
	identify();
	float s = Sin(theta);
	float c = Cos(theta);

	switch(axis)
	{
	case 1: //X-axis
		m11=1 ,  m12=0 ,  m13=0 ;
		m21=0 ,  m22=c ,  m23=s ;
		m31=0 ,  m32=-s ,  m33=c ;
		break;

	case 2: //Y-axis
		m11=c ,  m12=0 ,  m13=-s ;
		m21=0 ,  m22=1 ,  m23=0 ;
		m31=s ,  m32=0 ,  m33=c ;
		break;

	case 3: //Z-axis
		m11=c ,  m12=s ,  m13=0 ;
		m21=-s ,  m22=c ,  m23=0 ;
		m31=0 ,  m32=0 ,  m33=1 ;
		break;

	default:
		assert(false);
	}
	tx = ty = tz =0;
}

void Matrix::setRotate(const Vector3 &axis,  float theta) {

	identify();
	// Quick sanity check to make sure they passed in a unit vector
	// to specify the axis
	assert(fabs(axis*axis - 1.0f) < .01f);

	// Get sin and cosine of rotation angle

	float	s =Sin(theta),  c=Cos(theta);

	// Compute 1 - cos(theta) and some common subexpressions

	float	a = 1.0f - c;
	float	ax = a * axis.x;
	float	ay = a * axis.y;
	float	az = a * axis.z;

	// Set the matrix elements.  There is still a little more
	// opportunity for optimization due to the many common
	// subexpressions.  We'll let the compiler handle that...

	m11 = ax*axis.x + c;
	m12 = ax*axis.y + axis.z*s;
	m13 = ax*axis.z - axis.y*s;

	m21 = ay*axis.x - axis.z*s;
	m22 = ay*axis.y + c;
	m23 = ay*axis.z + axis.x*s;

	m31 = az*axis.x + axis.y*s;
	m32 = az*axis.y - axis.x*s;
	m33 = az*axis.z + c;

	// Reset the translation portion

	tx = ty = tz = 0.0f;
}

void Matrix::setScale(const Vector3 &s)
{
	m11 = s.x, m12 = 0, m13 = 0, m14 = 0;
	m21 = 0, m22 = s.y, m23 = 0, m24 = 0;
	m31 = 0, m32 = 0, m33 = s.z, m34 = 0;
	tx = 0, ty = 0, tz = 0, m44 = 1;
}

Matrix Matrix::transpose()
{
	Matrix t;
	t.m11 = m11, t.m12 = m21, t.m13 = m31, t.m14 = tx;
	t.m21 = m12, t.m22 = m22, t.m23 = m32, t.m24 = ty;
	t.m31 = m13, t.m32 = m23, t.m33 = m33, t.m34 = tz;
	t.tx = m14,  t.ty = m24,  t.tz = m34,  t.m44 = m44;
	return t;
}

float sub_determinant(float m11, float m12, float m13, float m21, float m22, float m23, float m31, float m32, float m33)
{
	return m11 * (m22*m33 - m32 * m23) - m12 * (m21*m33 - m31 * m23) + m13 * (m21*m32 - m31*m22);
}

// 行列式等于行或列的元素乘以对应的代数余子式（余子式的行列式*代数）之和
float Matrix::determinator()
{
	return m11 * sub_determinant(m22, m23, m24, m32, m33, m34, ty, tz, m44)
		 - m12 * sub_determinant(m21, m23, m24, m31, m33, m34, tx, tz, m44)
		 + m13 * sub_determinant(m21, m22, m24, m31, m32, m34, tx, ty, m44)
		 - m14 * sub_determinant(m21, m22, m23, m31, m32, m33, tx, ty, tz);
}

// 矩阵的逆等于伴随矩阵（代数余子式矩阵[(-1)^(i+j)*子行列式]的转置）除以行列式
Matrix Matrix::inverse()
{
	float det = determinator();
	assert(det > 0.000001f);
	float one_over_determinator = 1.0f / det;
	Matrix matrix;
	matrix.m11 = sub_determinant(m22, m23, m24, m32, m33, m34, ty, tz, m44);
	matrix.m12 = -sub_determinant(m21, m23, m24, m31, m33, m34, tx, tz, m44);
	matrix.m13 = sub_determinant(m21, m22, m24, m31, m32, m34, tx, ty, m44);
	matrix.m14 = -sub_determinant(m21, m22, m23, m31, m32, m33, tx, ty, tz);

	matrix.m21 = -sub_determinant(m12, m13, m14, m32, m33, m34, ty, tz, m44);
	matrix.m22 = sub_determinant(m11, m13, m14, m31, m33, m34, tx, tz, m44);
	matrix.m23 = -sub_determinant(m11, m12, m14, m31, m32, m34, tx, ty, m44);
	matrix.m24 = sub_determinant(m11, m12, m13, m31, m32, m33, tx, ty, tz);

	matrix.m31 = sub_determinant(m12, m13, m14, m22, m23, m24, ty, tz, m44);
	matrix.m32 = -sub_determinant(m11, m13, m14, m21, m23, m24, tx, tz, m44);
	matrix.m33 = sub_determinant(m11, m12, m14, m21, m22, m24, tx, ty, m44);
	matrix.m34 = -sub_determinant(m11, m12, m13, m21, m22, m23, tx, ty, tz);

	matrix.tx = -sub_determinant(m12, m13, m14, m22, m23, m24, m32, m33, m34);
	matrix.ty = sub_determinant(m11, m13, m14, m21, m23, m24, m31, m33, m34);
	matrix.tz = -sub_determinant(m11, m12, m14, m21, m22, m24, m31, m32, m34);
	matrix.m44 = sub_determinant(m11, m12, m13, m21, m22, m23, m31, m32, m33);
	
	return matrix.transpose() * one_over_determinator;
}


Vector3	operator*(const Vector3 &p,  const Matrix &m) {

	// Grind through the linear algebra.

	return Vector3(
		p.x*m.m11 + p.y*m.m21 + p.z*m.m31 + p.w*m.tx, 
		p.x*m.m12 + p.y*m.m22 + p.z*m.m32 + p.w*m.ty, 
		p.x*m.m13 + p.y*m.m23 + p.z*m.m33 + p.w*m.tz, 
		p.x*m.m14 + p.y*m.m24 + p.z*m.m34 + p.w*m.m44
		);
}

Vector3 &operator*=(Vector3 &p,  const Matrix &m) {
	p = p * m;
	return p;
}

Matrix operator*(const Matrix &a,  const Matrix &b) {

	Matrix r;

	// Compute the upper 3x3 (linear transformation) portion

	r.m11 = a.m11*b.m11 + a.m12*b.m21 + a.m13*b.m31 + a.m14*b.tx;
	r.m12 = a.m11*b.m12 + a.m12*b.m22 + a.m13*b.m32 + a.m14*b.ty;
	r.m13 = a.m11*b.m13 + a.m12*b.m23 + a.m13*b.m33 + a.m14*b.tz;
	r.m14 = a.m11*b.m14 + a.m12*b.m24 + a.m13*b.m34 + a.m14*b.m44;

	r.m21 = a.m21*b.m11 + a.m22*b.m21 + a.m23*b.m31 + a.m24*b.tx;
	r.m22 = a.m21*b.m12 + a.m22*b.m22 + a.m23*b.m32 + a.m24*b.ty;
	r.m23 = a.m21*b.m13 + a.m22*b.m23 + a.m23*b.m33 + a.m24*b.tz;
	r.m24 = a.m21*b.m14 + a.m22*b.m24 + a.m23*b.m34 + a.m24*b.m44;

	r.m31 = a.m31*b.m11 + a.m32*b.m21 + a.m33*b.m31 + a.m34*b.tx;
	r.m32 = a.m31*b.m12 + a.m32*b.m22 + a.m33*b.m32 + a.m34*b.ty;
	r.m33 = a.m31*b.m13 + a.m32*b.m23 + a.m33*b.m33 + a.m34*b.tz;
	r.m34 = a.m31*b.m14 + a.m32*b.m24 + a.m33*b.m34 + a.m34*b.m44;

	// Compute the translation portion

	r.tx = a.tx*b.m11 + a.ty*b.m21 + a.tz*b.m31 + a.m44*b.tx;
	r.ty = a.tx*b.m12 + a.ty*b.m22 + a.tz*b.m32 + a.m44*b.ty;
	r.tz = a.tx*b.m13 + a.ty*b.m23 + a.tz*b.m33 + a.m44*b.tz;
	r.m44 = a.tx*b.m14 + a.ty*b.m24 + a.tz*b.m34 + a.m44*b.m44;

	// Return it.  Ouch - involves a copy constructor call.  If speed
	// is critical,  we may need a seperate function which places the
	// result where we want it...

	return r;
}

Matrix &operator*=(Matrix &a,  const Matrix &b) {
	a = a * b;
	return a;
}

Matrix operator*(const Matrix &a, const float f)
{
	Matrix matrix;
	matrix.m11 = a.m11 * f, matrix.m12 = a.m12 * f, matrix.m13 = a.m13 * f, matrix.m14 = a.m14 * f;
	matrix.m21 = a.m21 * f, matrix.m22 = a.m22 * f, matrix.m23 = a.m23 * f, matrix.m24 = a.m24 * f;
	matrix.m31 = a.m31 * f, matrix.m32 = a.m32 * f, matrix.m33 = a.m33 * f, matrix.m34 = a.m34 * f;
	matrix.tx = a.tx * f,   matrix.ty = a.ty * f,   matrix.tz = a.tz * f,   matrix.m44 = a.m44 * f;
	return matrix;
}