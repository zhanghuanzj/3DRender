#ifndef RENDER_PIPELINE_H_
#define RENDER_PIPELINE_H_

#include <vector>
#include "Matrix.h"
#include "Vector3.h"
#include "Vertex.h"
#include "Rasterizer.h"

class Pipeline
{
public:
	static Pipeline& Instance();
	void draw();
	//图元装配(Input Assembly)
	void input_assembly(const std::vector<Vertex> &vertex);
	//顶点着色(Vertex Shader)
	void vertex_shader();
	//曲面细分(Tessellator)
	//几何着色(Geometry Shader)
	//裁剪(clip)
	//光栅化(Rasterization) 背面剔除
	void rasterize(const vector<Triangle> &triangles);
	bool is_back(const Vertex &v1, const Vertex &v2, const Vertex &v3);
	bool is_out_of_range(const Vector3 &v);
	vector<Triangle> get_front_triangle(const vector<Vertex> &vertexes);
	vector<Triangle> cvv_clip(const vector<Vertex> &vertexes, const vector<Triangle> &triangles);
	//片段着色(Fragment Shader)
	VColor fragment_shader(const Vertex &vertex);
	//输出合并(Output Merge)
	void set_texture(Texture *texture);

public:
	//Input Vertex
	std::vector<Vertex> inputVertex;

	//fragment shader
	std::vector<Vertex> fragmentVertex;

	//uniform definitions
	Matrix model;
	Matrix view;
	Matrix projection;
	Matrix screen_map;
	//common setting
	Light light;
	Vector3 viewPosition;

	Texture *pTexture = nullptr;

private:
	Pipeline() {}
	Rasterizer &rasterizer = Rasterizer::instance();
};
#endif
