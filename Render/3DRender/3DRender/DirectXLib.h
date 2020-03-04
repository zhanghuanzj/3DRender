#ifndef DIRECTXLIB_H_
#define DIRECTXLIB_H_

#include <d3d9.h>
#include <d3dx9.h>
#include <windows.h>
#include <iostream>

#include "Enum.h"
#include "Math.h"
#include "VColor.h"

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
	bool initialDirectX(HINSTANCE hInstance,  HWND hWnd,  int width,  int height);
	void fillSurface();
	void lockSurface();
	void unlockSurface();

	inline bool deepTest(int x, int y, float depth)
	{
		int index = y * (width_)+x;
		return z_buffer_[index] > depth;
	}

	//绘制像素到缓冲区
	inline void drawPixel(int x, int y, VColor color, float depth=0.0f)
	{
		/* 像素着色
		Pointer to the locked bits. If a RECT was provided to the LockRect call,
		pBits will be appropriately offset from the start of the surface.*/
		DWORD* pBits = (DWORD*)LockRect.pBits;
		int r = Clamp(color.r * 255, 0, 255);
		int g = Clamp(color.g * 255, 0, 255);
		int b = Clamp(color.b * 255, 0, 255);
		int a = color.a * 255;
		if (a > 255 || b > 255 || r > 255)
		{
			cout << a << endl;
		}
		pBits[x + y * (LockRect.Pitch >> 2)] = DWORD((a << 24) + (r << 16) + (g << 8) + b);//LockRect.Pitch表示WIDTH所占的字节数
		int index = y * (width_)+x;
		z_buffer_[index] = depth;
	}

	//缓冲区翻转显示
	void flipSurface();


	DWORD ARGB(int a, int r, int g, int b){return DWORD((a<<24)+(r<<16)+(g<<8)+b);}
	//析构
	~DirectX();

private:
	DirectX():pD3DXDevice(nullptr), pD3DSurface(nullptr){}

	IDirect3DDevice9* pD3DXDevice;
	IDirect3DSurface9* pD3DSurface;
	D3DLOCKED_RECT LockRect;

	float *z_buffer_;
	int width_;
	int height_;
	int buffer_size_;

};


#endif