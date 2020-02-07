﻿#include <windows.h>
#include <assert.h>
#include <iostream>
#include <set>
#include "DirectXLib.h"
#include "Math.h"
#include "Matrix.h"
#include "Vertex.h"
#include "Enum.h"
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

State g_game_state = MODEL_TRANSFORM;
RenderState g_render_state = COLOR;
const float g_rotate_theta = 3.5;
//Vector3 light_vector(-1,1,-1);
#define l0 1.0f
#define l1 0.6f
#define l2 0.5f
Vector3 light_vector(1, -1, 0);
Vector3 diffuse_light(l0,l0,l0,l0);
Vector3 specular_light(l1,l1,l1,l1);
Vector3 ambient_light(l2,l2,l2);

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
		else if (wparam==VK_TAB)
		{
			switch (g_render_state)
			{
			case WIREFRAME:
				g_render_state = TEXTURE;
				break;
			case TEXTURE:
				g_render_state = COLOR;
				break;
			case COLOR:
				g_render_state = WIREFRAME;
				break;
			default:
				break;
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
				g_model.rotate_theta = g_rotate_theta;
			}
			else if (wparam==VK_RIGHT)
			{
				g_model.rotate_theta = -g_rotate_theta;
			}
			else if (wparam==VK_OEM_PLUS)
			{
				g_model.scale = 1.01;
			}
			else if (wparam==VK_OEM_MINUS)
			{
				g_model.scale = 0.99;
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
	int cube_vertex[24][3] = { 
		{-1,1,-1},{-1,-1,-1},{1,-1,-1},{1,1,-1},
		{1,1,-1},{1,-1,-1},{1,-1,1},{1,1,1},
		{1,1,1},{1,-1,1},{-1,-1,1},{-1,1,1},
		{-1,1,1},{-1,-1,1},{-1,-1,-1},{-1,1,-1},
		{-1,1,1},{-1,1,-1},{1,1,-1},{1,1,1},
		{-1,-1,-1},{-1,-1,1},{1,-1,1},{1,-1,-1}
	};

	int colors[24][3] = {
		{255,0,0},{0,255,0},{0,0,255},
		{255,0,0},{0,255,0},{0,0,255},
		{255,0,0},{0,255,0},{0,0,255},
		{255,0,0},{0,255,0},{0,0,255},
		{255,0,0},{0,255,0},{0,0,255},
		{255,0,0},{0,255,0},{0,0,255},
		{255,0,0},{0,255,0},{0,0,255},
		{255,0,0},{0,255,0},{0,0,255},
	};

	int uv[4][2] = {
		{0,0},{0,1},{1,1},{1,0},
	};

	//string path = "pal5q.jpg";
	string path = "Naruto.bmp";
	model.texture_ = new Texture(path);
	for (int i = 0; i < 24; ++i)
	{
		Vector3 v(cube_vertex[i][0], cube_vertex[i][1], cube_vertex[i][2]);
		AColor color(0, colors[i][0], colors[i][1], colors[i][2]);
		Vertex vertex(v, color, uv[i % 4][0], uv[i % 4][1]);
		model.local_vertexes_.push_back(vertex);
		model.trans_vertexes_.push_back(Vertex(v + model.world_position_, color, uv[i % 4][0], uv[i % 4][1]));
	}

	Matrix model_move_matrix;
	model_move_matrix.identify();
	model_move_matrix.setTranslation(Vector3(0, 0, -5));
	model.world_position_ = model.world_position_ * model_move_matrix;
	// 模型空间旋转(初始化)
	Matrix model_rotateY_matrix;
	model_rotateY_matrix.setRotate(Vector3(0,1,0), 45);
	Matrix model_rotateX_matrix;
	model_rotateX_matrix.setRotate(Vector3(1,0,0), 45);
	Matrix model_rotateZ_matrix;
	model_rotateZ_matrix.setRotate(Vector3(0,0,1), 45);
	Matrix matrix = model_rotateX_matrix * model_rotateY_matrix * model_rotateZ_matrix;

	Matrix model_rotate_matrix;
	model_rotate_matrix.setRotate(Vector3(0,1,0), 60);
	Matrix rotate_matrix = model_rotate_matrix * matrix;
	//4.1.4 转换到世界坐标系
	int index = 0;
	for (auto &v : model.local_vertexes_)
	{
		v.position_ = v.position_ * rotate_matrix;
		model.trans_vertexes_[index++].position_ = v.position_ + model.world_position_;
	}
	model.poly_indices_.push_back(TrangleIndex(0,1,2));
	model.poly_indices_.push_back(TrangleIndex(2,3,0));  //front
	model.poly_indices_.push_back(TrangleIndex(4,5,6));
	model.poly_indices_.push_back(TrangleIndex(6,7,4));  //right
	model.poly_indices_.push_back(TrangleIndex(8,9,10));
	model.poly_indices_.push_back(TrangleIndex(10,11,8));  //back
	model.poly_indices_.push_back(TrangleIndex(12,13,14));
	model.poly_indices_.push_back(TrangleIndex(14,15,12));  //left
	model.poly_indices_.push_back(TrangleIndex(16,17,18));
	model.poly_indices_.push_back(TrangleIndex(18,19,16));  //top
	model.poly_indices_.push_back(TrangleIndex(20,21,22));
	model.poly_indices_.push_back(TrangleIndex(22,23,20));  //button
}

/************************************************************************/
/* 1.绘制一个像素                                                         */
/************************************************************************/
void draw_pixel()
{
	//g_directX.drawPixel(300,300,AColor(0,255,0,0));
	for (int i=0;i<100;++i)
	{
		for (int j=0;j<200;++j)
		{
			g_directX.drawPixel(i,j,AColor(0,255,0,0));
			//g_directX.drawPixel(i,j,model.texture_->get_color2(i,j));
		}
	}
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
				g_directX.drawLine(350,350,i,j,AColor(0,255,0,0));
			}
		}
	}
}

/************************************************************************/
/*3.三角形光栅化                                                           */
/************************************************************************/
void draw_triangle()
{
	//g_directX.drawTriangle(Vector2(350,100,Color(0,255,0,0)),
	//	Vector2(100,700,Color(0,0,255,0)),
	//	Vector2(700,700,Color(0,0,0,255)));
	//g_directX.drawTriangle(Vector2(350,100,Color(0,255,0,0)),
	//	Vector2(700,100,Color(0,0,255,0)),
	//	Vector2(700,700,Color(0,0,0,255)));
	//g_directX.drawTriangle(Vector2(100,100,Color(0,255,0,0)),
	//	Vector2(100,200,Color(0,0,255,0)),
	//	Vector2(200,100,Color(0,0,0,255)));
	//g_directX.drawTriangle(Vector2(200,200,Color(0,255,0,0)),
	//	Vector2(100,200,Color(0,0,255,0)),
	//	Vector2(200,100,Color(0,0,0,255)));
	//g_directX.drawTriangle(Vector2(100,100,Color(0,255,0,0)),
	//	Vector2(450,300,Color(0,0,255,0)),
	//	Vector2(200,200,Color(0,0,0,255)));
	g_directX.drawTriangle(Vertex(Vector3(200,200,0),AColor(0,255,0,0),0,0),
		Vertex(Vector3(450,300,0),AColor(0,0,255,0),0,0),
		Vertex(Vector3(550,500,0),AColor(0,0,0,255),0,0));
}

/************************************************************************/
/*4.画正方体                                                            */
/************************************************************************/
void draw_cube(RenderState renderState, float deltaTime)
{
	/************************************************************************/
	/* 4.1.世界变换                                                           */
	/************************************************************************/

	float ratio = 0.03 * deltaTime;
	//4.1.1 模型空间平移
	Matrix model_move_matrix;
	model_move_matrix.identify();
	model_move_matrix.setTranslation(Vector3(0, 0, g_model.move_z * ratio));
	model.world_position_ = model.world_position_ * model_move_matrix;

	//4.1.2 模型空间旋转
	Matrix model_rotate_matrix;
	model_rotate_matrix.setRotate(Vector3(0, 1, 0), g_model.rotate_theta * ratio);

	//4.1.3 模型空间缩放
	Matrix model_scale_matrix;
	Vector3 scale = Vector3(1 + (g_model.scale - 1.0) * ratio);
	model_scale_matrix.setScale(scale);

	Matrix model_transform = model_rotate_matrix * model_scale_matrix;


	/************************************************************************/
	/* 4.2  取景变换(相机空间)                                                         */
	/************************************************************************/
	Camera &camera = Camera::instance();

	//4.2.1 相机移动
	Matrix camera_move_matrix;
	camera_move_matrix.identify();
	camera_move_matrix.setTranslation(Vector3(g_camera.move_x, g_camera.move_y, g_camera.move_z) * ratio);

	//4.2.2 相机旋转
	camera.set_lookAt(Vector3(g_camera.rotateX, g_camera.rotateY, 0) * ratio);
	camera.set_position(camera_move_matrix);
	
	//4.1.4 转换到世界坐标系
	int index = 0;
	for (auto &v : model.local_vertexes_)
	{
		v.position_ = v.position_ * model_transform;
		model.trans_vertexes_[index].position_ = v.position_ + model.world_position_;
		
		//顶点法向量处理
		//(1)flat着色
		int base = index-index%4;
		/*Debug::instance()<<"Index:"<<index<<endl;
		Debug::instance()<<"Base:"<<base<<endl;
		Debug::instance()<<base+(index-1+4)%4<<" "<<base+(index+1+4)%4<<endl;*/
		//Vector3 vl = model.local_vertexes_[base+(index-1+4)%4].position_ - model.local_vertexes_[index].position_;
		//Vector3 vr = model.local_vertexes_[base+(index+1+4)%4].position_ -model.local_vertexes_[index].position_;
		//model.trans_vertexes_[index].set_normal(cross_product(vl,vr));
		//(2)Gourand着色
		model.trans_vertexes_[index].set_normal(model.trans_vertexes_[index].position_ - model.world_position_); 
		model.trans_vertexes_[index].normal_.normalize(); 
	
		//漫反射&环境光&镜面反射光
		Vector3 normal = model.trans_vertexes_[index].normal_;
		float diff_cos_theta = max(-light_vector * normal, 0);
		Vector3 view_vector = camera.get_position() - model.trans_vertexes_[index].position_;
		view_vector.normalize();
		float sepc_cos_theta = max(0, view_vector * (2 * (normal * light_vector) * normal + light_vector));
		model.trans_vertexes_[index].light_ = diffuse_light * diff_cos_theta + specular_light * sepc_cos_theta + ambient_light;
		model.trans_vertexes_[index].light_.color_adjust();
		++index;
	}

	//4.2.3 转换到相机坐标
	camera.view_transform(model.trans_vertexes_);


	/************************************************************************/
	/* 4.3  投影变换(裁剪空间)                                                        */
	/************************************************************************/
	// 4.3.1 CVV裁剪
	set<int> remove_triangle_index;
	// 4.3.2 记录剔除三角形
	int index2 = 0;
	for (auto v : model.poly_indices_)
	{
		//背面剔除
		Vector3 v1 = model.trans_vertexes_[v.indices[0]].position_;
		Vector3 v2 = model.trans_vertexes_[v.indices[1]].position_;
		Vector3 v3 = model.trans_vertexes_[v.indices[2]].position_;
		if (camera.is_back(v1,v2,v3))
		{
			remove_triangle_index.insert(index2);
		}
		index2++;
	}
	camera.canonicalview_volume(model.trans_vertexes_);

	// 4.3.3 透视除法
	for (int i=0; i < model.trans_vertexes_.size(); ++i)
	{
		model.trans_vertexes_[i].position_.x_ = model.trans_vertexes_[i].position_.x_ / model.trans_vertexes_[i].position_.w_; 
		model.trans_vertexes_[i].position_.y_ = model.trans_vertexes_[i].position_.y_ / model.trans_vertexes_[i].position_.w_; 
		model.trans_vertexes_[i].position_.z_ = model.trans_vertexes_[i].position_.z_ / model.trans_vertexes_[i].position_.w_; 
		model.trans_vertexes_[i].position_.w_ = 1 / model.trans_vertexes_[i].position_.w_;
	}
	set<int> remove_vertex_index;
	// 4.3.4 记录剔除点
	for (int i = 0; i < model.trans_vertexes_.size(); ++i)
	{
		if (camera.transform_check_cvv(model.trans_vertexes_[i].position_))
		{
			remove_vertex_index.insert(i);
		}
	}

	/************************************************************************/
	/* 4.4  窗口变换                                                         */
	/************************************************************************/
	int half_width = WIDTH / 2;
	int half_height = HEIGHT / 2;
	for (int i = 0; i < model.trans_vertexes_.size(); ++i)
	{
		model.trans_vertexes_[i].position_.x_ *= half_width;
		model.trans_vertexes_[i].position_.x_ += half_width;
		model.trans_vertexes_[i].position_.y_ *= half_height;
		model.trans_vertexes_[i].position_.y_ = half_height - model.trans_vertexes_[i].position_.y_;
	}

	//绘制线框模型
	switch (renderState)
	{
	case WIREFRAME:
		g_directX.draw_wireframe_model(model, remove_vertex_index, remove_triangle_index);
		break;
	case TEXTURE:
		g_directX.set_render_state(TEXTURE);
		g_directX.draw_mesh_model(model, remove_vertex_index, remove_triangle_index);
		break;
	case COLOR:
		g_directX.set_render_state(COLOR);
		g_directX.draw_mesh_model(model, remove_vertex_index, remove_triangle_index);
		break;
	default:
		break;
	}
}

HWND GameStart(HINSTANCE hInstance, int nShowCmd, string wcName, string title)
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
	HWND hwnd = CreateWindow(wcName.c_str(), title.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WIDTH, HEIGHT, NULL, NULL, hInstance, NULL);

	//4.调整大小，移动，显示，更新
	RECT window_rect = {0, 0, WIDTH, HEIGHT};
	AdjustWindowRectEx(&window_rect, GetWindowStyle(hwnd), GetMenu(hwnd) != NULL, GetWindowExStyle(hwnd));
	MoveWindow(hwnd, 300, 150, window_rect.right - window_rect.left, window_rect.bottom - window_rect.top, false);
	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);

	//DirectX初始化
	g_directX.initialDirectX(hInstance, hwnd, WIDTH, HEIGHT);

	//Cube初始化
	cube_init();
	transform_attribute_init();

	//相机初始化
	Camera &camera = Camera::instance();
	camera.set_w_h(WIDTH / HEIGHT);

	light_vector.normalize();
	return hwnd;
}

void GameUpdate(HWND hwnd, float deltaTime)
{
	g_directX.fillSurface();
	g_directX.lockSurface();

	//draw_pixel();
	draw_cube(g_render_state, deltaTime);
	//draw_triangle();
	g_directX.unlockSurface();
	//Sleep(1000);
	g_directX.flipSurface();
	transform_attribute_init();
}

void GameEnd(string wcName,HINSTANCE hInstance)
{
	//5.注销窗口类
	UnregisterClass(wcName.c_str(), hInstance);
}


int WINAPI WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd )
{
	//1.创建窗口
	string windowClassName = "MyWindow";
	string title = "3DRender";
	HWND hwnd = GameStart(hInstance, nShowCmd, windowClassName, title);

	//时间初始化
	DWORD curTime = GetTickCount();
	DWORD preTime = GetTickCount();
	//2.消息循环
	MSG msg = {0};
	while (msg.message != WM_QUIT)
	{
		//获取消息
		if (PeekMessage(&msg, 0, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			curTime = GetTickCount();
			if (curTime - preTime > 10)
			{
				GameUpdate(hwnd, curTime - preTime);
				preTime = curTime;
			}
		}
	}

	//3.游戏结束
	GameEnd(windowClassName, hInstance);
	return 0;
}

