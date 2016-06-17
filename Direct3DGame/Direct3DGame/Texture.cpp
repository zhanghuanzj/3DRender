#include "Texture.h"
using namespace Gdiplus;
Texture::Texture(string path)
{
	GdiplusStartupInput gdiplusstartupinput;
	ULONG_PTR gdiplustoken;
	GdiplusStartup(&gdiplustoken, &gdiplusstartupinput, NULL);

	wstring wstr(path.length(),L' ');
	copy(path.begin(), path.end(), wstr.begin());
	bitmap = new Gdiplus::Bitmap(wstr.c_str());
	width = bitmap->GetWidth();
	height = bitmap->GetHeight();

	for (int i=0;i<width;++i)
	{
		vector<AColor> pixel;
		for (int j=0;j<height;++j)
		{
			Gdiplus::Color color;
			bitmap->GetPixel(i,j,&color);
			int r = color.GetR();
			pixel.push_back( AColor(color.GetAlpha(),color.GetRed(),color.GetGreen(),color.GetGreen()) );
		}
		pixels.push_back(pixel);
	}
	GdiplusShutdown(gdiplustoken);
}

AColor Texture::get_color(float u,float v)
{
	return pixels[(int)((width-1)*u)][(int)((height-1)*v)];
}

AColor Texture::get_color2(int u,int v)
{
	return pixels[u][v];
}