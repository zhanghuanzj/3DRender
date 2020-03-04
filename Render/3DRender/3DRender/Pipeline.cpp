#include "Pipeline.h"

Pipeline& Pipeline::Instance()
{
	static Pipeline pipeline;
	return pipeline;
}

void Pipeline::set_texture(Texture *texture)
{
	if (pTexture != nullptr)
	{
		delete pTexture;
	}
	pTexture = texture;
}

void Pipeline::input_assembly(const std::vector<Vertex> &vertex)
{
	inputVertex = vertex;
}

bool Pipeline::is_back(const Vertex &v1, const Vertex &v2, const Vertex &v3)
{
	Vector3 to_triangle = v1.position - viewPosition;
	return crossproduct(v1.position - v2.position, v3.position - v2.position) * to_triangle > 0.0f;
}

vector<Triangle> Pipeline::get_front_triangle(const vector<Vertex> &vertexes)
{
	vector<Triangle> triangle;
	for (int i = 0; i < 6; ++i)
	{
		int index = i * 4;
		if (!is_back(vertexes[index], vertexes[index + 1], vertexes[index + 2]))
		{
			triangle.push_back(Triangle(index, index + 1, index + 2));
		}
		if (!is_back(vertexes[index + 2], vertexes[index + 3], vertexes[index]))
		{
			triangle.push_back(Triangle(index + 2, index + 3, index));
		}
	}
	return triangle;
}

bool Pipeline::is_out_of_range(const Vector3 &v)
{
	int check = 0;
	if (v.z < -1.0f) check |= 1;
	if (v.z > 1.0f) check |= 2;
	if (v.x < -1.0f) check |= 4;
	if (v.x > 1.0f) check |= 8;
	if (v.y < -1.0f) check |= 16;
	if (v.y > 1.0f) check |= 32;
	return check > 0;
}

vector<Triangle> Pipeline::cvv_clip(const vector<Vertex> &vertexes, const vector<Triangle> &triangles)
{
	vector<bool> vertex_set(vertexes.size(), true);
	vector<Triangle> new_triangles;

	int index = 0;
	for (auto &v : vertexes)
	{
		if (is_out_of_range(v.ndcPosition))
		{
			vertex_set[index] = false;
		}
		++index;
	}

	for (auto &triangle : triangles)
	{
		if (vertex_set[triangle.index1] && vertex_set[triangle.index2] && vertex_set[triangle.index3])
		{
			new_triangles.push_back(triangle);
		}
	}
	return new_triangles;
}

void Pipeline::rasterize(const vector<Triangle> &triangles)
{
	Rasterizer &rasterizer = Rasterizer::instance();
	for (auto &triangle : triangles)
	{
		rasterizer.draw_triangle(fragmentVertex[triangle.index1], fragmentVertex[triangle.index2], fragmentVertex[triangle.index3]);
	}
}

void Pipeline::vertex_shader()
{
	Matrix inverse_transpose_matrix = model.inverse().transpose();
	Matrix mvp = model * view * projection;
	fragmentVertex.resize(inputVertex.size());
	for (int i = 0; i < inputVertex.size(); ++i)
	{
		fragmentVertex[i].ndcPosition = inputVertex[i].position * mvp; // MVP
		fragmentVertex[i].inv_w = 1.0f / fragmentVertex[i].ndcPosition.w; //透视除法，此时为（x'z, y'z, z'z, z）
		fragmentVertex[i].ndcPosition = fragmentVertex[i].ndcPosition * fragmentVertex[i].inv_w; //转换为NDC坐标(x', y', z', 1）

		fragmentVertex[i].position = inputVertex[i].position * model;  //物体旋转，移动
		fragmentVertex[i].normal = inputVertex[i].normal * inverse_transpose_matrix;  //法线处理
		fragmentVertex[i].u = inputVertex[i].u * fragmentVertex[i].inv_w; // 透视校正初始化
		fragmentVertex[i].v = inputVertex[i].v * fragmentVertex[i].inv_w;
		fragmentVertex[i].color = inputVertex[i].color * fragmentVertex[i].inv_w;
	}
}

VColor Pipeline::fragment_shader(const Vertex &vertex)
{
	// 透视校正
	float w = 1.0f / vertex.inv_w;
	if (rasterizer.lightState == LightState::ON)
	{
		Vector3 light_vec = light.position - vertex.position;
		light_vec.normalize();

		//Ambient
		VColor ambient = light.color * 0.1f;

		//Diffuse
		float nl = vertex.normal * light_vec;
		VColor diffuse = max(nl, 0) * light.color;

		//Specular
		Vector3 view_vec = viewPosition - vertex.position;
		view_vec.normalize();
		Vector3 half_vec = (light_vec + view_vec) * 0.5f;
		half_vec.normalize();
		VColor specular = pow(max(vertex.normal * half_vec, 0.0), 32) * light.color;

		VColor tex_color = pTexture->get_color(vertex.u * w, vertex.v * w);
		return (ambient + diffuse + specular) * tex_color;
	} 
	else
	{
		return pTexture->get_color(vertex.u * w, vertex.v * w);
	}
}

void Pipeline::draw()
{
	vertex_shader();
	vector<Triangle> front_triangles = get_front_triangle(fragmentVertex); // 背面剔除
	vector<Triangle> triangles = cvv_clip(fragmentVertex, front_triangles); // cvv裁剪

	for (int i = 0; i < fragmentVertex.size(); ++i)
	{
		fragmentVertex[i].screenPosition = fragmentVertex[i].ndcPosition * screen_map; // 裁剪空间————(屏幕映射)————>屏幕空间
	}
	rasterize(triangles);
}