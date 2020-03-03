#include "GameManager.h"

int main()
{
	const int WIDTH = 800;
	const int HEIGHT = 800;
	GameManager::Instance().game_start(WIDTH, HEIGHT);
}