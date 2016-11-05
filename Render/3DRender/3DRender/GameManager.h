#ifndef RENDER_GAMEMANAGER_H_
#define RENDER_GAMEMANAGER_H_

#include "GameWindow.h"
#include "Rasterizer.h"
class GameManager
{
public:
	static GameManager& Instance();
	
	void game_start(HWND hwnd,const int WIDTH,const int HEIGHT);
	void game_update();
	void game_end();

private:
	GameManager(){}
	Rasterizer rasterizer;

	
};
#endif