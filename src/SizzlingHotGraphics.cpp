#include "SizzlingHotGraphics.h"
#include "Win32Graphics.h"

void SizzlingHotGraphics::loadSymbols()
{
	this->bmpSymbol[0] = LoadBitmap(WinGlobal::hInst, MAKEINTRESOURCE(SH_SYMBOL1));
	this->bmpSymbol[1] = LoadBitmap(WinGlobal::hInst, MAKEINTRESOURCE(SH_SYMBOL2));
	this->bmpSymbol[2] = LoadBitmap(WinGlobal::hInst, MAKEINTRESOURCE(SH_SYMBOL3));
	this->bmpSymbol[3] = LoadBitmap(WinGlobal::hInst, MAKEINTRESOURCE(SH_SYMBOL4));
	this->bmpSymbol[4] = LoadBitmap(WinGlobal::hInst, MAKEINTRESOURCE(SH_SYMBOL5));
	this->bmpSymbol[5] = LoadBitmap(WinGlobal::hInst, MAKEINTRESOURCE(SH_SYMBOL6));
	this->bmpSymbol[6] = LoadBitmap(WinGlobal::hInst, MAKEINTRESOURCE(SH_SYMBOL7));
	this->bmpSymbol[7] = LoadBitmap(WinGlobal::hInst, MAKEINTRESOURCE(SH_SYMBOL8));
}