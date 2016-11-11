#ifndef RENDER_VERTEX_H_
#define RENDER_VERTEX_H_

#include "Vector3.h"
#include "Math.h"

class Vertex
{
public:
	Vertex(const Vector3 &position_,const AColor &color_,float u_,float v_):position(position_),color(color_),u(u_),v(v_),inv_w(1.0f){}
	Vertex():position(),color(0.0f,0.0f,0.0f,0.0f),u(0.0f),v(0.0f){}
	Vertex interp(const Vertex &v,float factor);
	bool isxy_same(const Vertex &v)
	{
		return position.x==v.position.x&&position.y==v.position.y;
	}
	void add(const Vertex &vertex)
	{
		position = position+vertex.position;
		color = color+vertex.color;
		u += vertex.u;
		v += vertex.v;
		inv_w += vertex.inv_w;
	}

	Vector3 position;
	AColor color;
	float u;
	float v;
	float inv_w;
};
#endif