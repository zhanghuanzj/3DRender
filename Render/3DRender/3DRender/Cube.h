#ifndef RENDER_CUBE_H_
#define RENDER_CUBE_H_

#include <string>
#include <vector>
#include "Vertex.h"
#include "Texture.h"
class Cube
{
public:
	Cube(int n):local_vertexes(24,Vertex()),trans_vertexes(24,Vertex())
	{
		int cube_vertex[24][3] = { 
			{-n,n,-n},{-n,-n,-n},{n,-n,-n},{n,n,-n},
			{n,n,-n},{n,-n,-n},{n,-n,n},{n,n,n},
			{n,n,n},{n,-n,n},{-n,-n,n},{-n,n,n},
			{-n,n,n},{-n,-n,n},{-n,-n,-n},{-n,n,-n},
			{-n,n,n},{-n,n,-n},{n,n,-n},{n,n,n},
			{-n,-n,-n},{-n,-n,n},{n,-n,n},{n,-n,-n}
		};

		int colors[24][3] = {
			{1.0f, 0.0f, 1.0f},{1.0f, 0.0f, 0.0f},{1.0f, 1.0f, 0.0f},{1.0f, 1.0f, 1.0f}, //前
			{1.0f, 1.0f, 1.0f},{1.0f, 1.0f, 0.0f},{0.0f, 1.0f, 0.0f},{0.0f, 1.0f, 1.0f}, //右
			{0.0f, 1.0f, 1.0f},{0.0f, 1.0f, 0.0f},{1.0f, 1.0f, 1.0f},{0.0f, 0.0f, 1.0f}, //后
			{0.0f, 0.0f, 1.0f},{1.0f, 1.0f, 1.0f},{1.0f, 0.0f, 0.0f},{1.0f, 0.0f, 1.0f}, //左
			{0.0f, 0.0f, 1.0f},{1.0f, 0.0f, 1.0f},{1.0f, 1.0f, 1.0f},{0.0f, 1.0f, 1.0f}, //上
			{1.0f, 0.0f, 0.0f},{1.0f, 1.0f, 1.0f},{0.0f, 1.0f, 0.0f},{1.0f, 1.0f, 0.0f}, //下
		};

		int uv[4][2] = {
			{0,0},{0,1},{1,1},{1,0},
		};

		for (int i=0;i<24;++i)
		{
			Vector3 v(cube_vertex[i][0],cube_vertex[i][1],cube_vertex[i][2]);
			AColor color(0,colors[i][0],colors[i][1],colors[i][2]);
			Vertex vertex(v,color,uv[i%4][0],uv[i%4][1]);
			local_vertexes[i] = vertex;
			trans_vertexes[i] = vertex;
		}

	}

	vector<Vertex> local_vertexes;
	vector<Vertex> trans_vertexes;
};
#endif