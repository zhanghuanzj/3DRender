#ifndef VERTEX_H_
#define VERTEX_H_

#include "Vector3.h"
#include "Math.h"

class Vertex
{
public:
	Vertex(Vector3 position,Color color):position_(position),color_(color){}

	Vector2 to_vector2(){ return Vector2(position_.x_,position_.y_,color_);}
	Vector3 position_;
	Color color_;
};
#endif