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