#ifndef RENDER_RASTERIZER_H_
#define RENDER_RASTERIZER_H_

#include "DirectXLib.h"
#include "Vertex.h"
#include "Texture.h"

struct Scanline
{
	Scanline(const Vertex &v_,const Vertex &Step_,int x_,int y_,float width_):v(v_),step(Step_),x(x_),y(y_),width(width_){}
	Vertex v;
	Vertex step;
	int x;
	int y;
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
	Rasterizer():texture(nullptr){}
	~Rasterizer()
	{
		if (texture!=nullptr)
		{
			delete texture;
		}
	}
	void sort_vertex(Vertex &v1,Vertex &v2,Vertex &v3);

	Scanline generate_scanline(Vertex vl,Vertex vr,int y);

	void drawline_dda(int x1,int y1,int x2,int y2,AColor color);
	void drawline_mid(int x1,int y1,int x2,int y2,AColor color);
	void drawline_bresenham(int x1,int y1,int x2,int y2,AColor color);

	void draw_scanline(Vertex vl,Vertex vr,int y);

	void draw_top_flat_triangle(Vertex v1,Vertex v2,Vertex v3);

	void draw_button_flat_triangle(Vertex v1,Vertex v2,Vertex v3);

	void draw_triangle(Vertex v1,Vertex v2,Vertex v3);

	void set_texture(string path);

private:
	Texture *texture;
};
#endif