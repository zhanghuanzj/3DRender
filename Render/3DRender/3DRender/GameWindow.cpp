#include "GameWindow.h"

static POINT center;

GameWindow::GameWindow(const string name_t,const string title_t,int width,int height):name(name_t),title(title_t),WIDTH(width),HEIGHT(height)
{
	//1.创建窗口类
	WNDCLASSEX wndClass = {};
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = GetModuleHandle(NULL);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = name.c_str();

	//2.注册窗口类
	assert(RegisterClassEx(&wndClass));

	//3.创建窗口
	hwnd = CreateWindow(name.c_str(),title.c_str(),WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,WIDTH,HEIGHT,NULL,NULL,wndClass.hInstance,NULL);

	//4.调整大小，移动，显示，更新
	RECT window_rect = {0,0,WIDTH,HEIGHT};
	AdjustWindowRectEx(&window_rect, GetWindowStyle(hwnd), GetMenu(hwnd) != NULL, GetWindowExStyle(hwnd));
	MoveWindow(hwnd,300,150,window_rect.right-window_rect.left, window_rect.bottom-window_rect.top,false);
	ShowWindow(hwnd,SW_NORMAL);
	UpdateWindow(hwnd);

	//5.隐藏鼠标，设为屏幕中心
	ShowCursor(false);
	center.x = WIDTH/2;
	center.y = HEIGHT/2;
	ClientToScreen(hwnd,&center);
	SetCursorPos(center.x,center.y);
	//6.限定鼠标在窗口内
	RECT rect;
	GetClientRect(hwnd,&rect);
	POINT left_top;
	left_top.x = rect.left;
	left_top.y = rect.top;
	POINT right_bottom;
	right_bottom.x = rect.right;
	right_bottom.y = rect.bottom;
	ClientToScreen(hwnd,&left_top);
	ClientToScreen(hwnd,&right_bottom);
	rect.left = left_top.x;
	rect.top = left_top.y;
	rect.right = right_bottom.x;
	rect.bottom = right_bottom.y;
	ClipCursor(&rect);
}

void GameWindow::message_dispatch()
{
	//时间初始化
	DWORD curTime = GetTickCount();
	DWORD preTime = GetTickCount();
	//2.消息循环
	MSG msg = {0};
	while (msg.message!=WM_QUIT)
	{
		//获取消息
		if (PeekMessage(&msg,0,NULL,NULL,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			curTime = GetTickCount();
			//cout<<1000.0f/(curTime-preTime)<<endl;
			if (curTime-preTime>30)
			{
				preTime = curTime;
				GameManager::Instance().game_update();
			}
		}
	}
}

LRESULT CALLBACK GameWindow::WndProc(HWND hwnd,UINT message,WPARAM wparam,LPARAM lParam)
{
	float units = 0.05f;
	static POINT point;

	switch (message)
	{
	case WM_DESTROY:
		GameManager::Instance().game_end();
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		if (wparam==VK_ESCAPE)DestroyWindow(hwnd);
		else if (wparam=='L')
		{
			if (Rasterizer::lightState==LightState::ON)
			{
				Rasterizer::lightState=LightState::OFF;
			}
			else
			{
				Rasterizer::lightState=LightState::ON;
			}
		}
		else if (wparam==VK_UP) Camera::instance().move_along_up_vector(units);
		else if (wparam==VK_DOWN) Camera::instance().move_along_up_vector(-units);
		else if (wparam==VK_LEFT) Camera::instance().move_along_right_vector(-units);
		else if (wparam==VK_RIGHT) Camera::instance().move_along_right_vector(units);
		else if (wparam==VK_OEM_PLUS) Camera::instance().move_along_lookat_vector(units);
		else if (wparam==VK_OEM_MINUS) Camera::instance().move_along_lookat_vector(-units);
		else if (wparam==VK_TAB)
		{
			switch (Rasterizer::renderState)
			{
			case RenderState::WIREFRAME:
				Rasterizer::renderState = RenderState::TEXTURE;
				break;
			case RenderState::TEXTURE:
				Rasterizer::renderState = RenderState::COLOR;
				break;
			case RenderState::COLOR:
				Rasterizer::renderState = RenderState::WIREFRAME;
				break;
			default:
				break;
			}
		}
		break;
	case WM_MOUSEMOVE:
		GetCursorPos(&point);
		if (center.x!=point.x||center.y!=point.y)
		{
			float angle = 0.4f;
			if (point.x>center.x) Camera::instance().rotate_along_up_vector(-angle);
			else if (point.x<center.x) Camera::instance().rotate_along_up_vector(angle);
			else if (point.y>center.y) Camera::instance().rotate_along_right_vector(-angle);
			else if (point.y<center.y) Camera::instance().rotate_along_right_vector(angle);
			point = center;
			SetCursorPos(center.x,center.y);
		}
		
		break;
	default:
		return DefWindowProc(hwnd,message,wparam,lParam);
	}
	return 0;
}



GameWindow::~GameWindow()
{
	//5.注销窗口类
	UnregisterClass(name.c_str(),GetModuleHandle(NULL));
}
