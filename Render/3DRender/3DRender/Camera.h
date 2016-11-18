#ifndef RENDER_CAMERA_H_
#define RENDER_CAMERA_H_

#include <vector>
#include "Vector3.h"
#include "Vertex.h"
#include "Matrix.h"

class Camera
{
private:
	Camera(int width_=800,int height_=600,float near_=1.0f,float far_=10000.0f,float fov_=60):
		position(0.0f,0.0f,-1.0f),target(0.0f,0.0f,0.0f),up(0.0f,1.0f,0.0f),lookat(0.0f,0.0f,1.0f),right(1.0f,0.0f,0.0f),
		Near(near_),Far(far_),fov(fov_),aspect(width_/height_),width(width_),height(height_){}
public:
	static Camera& instance(int width_=800,int height_=600)
	{
		static Camera camera(width_,height_);
		return camera;
	}

	void move_along_up_vector(float units)
	{
		position += up*units;
		target += up*units;
	}

	void move_along_right_vector(float units)
	{
		position += right*units;
		target += right*units;
	}

	void move_along_lookat_vector(float units)
	{
		position += lookat*units;
		target += lookat*units;
	}

	void rotate_along_up_vector(float angle)
	{
		Matrix matrix;
		matrix.setRotate(up,angle);
		right = right*matrix;
		lookat = lookat*matrix;
		target = lookat*target.length();
	}

	void rotate_along_right_vector(float angle)
	{
		Matrix matrix;
		matrix.setRotate(right,angle);
		up = up*matrix;
		lookat = lookat*matrix;
		target = lookat*target.length();
	}

	void rotate_along_lookat_vector(float angle)
	{
		Matrix matrix;
		matrix.setRotate(lookat,angle);
		up = up*matrix;
		right = right*matrix;
	}

	void eye_transform(vector<Vertex> &vertexes )
	{
		Matrix translation_matrix;
		translation_matrix.identify();
		translation_matrix.setTranslation(Vector3(-position.x,-position.y,-position.z));
		Matrix rotate_matrix;
		rotate_matrix.identify();
		rotate_matrix.m11 = right.x;
		rotate_matrix.m12 = right.y;
		rotate_matrix.m13 = right.z;
		rotate_matrix.m21 = up.x;
		rotate_matrix.m22 = up.y;
		rotate_matrix.m23 = up.z;
		rotate_matrix.m31 = lookat.x;
		rotate_matrix.m32 = lookat.y;
		rotate_matrix.m33 = lookat.z;

		Matrix matrix = translation_matrix*rotate_matrix;
		for (Vertex &v : vertexes) 
		{
			v.position = v.position*matrix;
		}
	}

	bool is_out_of_range(Vector3 v)
	{
		int check = 0;
		if (v.z < -1.0f) check |= 1;
		if (v.z >  1.0f) check |= 2;
		if (v.x < -1.0f) check |= 4;
		if (v.x >  1.0f) check |= 8;
		if (v.y < -1.0f) check |= 16;
		if (v.y >  1.0f) check |= 32;
		return check>0;
	}

	vector<Triangle> cvv_clip(vector<Vertex> &vertexes,vector<Triangle> &triangles)
	{
		vector<bool> vertex_set(vertexes.size(),true);
		vector<Triangle> new_triangles;

		Matrix cvv_matrix;
		cvv_matrix.identify();
		cvv_matrix.m11 = Cot(fov/2)/aspect;        
		cvv_matrix.m22 = Cot(fov/2);			   
		cvv_matrix.m33 = (Far+Near)/(Far-Near);
		cvv_matrix.tz = -2*Near*Far/(Far-Near);
		cvv_matrix.m44 = 0;
		cvv_matrix.m34 = 1;

		int index = 0;
		for (auto &v : vertexes) 
		{
			//Í¸ÊÓ×ª»»
			v.position = v.position*cvv_matrix;
			//Í¸ÊÓ³ý·¨
			v.inv_w = 1/v.position.w;
			v.position = v.position * v.inv_w;
			v.u *= v.inv_w;
			v.v *= v.inv_w;
			v.color = v.color * v.inv_w;
			if (is_out_of_range(v.position))
			{
				vertex_set[index] = false;
			}
			++index;
		}

		for (auto triangle:triangles) 
		{
			if (vertex_set[triangle.index1]&&vertex_set[triangle.index2]&&vertex_set[triangle.index3])
			{
				new_triangles.push_back(triangle);
			}
		}
		return new_triangles;
	}

	void screen_map(vector<Vertex> &vertexes)
	{
		Matrix screen_matrix;
		screen_matrix.identify();
		screen_matrix.m11 = width/2;
		screen_matrix.tx = width/2;
		screen_matrix.m22 = -height/2;
		screen_matrix.ty = height/2;
		screen_matrix.m33 = (Far-Near)/2;
		screen_matrix.tz = (Far+Near)/2;
		for (Vertex &v : vertexes) 
		{
			//ÆÁÄ»ÊÊÅä
			v.position = v.position*screen_matrix;
		}
	}

	bool is_back(const Vertex &v1,const Vertex &v2,const Vertex &v3)
	{
		Vector3 to_triangle = v1.position - position;
		return crossproduct(v1.position-v2.position,v3.position-v2.position)*v1.position>0;
	}

	vector<Triangle> get_front_triangle(vector<Vertex> &vertexes)
	{
		vector<Triangle> triangle;
		for (int i=0;i<6;++i) 
		{
			int index = i*4;
			if (!Camera::instance().is_back(vertexes[index],vertexes[index+1],vertexes[index+2]))
			{
				triangle.push_back(Triangle(index,index+1,index+2));
			}
			if (!Camera::instance().is_back(vertexes[index+2],vertexes[index+3],vertexes[index]))
			{
				triangle.push_back(Triangle(index+2,index+3,index));
			}
		}
		return triangle;
	}

	Vector3 get_position()
	{
		return position;
	}
private:
	int width;
	int height;
	float Near;
	float Far;
	float fov;
	float aspect;
	
	Vector3 target;
	Vector3 position;
	Vector3 up;
	Vector3 lookat;
	Vector3 right;

};
#endif