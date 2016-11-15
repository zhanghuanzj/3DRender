#include "GameManager.h"
#include <cmath>

int main()
{
	const int WIDTH = 800;
	const int HEIGHT = 800;
	GameManager::Instance().game_start(WIDTH,HEIGHT);
}