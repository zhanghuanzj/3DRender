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
	int index = x*(width_)+y;
	if (z_buffer_[index]>depth)
	{
		DWORD* pBits = (DWORD*)LockRect.pBits;
		pBits[x + y * (LockRect.Pitch >> 2)] = ARGB( color.a_ , color.r_ , color.g_ , color.b_ ); 
		z_buffer_[index] = depth;
	}

}

/************************************************************************/
/* 绘制直线(中点法)
 * 隐式方程f(x,y)=(y0-y1)x+(x1-x0)y+x0y1-x1y0=0
 */
/************************************************************************/
void DirectX::drawLine(int x1,int y1,int x2,int y2,AColor color)
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
		float A = y2-y1;
		float B = x1-x2;
		float C = x2*y1-x1*y2;
		int incrementY = (y2>y1)?1:-1;
		for (int x=x1,y=y1;x<=x2;++x)
		{
			drawPixel(x,y,color);
			float k = A*(x+1)+B*(y+incrementY)+C;
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
		float A = y2-y1;
		float B = x1-x2;
		float C = x2*y1-x1*y2;
		int incrementX = (x2>x1)?1:-1;
		for (int x=x1,y=y1;y<=y2;++y)
		{
			drawPixel(x,y,color);
			float k = A*(x+incrementX)+B*(y+1)+C;
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
void DirectX::sortTriangleVector2( Vertex &v1, Vertex &v2, Vertex &v3)
{
	if (v1.position_.y_>v2.position_.y_)
	{
		swap(v1,v2);
	}
	if(v3.position_.y_<v2.position_.y_)
	{
		swap(v2,v3);
	}
	if (v1.position_.y_>v2.position_.y_)
	{
		swap(v1,v2);
	}
}
/************************************************************************/
/* 绘制插值（颜色变化）直线                                               */
/************************************************************************/
void DirectX::drawScanLine( Vertex &v1, Vertex &v2)
{
	if (v1.position_.x_>v2.position_.x_)
	{
		swap(v1,v2);
	}
	int x_start = v1.position_.x_+0.5;
	int x_end = v2.position_.x_+0.5;
	bool isZero = x_end-x_start?false:true;

	for (int i=x_start;i<=x_end;++i)
	{
		float factor = 0;
		if (!isZero)
		{
			factor = (float(i-x_start))/(x_end-x_start);
		}
		Vertex v = v1.interp(v2,factor);
		if (render_state_==TEXTURE)
		{
			drawPixel(i,v.position_.y_,p_texture->get_color(v.u_,v.v_),1/v.position_.w_);
		}
		else
		{
			drawPixel(i,v.position_.y_,v.color_,1/v.position_.w_);
		}
		
	}
}
/************************************************************************/
/* 绘制底平三角形	 v1为上顶点	(v2左下，v3右下)		
*/
/************************************************************************/
void DirectX::drawTriangleBottomFlat( Vertex &v1, Vertex &v2, Vertex &v3)
{
	if (v2.position_.x_>v3.position_.x_)
	{
		swap(v2,v3);
	}
	int startY = v1.position_.y_+0.5;
	int endY = v2.position_.y_+0.5;
	for (int y=startY;y<endY;y++)
	{
		float factor = 0;
		if (startY-endY!=0)
		{
			factor = (float(float(y)+0.5-v1.position_.y_))/(v2.position_.y_-v1.position_.y_);
		}
		Vertex vl = v1.interp(v2,factor);
		Vertex vr = v1.interp(v3,factor);
		
		drawScanLine(vl,vr);
	}
}

/************************************************************************/
/* 绘制顶平三角形		v3为底顶点	(v1左上，v2右上)		
*/
/************************************************************************/
void DirectX::drawTriangleTopFlat(Vertex &v1, Vertex &v2, Vertex &v3)
{
	if (v1.position_.x_>v2.position_.x_)
	{
		swap(v1,v2);
	}
	int startY = v2.position_.y_+0.5;
	int endY = v3.position_.y_+0.5;
	for (int y=startY;y<endY;y++)
	{
		float factor =0;
		if (startY-endY!=0)
		{
			factor = (float(float(y)+0.5-v2.position_.y_))/(v3.position_.y_-v2.position_.y_);
		}
		Vertex vl = v1.interp(v3,factor);
		Vertex vr = v2.interp(v3,factor);
		drawScanLine(vl,vr);
	}
}

/************************************************************************/
/* 绘制三角形
*/
/************************************************************************/
void DirectX::drawTriangle( Vertex &v1, Vertex &v2, Vertex &v3)
{
	sortTriangleVector2(v1,v2,v3);
	if (v1.isxy_same(v2)&&v2.isxy_same(v3))
	{
		drawPixel(v1.position_.x_,v1.position_.y_,v1.color_);
	}
	else if (v1.isxy_same(v2))
	{
		drawLine(v1.position_.x_,v1.position_.y_,v3.position_.x_,v3.position_.y_,v1.color_);
	}
	else if(v1.isxy_same(v3))
	{
		drawLine(v1.position_.x_,v1.position_.y_,v2.position_.x_,v2.position_.y_,v1.color_);
	}
	else if (v2.isxy_same(v3))
	{
		drawLine(v1.position_.x_,v1.position_.y_,v3.position_.x_,v3.position_.y_,v1.color_);
	}
	else
	{
		if (v1.position_.y_==v2.position_.y_)
		{
			drawTriangleTopFlat(v1,v2,v3);
		}
		else if (v2.position_.y_==v3.position_.y_)
		{
			drawTriangleBottomFlat(v1,v2,v3);
		}
		else
		{
			float factor = (v2.position_.y_-v1.position_.y_)/(v3.position_.y_-v1.position_.y_);
			Vertex v4 = v1.interp(v3,factor);
			drawTriangleBottomFlat(v1,v2,v4);
			drawTriangleTopFlat(v2,v4,v3);
		}
	}
}

bool DirectX::is_out(TrangleIndex &triangle,const set<int> &remove_vertex_index)
{
	for (auto i : triangle.indices)
	{
		if (remove_vertex_index.count(i))
		{
			return true;
		}
	}
	return false;
}

void DirectX::draw_wireframe_model(Model& model,const set<int> &remove_vertex_index,const set<int> &remove_triangle_index)
{

	int index = 0;
	for (int index=0;index<model.poly_indices_.size();++index)
	{
		//剔除或不在视锥内
		if (remove_triangle_index.count(index)||is_out(model.poly_indices_[index],remove_vertex_index))
		{
			continue;
		}
		Vertex v1 = model.trans_vertexes_[model.poly_indices_[index].indices[0]];
		Vertex v2 = model.trans_vertexes_[model.poly_indices_[index].indices[1]];
		Vertex v3 = model.trans_vertexes_[model.poly_indices_[index].indices[2]];
		drawLine(v1.position_.x_,v1.position_.y_,v2.position_.x_,v2.position_.y_,AColor(0,255,0,0));
		drawLine(v3.position_.x_,v3.position_.y_,v2.position_.x_,v2.position_.y_,AColor(0,255,0,0));
		drawLine(v1.position_.x_,v1.position_.y_,v3.position_.x_,v3.position_.y_,AColor(0,255,0,0));
	}
}

void DirectX::draw_mesh_model(Model& model,const set<int> &remove_vertex_index,const set<int> &remove_triangle_index)
{
	p_texture = model.texture_;
	int index = 0;
	for (int index=0;index<model.poly_indices_.size();++index)
	{
		//剔除或不在视锥内
		if (remove_triangle_index.count(index)||is_out(model.poly_indices_[index],remove_vertex_index))
		{
			continue;
		}
		Vertex v1 = model.trans_vertexes_[model.poly_indices_[index].indices[0]];
		Vertex v2 = model.trans_vertexes_[model.poly_indices_[index].indices[1]];
		Vertex v3 = model.trans_vertexes_[model.poly_indices_[index].indices[2]];
		
		drawTriangle(v1,v2,v3);
		//drawLine(v1.position_.x_,v1.position_.y_,v2.position_.x_,v2.position_.y_,Color(0,255,0,0));
		//drawLine(v3.position_.x_,v3.position_.y_,v2.position_.x_,v2.position_.y_,Color(0,255,0,0));
		//drawLine(v1.position_.x_,v1.position_.y_,v3.position_.x_,v3.position_.y_,Color(0,255,0,0));
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