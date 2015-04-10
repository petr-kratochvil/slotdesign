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
		this->bmpSymbol[0] = LoadBitmap(WinGlobal::hInst, MAKEINTRESOURCE(SZ_SYMBOL1));
		this->bmpSymbol[1] = LoadBitmap(WinGlobal::hInst, MAKEINTRESOURCE(SZ_SYMBOL2));
		this->bmpSymbol[2] = LoadBitmap(WinGlobal::hInst, MAKEINTRESOURCE(SZ_SYMBOL3));
		this->bmpSymbol[3] = LoadBitmap(WinGlobal::hInst, MAKEINTRESOURCE(SZ_SYMBOL4));
		this->bmpSymbol[4] = LoadBitmap(WinGlobal::hInst, MAKEINTRESOURCE(SZ_SYMBOL5));
		this->bmpSymbol[5] = LoadBitmap(WinGlobal::hInst, MAKEINTRESOURCE(SZ_SYMBOL6));
		this->bmpSymbol[6] = LoadBitmap(WinGlobal::hInst, MAKEINTRESOURCE(SZ_SYMBOL7));
		this->bmpSymbol[7] = LoadBitmap(WinGlobal::hInst, MAKEINTRESOURCE(SZ_SYMBOL8));
	}

public:
	void paint(HDC hdc)
	{
		if (!this->wasInitialized)
			return;
		if (!WinGlobal::game->isWindowReady())
			return;

		Gdiplus::Graphics graphics(hdc);
		graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

		// Draw symbols
		for (int i = 0; i<Settings::reelCount; i++)
			for (int j=0; j<Settings::windowSize; j++)
			{
				int symbolID = WinGlobal::game->getWindow().getSymbol(i, j);
				assert((symbolID >= 0) && (symbolID < Settings::symbolCount));
				DrawState(hdc, NULL, NULL, LPARAM(this->bmpSymbol[symbolID]), 0
					,this->offsetX + i * this->symbolW, this->offsetY + j * this->symbolH
					, 0, 0, DST_BITMAP);
			}

		// Draw the grid
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

		// Draw number values
		wchar_t txtWin[50];
		swprintf(txtWin, L"Kredit: %d", WinGlobal::game->getTotalWin() + Settings::startingCredit);
		TextOut(hdc, this->offsetX, 350, txtWin, wcslen(txtWin));
		swprintf(txtWin, L"Výhra: %d", WinGlobal::game->getLastWinAmount());
		TextOut(hdc, this->width - 1.5*this->offsetX, 350, txtWin, wcslen(txtWin));
	}
};

#endif