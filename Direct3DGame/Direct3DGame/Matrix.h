#ifndef MATRIX_H_
#define MATRIX_H_

#include <cassert>
#include "Vector3.h"
#include "Math.h"


class Matrix
{
public:
	float m11 , m12 , m13 ;
	float m21 , m22 , m23 ;
	float m31 , m32 , m33 ;
	float tx  , ty  , tz  ;

	void identify();
	void setTranslation(const Vector3& d);
	void zeroTranslation();
	void setRotate(int axis,float theta);
	void setRotate(const Vector3& axis,float theta);
	void setScale(const Vector3 &s);
};

Vector3	operator*(const Vector3 &p, const Matrix &m);
Vector3 &operator*=(Vector3 &p, const Matrix &m);
Matrix operator*(const Matrix &a, const Matrix &b);
Matrix &operator*=(Matrix &a, const Matrix &b);



#endif