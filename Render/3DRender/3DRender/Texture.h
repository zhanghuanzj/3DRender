#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <string>
#include <vector>
#include <windows.h>
#include <gdiplus.h>  
#include "Math.h"
#include "Math.h"
#pragma comment(lib, "gdiplus.lib") 

using namespace std;


class Texture
{
public:
	Texture(string path);
	~Texture();
	inline AColor get_color(float u,float v)
	{
		return pixels[(int)((width-1)*u)*width+(int)((height-1)*v)];
	}
	Gdiplus::Bitmap *bitmap;
	int width;
	int height;
	AColor* pixels;
};




#endif