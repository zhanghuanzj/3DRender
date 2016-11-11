#include "GameManager.h"

GameManager& GameManager::Instance()
{
	static GameManager gameManager;
	return gameManager;
}

void GameManager::game_start(HWND hwnd,const int WIDTH,const int HEIGHT)
{
	DirectX::instance().initialDirectX(GetModuleHandle(nullptr),hwnd,WIDTH,HEIGHT);
	rasterizer.set_texture("Texture1.jpg");
}

void GameManager::game_update()
{
	DirectX &directX = DirectX::instance();
	directX.fillSurface();
	directX.lockSurface();
	//for (int i=0;i<100;++i) {
	//	for (int j=0;j<100;++j) {
	//		directX.drawPixel(i,j,AColor(0,255,0,0));
	//	}
	//}

	//directX.drawLine(10,200,530,30,AColor(0,255,0,0));
	//directX.drawLine(10,10,530,530,AColor(0,255,0,0));
	//directX.drawLine(30,30,60,500,AColor(0,255,0,0));
	//directX.drawLine(60,300,600,500,AColor(0,255,0,0));
	//directX.drawLine(80,80,80,500,AColor(0,255,0,0));

	//for (int i=0;i<580;i+=5)
	//{
	//	for (int j=0;j<580;j+=5)
	//	{
	//		double v = sqrt(pow(i-350,2)+pow(j-350,2))-200;
	//		if ( v>=0&&v < 2)
	//		{
	//			rasterizer.drawline_bresenham(350,350,i,j,AColor(0,(1+rand()%255)/255.0f,(1+rand()%255)/255.0f,(1+rand()%255)/255.0f));
	//		}
	//	}
	//}

	//Vertex v1(Vector3(400,30,0),AColor(0,1.0f,0,0),0,0);
	//Vertex v2(Vector3(200,300,0),AColor(0,0,1.0f,0),0,0);
	//Vertex v3(Vector3(600,300,0),AColor(0,0,0,1.0f),0,0);
	//Vertex v4(Vector3(700,100,0),AColor(0,0,0,1.0f),0,0);
	//rasterizer.draw_triangle(v1,v2,v3);
	//rasterizer.draw_triangle(v1,v3,v4);


	Vector3 rotate_vector(1.0f,1.0f,1.0f);
	rotate_vector.normalize();
	Matrix cube_rotate_matrix;
	cube_rotate_matrix.identify();
	cube_rotate_matrix.setRotate(rotate_vector,2);

	

	Matrix model_move_matrix;
	model_move_matrix.identify();
	model_move_matrix.setTranslation(Vector3(0,0,5));

	


	//1.世界变换
	for (int i=0;i<cube.trans_vertexes.size();++i) 
	{
		cube.local_vertexes[i].position = cube.local_vertexes[i].position*cube_rotate_matrix;  //物体旋转
		cube.trans_vertexes[i] = cube.local_vertexes[i];
		cube.trans_vertexes[i].position = cube.trans_vertexes[i].position*model_move_matrix;   //物体移动
	}
	//2.视角变换
	Camera::instance().eye_transform(cube.trans_vertexes); 
	//背面剔除
	vector<Triangle> triangles = Camera::instance().get_front_triangle(cube.trans_vertexes);
	//3.CVV裁剪
	Camera::instance().cvv_clip(cube.trans_vertexes);
	//4.屏幕适配
	Camera::instance().screen_map(cube.trans_vertexes);

	for (auto &triangle : triangles) 
	{
		rasterizer.draw_triangle(cube.trans_vertexes[triangle.index1],cube.trans_vertexes[triangle.index2],cube.trans_vertexes[triangle.index3]);
	}


	//Vertex v1(Vector3(500,294,0),AColor(0,1.0f,0,0),0,0);
	//Vertex v2(Vector3(200,300,0),AColor(0,0,1.0f,0),0,0);
	//Vertex v3(Vector3(270,305,0),AColor(0,0,0,1.0f),0,0);
	//Vertex v4(Vector3(600,302,0),AColor(0,0,0,1.0f),0,0);
	//rasterizer.draw_triangle(v1,v2,v3);
	//rasterizer.draw_triangle(v1,v3,v4);
	directX.unlockSurface();
	directX.flipSurface();
}

void GameManager::game_end()
{
	cout<<"Game end!"<<endl;
}