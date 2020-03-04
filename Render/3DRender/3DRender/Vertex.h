#ifndef RENDER_VERTEX_H_
#define RENDER_VERTEX_H_

#include "Vector3.h"
#include "Math.h"
#include "VColor.h"


class Vertex
{
public:
	Vertex(const Vector3 &position_, const VColor &color_, float u_, float v_, Vector3 normal_=(Vector3())):position(position_), color(color_), u(u_), v(v_), inv_w(1.0f), normal(normal_){}
	Vertex(const Vertex &v):position(v.position), normal(v.normal), screenPosition(v.screenPosition), ndcPosition(v.ndcPosition), color(v.color), u(v.u), v(v.v), inv_w(v.inv_w){}
	Vertex():position(), color(0.0f, 0.0f, 0.0f, 0.0f), u(0.0f), v(0.0f){}
	Vertex interp(const Vertex &v, float factor)const;

	Vertex& operator=(const Vertex &vt)
	{
		position = vt.position;
		normal = vt.normal;
		screenPosition = vt.screenPosition;
		ndcPosition = vt.ndcPosition;
		color = vt.color;
		u = vt.u;
		v = vt.v;
		inv_w = vt.inv_w;
		return *this;
	}

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
	Vector3 screenPosition;
	Vector3 ndcPosition;
	VColor color;
	float u;
	float v;
	float inv_w;
};

inline Vertex operator+(const Vertex &v1, const Vertex &v2)
{
	Vertex v;
	v.position = v1.position + v2.position;
	v.normal = v1.normal + v2.normal;
	v.screenPosition = v1.screenPosition + v2.screenPosition;
	v.ndcPosition = v1.ndcPosition + v2.ndcPosition;
	v.color = v1.color + v2.color;
	v.u = v1.u + v2.u;
	v.v = v1.v + v2.v;
	v.inv_w = v1.inv_w + v2.inv_w;

	return v;
}

inline Vertex operator-(const Vertex &v1, const Vertex &v2)
{
	Vertex v;
	v.position = v1.position - v2.position;
	v.normal = v1.normal - v2.normal;
	v.screenPosition = v1.screenPosition - v2.screenPosition;
	v.ndcPosition = v1.ndcPosition - v2.ndcPosition;
	v.color = v1.color - v2.color;
	v.u = v1.u - v2.u;
	v.v = v1.v - v2.v;
	v.inv_w = v1.inv_w - v2.inv_w;

	return v;
}

inline Vertex operator*(const Vertex &v1, const float f)
{
	Vertex v;
	v.position = v1.position * f;
	v.normal = v1.normal * f;
	v.screenPosition = v1.screenPosition * f;
	v.ndcPosition = v1.ndcPosition * f;
	v.color = v1.color * f;
	v.u = v1.u * f;
	v.v = v1.v * f;
	v.inv_w = v1.inv_w * f;

	return v;
}

inline Vertex operator/(const Vertex &v1, const float f)
{
	Vertex v;
	v.position = v1.position / f;
	v.normal = v1.normal / f;
	v.screenPosition = v1.screenPosition / f;
	v.ndcPosition = v1.ndcPosition / f;
	v.color = v1.color / f;
	v.u = v1.u / f;
	v.v = v1.v / f;
	v.inv_w = v1.inv_w / f;

	return v;
}
#endif