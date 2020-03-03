#include "DirectXLib.h"

bool DirectX::initialDirectX(HINSTANCE hInstance,  HWND hWnd,  int width,  int height)
{
	//1.�����ӿ�
	IDirect3D9* d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

	//2.��ȡӲ����Ϣ��ȷ�����㴦��ʽ
	D3DCAPS9 caps;
	int vertexProcessing;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);    //(��ǰ�Կ���Ӳ���豸)
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
	d3dpp.BackBufferCount            = 2;
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
	d3d9->CreateDevice(D3DADAPTER_DEFAULT,  D3DDEVTYPE_REF,  hWnd,  vertexProcessing,  &d3dpp,  &pD3DXDevice);
	d3d9->Release();
	//����һ�����Ʊ���
	pD3DXDevice->CreateOffscreenPlainSurface(width,  height,  D3DFMT_A8R8G8B8,  D3DPOOL_DEFAULT,  &pD3DSurface,  0);
	

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
	pD3DXDevice->ColorFill(pD3DSurface, NULL, D3DCOLOR_XRGB(30, 30, 30));
	fill(z_buffer_, z_buffer_+buffer_size_, FLT_MAX);
}

void DirectX::lockSurface()
{
	// ��������ʼ����������
	memset(&LockRect,  0,  sizeof(LockRect));
	// ����
	pD3DSurface->LockRect(&LockRect, NULL, D3DLOCK_DISCARD);

}

void DirectX::unlockSurface()
{
	// ����
	pD3DSurface->UnlockRect();
}


/************************************************************************/
/*                    Swap FrameBuffer                                  */
/************************************************************************/
void DirectX::flipSurface()
{
	// ��ȡ��̨����
	IDirect3DSurface9* backBuffer = 0;
	pD3DXDevice->GetBackBuffer(0,  0,  D3DBACKBUFFER_TYPE_MONO,  &backBuffer);

	// ʹ���Զ����������̨����
	pD3DXDevice->StretchRect(pD3DSurface,  0,  backBuffer,  0,  D3DTEXF_NONE);

	// �������ü�����������ڴ�й¶
	backBuffer->Release();

	// ���������еĺ�̨������ʾ
	pD3DXDevice->Present(NULL,  NULL,  NULL,  NULL);
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