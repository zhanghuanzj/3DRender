#include <windows.h>
#include <assert.h>
#include <iostream>
#include "DirectXLib.h"
#include "Math.h"
#include "Matrix.h"
#include <WindowsX.h>

using namespace std;

const int WIDTH = 800;
const int HEIGHT = 800;
DirectX &g_directX = DirectX::instance();

Model model(Vector3(0,0,8)); 

struct ModelTransAttribute
{
	float move_z;
	float rotate_theta;
	float scale;
}g_model;

struct CameraTransAttribute
{
	float move_x,move_y,move_z;
	float rotateX,rotateY;
}g_camera;

void transform_attribute_init()
{
	g_model.move_z = 0.0f;
	g_model.rotate_theta = 0.0f;
	g_model.scale = 1.0f;

	g_camera.move_x = g_camera.move_y = g_camera.move_z = 0.0f;
	g_camera.rotateX = g_camera.rotateY = 0.0f;
}

enum State
{
	MODEL_TRANSFORM,
	CAMERA_TRANSFORM
};

State g_game_state = MODEL_TRANSFORM;

LRESULT CALLBACK WndProc(HWND hwnd,UINT message,WPARAM wparam,LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);

	case WM_KEYDOWN:
		if (wparam==VK_ESCAPE)
		{
			DestroyWindow(hwnd);
		}
		else if (wparam==VK_SPACE)
		{
			if (g_game_state==MODEL_TRANSFORM)
			{
				g_game_state = CAMERA_TRANSFORM;
			}
			else
			{
				g_game_state = MODEL_TRANSFORM;
			}
		}
		switch (g_game_state)
		{
		case MODEL_TRANSFORM:
			if (wparam==VK_UP)
			{
				g_model.move_z = 0.1f;
			}
			else if(wparam==VK_DOWN)
			{
				g_model.move_z = -0.1f;
			}
			else if (wparam==VK_LEFT)
			{
				g_model.rotate_theta = 10;
			}
			else if (wparam==VK_RIGHT)
			{
				g_model.rotate_theta = -10;
			}
			else if (wparam==VK_OEM_PLUS)
			{
				g_model.scale = 0.01;
			}
			else if (wparam==VK_OEM_MINUS)
			{
				g_model.scale = -0.01;
			}
			break;
		case CAMERA_TRANSFORM:
			if (wparam==VK_UP)
			{
				g_camera.move_y = 0.1f;
			}
			else if(wparam==VK_DOWN)
			{
				g_camera.move_y = -0.1f;
			}
			if (wparam==VK_LEFT)
			{
				g_camera.move_x = -0.1f;
			}
			else if (wparam==VK_RIGHT)
			{
				g_camera.move_x = 0.1;
			}
			else if (wparam==VK_OEM_PLUS)
			{
				g_camera.move_z = 0.1;
			}
			else if (wparam==VK_OEM_MINUS)
			{
				g_camera.move_z = -0.1;
			}
			else if (wparam==VK_NUMPAD8)
			{
				g_camera.rotateY = 1;
			}
			else if (wparam==VK_NUMPAD2)
			{
				g_camera.rotateY = -1;
			}
			else if (wparam==VK_NUMPAD4)
			{
				g_camera.rotateX = 1;
			}
			else if (wparam==VK_NUMPAD6)
			{
				g_camera.rotateX = -1;
			}
			break;
		default:
			break;
		}

		
		break;
	case WM_PAINT:
		{
			hdc = BeginPaint(hwnd, &ps);
			EndPaint(hwnd, &ps);
			return 0;
		} break;
	default:
		return DefWindowProc(hwnd,message,wparam,lParam);
	}
	return 0;
}

void cube_init()
{        
	int cube_vertex[8][3] = { 
		{-1,-1,-1},{-1,1,-1},{1,-1,-1},{1,1,-1},
		{-1,-1,1},{-1,1,1},{1,-1,1},{1,1,1}
	};

	for (int i=0;i<8;++i)
	{
		Vector3 v(cube_vertex[i][0],cube_vertex[i][1],cube_vertex[i][2]);
		model.local_vertexes_.push_back(v);
		model.trans_vertexes_.push_back(v+model.world_position_);
	}

	model.poly_indices_.push_back(TrangleIndex(0,1,2));
	model.poly_indices_.push_back(TrangleIndex(1,2,3));
	model.poly_indices_.push_back(TrangleIndex(2,3,6));
	model.poly_indices_.push_back(TrangleIndex(3,6,7));
	model.poly_indices_.push_back(TrangleIndex(4,6,7));
	model.poly_indices_.push_back(TrangleIndex(4,7,5));
	model.poly_indices_.push_back(TrangleIndex(0,4,5));
	model.poly_indices_.push_back(TrangleIndex(0,5,1));
	model.poly_indices_.push_back(TrangleIndex(1,3,7));
	model.poly_indices_.push_back(TrangleIndex(1,7,5));
	model.poly_indices_.push_back(TrangleIndex(0,2,6));
	model.poly_indices_.push_back(TrangleIndex(0,6,4));
}

/************************************************************************/
/* 1.绘制一个像素                                                         */
/************************************************************************/
void draw_pixel()
{
	g_directX.drawPixel(300,300,Color(0,255,0,0));
}

/************************************************************************/
/* 2.直线光栅化                                                       */
/************************************************************************/
void draw_line()
{

	/*g_directX.drawLine(350,350,250,100,Color(0,255,0,0));
	g_directX.drawLine(200,200,200,500,Color(0,255,0,0));
	g_directX.drawLine(200,200,500,200,Color(0,255,0,0));
	g_directX.drawLine(600,250,200,250,Color(0,255,0,0));
	g_directX.drawLine(679,356,300,300,Color(0,255,0,0));
	g_directX.drawLine(400,100,450,600,Color(0,255,0,0));
	g_directX.drawLine(350,350,600,600,Color(0,255,0,0));*/
	//用直线画圆
	for (int i=0;i<750;i+=1)
	{
		for (int j=0;j<750;j+=1)
		{
			double v = sqrt(pow(i-350,2)+pow(j-350,2))-200;
			if ( v>=0&&v < 2)
			{
				g_directX.drawLine(350,350,i,j,Color(0,255,0,0));
			}
		}
	}
}

/************************************************************************/
/*3.三角形光栅化                                                           */
/************************************************************************/
void draw_triangle()
{
	g_directX.drawTriangle(Vector2(350,100,Color(0,255,0,0)),
		Vector2(100,700,Color(0,0,255,0)),
		Vector2(700,700,Color(0,0,0,255)));
}

/************************************************************************/
/*4.画正方体                                                            */
/************************************************************************/
void draw_cube()
{
	/************************************************************************/
	/* 4.1.世界变换                                                           */
	/************************************************************************/

	//4.1.1 模型空间平移
	Matrix model_move_matrix;
	model_move_matrix.identify();
	model_move_matrix.setTranslation(Vector3(0,0,g_model.move_z));
	model.world_position_ = model.world_position_*model_move_matrix;


	//4.1.2 模型空间旋转
	Matrix model_rotate_matrix;
	model_rotate_matrix.setRotate(Vector3(0,1,0),g_model.rotate_theta);

	//4.1.3 模型空间缩放
	Matrix model_scale_matrix;
	model_scale_matrix.setScale(Vector3(g_model.scale,g_model.scale,g_model.scale));

	Matrix model_transform = model_rotate_matrix*model_scale_matrix;

	//4.1.4 转换到世界坐标系
	int index = 0;
	for (auto &v : model.local_vertexes_)
	{
		v = v*model_transform;
		model.trans_vertexes_[index++] = v+model.world_position_;
	}


	/************************************************************************/
	/* 4.2  取景变换                                                         */
	/************************************************************************/
	Camera &camera = Camera::instance();

	//4.2.1 相机移动
	Matrix camera_move_matrix;
	camera_move_matrix.identify();
	camera_move_matrix.setTranslation(Vector3(g_camera.move_x,g_camera.move_y,g_camera.move_z));

	//4.2.2 相机旋转
	camera.set_lookAt(Vector3(g_camera.rotateX,g_camera.rotateY,0));
	camera.set_position(camera_move_matrix);
	//4.2.3 转换到相机坐标
	camera.view_transform(model.trans_vertexes_);


	/************************************************************************/
	/* 4.3  投影变换                                                         */
	/************************************************************************/
	for (int i=0;i<model.trans_vertexes_.size();++i)
	{
		model.trans_vertexes_[i].x_ = model.trans_vertexes_[i].x_/model.trans_vertexes_[i].z_; 
		model.trans_vertexes_[i].y_ = model.trans_vertexes_[i].y_/model.trans_vertexes_[i].z_; 
	}

	/************************************************************************/
	/* 4.4  窗口变换                                                         */
	/************************************************************************/

	for (int i=0;i<model.trans_vertexes_.size();++i)
	{
		model.trans_vertexes_[i].x_ *= WIDTH/2;
		model.trans_vertexes_[i].x_ += WIDTH/2;
		model.trans_vertexes_[i].y_ *= HEIGHT/2;
		model.trans_vertexes_[i].y_ = HEIGHT/2-model.trans_vertexes_[i].y_;
	}

	//绘制线框模型
	g_directX.draw_wireframe_model(model);

}

HWND GameStart(HINSTANCE hInstance,int nShowCmd,string wcName,string title)
{
	//1.创建窗口类
	WNDCLASSEX wndClass = {};
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = wcName.c_str();

	//2.注册窗口类
	assert(RegisterClassEx(&wndClass));

	//3.创建窗口
	HWND hwnd = CreateWindow(wcName.c_str(),title.c_str(),WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,WIDTH,HEIGHT,NULL,NULL,hInstance,NULL);

	//4.调整大小，移动，显示，更新
	RECT window_rect = {0,0,WIDTH,HEIGHT};
	AdjustWindowRectEx(&window_rect, GetWindowStyle(hwnd), GetMenu(hwnd) != NULL, GetWindowExStyle(hwnd));
	MoveWindow(hwnd,300,150,window_rect.right-window_rect.left, window_rect.bottom-window_rect.top,false);
	ShowWindow(hwnd,nShowCmd);
	UpdateWindow(hwnd);

	//DirectX初始化
	g_directX.initialDirectX(hInstance,hwnd,WIDTH,HEIGHT);

	//Cube初始化
	cube_init();
	transform_attribute_init();

	return hwnd;
}

void GameUpdate(HWND hwnd)
{
	g_directX.fillSurface();
	g_directX.lockSurface();

	draw_cube();

	g_directX.unlockSurface();
	//Sleep(1000);
	g_directX.flipSurface();
	transform_attribute_init();
}

void GameEnd(string wcName,HINSTANCE hInstance)
{
	//5.注销窗口类
	UnregisterClass(wcName.c_str(),hInstance);
}


int WINAPI WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd )
{
	//1.创建窗口
	string windowClassName = "MyWindow";
	string title = "3DRender";
	HWND hwnd = GameStart(hInstance,nShowCmd,windowClassName,title);

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
			if (curTime-preTime>30)
			{
				preTime = curTime;
				GameUpdate(hwnd);
			}
		}
	}

	//3.游戏结束
	GameEnd(windowClassName,hInstance);
	return 0;
}

