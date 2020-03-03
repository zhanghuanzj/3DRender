#include "Texture.h"
using namespace Gdiplus;
Texture::Texture(string path):pixels()
{
	GdiplusStartupInput gdiplusstartupinput;
	ULONG_PTR gdiplustoken;
	GdiplusStartup(&gdiplustoken,  &gdiplusstartupinput,  NULL);

	wstring wstr(path.length(), L' ');
	copy(path.begin(),  path.end(),  wstr.begin());
	bitmap = new Gdiplus::Bitmap(wstr.c_str());
	width = bitmap->GetWidth();
	height = bitmap->GetHeight();

	pixels = new VColor[height * width];
	for (int i=0; i<width; ++i)
	{
		for (int j=0; j<height; ++j)
		{
			Gdiplus::Color color;
			bitmap->GetPixel(i, j, &color);
			float denominator = 1.0f / 255;
			pixels[j * width + i] = VColor(color.GetAlpha(), color.GetRed(), color.GetGreen(), color.GetBlue()) * denominator;
		}
	}
	GdiplusShutdown(gdiplustoken);
}


Texture::~Texture()
{
	delete []pixels;
}
