#include "DirectXLib.h"

int DirectX::CMID(int x, int min, int max) 
{ 
	return (x < min)? min : ((x > max)? max : x); 
}

bool DirectX::initialDirectX(HINSTANCE hInstance, HWND hWnd, int width, int height)
{
	//1.创建接口
	IDirect3D9* d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

	//2.获取硬件信息，确定顶点处理方式
	D3DCAPS9 caps;
	int vertexProcessing;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,&caps);    //(当前显卡，硬件设备)
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		vertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		vertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	//3.填写D3D参数
	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth            = width;
	d3dpp.BackBufferHeight           = height;
	d3dpp.BackBufferFormat           = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount            = 1;
	d3dpp.MultiSampleType            = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality         = 0;
	d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow              = hWnd;
	d3dpp.Windowed                   = true;
	d3dpp.EnableAutoDepthStencil     = true;
	d3dpp.AutoDepthStencilFormat     = D3DFMT_D24S8;
	d3dpp.Flags                      = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;

	//4.创建设备
	d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, vertexProcessing, &d3dpp, &pD3DXDevice);
	d3d9->Release();
	//创建一个绘制表面
	pD3DXDevice->CreateOffscreenPlainSurface(width, height, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pD3DSurface, 0);
	

	//保存屏幕宽、高、Z缓冲区大小
	width_ = width;
	height_ = height;
	buffer_size_ = width_*height_;

	//z_buffer开辟空间
	z_buffer_ = new float[buffer_size_];
	return true;
}

/************************************************************************/
/* 屏幕填充刷新，Z缓冲初始化
*/
/************************************************************************/
void DirectX::fillSurface()
{
	pD3DXDevice->ColorFill(pD3DSurface,NULL,D3DCOLOR_XRGB(0,0,0));
	fill(z_buffer_,z_buffer_+buffer_size_,FLT_MAX);
}

void DirectX::lockSurface()
{
	// 创建并初始化锁定区域
	memset(&LockRect, 0, sizeof(LockRect));
	// 锁定
	pD3DSurface->LockRect(&LockRect,NULL,D3DLOCK_DISCARD);

}

void DirectX::unlockSurface()
{
	// 解锁
	pD3DSurface->UnlockRect();
}

void DirectX::drawPixel(int x,int y, AColor color,float depth)
{
	/* 像素着色
	Pointer to the locked bits. If a RECT was provided to the LockRect call, 
	pBits will be appropriately offset from the start of the surface.*/
	int index = y*(width_)+x;
	if (z_buffer_[index]>depth)
	{
		DWORD* pBits = (DWORD*)LockRect.pBits;
		pBits[x + y * (LockRect.Pitch >> 2)] = ARGB( color.a_ , color.r_ , color.g_ , color.b_ ); //LockRect.Pitch表示WIDTH所占的字节数
		z_buffer_[index] = depth;
	}

}


/************************************************************************/
/*                    Swap FrameBuffer                                  */
/************************************************************************/
void DirectX::flipSurface()
{
	// 获取后台缓存
	IDirect3DSurface9* backBuffer = 0;
	pD3DXDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);

	// 使用自定义表面填充后台缓存
	pD3DXDevice->StretchRect(pD3DSurface, 0, backBuffer, 0, D3DTEXF_NONE);

	// 减少引用计数，否则会内存泄露
	backBuffer->Release();

	// 将交换链中的后台缓存显示
	pD3DXDevice->Present(NULL, NULL, NULL, NULL);
}

IDirect3DDevice9* DirectX::getDevice()
{
	return pD3DXDevice;
}

IDirect3DSurface9* DirectX::getSurface()
{
	return pD3DSurface;
}

DirectX::~DirectX()
{
	if (pD3DSurface!=NULL)
	{
		pD3DSurface->Release();
	}
	if (pD3DXDevice!=NULL)
	{
		pD3DXDevice->Release();
	}
}