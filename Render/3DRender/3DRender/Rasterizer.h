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
	Scanline(const Vertex &vl, const Vertex &vr)
	{
		//Scanline scanline(vl.color, (vr.color - vl.color) * divide);
		float divide = 1.0f / (vr.screenPosition.x - vl.screenPosition.x);
		x = static_cast<int>(vl.screenPosition.x + 0.5f);
		y = vl.screenPosition.y;
		//scanline.inv_w = vl.inv_w;
		//scanline.dinv_w = (vr.inv_w-vl.inv_w)*divide;
		width = static_cast<int>(vr.screenPosition.x + 0.5f) - x;

		vertex = vl;
		dPosition = (vr.position - vl.position) * divide;
		dNormal = (vr.normal - vl.normal) * divide;
		dz = (vr.ndcPosition.z - vl.ndcPosition.z) * divide;
		dColor = (vr.color - vl.color) * divide;
		du = (vr.u - vl.u) * divide;
		dv = (vr.v - vl.v) * divide;
	}

	void to_next_color()
	{
		vertex.color += dColor;
	}

	void to_next_texture()
	{
		vertex.u += du;
		vertex.v += dv;
		vertex.position.x += dPosition.x;
		vertex.position.y += dPosition.y;
		vertex.position.z += dPosition.z;
		vertex.normal.x += dNormal.x;
		vertex.normal.y += dNormal.y;
		vertex.normal.z += dNormal.z;
		vertex.ndcPosition.z += dz;
	}
	Vertex vertex;
	Vector3 dPosition;
	Vector3 dNormal;
	int x;
	int y;
	VColor dColor;
	float du, dv, dinv_w, dz;
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

	void draw_top_flat_triangle(Vertex v1, Vertex v2, Vertex v3);

	void draw_button_flat_triangle(Vertex v1, Vertex v2, Vertex v3);

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