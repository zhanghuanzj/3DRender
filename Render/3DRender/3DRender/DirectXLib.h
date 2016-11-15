#ifndef DIRECTXLIB_H_
#define DIRECTXLIB_H_

#include <d3d9.h>
#include <d3dx9.h>
#include <windows.h>
#include <iostream>

#include "Enum.h"
#include "Math.h"

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
	void drawPixel(int x,int y,AColor color,float depth=0.0f);

	//缓冲区翻转显示
	void flipSurface();

	//设置渲染模式
	void set_render_state(RenderState render_sate){render_state_=render_sate;}


	DWORD ARGB(int a,int r,int g,int b){return DWORD((a<<24)+(r<<16)+(g<<8)+b);}
	//析构
	~DirectX();

private:
	DirectX():pD3DXDevice(nullptr),pD3DSurface(nullptr),render_state_(RenderState::COLOR){}

	IDirect3DDevice9* pD3DXDevice;
	IDirect3DSurface9* pD3DSurface;
	D3DLOCKED_RECT LockRect;

	float *z_buffer_;
	int width_;
	int height_;
	int buffer_size_;
	RenderState render_state_;

};


#endif