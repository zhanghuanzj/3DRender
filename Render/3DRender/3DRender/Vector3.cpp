#include "Vector3.h"

void Vector3::normalize() {
	float factor = 0;
	float length = this->length();
	if(length > 0) {
		factor = 1 / length;
	}
	x *= factor;
	y *= factor;
	z *= factor;
}

Vector3 Vector3::interp(const Vector3 &v,float factor)
{
	float x_ = x+(v.x-x)*factor;
	float y_ = y+(v.y-y)*factor;
	float z_ = z+(v.z-z)*factor;
	return Vector3(x_,y_,z_);
}