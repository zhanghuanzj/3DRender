#include "GameManager.h"

GameManager& GameManager::Instance()
{
	static GameManager gameManager;
	return gameManager;
}

void GameManager::game_start(const int WIDTH,const int HEIGHT)
{
	//Window
	GameWindow myWindow("Render","3DRender",WIDTH,HEIGHT);
	//Camera
	Camera::instance(WIDTH,HEIGHT);
	//DirectX
	DirectX::instance().initialDirectX(GetModuleHandle(nullptr),myWindow.get_hwnd(),WIDTH,HEIGHT);
	//Cube
	pcube = new Cube(1,"pal5q.jpg",Vector3(0,0,6));
	//Rasterizer
	rasterizer.set_texture(&pcube->texture);
	//Light
	light.position = Vector3(-1000,1000,7);
	light.diffuse = VColor(0.0f,0.7f,0.7f,0.7f);
	light.specular = VColor(0.0f,1.0f,1.0f,1.0f);
	light.ambient = VColor(0.0f,0.4f,0.4f,0.4f);
	//Message Loop
	myWindow.message_dispatch();
}

void GameManager::game_update()
{
	DirectX &directX = DirectX::instance();
	directX.fillSurface();
	directX.lockSurface();


	//draw_cube();
	draw_circle(400,400,200);
	draw_ellipse(400,400,200,100);

	directX.unlockSurface();
	directX.flipSurface();
}

void GameManager::game_end()
{
	cout<<"Game end!"<<endl;
}

void GameManager::fill_point(int cx,int cy,int x,int y)
{
	DirectX &directX = DirectX::instance();
	AColor color(0,255,0,0);
	
	directX.drawPixel(x+cx,y+cy,color);
	directX.drawPixel(-x+cx,y+cy,color);
	directX.drawPixel(x+cx,-y+cy,color);
	directX.drawPixel(-x+cx,-y+cy,color);
}

void GameManager::draw_pixels()
{
	DirectX &directX = DirectX::instance();
	for (int i=0;i<100;++i) {
		for (int j=0;j<100;++j) {
			directX.drawPixel(i,j,AColor(0,255,0,0));
		}
	}
}

void GameManager::draw_lines()
{
	rasterizer.drawline_bresenham(10,200,530,30,AColor(0,255,0,0));
	rasterizer.drawline_bresenham(10,10,530,530,AColor(0,255,0,0));
	rasterizer.drawline_bresenham(30,30,60,500,AColor(0,255,0,0));
	rasterizer.drawline_bresenham(60,300,600,500,AColor(0,255,0,0));
	rasterizer.drawline_bresenham(80,80,80,500,AColor(0,255,0,0));
}

void GameManager::draw_circle()
{
	for (int i=0;i<580;i+=5)
	{
		for (int j=0;j<580;j+=5)
		{
			double v = sqrt(pow(i-350,2)+pow(j-350,2))-200;
			if ( v>=0&&v < 2)
			{
				rasterizer.drawline_bresenham(350,350,i,j,AColor(0,1+rand()%255,1+rand()%255,1+rand()%255));
			}
		}
	}
}
void GameManager::draw_triangle()
{
	Vertex v1(Vector3(400,30,0),VColor(0,1.0f,0,0),0,0);
	Vertex v2(Vector3(200,300,0),VColor(0,0,1.0f,0),0,0);
	Vertex v3(Vector3(600,300,0),VColor(0,0,0,1.0f),0,0);
	Vertex v4(Vector3(700,100,0),VColor(0,0,0,1.0f),0,0);
	rasterizer.draw_triangle(v1,v2,v3);
	rasterizer.draw_triangle(v1,v3,v4);

}

void GameManager::draw_cube()
{
	Camera &camera = Camera::instance();
	//1.模型空间————(模型变换)————>世界空间
	Vector3 rotate_vector(1.0f,1.0f,1.0f);
	rotate_vector.normalize();
	Matrix cube_rotate_matrix;
	cube_rotate_matrix.identify();
	cube_rotate_matrix.setRotate(rotate_vector,1);
	

	Matrix model_move_matrix;
	model_move_matrix.identify();
	model_move_matrix.setTranslation(pcube->position);

	for (int i=0;i<pcube->trans_vertexes.size();++i) 
	{
		pcube->local_vertexes[i].position = pcube->local_vertexes[i].position*cube_rotate_matrix;  //物体旋转
		pcube->trans_vertexes[i] = pcube->local_vertexes[i];
		pcube->trans_vertexes[i].position = pcube->trans_vertexes[i].position*model_move_matrix;   //物体移动
		pcube->trans_vertexes[i].normal = pcube->trans_vertexes[i].position - pcube->position;
		pcube->trans_vertexes[i].normal.normalize();
		//光照处理
		VColor light_color;
		//Diffuse
		Vector3 l = light.position - pcube->trans_vertexes[i].position;
		l.normalize();
		float nl = pcube->trans_vertexes[i].normal*l;
		pcube->trans_vertexes[i].light += max(nl,0)*light.diffuse*pcube->material.diffuse;
		//Specular
		if (nl>0)
		{
			Vector3 r = 2*pcube->trans_vertexes[i].normal*nl - l;
			r.normalize();
			Vector3 v = camera.get_position() - pcube->trans_vertexes[i].position;
			v.normalize();
			pcube->trans_vertexes[i].light += max(r*v,0)*light.specular*pcube->material.specular;
		}
		
		//Ambient
		pcube->trans_vertexes[i].light += light.ambient*pcube->material.ambient;
	}
	//2.世界空间————(观察变换)————>相机空间
	camera.eye_transform(pcube->trans_vertexes); 
	//背面剔除
	vector<Triangle> triangles = Camera::instance().get_front_triangle(pcube->trans_vertexes);
	//3.相机空间————(投影变换)————>裁剪空间（CVV裁剪）
	triangles = Camera::instance().cvv_clip(pcube->trans_vertexes,triangles);
	//4.裁剪空间————(屏幕映射)————>屏幕空间
	camera.screen_map(pcube->trans_vertexes);
	//5.光栅化
	for (auto &triangle : triangles) 
	{
		rasterizer.draw_triangle(pcube->trans_vertexes[triangle.index1],pcube->trans_vertexes[triangle.index2],pcube->trans_vertexes[triangle.index3]);
	}
}


/************************************************************************/
/* 中点画圆法
 *  其它方法：
 *     （1）：x步进求y法
 *	   （2）：极坐标角度步进法
 */
/************************************************************************/
void GameManager::draw_circle(int xc,int yc,int r)
{
	int x = 0,y = r;
	int p = 1-r;
	while (x<=y)
	{
		fill_point(xc,yc,x,y);
		fill_point(xc,yc,y,x);
		if (p<0)
		{
			p += 2*x+3;
		}
		else
		{
			p += 2*(x-y)+5;
			--y;
		}
		++x;
	}
}

/************************************************************************/
/* 中点画椭圆                                                            */
/************************************************************************/
void GameManager::draw_ellipse(int xe,int ye,int rx,int ry)
{
	int A = rx*rx,B = ry*ry,C = A*B;
	int px = B-A*ry+A/4;
	int x = 0,y = ry;
	while (x*B<y*A)
	{
		fill_point(xe,ye,x,y);
		if (px<0)
		{
			px += B*(2*x+3);
		}
		else
		{
			px += B*(2*x+3)+A*(3-2*y);
			--y;
		}
		++x;
	}
	int py = B*(x*x+x)+B/4+A*(y*y+1-2*y)-C;
	while (y>=0)
	{
		fill_point(xe,ye,x,y);
		if (py<0)
		{
			py += A*(3-2*y)+B*(2+2*x);
			++x;
		}
		else
		{
			py += A*(3-2*y);
		}
		--y;
	}
}