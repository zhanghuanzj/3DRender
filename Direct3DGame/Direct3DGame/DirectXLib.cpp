#include "DirectXLib.h"

int DirectX::CMID(int x, int min, int max) { return (x < min)? min : ((x > max)? max : x); }

bool DirectX::initialDirectX(HINSTANCE hInstance, HWND hWnd, int width, int height)
{
	//1.�����ӿ�
	IDirect3D9* d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

	//2.��ȡӲ����Ϣ��ȷ�����㴦��ʽ
	D3DCAPS9 caps;
	int vertexProcessing;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,&caps);    //(��ǰ�Կ���Ӳ���豸)
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		vertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		vertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	//3.��дD3D����
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

	//4.�����豸
	d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, vertexProcessing, &d3dpp, &pD3DXDevice);
	d3d9->Release();
	//����һ�����Ʊ���
	pD3DXDevice->CreateOffscreenPlainSurface(width, height, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pD3DSurface, 0);
	

	//������Ļ���ߡ�Z��������С
	width_ = width;
	height_ = height;
	buffer_size_ = width_*height_;

	//z_buffer���ٿռ�
	z_buffer_ = new float[buffer_size_];
	return true;
}

/************************************************************************/
/* ��Ļ���ˢ�£�Z�����ʼ��
*/
/************************************************************************/
void DirectX::fillSurface()
{
	pD3DXDevice->ColorFill(pD3DSurface, NULL, D3DCOLOR_XRGB(0,0,0));
	fill(z_buffer_, z_buffer_ + buffer_size_, FLT_MAX);
}

void DirectX::lockSurface()
{
	// ��������ʼ����������
	memset(&LockRect, 0, sizeof(LockRect));
	// ����
	pD3DSurface->LockRect(&LockRect, NULL, D3DLOCK_DISCARD);

}

void DirectX::unlockSurface()
{
	// ����
	pD3DSurface->UnlockRect();
}

void DirectX::drawPixel(int x, int y, AColor color, float depth)
{
	if (x < 0 || x > width_ || y < 0 || y > height_)
	{
		return;
	}
	/* ������ɫ
	Pointer to the locked bits. If a RECT was provided to the LockRect call, 
	pBits will be appropriately offset from the start of the surface.*/
	int index = x * width_ + y;
	if (z_buffer_[index] > depth)
	{
		DWORD* pBits = (DWORD*)LockRect.pBits;
		pBits[x + y * (LockRect.Pitch >> 2)] = ARGB( color.a_, color.r_, color.g_, color.b_); 
		z_buffer_[index] = depth;
	}
}

/************************************************************************/
/* ����ֱ��(�е㷨)
 * ��ʽ����f(x,y)=(y0-y1)x+(x1-x0)y+x0y1-x1y0=0
 */
/************************************************************************/
void DirectX::drawLine(int x1, int y1, int x2, int y2, AColor color)
{
	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1);
	if (dx >= dy)
	{
		if (x1 > x2)
		{
			swap(x1, x2);
			swap(y1, y2);
		}
		float A = y2 - y1;
		float B = x1 - x2;
		float C = x2 * y1 - x1 * y2;
		int incrementY = (y2 > y1) ? 1 : -1;
		for (int x = x1, y = y1; x <= x2; ++x)
		{
			drawPixel(x, y, color);
			float k = A * (x + 1) + B * (y + incrementY) + C;
			if (k * incrementY >= 0)
			{
				y += incrementY;
			}
		}
	}
	else
	{
		if (y1 > y2)
		{
			swap(x1, x2);
			swap(y1, y2);
		}
		float A = y2 - y1;
		float B = x1 - x2;
		float C = x2 * y1 - x1 * y2;
		int incrementX = (x2 > x1) ? 1 : -1;
		for (int x = x1, y = y1; y <= y2; ++y)
		{
			drawPixel(x, y, color);
			float k = A * (x + incrementX) + B * (y + 1) + C;
			if (k * incrementX <= 0)
			{
				x += incrementX;
			}
		}
	}	
}

/************************************************************************/
/* �������ζ�����y��С��������                                            */
/************************************************************************/
void DirectX::sortTriangleVector2(Vertex &v1, Vertex &v2, Vertex &v3)
{
	if (v1.position_.y_ > v2.position_.y_)
	{
		swap(v1, v2);
	}
	if(v3.position_.y_ < v2.position_.y_)
	{
		swap(v2, v3);
	}
	if (v1.position_.y_ > v2.position_.y_)
	{
		swap(v1, v2);
	}
}

/************************************************************************/
/* ���Ʋ�ֵ����ɫ�仯��ֱ��                                               */
/************************************************************************/
void DirectX::drawScanLine(Vertex &v1, Vertex &v2)
{
	if (v1.position_.x_ > v2.position_.x_)
	{
		swap(v1, v2);
	}

	//���ɼӷ����ٶȿ��˺ö�
	int x_start = v1.position_.x_ + 0.5;
	int x_end = v2.position_.x_ + 0.5;
	bool isZero = x_end - x_start ? false : true;
	float df = 1.0f / (x_end - x_start);
	//depth
	float dd = (v2.position_.w_ - v1.position_.w_) * df;
	float one_depth = v1.position_.w_;
	//u,v
	float w = v1.position_.w_;
	float dw = (v2.position_.w_ - v1.position_.w_) * df;

	float uw = v1.u_ * v1.position_.w_;
	float vw = v1.v_ * v1.position_.w_;
	float duw = (v2.u_ * v2.position_.w_-v1.u_ * v1.position_.w_) * df;
	float dvw = (v2.v_ * v2.position_.w_ - v1.v_ * v1.position_.w_) * df;

	//light
	float r = v1.light_.x_;
	float dr = (v2.light_.x_ - v1.light_.x_) * df;
	float g = v1.light_.y_;
	float dg = (v2.light_.y_ - v1.light_.y_) * df;
	float b = v1.light_.z_;
	float db = (v2.light_.z_ - v1.light_.z_) * df;
	for (int i = x_start; i < x_end; ++i)
	{
		if (render_state_ == TEXTURE)
		{
			AColor color = p_texture->get_color(uw / w, vw / w);
			color.r_ = CMID(color.r_ * r, 0, 255);
			color.g_ = CMID(color.g_ * g, 0, 255);
			color.b_ = CMID(color.b_ * b, 0, 255);
			drawPixel(i,v1.position_.y_, color, 1 / one_depth);
		}
		else
		{
			float factor = min(1.0f, df * (i - x_start));
			AColor color = v1.color_.interp(v2.color_, factor);
			color.r_ = CMID(color.r_ * r, 0, 255);
			color.g_ = CMID(color.g_ * g, 0, 255);
			color.b_ = CMID(color.b_ * b, 0, 255);
			drawPixel(i, v1.position_.y_, color, 1 / one_depth);
		}
		one_depth += dd;
		uw += duw;
		vw += dvw;
		w += dw;
		r += dr;
		g += dg;
		b += db;
	}
}

/************************************************************************/
/* ���Ƶ�ƽ������	 v1Ϊ�϶���	(v2���£�v3����)		
*/
/************************************************************************/
void DirectX::drawTriangleBottomFlat(Vertex &v1, Vertex &v2, Vertex &v3)
{
	if (v2.position_.x_ > v3.position_.x_)
	{
		swap(v2, v3);
	}
	int startY = v1.position_.y_ + 0.5;
	int endY = v2.position_.y_ + 0.5;
	for (int y = startY; y < endY; y++)
	{
		float factor = 0;
		if (startY - endY != 0)
		{
			factor = (float(float(y) + 0.5 - v1.position_.y_)) / (v2.position_.y_ - v1.position_.y_);
		}
		Vertex vl = v1.interp(v2,factor);
		Vertex vr = v1.interp(v3,factor);
		
		drawScanLine(vl,vr);
	}
}

/************************************************************************/
/* ���ƶ�ƽ������		v3Ϊ�׶���	(v1���ϣ�v2����)		
*/
/************************************************************************/
void DirectX::drawTriangleTopFlat(Vertex &v1, Vertex &v2, Vertex &v3)
{
	if (v1.position_.x_ > v2.position_.x_)
	{
		swap(v1, v2);
	}
	int startY = v2.position_.y_ + 0.5;
	int endY = v3.position_.y_ + 0.5;
	for (int y = startY; y < endY; y++)
	{
		float factor =0;
		if (startY - endY!=0)
		{
			factor = (float(float(y) + 0.5 - v2.position_.y_)) / (v3.position_.y_ - v2.position_.y_);
		}
		Vertex vl = v1.interp(v3, factor);
		Vertex vr = v2.interp(v3, factor);
		drawScanLine(vl, vr);
	}
}

/************************************************************************/
/* ����������
*/
/************************************************************************/
void DirectX::drawTriangle(Vertex &v1, Vertex &v2, Vertex &v3)
{
	sortTriangleVector2(v1, v2, v3);
	if (v1.isxy_same(v2) && v2.isxy_same(v3))
	{
		drawPixel(v1.position_.x_, v1.position_.y_, v1.color_);
	}
	else if (v1.isxy_same(v2))
	{
		drawLine(v1.position_.x_, v1.position_.y_, v3.position_.x_, v3.position_.y_, v1.color_);
	}
	else if(v1.isxy_same(v3))
	{
		drawLine(v1.position_.x_, v1.position_.y_, v2.position_.x_, v2.position_.y_, v1.color_);
	}
	else if (v2.isxy_same(v3))
	{
		drawLine(v1.position_.x_, v1.position_.y_, v3.position_.x_, v3.position_.y_, v1.color_);
	}
	else
	{
		if (v1.position_.y_ == v2.position_.y_)
		{
			drawTriangleTopFlat(v1, v2, v3);
		}
		else if (v2.position_.y_ == v3.position_.y_)
		{
			drawTriangleBottomFlat(v1, v2, v3);
		}
		else
		{
			float factor = (v2.position_.y_ - v1.position_.y_) / (v3.position_.y_ - v1.position_.y_);
			Vertex v4 = v1.interp(v3, factor);
			drawTriangleBottomFlat(v1, v2, v4);
			drawTriangleTopFlat(v2, v4, v3);
		}
	}
}

bool DirectX::is_out(TrangleIndex &triangle, const set<int> &remove_vertex_index)
{
	int count = 0;
	for (auto i : triangle.indices)
	{
		if (remove_vertex_index.count(i))
		{
			++count;
		}
	}
	return count == 3;
}

void DirectX::draw_wireframe_model(Model& model, const set<int> &remove_vertex_index, const set<int> &remove_triangle_index)
{
	int index = 0;
	for (int index = 0; index < model.poly_indices_.size(); ++index)
	{
		//�޳�������׶��
		if (remove_triangle_index.count(index) || is_out(model.poly_indices_[index], remove_vertex_index))
		{
			continue;
		}
		Vertex v1 = model.trans_vertexes_[model.poly_indices_[index].indices[0]];
		Vertex v2 = model.trans_vertexes_[model.poly_indices_[index].indices[1]];
		Vertex v3 = model.trans_vertexes_[model.poly_indices_[index].indices[2]];
		drawLine(v1.position_.x_, v1.position_.y_, v2.position_.x_, v2.position_.y_, AColor(0, 255, 0, 0));
		drawLine(v3.position_.x_, v3.position_.y_, v2.position_.x_, v2.position_.y_, AColor(0, 255, 0, 0));
		drawLine(v1.position_.x_, v1.position_.y_, v3.position_.x_, v3.position_.y_, AColor(0, 255, 0, 0));
	}
}

void DirectX::draw_mesh_model(Model& model, const set<int> &remove_vertex_index, const set<int> &remove_triangle_index)
{
	p_texture = model.texture_;
	int index = 0;
	for (int index = 0; index < model.poly_indices_.size(); ++index)
	{
		//�޳�������׶��
		if (remove_triangle_index.count(index) || is_out(model.poly_indices_[index], remove_vertex_index))
		{
			continue;
		}
		Vertex v1 = model.trans_vertexes_[model.poly_indices_[index].indices[0]];
		Vertex v2 = model.trans_vertexes_[model.poly_indices_[index].indices[1]];
		Vertex v3 = model.trans_vertexes_[model.poly_indices_[index].indices[2]];
		
		drawTriangle(v1, v2, v3);
		//drawLine(v1.position_.x_,v1.position_.y_,v2.position_.x_,v2.position_.y_,Color(0,255,0,0));
		//drawLine(v3.position_.x_,v3.position_.y_,v2.position_.x_,v2.position_.y_,Color(0,255,0,0));
		//drawLine(v1.position_.x_,v1.position_.y_,v3.position_.x_,v3.position_.y_,Color(0,255,0,0));
	}
}


void DirectX::flipSurface()
{
	// ��ȡ��̨����
	IDirect3DSurface9* backBuffer = 0;
	pD3DXDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);

	// ʹ���Զ����������̨����
	pD3DXDevice->StretchRect(pD3DSurface, 0, backBuffer, 0, D3DTEXF_NONE);

	// GetBackBuffer���õĻ�����Ҫ���ͷţ�������ڴ�й¶
	backBuffer->Release();

	// ���������еĺ�̨������ʾ
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
	if (pD3DSurface != NULL)
	{
		pD3DSurface->Release();
	}
	if (pD3DXDevice != NULL)
	{
		pD3DXDevice->Release();
	}
}