#include "DirectXLib.h"


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
	d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pD3DXDevice);
	d3d9->Release();

	pD3DXDevice->CreateOffscreenPlainSurface(width, height, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pD3DSurface, 0);
	

	//保存屏幕宽，高
	width_ = width;
	height_ = height;

	return true;
}

void DirectX::fillSurface()
{
	pD3DXDevice->ColorFill(pD3DSurface,NULL,D3DCOLOR_XRGB(0,0,0));
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
void DirectX::drawPixel(int x,int y, Color color)
{
	/* 像素着色
	Pointer to the locked bits. 
	If a RECT was provided to the LockRect call, 
	pBits will be appropriately offset from the start of the surface.*/
	DWORD* pBits = (DWORD*)LockRect.pBits;
	pBits[x + y * (LockRect.Pitch >> 2)] = ARGB( color.a_ , color.r_ , color.g_ , color.b_ );      

}

/************************************************************************/
/* 绘制直线(中点法)
 * 隐式方程f(x,y)=(y0-y1)x+(x1-x0)y+x0y1-x1y0=0
 */
/************************************************************************/
void DirectX::drawLine(int x1,int y1,int x2,int y2,Color color)
{
	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1);
	if (dx>=dy)
	{
		if (x1>x2)
		{
			swap(x1,x2);
			swap(y1,y2);
		}
		double A = y2-y1;
		double B = x1-x2;
		double C = x2*y1-x1*y2;
		int incrementY = (y2>y1)?1:-1;
		for (int x=x1,y=y1;x<=x2;++x)
		{
			drawPixel(x,y,color);
			double k = A*(x+1)+B*(y+incrementY)+C;
			if (k*incrementY>=0)
			{
				y += incrementY;
			}
		}
	}
	else
	{
		if (y1>y2)
		{
			swap(x1,x2);
			swap(y1,y2);
		}
		double A = y2-y1;
		double B = x1-x2;
		double C = x2*y1-x1*y2;
		int incrementX = (x2>x1)?1:-1;
		for (int x=x1,y=y1;y<=y2;++y)
		{
			drawPixel(x,y,color);
			double k = A*(x+incrementX)+B*(y+1)+C;
			if (k*incrementX<=0)
			{
				x += incrementX;
			}
		}
	}	
}

/************************************************************************/
/* 对三角形顶点以y从小到大排序                                            */
/************************************************************************/
void DirectX::sortTriangleVector2( Vector2 &v1, Vector2 &v2, Vector2 &v3)
{
	if (v1.y>v2.y)
	{
		swap(v1,v2);
	}
	if (v3.y<v1.y)
	{
		swap(v1,v3);
		swap(v2,v3);
	}
	else if(v3.y<v2.y)
	{
		swap(v2,v3);
	}
}

/************************************************************************/
/* 绘制插值（颜色变化）直线                                               */
/************************************************************************/
void DirectX::drawScanLine( Vector2 &v1, Vector2 &v2)
{
	if (v1.x>v2.x)
	{
		swap(v1,v2);
	}
	int x_end = v2.x;
	Color color = v1.color;
	Color d_color = v2.color-v1.color;
	for (int x=v1.x;x<=x_end;++x)
	{
		drawPixel(x,v1.y,color);
		color = v1.color + d_color*(x-v1.x)/(v2.x-v1.x); 
	}
}
/************************************************************************/
/* 绘制底平三角形	 v1为上顶点												*/
/************************************************************************/
void DirectX::drawTriangleBottomFlat( Vector2 &v1, Vector2 &v2, Vector2 &v3)
{
	int startY = v1.y;
	int endY = v2.y;
	double LX = v1.x,RX = v1.x;
	double ldx = (v2.x - v1.x) / (v2.y - v1.y);
	double rdx = (v3.x - v1.x) / (v3.y - v1.y);
	Color d_left_color = v2.color-v1.color;
	Color d_right_color = v3.color-v1.color;
	for (;startY<=endY;++startY)
	{
		double factor = (startY-v1.y)/(endY-v1.y);
		Color  co = d_left_color*factor;
		Color left_color = v1.color + d_left_color*factor;
		Color right_color = v1.color + d_right_color*factor;
		drawScanLine(Vector2(LX,startY,left_color),Vector2(RX,startY,right_color));
		//drawScanLine(Vector2(LX,startY,Color(0,0,255,0)),Vector2(RX,startY,Color(0,0,0,0)));
		LX += ldx;
		RX += rdx;
	}
}

/************************************************************************/
/* 绘制顶平三角形		v3为底顶点											*/
/************************************************************************/
void DirectX::drawTriangleTopFlat(Vector2 &v1, Vector2 &v2, Vector2 &v3)
{
	int startY = v3.y;
	int endY = v2.y;
	double LX = v3.x,RX = v3.x;
	double ldx = ((double)(v3.x - v1.x)) / (v3.y - v1.y);
	double rdx = ((double)(v3.x - v2.x)) / (v3.y - v2.y);
	for (;startY>=endY;--startY)
	{
		double factor = (v3.y-startY)/(v3.y-endY);
		Color left_color = v3.color + (v1.color-v3.color)*factor;
		Color right_color = v3.color + (v2.color-v3.color)*factor;
		drawScanLine(Vector2(LX,startY,left_color),Vector2(RX,startY,right_color));
		LX -= ldx;
		RX -= rdx;
	}
}

void DirectX::drawTriangle( Vector2 &v1, Vector2 &v2, Vector2 &v3)
{
	sortTriangleVector2(v1,v2,v3);
	if (v1==v2&&v2==v3)
	{
		drawPixel(v1.x,v2.x,v1.color);
	}
	else if (v1==v2)
	{
		drawLine(v1.x,v1.y,v3.x,v3.y,v1.color);
	}
	else if(v1==v3)
	{
		 drawLine(v1.x,v1.y,v2.x,v2.y,v1.color);
	}
	else if (v2==v3)
	{
		drawLine(v1.x,v1.y,v3.x,v3.y,v1.color);
	}
	else
	{
		if (v1.y==v2.y)
		{
			drawTriangleTopFlat(v1,v2,v3);
		}
		else if (v2.y==v3.y)
		{
			drawTriangleBottomFlat(v1,v2,v3);
		}
		else
		{
			Color color = v3.color + (v1.color-v3.color)*(v2.y-v3.y)/(v1.y-v3.y);
			Vector2 v4(v1.x+(double(v2.y-v1.y)/(v3.y-v1.y))*(v3.x-v1.x),v2.y,color);
			drawTriangleBottomFlat(v1,v2,v4);
			drawTriangleTopFlat(v2,v4,v3);
		}
	}
}

void DirectX::draw_wireframe_model(Model& model)
{


	for (auto v :model.poly_indices_)
	{
		drawLine(model.trans_vertexes_[v.indices[0]].x_,model.trans_vertexes_[v.indices[0]].y_,model.trans_vertexes_[v.indices[1]].x_,model.trans_vertexes_[v.indices[1]].y_,Color(0,255,0,0));
		drawLine(model.trans_vertexes_[v.indices[2]].x_,model.trans_vertexes_[v.indices[2]].y_,model.trans_vertexes_[v.indices[1]].x_,model.trans_vertexes_[v.indices[1]].y_,Color(0,255,0,0));
		drawLine(model.trans_vertexes_[v.indices[0]].x_,model.trans_vertexes_[v.indices[0]].y_,model.trans_vertexes_[v.indices[2]].x_,model.trans_vertexes_[v.indices[2]].y_,Color(0,255,0,0));
	}
}

void DirectX::flipSurface()
{
	// 获取后台缓存
	IDirect3DSurface9* backBuffer = 0;
	pD3DXDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);

	// 使用自定义表面填充后台缓存
	pD3DXDevice->StretchRect(pD3DSurface, 0, backBuffer, 0, D3DTEXF_NONE);

	// GetBackBuffer所得的缓存需要被释放，否则会内存泄露
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