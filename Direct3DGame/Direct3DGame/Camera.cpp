#include "Camera.h"

void Camera::set_position(const Matrix &m)
{
	world_position_ = world_position_ * m;
}

void Camera::set_lookAt(const Vector3& rotate_angle)
{
	look_at_theta_ = look_at_theta_ + rotate_angle;
}

void Camera::view_transform(vector<Vertex> &vertexes)
{
	Matrix view_move_matrix;
	view_move_matrix.identify();
	view_move_matrix.setTranslation(-1 * world_position_);

	Matrix view_rotateX_matrix;
	view_rotateX_matrix.setRotate(1, look_at_theta_.y_);
	Matrix view_rotateY_matrix;
	view_rotateY_matrix.setRotate(2, look_at_theta_.x_);

	Matrix view_transform_matrix = view_move_matrix * view_rotateX_matrix * view_rotateY_matrix;
	for (auto &v : vertexes)
	{
		v.position_ *= view_transform_matrix;
	}
}

//转换到cvv裁剪空间
void Camera::canonicalview_volume(vector<Vertex> &vertexes)
{
	Matrix cvv_matrix;
	cvv_matrix.identify();
	cvv_matrix.m11 = Tan(angle_ / 2) / w_h_;       //zoomx
	cvv_matrix.m22 = Tan(angle_ / 2);			   //zoomy
	cvv_matrix.m33 = (far_ + near_) / (far_ - near_);
	cvv_matrix.tz = 2 * near_ * far_ / (near_ - far_);
	cvv_matrix.m44 = 0;
	cvv_matrix.m34 = 1;
	for (auto &v : vertexes)
	{
		v.position_ *= cvv_matrix;
	}
}

// 检查齐次坐标同 cvv 的边界用于视锥裁剪
int Camera::transform_check_cvv(const Vector3 &v) {
	int check = 0;
	if (v.z_ < -1.0f) check |= 1;
	if (v.z_ >  1.0f) check |= 2;
	if (v.x_ < -1.0f) check |= 4;
	if (v.x_ >  1.0f) check |= 8;
	if (v.y_ < -1.0f) check |= 16;
	if (v.y_ >  1.0f) check |= 32;
	return check;
}

bool Camera::is_back(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3)
{
	Vector3 vec13 = v3 - v1;
	Vector3 vec12 = v2 - v1;
	Vector3 vv = cross_product(vec13, vec12);
	return (vv * v1) > 0;
}