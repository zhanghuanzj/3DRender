#include "Vertex.h"

//Vertex Vertex::interp(const Vertex &v,float factor)
//{
//	Vector3 position = position_.interp(v.position_,factor);
//	AColor color = color_.interp(v.color_,factor);
//	float oneoverz = 1/position_.w_+(1/v.position_.w_-1/position_.w_)*factor;
//	float u_z = u_/position_.w_ + (v.u_/v.position_.w_-u_/position_.w_)*factor;
//	float v_z = v_/position_.w_ + (v.v_/v.position_.w_-v_/position_.w_)*factor;
//	float tu = u_z/oneoverz;
//	float tv = v_z/oneoverz;
//	position.w_ = 1/oneoverz;
//	return Vertex(position,color,tu,tv);
//}

Vertex Vertex::interp(const Vertex &v,float factor)
{
	Vector3 position = position_.interp(v.position_,factor);
	AColor color = color_.interp(v.color_,factor);
	float w = position_.w_+(v.position_.w_-position_.w_)*factor;
	float u_z = u_*position_.w_ + (v.u_*v.position_.w_-u_*position_.w_)*factor;
	float v_z = v_*position_.w_ + (v.v_*v.position_.w_-v_*position_.w_)*factor;
	float tu = u_z/w;
	float tv = v_z/w;
	position.w_ = w;
	Vertex vertex(position,color,tu,tv);
	vertex.light_.x_ = light_.x_ + (v.light_.x_ - light_.x_)*factor;
	vertex.light_.y_ = light_.y_ + (v.light_.y_ - light_.y_)*factor;
	vertex.light_.z_ = light_.z_ + (v.light_.z_ - light_.z_)*factor;
	return vertex;
}

void Vertex::interp_u_v(const Vertex &ver,float factor,float &u,float &v)
{
	float w = position_.w_+(ver.position_.w_-position_.w_)*factor;
	float u_z = u_*position_.w_ + (ver.u_*ver.position_.w_-u_*position_.w_)*factor;
	float v_z = v_*position_.w_ + (ver.v_*ver.position_.w_-v_*position_.w_)*factor;
	u = u_z/w;
	v = v_z/w;
}
