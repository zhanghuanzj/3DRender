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
	
	void game_start(const int WIDTH,const int HEIGHT);
	void game_update();
	void game_end();

	void draw_pixels();
	void draw_lines();
	void draw_circle();
	void draw_triangle();
	void draw_cube();
private:
	GameManager():pcube(nullptr){}
	~GameManager()
	{
		if (pcube!=nullptr)
		{
			delete pcube;
		}
	}
	Rasterizer rasterizer;
	Cube *pcube;
	Light light;
	
};
#endif