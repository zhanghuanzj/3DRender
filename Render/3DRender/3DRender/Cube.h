#ifndef RENDER_CUBE_H_
#define RENDER_CUBE_H_

#include <string>
#include <vector>
#include "Vertex.h"
#include "Texture.h"

struct Material
{
	VColor diffuse;
	VColor specular;
	VColor ambient;
};

class Cube
{
public:
	Cube(float n, string path, Vector3 pos):local_vertexes(24, Vertex()), trans_vertexes(24, Vertex()), texture(path), position(pos)
	{
		float cube_vertex[24][3] = { 
			{-n, n, -n}, {-n, -n, -n}, {n, -n, -n}, {n, n, -n}, 
			{n, n, -n}, {n, -n, -n}, {n, -n, n}, {n, n, n}, 
			{n, n, n}, {n, -n, n}, {-n, -n, n}, {-n, n, n}, 
			{-n, n, n}, {-n, -n, n}, {-n, -n, -n}, {-n, n, -n}, 
			{-n, n, n}, {-n, n, -n}, {n, n, -n}, {n, n, n}, 
			{-n, -n, -n}, {-n, -n, n}, {n, -n, n}, {n, -n, -n}
		};

		float colors[24][3] = {
			//{1.0f,  0.0f,  1.0f}, {1.0f,  0.0f,  0.0f}, {1.0f,  1.0f,  0.0f}, {1.0f,  1.0f,  1.0f},  //前
			//{1.0f,  1.0f,  1.0f}, {1.0f,  1.0f,  0.0f}, {0.0f,  1.0f,  0.0f}, {0.0f,  1.0f,  1.0f},  //右
			//{0.0f,  1.0f,  1.0f}, {0.0f,  1.0f,  0.0f}, {1.0f,  1.0f,  1.0f}, {0.0f,  0.0f,  1.0f},  //后
			//{0.0f,  0.0f,  1.0f}, {1.0f,  1.0f,  1.0f}, {1.0f,  0.0f,  0.0f}, {1.0f,  0.0f,  1.0f},  //左
			//{0.0f,  0.0f,  1.0f}, {1.0f,  0.0f,  1.0f}, {1.0f,  1.0f,  1.0f}, {0.0f,  1.0f,  1.0f},  //上
			//{1.0f,  0.0f,  0.0f}, {1.0f,  1.0f,  1.0f}, {0.0f,  1.0f,  0.0f}, {1.0f,  1.0f,  0.0f},  //下

			{0.0f,  1.0f,  0.0f}, {0.0f,  0.0f,  0.0f}, {1.0f,  0.0f,  0.0f}, {1.0f,  1.0f,  0.0f},  //前
			{1.0f,  1.0f,  0.0f}, {1.0f,  0.0f,  0.0f}, {1.0f,  0.0f,  1.0f}, {1.0f,  1.0f,  1.0f},  //右
			{1.0f,  1.0f,  1.0f}, {1.0f,  0.0f,  1.0f}, {0.0f,  0.0f,  1.0f}, {0.0f,  1.0f,  1.0f},  //后
			{0.0f,  1.0f,  1.0f}, {0.0f,  0.0f,  1.0f}, {0.0f,  0.0f,  0.0f}, {0.0f,  1.0f,  0.0f},  //左
			{0.0f,  1.0f,  1.0f}, {0.0f,  1.0f,  0.0f}, {1.0f,  1.0f,  0.0f}, {1.0f,  1.0f,  1.0f},  //上
			{0.0f,  0.0f,  0.0f}, {0.0f,  0.0f,  1.0f}, {1.0f,  0.0f,  1.0f}, {1.0f,  0.0f,  0.0f},  //下
		};

		int uv[4][2] = {
			{0, 0}, {0, 1}, {1, 1}, {1, 0}, 
		};

		for (int i=0; i<24; ++i)
		{
			Vector3 v(cube_vertex[i][0], cube_vertex[i][1], cube_vertex[i][2]);
			VColor color(0, colors[i][0], colors[i][1], colors[i][2]);
			Vertex vertex(v, color, uv[i%4][0], uv[i%4][1]);
			v.normalize();
			vertex.set_normal(v);
			local_vertexes[i] = vertex;
			trans_vertexes[i] = vertex;
		}

		float m = 0.7f;
		material.diffuse = VColor(1.0f, m, m, m);
		material.specular = VColor(1.0f, m, m, m);
		material.ambient = VColor(1.0f, m, m, m);

		//金属材质
		/*material.diffuse = VColor(0.0f, 0.752f, 0.606f, 0.226f);
		material.specular = VColor(0.0f, 0.628f, 0.556f, 0.366f);
		material.ambient = VColor(0.0f, 0.247f, 0.200f, 0.075f);*/

	}
	Texture texture;
	Vector3 position;
	vector<Vertex> local_vertexes;
	vector<Vertex> trans_vertexes;
	Material material;
};
#endif