#ifndef RENDER_MYWINDOW_H_
#define RENDER_MYWINDOW_H_

#include <iostream>
#include <cassert>
#include <windows.h>
#include <windowsx.h>

#include "GameManager.h"
#include "DirectXLib.h"

using namespace std;

class GameWindow
{
public:
	GameWindow(const string name_t,const string title_t,int width = 800,int height = 600 );
	~GameWindow();

	void message_dispatch();
	HWND get_hwnd(){return hwnd;}
private:
	string name;
	string title;
	const int WIDTH;
	const int HEIGHT;
	HWND hwnd;

	static LRESULT CALLBACK WndProc(HWND hwnd,UINT message,WPARAM wparam,LPARAM lParam);
};

#endif