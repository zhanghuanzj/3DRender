#include "Vertex.h"

Vertex Vertex::interp(const Vertex &v,float factor)
{
	Vector3 pos = position.interp(v.position,factor);
	Vector3 nor = normal.interp(v.normal,factor);
	AColor acolor = color.interp(v.color,factor);
	return Vertex(pos,nor,acolor,0,0);
}