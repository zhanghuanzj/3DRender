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
			pixel.push_back( AColor(static_cast<float>(color.GetAlpha())/255,static_cast<float>(color.GetRed())/255,static_cast<float>(color.GetGreen())/255,static_cast<float>(color.GetGreen())/255) );
		}
		pixels.push_back(pixel);
	}
	GdiplusShutdown(gdiplustoken);
}

AColor Texture::get_color(float u,float v)
{
	/*cout<<u<<" "<<v<<endl;
	cout<<(int)((width-1)*u)<<" "<<(int)((height-1)*v)<<endl;*/
	u = CMID(u,0.0f,1.0f);
	v = CMID(v,0.0f,1.0f);
	return pixels[(int)((width-1)*u)][(int)((height-1)*v)];
}

AColor Texture::get_color2(int u,int v)
{
	return pixels[u][v];
}