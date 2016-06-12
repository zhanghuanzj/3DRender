#include "Camera.h"

void Camera::set_position(const Matrix &m)
{
	world_position_ = world_position_*m;
}

void Camera::set_lookAt(const Vector3& rotate_angle)
{
	look_at_theta_ = look_at_theta_+rotate_angle;
}

void Camera::view_transform(vector<Vector3> &vertexes)
{
	Matrix view_move_matrix;
	view_move_matrix.identify();
	view_move_matrix.setTranslation(-1*world_position_);

	Matrix view_rotateX_matrix;
	view_rotateX_matrix.setRotate(1,look_at_theta_.y_);
	Matrix view_rotateY_matrix;
	view_rotateY_matrix.setRotate(2,look_at_theta_.x_);

	Matrix view_transform_matrix = view_move_matrix*view_rotateX_matrix*view_rotateY_matrix;
	for (auto &v : vertexes)
	{
		v *= view_transform_matrix;
	}
}