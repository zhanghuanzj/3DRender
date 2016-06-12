#ifndef CAMERA_H_
#define CAMERA_H_

#include <vector>
#include "Vector3.h"
#include "Matrix.h"
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
	void view_transform(vector<Vector3> &vertexes);
private:
	Camera():world_position_(),look_at_theta_(Vector3(0,0,0)),angle_(45){}
	
	Vector3 world_position_;
	Vector3 look_at_theta_;
	float angle_;
};


#endif // !CAMERA_H_
