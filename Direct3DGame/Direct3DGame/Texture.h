#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <string>
#include <vector>
#include <windows.h>
#include <gdiplus.h>  
#include "Math.h"
#pragma comment(lib, "gdiplus.lib") 

using namespace std;


class Texture
{
public:
	Texture(string path);
	AColor get_color(float u,float v);
	AColor Texture::get_color2(int u,int v);
	Gdiplus::Bitmap *bitmap;
	int width;
	int height;
	vector<vector<AColor>> pixels;
};




#endif