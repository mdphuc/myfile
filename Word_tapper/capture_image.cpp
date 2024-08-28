#include <windows.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <gdiplus.h> 
#include <strsafe.h>
#include <string.h>

using namespace std;
using namespace Gdiplus;

void Screenshot(){
    // image/bmp  : {557cf400-1a04-11d3-9a73-0000f81ef32e}
    // image/jpeg : {557cf401-1a04-11d3-9a73-0000f81ef32e} 
    // image/gif  : {557cf402-1a04-11d3-9a73-0000f81ef32e} 
    // image/tiff : {557cf405-1a04-11d3-9a73-0000f81ef32e}
    // image/png  : {557cf406-1a04-11d3-9a73-0000f81ef32e}

  const CLSID png = { 0x557cf406, 0x1a04, 0x11d3,{ 0x9a,0x73,0x00,0x00,0xf8,0x1e,0xf3,0x2e } }; 

	HDC hdcScreen = GetDC(NULL);
	HDC hdcMemDC = CreateCompatibleDC(hdcScreen);
	HBITMAP hbitmap;

  int width = GetSystemMetrics(SM_CXFULLSCREEN);
  int height = GetSystemMetrics(SM_CYFULLSCREEN); 

	GdiplusStartupInput gdip;
	ULONG_PTR gdipToken;
	GdiplusStartup(&gdipToken, &gdip, NULL);
	
	hbitmap = CreateCompatibleBitmap(hdcScreen, width, height);
	SelectObject(hdcMemDC, hbitmap);
	
	BitBlt(hdcMemDC, 0, 0, width, height, hdcScreen, 0, 0, SRCCOPY);

  wchar_t path[MAX_PATH] = L"C:\\Users\\Phuc\\Desktop\\Word_tapper\\Screenshot.jpg";

	Bitmap* bmp = new Bitmap(hbitmap, (HPALETTE)0);
	bmp->Save(path, &png, NULL);
	
	GdiplusShutdown(gdipToken);
	
	DeleteObject(hbitmap);
	DeleteObject(hdcMemDC);
	ReleaseDC(NULL, hdcScreen);
}


int main(){
	ShowWindow(GetConsoleWindow(), SW_HIDE);
    Screenshot();
}