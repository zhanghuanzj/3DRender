#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

using namespace std;
using namespace Gdiplus;

int main() {
	GdiplusStartupInput gdiplusstartupinput;
	ULONG_PTR gdiplustoken;
	GdiplusStartup(&gdiplustoken, &gdiplusstartupinput, NULL);

	wstring infilename(L"1.jpg");
	string outfilename("color.txt");

	Bitmap* bmp = new Bitmap(infilename.c_str());
	UINT height = bmp->GetHeight();
	UINT width  = bmp->GetWidth();
	cout << "width " << width << ", height " << height << endl;

	AColor color;
	ofstream fout(outfilename.c_str());

	for (UINT y = 0; y < height; y++)
		for (UINT x = 0; x < width ; x++) {
			bmp->GetPixel(x, y, &color);
			fout << x << "," << y << ";"
				<< (int)color.GetRed()   << ","
				<< (int)color.GetGreen() << ","
				<< (int)color.GetBlue()  << endl;
		}

		fout.close();

		delete bmp;
		GdiplusShutdown(gdiplustoken);
		return 0;
}