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

void Win32Graphics::paint(HDC hdc)
{
	this->paintBasic(hdc);
}

void Win32Graphics::paintBasic(HDC hdc)
{
	if (!this->wasInitialized)
		return;

	Gdiplus::Graphics graphics(hdc);
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);

	// Draw the frame around
	int padding = 15;
	graphics.DrawRectangle(this->penFrame, this->offsetX - padding, this->offsetY - padding
							, 2 * padding + this->reelCount * this->symbolW
							, 2 * padding + this->rowCount * this->symbolH);

	// Draw symbols
	if (WinGlobal::game->isWindowReady())
	{
		for (int i = 0; i < this->reelCount; i++)
			for (int j = 0; j < this->rowCount; j++)
			{
				int symbolID = WinGlobal::game->getWindow().getSymbol(i, j);
				assert((symbolID >= 0) && (symbolID < this->symbolCount));
				DrawState(hdc, NULL, NULL, LPARAM(this->bmpSymbol[symbolID]), 0
					,this->offsetX + i * this->symbolW, this->offsetY + j * this->symbolH
					, 0, 0, DST_BITMAP);
		}
	}

	// Draw the grid
	for (int j = 0; j <= this->rowCount; j++)
	{
		graphics.DrawLine(this->penGrid
			, this->offsetX, this->offsetY + j * this->symbolH
			, this->offsetX + this->reelCount * this->symbolW, this->offsetY + j * this->symbolH);
	}
	for (int i = 0; i <= this->reelCount; i++)
	{
			graphics.DrawLine(this->penGrid
			, this->offsetX + i * this->symbolW, this->offsetY 
			, this->offsetX + i * this->symbolW, this->offsetY + this->rowCount * this->symbolH);
	}

	// Draw higlights
	int margin = 4;
	for (int i = 0; i < this->reelCount; i++)
		for (int j = 0; j < this->rowCount; j++)
		{
			if (WinGlobal::game->highlighted(i, j))
				graphics.DrawRectangle(this->penHighlight
				, this->offsetX + i * this->symbolW + margin
				, this->offsetY + j * this->symbolH + margin
				, this->symbolW - 2*margin
				, this->symbolH - 2*margin);
		}

	// Draw number values
	wchar_t txtWin[50];
	swprintf(txtWin, L"Kredit: %d", Settings::startingCredit + WinGlobal::game->getCredit());
	TextOut(hdc, this->offsetX, 350, txtWin, wcslen(txtWin));
	swprintf(txtWin, L"Výhra: %d", WinGlobal::game->getLastWinAmount());
	TextOut(hdc, this->width - 1.5*this->offsetX, 350, txtWin, wcslen(txtWin));

	for (int i = 0; i < WinGlobal::Controls::values.size(); i++)
		WinGlobal::Controls::values[i]->paint(graphics);
}

ValueWidget::ValueWidget(std::wstring caption, int xpos, int ypos, int width, int height)
	: xpos(xpos)
	, ypos(ypos)
	, width(width)
	, height(height)
	, caption(caption)
	, format(ValueFormat::FormatInt)
{}

void ValueWidget::paint(Gdiplus::Graphics& graphics)
{
	Gdiplus::Pen* pen = new Gdiplus::Pen(Gdiplus::Color(255, 255, 255, 0), 4.0);
	graphics.DrawLine(pen, xpos, ypos, xpos+width, ypos+height);
	Gdiplus::StringFormat stringFormat;
	stringFormat.SetAlignment(Gdiplus::StringAlignmentCenter);
	stringFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter);
	Gdiplus::Font font(L"Arial", 12);
	Gdiplus::RectF layoutRect(this->xpos, this->ypos, this->width, this->height);
	Gdiplus::SolidBrush brush(Gdiplus::Color(123, 25, 60));
	graphics.DrawString(this->caption.c_str(), this->caption.length(), &font, layoutRect, &stringFormat, &brush);
}