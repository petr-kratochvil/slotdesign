// link the Gdiplus library
#pragma comment(lib,"gdiplus.lib")

#include <windows.h>
#include <GdiPlus.h>
#include "src_common/Settings.hpp"
#include "WinMain.h"
#include "vs_slotview/resource.h"
#include "src_common\Random.hpp"

#ifndef WIN32GRAPHICS_HPP
#define WIN32GRAPHICS_HPP

class Win32Graphics
{
	int width, height;
	int offsetX, offsetY;
	int symbolW, symbolH;
	bool wasInitialized;
	HBITMAP bmpSymbol[Settings::symbolCount];

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::Pen* penGrid;
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
		this->penGrid = new Gdiplus::Pen(Gdiplus::Color(255, 250, 250, 250), 1.0);

		this->wasInitialized = true;
	}
private:
	void loadSymbols()
	{
		this->bmpSymbol[0] = LoadBitmap(WinMainGlobal::hInst, MAKEINTRESOURCE(SZ_SYMBOL1));
		this->bmpSymbol[1] = LoadBitmap(WinMainGlobal::hInst, MAKEINTRESOURCE(SZ_SYMBOL2));
		this->bmpSymbol[2] = LoadBitmap(WinMainGlobal::hInst, MAKEINTRESOURCE(SZ_SYMBOL3));
		this->bmpSymbol[3] = LoadBitmap(WinMainGlobal::hInst, MAKEINTRESOURCE(SZ_SYMBOL4));
		this->bmpSymbol[4] = LoadBitmap(WinMainGlobal::hInst, MAKEINTRESOURCE(SZ_SYMBOL5));
		this->bmpSymbol[5] = LoadBitmap(WinMainGlobal::hInst, MAKEINTRESOURCE(SZ_SYMBOL6));
		this->bmpSymbol[6] = LoadBitmap(WinMainGlobal::hInst, MAKEINTRESOURCE(SZ_SYMBOL7));
		this->bmpSymbol[7] = LoadBitmap(WinMainGlobal::hInst, MAKEINTRESOURCE(SZ_SYMBOL8));
	}

public:
	void paint(HDC hdc)
	{
		if (!this->wasInitialized)
			return;

		Gdiplus::Graphics graphics(hdc);
		graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

		for (int i = 0; i<Settings::reelCount; i++)
			for (int j=0; j<Settings::windowSize; j++)
				DrawState(hdc, NULL, NULL, LPARAM(this->bmpSymbol[Random::gen(0, 7)]), 0
					,this->offsetX + i * this->symbolW, this->offsetY + j * this->symbolH
					, 0, 0, DST_BITMAP);

		for (int j=0; j<=Settings::windowSize; j++)
		{
			graphics.DrawLine(this->penGrid
				, this->offsetX, this->offsetY + j * this->symbolH
				, this->offsetX + Settings::reelCount * this->symbolW, this->offsetY + j * this->symbolH);
		}
		for (int i = 0; i<=Settings::reelCount; i++)
		{
				graphics.DrawLine(this->penGrid
				, this->offsetX + i * this->symbolW, this->offsetY 
				, this->offsetX + i * this->symbolW, this->offsetY + Settings::windowSize * this->symbolH);
		}

	}
};

#endif