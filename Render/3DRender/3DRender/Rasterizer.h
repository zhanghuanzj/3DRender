#ifndef RENDER_RASTERIZER_H_
#define RENDER_RASTERIZER_H_

#include "DirectXLib.h"
#include "Vertex.h"
#include "Texture.h"


struct Light
{
	VColor color;
	Vector3 position;
};

struct Scanline
{
	Scanline(Vertex vl, Vertex vr)
	{
		float divide = 1.0f / (vr.screenPosition.x - vl.screenPosition.x);
		x = static_cast<int>(vl.screenPosition.x + 0.5f);
		y = vl.screenPosition.y;
		width = static_cast<int>(vr.screenPosition.x + 0.5f) - x;
		vertex = vl;

		//²åÖµÓÃµÄdelta
		dv.inv_w = (vr.inv_w - vl.inv_w) * divide;
		dv.position = (vr.position - vl.position) * divide;
		dv.normal = (vr.normal - vl.normal) * divide;
		dv.ndcPosition.z = (vr.ndcPosition.z - vl.ndcPosition.z) * divide;
		dv.color = (vr.color - vl.color) * divide;
		dv.u = (vr.u - vl.u) * divide;
		dv.v = (vr.v - vl.v) * divide;
	}

	void to_next_color()
	{
		vertex.color += dv.color;
		vertex.inv_w += dv.inv_w;
	}

	void to_next_fragment()
	{
		vertex.u += dv.u;
		vertex.v += dv.v;
		vertex.position.x += dv.position.x;
		vertex.position.y += dv.position.y;
		vertex.position.z += dv.position.z;
		vertex.normal.x += dv.normal.x;
		vertex.normal.y += dv.normal.y;
		vertex.normal.z += dv.normal.z;
		vertex.ndcPosition.z += dv.ndcPosition.z;
		vertex.inv_w += dv.inv_w;
	}

	Vertex vertex;
	Vertex dv;
	int x;
	int y;
	int width;
};

class Triangle
{
public:
	Triangle(int a, int b, int c):index1(a), index2(b), index3(c){}
	int index1;
	int index2;
	int index3;
};

class Rasterizer
{
public:
	static Rasterizer& instance()
	{
		static Rasterizer rasterizer;
		return rasterizer;
	}

	void sort_vertex(Vertex &v1, Vertex &v2, Vertex &v3);

	void drawline_dda(int x1, int y1, int x2, int y2, VColor color);
	void drawline_mid(int x1, int y1, int x2, int y2, VColor color);
	void drawline_bresenham(int x1, int y1, int x2, int y2, VColor color);

	void draw_scanline(Vertex vl, Vertex vr, int y);

	void draw_top_flat_triangle(const Vertex &v1, const Vertex &v2, const Vertex &v3);

	void draw_button_flat_triangle(const Vertex &v1, const Vertex &v2, const Vertex &v3);

	void draw_triangle(Vertex v1, Vertex v2, Vertex v3);
	void draw_triangle_wireframe(Vertex v1, Vertex v2, Vertex v3);

private:
	Rasterizer() :directX(DirectX::instance()) {}
	DirectX &directX;

public:

	static RenderState renderState;
	static LightState lightState;
};


#endif