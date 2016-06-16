#ifndef MODEL_H_
#define MODEL_H_
#include <vector>
#include "Vector3.h"
#include "Texture.h"
using namespace std;

class TrangleIndex
{
	
public:
	TrangleIndex(int i,int j,int k){indices[0] = i;indices[1]=j;indices[2]=k;}
	TrangleIndex(){}
	int indices[3];
};
class Model
{
public:
	Model(Vector3 position):world_position_(position),texture_(nullptr){}

	Vector3 world_position_;
	vector<Vertex> local_vertexes_;
	vector<Vertex> trans_vertexes_;
	vector<TrangleIndex> poly_indices_;
	Texture *texture_;
};
#endif