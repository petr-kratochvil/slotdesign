#include <windows.h>
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
public:
	Win32Graphics(int width, int height)
		: width(width)
		, height(height)
		, wasInitialized(false)
	{}
	void init()
	{
		this->loadSymbols();
		this->wasInitialized = true;
		this->offsetX = 50;
		this->offsetY = 50;
		this->symbolW = 88;
		this->symbolH = 88;
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

		for (int i = 0; i<Settings::reelCount; i++)
			for (int j=0; j<Settings::windowSize; j++)
				DrawState(hdc, NULL, NULL, LPARAM(this->bmpSymbol[Random::gen(0, 7)]), 0
					,this->offsetX + i * this->symbolW, this->offsetY + j * this->symbolH
					, 0, 0, DST_BITMAP);

	}
};

#endif