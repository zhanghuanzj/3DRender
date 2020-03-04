#include "Vertex.h"
#include "Math.h"

Vertex Vertex::interp(const Vertex &ver, float factor) const
{
	Vector3 pos = position.interp(ver.position, factor);
	VColor vcolor = color.interp(ver.color, factor);
	Vertex vertex(pos, vcolor, 0, 0);
	vertex.inv_w = Lerp(inv_w, ver.inv_w, factor);
	vertex.u = Lerp(u, ver.u, factor);
	vertex.v = Lerp(v, ver.v, factor);
	vertex.screenPosition = screenPosition.interp(ver.screenPosition, factor);
	vertex.normal = normal.interp(ver.normal, factor);
	return vertex;
}