#include "Vector3.h"

void Vector3::normalize() {
	float factor = 0;
	float length = this->length();
	if(length > 0) {
		factor = 1 / length;
	}
	x_ *= factor;
	y_ *= factor;
	z_ *= factor;
}

void Vector3::color_adjust()
{
	x_ = x_>1.0f?1.0f:x_;
	y_ = y_>1.0f?1.0f:y_;
	z_ = z_>1.0f?1.0f:z_;
}

Vector3 Vector3::interp(const Vector3 &v,float factor)
{
	float x = x_+(v.x_-x_)*factor;
	float y = y_+(v.y_-y_)*factor;
	float z = z_+(v.z_-z_)*factor;
	return Vector3(x,y,z);
}