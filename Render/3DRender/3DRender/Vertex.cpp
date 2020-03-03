#include "Vertex.h"
#include "Math.h"

Vertex Vertex::interp(const Vertex &ver, float factor)
{
	float inv = inv_w + (ver.inv_w - inv_w) * factor;
	Vector3 pos = position.interp(ver.position, factor);
	VColor vcolor = color.interp(ver.color, factor);
	Vertex vertex(pos, vcolor, 0, 0);
	//vertex.inv_w = inv;
	vertex.u = Lerp(u, ver.u, factor);
	vertex.v = Lerp(v, ver.v, factor);
	vertex.screenPosition = screenPosition.interp(ver.screenPosition, factor);
	vertex.normal = normal.interp(ver.normal, factor);
	return vertex;


	//float inv = inv_w + (ver.inv_w - inv_w) * factor;
	//Vector3 pos = position.interp(ver.position, factor);
	//VColor vcolor = color.interp(ver.color, factor);
	//Vertex vertex(pos, vcolor, 0, 0);
	//vertex.inv_w = inv;
	//vertex.u = u + (ver.u - u) * factor;
	//vertex.v = v + (ver.v - v) * factor;
	//return vertex;
}