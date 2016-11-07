#ifndef RENDER_VERTEX_H_
#define RENDER_VERTEX_H_

#include "Vector3.h"
#include "Math.h"

class Vertex
{
public:
	Vertex(const Vector3 &position_,Vector3 normal_,const AColor &color_,float u_,float v_):position(position_),normal(normal_),color(color_),u(u_),v(v_){}
	Vertex interp(const Vertex &v,float factor);
	bool isxy_same(const Vertex &v)
	{
		return position.x==v.position.x&&position.y==v.position.y;
	}
	void add(const Vertex &v)
	{
		position = position+v.position;
		normal = normal+v.normal;
		color = color+v.color;
	}

	Vector3 position;
	Vector3 normal;
	AColor color;
	float u;
	float v;
};
#endif