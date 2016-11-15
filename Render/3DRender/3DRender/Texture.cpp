#include "Texture.h"
using namespace Gdiplus;
Texture::Texture(string path):pixels()
{
	GdiplusStartupInput gdiplusstartupinput;
	ULONG_PTR gdiplustoken;
	GdiplusStartup(&gdiplustoken, &gdiplusstartupinput, NULL);

	wstring wstr(path.length(),L' ');
	copy(path.begin(), path.end(), wstr.begin());
	bitmap = new Gdiplus::Bitmap(wstr.c_str());
	width = bitmap->GetWidth();
	height = bitmap->GetHeight();

	pixels = new AColor[height*width];
		;vector<vector<AColor>>(height,vector<AColor>(width,AColor(0.0f,0.0f,0.0f,0.0f)));
	for (int i=0;i<width;++i)
	{
		for (int j=0;j<height;++j)
		{
			Gdiplus::Color color;
			bitmap->GetPixel(i,j,&color);
			pixels[i*width+j] = ( AColor(color.GetAlpha(),color.GetRed(),color.GetGreen(),color.GetGreen()) );
		}
	}
	GdiplusShutdown(gdiplustoken);
}


Texture::~Texture()
{
	delete []pixels;
}
