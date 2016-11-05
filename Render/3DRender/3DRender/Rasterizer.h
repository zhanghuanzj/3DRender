#ifndef RENDER_RASTERIZER_H_
#define RENDER_RASTERIZER_H_

#include "DirectXLib.h"

class Rasterizer
{
public:
	void drawline_dda(int x1,int y1,int x2,int y2,AColor color);
	void drawline_mid(int x1,int y1,int x2,int y2,AColor color);
	void drawline_bresenham(int x1,int y1,int x2,int y2,AColor color);
};
#endif