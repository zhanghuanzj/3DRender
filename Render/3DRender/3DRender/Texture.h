#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <string>
#include <vector>
#include <windows.h>
#include <gdiplus.h>  
#include "Math.h"
#include "VColor.h"
#pragma comment(lib,  "gdiplus.lib") 

using namespace std;


class Texture
{
public:
	Texture(string path);
	~Texture();
	inline VColor get_color(float u, float v)
	{
		return pixels[(int)((height-1) * v) * width + (int)((width-1) * u)];
	}
	Gdiplus::Bitmap *bitmap;
	int width;
	int height;
	VColor *  pixels;
};




#endif