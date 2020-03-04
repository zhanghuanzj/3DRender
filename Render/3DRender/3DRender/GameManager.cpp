#include "GameManager.h"
#include "Pipeline.h"

GameManager& GameManager::Instance()
{
	static GameManager gameManager;
	return gameManager;
}

void GameManager::game_start(const int WIDTH, const int HEIGHT)
{
	//Window
	GameWindow myWindow("Render", "3DRender", WIDTH, HEIGHT);
	//Camera
	Camera::instance(WIDTH, HEIGHT);
	//DirectX
	DirectX::instance().initialDirectX(GetModuleHandle(nullptr), myWindow.get_hwnd(), WIDTH, HEIGHT);
	//Cube
	pCube = new Cube(1.0f, "pal5q.jpg", Vector3(0.0f, 0.0f, 0.0f));

	//Light
	Pipeline &pipeline = Pipeline::Instance();
	pipeline.set_texture(&pCube->texture);
	pipeline.light.position = Vector3(-1000.0f, 1000.0f, 7.0f);
	pipeline.light.color = VColor(0.0f, 1.0f, 1.0f, 1.0f);
	//Message Loop
	myWindow.message_dispatch();
}

void GameManager::game_update()
{
	DirectX &directX = DirectX::instance();
	directX.fillSurface();
	directX.lockSurface();


	draw_cube();
	//draw_circle(400, 400, 200);
	//draw_ellipse(400, 400, 200, 100);

	directX.unlockSurface();
	directX.flipSurface();
}

void GameManager::game_end()
{
	cout<<"Game end!"<<endl;
}

void GameManager::fill_point(int cx, int cy, int x, int y)
{
	DirectX &directX = DirectX::instance();
	VColor color(0, 1, 0, 0);
	
	directX.drawPixel(x+cx, y+cy, color);
	directX.drawPixel(-x+cx, y+cy, color);
	directX.drawPixel(x+cx, -y+cy, color);
	directX.drawPixel(-x+cx, -y+cy, color);
}

void GameManager::draw_pixels()
{
	DirectX &directX = DirectX::instance();
	for (int i=0; i<100; ++i) {
		for (int j=0; j<100; ++j) {
			directX.drawPixel(i, j, VColor(0, 1, 0, 0));
		}
	}
}

void GameManager::draw_lines()
{
	Rasterizer &rasterizer = Rasterizer::instance();
	rasterizer.drawline_bresenham(10, 200, 530, 30, VColor(0, 1, 0, 0));
	rasterizer.drawline_bresenham(10, 10, 530, 530, VColor(0, 1, 0, 0));
	rasterizer.drawline_bresenham(30, 30, 60, 500, VColor(0, 1, 0, 0));
	rasterizer.drawline_bresenham(60, 300, 600, 500, VColor(0, 1, 0, 0));
	rasterizer.drawline_bresenham(80, 80, 80, 500, VColor(0, 1, 0, 0));
}

void GameManager::draw_circle()
{
	Rasterizer &rasterizer = Rasterizer::instance();
	for (int i=0; i<580; i+=5)
	{
		for (int j=0; j<580; j+=5)
		{
			double v = sqrt(pow(i-350, 2)+pow(j-350, 2))-200;
			if (v >= 0 && v < 2)
			{
				rasterizer.drawline_bresenham(350, 350, i, j, VColor(0, rand(), rand(), rand()));
			}
		}
	}
}

void GameManager::draw_triangle()
{
	Rasterizer &rasterizer = Rasterizer::instance();
	Vertex v1(Vector3(400, 30, 0), VColor(0, 1.0f, 0, 0), 0, 0);
	Vertex v2(Vector3(200, 300, 0), VColor(0, 0, 1.0f, 0), 0, 0);
	Vertex v3(Vector3(600, 300, 0), VColor(0, 0, 0, 1.0f), 0, 0);
	Vertex v4(Vector3(700, 100, 0), VColor(0, 0, 0, 1.0f), 0, 0);
	rasterizer.draw_triangle(v1, v2, v3);
	rasterizer.draw_triangle(v1, v3, v4);
}

void GameManager::draw_cube()
{
	Camera &camera = Camera::instance();
	Pipeline &pipeline = Pipeline::Instance();
	//1.模型空间————(模型变换)————>世界空间
	Vector3 rotate_vector(1.0f, 1.0f, 1.0f);
	rotate_vector.normalize();
	Matrix cube_rotate_matrix;
	cube_rotate_matrix.identify();
	cube_rotate_matrix.setRotate(rotate_vector, static_cast<float>(rotateAngle));
	rotateAngle = (rotateAngle + 1) % 360;
	
	Matrix model_move_matrix;
	model_move_matrix.identify();
	model_move_matrix.setTranslation(pCube->position);

	//图元装配(Input Assembly)
	pipeline.inputVertex = pCube->local_vertexes;
	pipeline.model = cube_rotate_matrix * model_move_matrix;
	pipeline.view = camera.get_view();
	pipeline.screen_map = camera.get_screen_map();
	pipeline.projection = camera.get_projection();
	pipeline.viewPosition = camera.get_position();

	pipeline.draw();
}

/************************************************************************/
/* 中点画圆法
 *  其它方法：
 *     （1）：x步进求y法
 *	   （2）：极坐标角度步进法
 */
/************************************************************************/
void GameManager::draw_circle(int xc, int yc, int r)
{
	int x = 0, y = r;
	int p = 1-r;
	while (x<=y)
	{
		fill_point(xc, yc, x, y);
		fill_point(xc, yc, y, x);
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
void GameManager::draw_ellipse(int xe, int ye, int rx, int ry)
{
	int A = rx*rx, B = ry*ry, C = A*B;
	int px = B-A*ry+A/4;
	int x = 0, y = ry;
	while (x*B<y*A)
	{
		fill_point(xe, ye, x, y);
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
		fill_point(xe, ye, x, y);
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
