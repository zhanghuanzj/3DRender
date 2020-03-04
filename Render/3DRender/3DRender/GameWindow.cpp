#include "GameWindow.h"

static POINT center;
// �ྲ̬��Ա��ʼ��
float GameWindow::deltaTime = 0.0f;
int GameWindow::fps = 0;
bool GameWindow::isFirst = true;
bool GameWindow::isLeftClickDown = false;
bool GameWindow::isRightClickDown = false;

GameWindow::GameWindow(const string name_t, const string title_t, int width, int height):name(name_t), title(title_t), WIDTH(width), HEIGHT(height)
{
	// 1.����������
	WNDCLASSEX wndClass = {};
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = GetModuleHandle(NULL);
	wndClass.hIcon = LoadIcon(NULL,  IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL,  IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = name.c_str();

	// 2.ע�ᴰ����
	assert(RegisterClassEx(&wndClass));

	// 3.��������
	hwnd = CreateWindow(name.c_str(), title.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WIDTH, HEIGHT, NULL, NULL, wndClass.hInstance, NULL);

	// 4.������С���ƶ�����ʾ������
	RECT window_rect = {0, 0, WIDTH, HEIGHT};
	AdjustWindowRectEx(&window_rect,  GetWindowStyle(hwnd),  GetMenu(hwnd) != NULL,  GetWindowExStyle(hwnd));
	MoveWindow(hwnd, 300, 150, window_rect.right-window_rect.left,  window_rect.bottom-window_rect.top, false);
	ShowWindow(hwnd, SW_NORMAL);
	UpdateWindow(hwnd);

	// 5.������꣬��Ϊ��Ļ����
	ShowCursor(true);
	center.x = WIDTH / 2;
	center.y = HEIGHT / 2;
	ClientToScreen(hwnd, &center);
	SetCursorPos(center.x, center.y);
	// 6.�޶�����ڴ�����
	//RECT rect;
	//GetClientRect(hwnd, &rect);
	//POINT left_top;
	//left_top.x = rect.left;
	//left_top.y = rect.top;
	//POINT right_bottom;
	//right_bottom.x = rect.right;
	//right_bottom.y = rect.bottom;
	//ClientToScreen(hwnd, &left_top);
	//ClientToScreen(hwnd, &right_bottom);
	//rect.left = left_top.x;
	//rect.top = left_top.y;
	//rect.right = right_bottom.x;
	//rect.bottom = right_bottom.y;
	//ClipCursor(&rect);
}

void GameWindow::message_dispatch()
{
	// ʱ���ʼ��
	DWORD curTime = GetTickCount();
	DWORD preTime = GetTickCount();
	float countTime = 0.0f;
	// 2.��Ϣѭ��
	MSG msg = {0};
	while (msg.message != WM_QUIT)
	{
		// ��ȡ��Ϣ
		if (PeekMessage(&msg, 0, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			curTime = GetTickCount();
			if (curTime - preTime > 5)
			{
				deltaTime = (curTime - preTime) / 1000.0f;
				countTime += deltaTime;
				++fps;
				if (countTime > 1.0f)
				{
					cout << "FPS:" << fps << endl;
					countTime -= 1.0f;
					fps = 0;
				}
				preTime = curTime;
				GameManager::Instance().game_update();
			}
		}
	}
}

LRESULT CALLBACK GameWindow::WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lParam)
{
	float units = 10.0f * deltaTime;
	static POINT point = center;

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
		else if (wparam=='A') Camera::instance().move_along_right_vector(-units);
		else if (wparam=='D') Camera::instance().move_along_right_vector(units);
		else if (wparam=='W') Camera::instance().move_along_lookat_vector(units);
		else if (wparam=='S') Camera::instance().move_along_lookat_vector(-units);
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
	case WM_LBUTTONDOWN:
		isLeftClickDown = true;
		isFirst = true;
		break;
	case WM_LBUTTONUP:
		isLeftClickDown = false;
		break;
	case WM_RBUTTONDOWN:
		isRightClickDown = true;
		isFirst = true;
		break;
	case WM_RBUTTONUP:
		isRightClickDown = false;
		break;
	case WM_MOUSEMOVE:
		if (isLeftClickDown || isRightClickDown)
		{
			POINT curPoint;
			GetCursorPos(&curPoint);
			if (!isFirst)
			{
				if (isLeftClickDown)
				{
					Camera::instance().rotate_around_model(curPoint.x - point.x, curPoint.y - point.y);
				}
				else
				{
					Camera::instance().move_around_model((curPoint.x - point.x) * deltaTime, (curPoint.y - point.y)  * deltaTime);
				}
				
			}
			isFirst = false;
			point = curPoint;
		}
		break;
	case WM_MOUSEWHEEL:
		if (HIWORD(wparam) == 120)
		{
			Camera::instance().move_along_lookat_vector(units);
		}
		else
		{
			Camera::instance().move_along_lookat_vector(units);
		}
		break;
	default:
		return DefWindowProc(hwnd, message, wparam, lParam);
	}
	return 0;
}


GameWindow::~GameWindow()
{
	// 5.ע��������
	UnregisterClass(name.c_str(), GetModuleHandle(NULL));
}
