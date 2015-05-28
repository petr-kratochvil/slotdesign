// link the Gdiplus library
#pragma comment(lib,"gdiplus.lib")

#include <assert.h>
#include <stdio.h>
#include <windows.h>
#include <GdiPlus.h>
#include "WinMain.h"
#include "vs_slotview/resource.h"
#include "Settings.hpp"
#include "Game.hpp"
#include "Reel.hpp"

#ifndef WIN32GRAPHICS_HPP
#define WIN32GRAPHICS_HPP

class Win32Graphics
{
protected:
	int width, height;
	int offsetX, offsetY;
	int symbolW, symbolH;
	bool wasInitialized;
	HBITMAP bmpSymbol[Settings::symbolCount];

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::Pen* penGrid;
	Gdiplus::Pen* penFrame;
	Gdiplus::Pen* penFrameWin;
	Gdiplus::Pen* penHighlight;
public:
	Win32Graphics(int width, int height)
		: width(width)
		, height(height)
		, wasInitialized(false)
	{}
	~Win32Graphics()
	{
		for (int i = 0; i < Settings::symbolCount; i++)
			DeleteObject(this->bmpSymbol[i]);
		delete this->penGrid;
		delete this->penFrame;
	}
	void init()
	{
		// Load bitmaps
		this->loadSymbols();
		
		// Set dimensions
		this->symbolW = 88;
		this->symbolH = 88;
		this->offsetX = (this->width-Settings::reelCount*this->symbolW)/2;
		this->offsetY = 50;

		// Init Gdiplus
		Gdiplus::GdiplusStartup(&this->gdiplusToken, &this->gdiplusStartupInput, NULL);
		this->penGrid = new Gdiplus::Pen(Gdiplus::Color(255, 220, 220, 220), 2.0);
		this->penFrame = new Gdiplus::Pen(Gdiplus::Color(255, 0, 0, 120), 3.0);
		this->penHighlight = new Gdiplus::Pen(Gdiplus::Color(255, 255, 255, 0), 4.0);

		this->wasInitialized = true;
	}
public:
	virtual void paint(HDC hdc) = 0;
private:
	void loadSymbols()
	{
		this->bmpSymbol[0] = LoadBitmap(WinGlobal::hInst, MAKEINTRESOURCE(SZ_SYMBOL1));
		this->bmpSymbol[1] = LoadBitmap(WinGlobal::hInst, MAKEINTRESOURCE(SZ_SYMBOL2));
		this->bmpSymbol[2] = LoadBitmap(WinGlobal::hInst, MAKEINTRESOURCE(SZ_SYMBOL3));
		this->bmpSymbol[3] = LoadBitmap(WinGlobal::hInst, MAKEINTRESOURCE(SZ_SYMBOL4));
		this->bmpSymbol[4] = LoadBitmap(WinGlobal::hInst, MAKEINTRESOURCE(SZ_SYMBOL5));
		this->bmpSymbol[5] = LoadBitmap(WinGlobal::hInst, MAKEINTRESOURCE(SZ_SYMBOL6));
		this->bmpSymbol[6] = LoadBitmap(WinGlobal::hInst, MAKEINTRESOURCE(SZ_SYMBOL7));
		this->bmpSymbol[7] = LoadBitmap(WinGlobal::hInst, MAKEINTRESOURCE(SZ_SYMBOL8));
	}
};

#endif