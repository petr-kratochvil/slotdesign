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
	const int symbolCount;
	const int reelCount;
	const int rowCount;
	int width, height;
	int offsetX, offsetY;
	int symbolW, symbolH;
	bool wasInitialized;
	HBITMAP* bmpSymbol;

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
		, symbolCount(WinGlobal::game->symbolCount)
		, reelCount(WinGlobal::game->reelCount)
		, rowCount(WinGlobal::game->rowCount)
	{
		this->bmpSymbol = new HBITMAP [this->symbolCount];
	}
	~Win32Graphics()
	{
		for (int i = 0; i < 8; i++)
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
		this->offsetX = (this->width-this->reelCount*this->symbolW)/2;
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
	virtual void loadSymbols()
	{}
};

#endif