#ifndef RENDER_CAMERA_H_
#define RENDER_CAMERA_H_

#include <vector>
#include "Vector3.h"
#include "Vertex.h"
#include "Matrix.h"

class Camera
{
private:
	Camera(int width_ = 800, int height_ = 600, float near_ = 1.0f, float far_ = 10000.0f, float fov_ = 60) :
		position(0.0f, 0.0f, -8.0f), up(0.0f, 1.0f, 0.0f), lookat(0.0f, 0.0f, 1.0f), right(1.0f, 0.0f, 0.0f),
		Near(near_), Far(far_), fov(fov_), aspect(float(width_) / height_), width(width_), height(height_) {}

public:
	static Camera& instance(int width_=800, int height_=600)
	{
		static Camera camera(width_, height_);
		return camera;
	}

	void move_along_up_vector(float units)
	{
		position += up*units;
	}

	void move_along_right_vector(float units)
	{
		position += right*units;
	}

	void move_along_lookat_vector(float units)
	{
		position += lookat*units;
	}

	void rotate_along_up_vector(float angle)
	{
		Matrix matrix;
		matrix.setRotate(up, angle);
		right = right*matrix;
		lookat = lookat*matrix;
	}

	void rotate_along_right_vector(float angle)
	{
		Matrix matrix;
		matrix.setRotate(right, angle);
		up = up*matrix;
		lookat = lookat*matrix;
	}

	void rotate_along_lookat_vector(float angle)
	{
		Matrix matrix;
		matrix.setRotate(lookat, angle);
		up = up*matrix;
		right = right*matrix;
	}

	void rotate_around_model(float x, float y)
	{
		Matrix y_matrix;
		y_matrix.setRotate(up, x);
		Matrix x_matrix;
		x_matrix.setRotate(right, y);
		Matrix matrix = x_matrix * y_matrix;
		up = up * matrix;
		up.normalize();
		right = right * matrix;
		right.normalize();
		lookat = lookat * matrix;
		lookat.normalize();
		position = position * matrix;
	}

	void move_around_model(float x, float y)
	{
		move_along_right_vector(-x);
		move_along_up_vector(y);
	}

	Matrix get_view()
	{
		Matrix translation_matrix;
		translation_matrix.identify();
		translation_matrix.setTranslation(Vector3(-position.x, -position.y, -position.z));
		Matrix rotate_matrix;
		rotate_matrix.identify();
		rotate_matrix.m11 = right.x;
		rotate_matrix.m21 = right.y;
		rotate_matrix.m31 = right.z;
		rotate_matrix.m12 = up.x;
		rotate_matrix.m22 = up.y;
		rotate_matrix.m32 = up.z;
		rotate_matrix.m13 = lookat.x;
		rotate_matrix.m23 = lookat.y;
		rotate_matrix.m33 = lookat.z;

		return translation_matrix * rotate_matrix;
	}

	Matrix get_projection()
	{
		Matrix proj_matrix;
		proj_matrix.identify();
		proj_matrix.m11 = Cot(fov / 2) / aspect;
		proj_matrix.m22 = Cot(fov / 2);
		proj_matrix.m33 = (Far + Near) / (Far - Near);
		proj_matrix.tz = -2 * Near * Far / (Far - Near);
		proj_matrix.m34 = 1;
		proj_matrix.m44 = 0;

		return proj_matrix;
	}

	Matrix get_screen_map()
	{
		Matrix screen_matrix;
		screen_matrix.identify();
		screen_matrix.m11 = width / 2;
		screen_matrix.tx = width / 2;
		screen_matrix.m22 = -height / 2;
		screen_matrix.ty = height / 2;
		screen_matrix.m33 = (Far - Near) / 2;
		screen_matrix.tz = (Far + Near) / 2;
		return screen_matrix;
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
	
	Vector3 position;
	Vector3 up;
	Vector3 lookat;
	Vector3 right;
};
#endif