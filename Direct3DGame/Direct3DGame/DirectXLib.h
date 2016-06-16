#ifndef DIRECTXLIB_H_
#define DIRECTXLIB_H_

#include<d3d9.h>
#include<d3dx9.h>
#include <windows.h>
#include <iostream>
#include <set>
#include "Math.h"
#include "Camera.h"
#include "Model.h"

using namespace std;


class DirectX
{
public:
	//单例获取
	static DirectX& instance()
	{
		static DirectX* pDirectX = new DirectX();
		return *pDirectX;
	}
	//初始化DirectX
	bool initialDirectX(HINSTANCE hInstance, HWND hWnd, int width, int height);
	void fillSurface();
	void lockSurface();
	void unlockSurface();
	//绘制像素到缓冲区
	void drawPixel(int x,int y,AColor color);

	//缓冲区翻转显示
	void flipSurface();

	//绘制直线
	void drawLine(int ,int ,int ,int,AColor  );

	//绘制三角形
	bool compare(const Vertex &v1,const Vertex &v2){return v1.position_.y_<v2.position_.y_;}
	void sortTriangleVector2( Vertex &v1, Vertex &v2, Vertex &v3);
	void drawScanLine( Vertex &v1, Vertex &v2);
	void drawTriangleBottomFlat( Vertex &v1, Vertex &v2, Vertex &v3);
	void drawTriangleTopFlat( Vertex &v1, Vertex &v2, Vertex &v3);
	void drawTriangle( Vertex &v1, Vertex &v2, Vertex &v3);

	//绘制线框模型
	void draw_wireframe_model(Model& model,const set<int> &remove_vertex_index,const set<int> &remove_triangle_index);
	void draw_mesh_model(Model& model,const set<int> &remove_vertex_index,const set<int> &remove_triangle_index);
	//获取设备
	IDirect3DDevice9* getDevice();

	//获取缓冲区
	IDirect3DSurface9* getSurface();

	DWORD ARGB(int a,int r,int g,int b){return DWORD((a<<24)+(r<<16)+(g<<8)+b);}
	//析构
	~DirectX();
	D3DLOCKED_RECT LockRect;
private:
	DirectX():pD3DXDevice(nullptr),pD3DSurface(nullptr),camera(Camera::instance()){}
	IDirect3DDevice9* pD3DXDevice;
	IDirect3DSurface9* pD3DSurface;
	Camera &camera;
	int width_;
	int height_;
	Texture *p_texture;
	 

};


#endif