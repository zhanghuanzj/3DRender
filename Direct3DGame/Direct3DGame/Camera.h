#ifndef CAMERA_H_
#define CAMERA_H_

#include <vector>
#include "Vector3.h"
#include "Matrix.h"
#include "Vertex.h"
using namespace std;

class Camera
{
public:
	static Camera& instance()
	{
		static Camera *camera = new Camera(); 
		return *camera;
	}
	void set_position(const Matrix &m);
	void set_lookAt(const Vector3& point);
	void set_w_h(float w_h){w_h_=w_h;}
	void canonicalview_volume(vector<Vertex> &vertexes);
	void view_transform(vector<Vertex> &vertexes);
	int transform_check_cvv(const Vector3 &v);
	bool is_back(const Vector3 &v1,const Vector3 &v2,const Vector3 &v3);
	float get_far()const{return far_;}
	float get_near()const{return near_;}
	Vector3 get_position(){return world_position_;}

private:
	Camera():world_position_(),look_at_theta_(Vector3(0,0,0)),angle_(90),near_(1),far_(10000){}

	Vector3 world_position_;
	Vector3 look_at_theta_;
	float angle_;        //Y轴方向视角度数
	float w_h_;          //屏幕宽高比
	float near_;
	float far_;
};



#endif // !CAMERA_H_
