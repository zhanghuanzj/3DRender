#ifndef RENDER_VERTEX_H_
#define RENDER_VERTEX_H_

#include "Vector3.h"
#include "Math.h"
#include "VColor.h"


class Vertex
{
public:
	Vertex(const Vector3 &position_,const VColor &color_,float u_,float v_,Vector3 normal_=(Vector3())):position(position_),color(color_),u(u_),v(v_),inv_w(1.0f),normal(normal_){}
	Vertex():position(),color(0.0f,0.0f,0.0f,0.0f),u(0.0f),v(0.0f){}
	Vertex interp(const Vertex &v,float factor);
	bool isxy_same(const Vertex &v)
	{
		return position.x==v.position.x&&position.y==v.position.y;
	}
	void set_normal(Vector3 n)
	{
		normal = n;
	}

	Vector3 position;
	Vector3 normal;
	VColor color;
	VColor light;
	float u;
	float v;
	float inv_w;
};
#endif