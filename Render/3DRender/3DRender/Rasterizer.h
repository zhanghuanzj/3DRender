#ifndef RENDER_RASTERIZER_H_
#define RENDER_RASTERIZER_H_

#include "DirectXLib.h"
#include "Vertex.h"
#include "Texture.h"


struct Light
{
	VColor diffuse;
	VColor specular;
	VColor ambient;
	Vector3 position;
};
struct Scanline
{
	Scanline(VColor color_,VColor step_):color(color_),step(step_){}
	void to_next_color()
	{
		color += step;
		inv_w += dinv_w;
	}
	void to_next_texture()
	{
		u += du;
		v += dv;
		inv_w += dinv_w;
		light += dl;
	}
	
	int x;
	int y;
	VColor color;
	VColor step;
	VColor light;
	VColor dl;
	float u,v,inv_w;
	float du,dv,dinv_w;
	int width;

};

class Triangle
{
public:
	Triangle(int a,int b,int c):index1(a),index2(b),index3(c){}
	int index1;
	int index2;
	int index3;
};

class Rasterizer
{
public:
	Rasterizer():texture(nullptr),directX(DirectX::instance()){}

	void sort_vertex(Vertex &v1,Vertex &v2,Vertex &v3);

	Scanline generate_scanline(Vertex vl,Vertex vr,int y);

	void drawline_dda(int x1,int y1,int x2,int y2,AColor color);
	void drawline_mid(int x1,int y1,int x2,int y2,AColor color);
	void drawline_bresenham(int x1,int y1,int x2,int y2,AColor color);

	void draw_scanline(Vertex vl,Vertex vr,int y);

	void draw_top_flat_triangle(Vertex v1,Vertex v2,Vertex v3);

	void draw_button_flat_triangle(Vertex v1,Vertex v2,Vertex v3);

	void draw_triangle(Vertex v1,Vertex v2,Vertex v3);
	void draw_triangle_wireframe(Vertex v1,Vertex v2,Vertex v3);

	void set_texture(Texture *ptexture);

private:
	Texture *texture;
	DirectX &directX;
public:
	static RenderState renderState;
};


#endif