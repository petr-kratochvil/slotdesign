#include <assert.h>
#include <stdio.h>
#include <windows.h>
#include <GdiPlus.h>
#include "Win32Graphics.h"
#include "WinGlobal.h"
#include "resource.h"
#include "Settings.h"
#include "Game.h"
#include "Reel.h"

Win32Graphics::Win32Graphics(int width, int height)
	: width(width)
	, height(height)
	, wasInitialized(false)
	, symbolCount(WinGlobal::game->symbolCount)
	, reelCount(WinGlobal::game->reelCount)
	, rowCount(WinGlobal::game->rowCount)
{
	this->bmpSymbol = new HBITMAP [this->symbolCount];
}
Win32Graphics::~Win32Graphics()
{
	for (int i = 0; i < 8; i++)
		DeleteObject(this->bmpSymbol[i]);
	delete this->penGrid;
	delete this->penFrame;
}
void Win32Graphics::init()
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