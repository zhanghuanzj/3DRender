#ifndef RENDER_GAMEMANAGER_H_
#define RENDER_GAMEMANAGER_H_

#include "GameWindow.h"
#include "Rasterizer.h"
#include "Cube.h"
#include "Matrix.h"
#include "Camera.h"
class GameManager
{
public:
	static GameManager& Instance();
	
	void game_start(const int WIDTH, const int HEIGHT);
	void game_update();
	void game_end();

	void fill_point(int cx, int cy, int x, int y);
	void draw_pixels();
	void draw_lines();
	void draw_circle();
	void draw_triangle();
	void draw_cube();
	void draw_circle(int xc, int yc, int r);
	void draw_ellipse(int xe, int ye, int rx, int ry);

	~GameManager()
	{
		if (pCube != nullptr)
		{
			delete pCube;
		}
	}

private:
	GameManager():pCube(nullptr){}
	Cube  * pCube;
	unsigned int rotateAngle = 0;
};
#endif