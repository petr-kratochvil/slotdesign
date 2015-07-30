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
	delete this->mainBrush;
	delete this->penGrid;
	delete this->penFrame;
	delete this->penFrameWin;
	delete this->penHighlightEven;
	delete this->penHighlightOdd;
	delete this->highlightBrushEven;
	delete this->highlightBrushOdd;
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
	this->highlightMargin = 5;

	// Set colors
	this->mainFrameColor1 = Gdiplus::Color(150, 150, 140);
	this->mainFrameColor2 = Gdiplus::Color(255, 255, 248);
	this->gridColor = Gdiplus::Color(220, 220, 220);
	this->highlightColor1 = Gdiplus::Color(80, 80, 0);
	this->highlightColor2 = Gdiplus::Color(255, 255, 25);

	// Init Gdiplus
	Gdiplus::GdiplusStartup(&this->gdiplusToken, &this->gdiplusStartupInput, NULL);

	this->penGrid = new Gdiplus::Pen(this->gridColor, 1.0);

	this->mainBrush = new Gdiplus::LinearGradientBrush(Gdiplus::Point(50, 500), Gdiplus::Point(650, 0)
						, this->mainFrameColor1, this->mainFrameColor2);
	this->penFrame = new Gdiplus::Pen(this->mainBrush, 12.0);

	this->highlightBrushEven = new Gdiplus::LinearGradientBrush(Gdiplus::Point(0, this->symbolH), Gdiplus::Point(this->symbolW, 0)
							, this->highlightColor1, this->highlightColor2);
	this->highlightBrushOdd = new Gdiplus::LinearGradientBrush(Gdiplus::Point(0, 2*(this->symbolH)), Gdiplus::Point(this->symbolW, this->symbolH)
							, this->highlightColor1, this->highlightColor2);

	this->highlightBrushEven->TranslateTransform(this->offsetX-this->highlightMargin/2, this->offsetY-this->highlightMargin/2);
	this->highlightBrushOdd->TranslateTransform(this->offsetX-this->highlightMargin/2, this->offsetY-this->highlightMargin);
	
	this->penHighlightEven = new Gdiplus::Pen(this->highlightBrushEven, 7.0);
	this->penHighlightOdd = new Gdiplus::Pen(this->highlightBrushOdd, 7.0);

	this->penHighlightEven->SetLineJoin(Gdiplus::LineJoin::LineJoinRound);
	this->penHighlightOdd->SetLineJoin(Gdiplus::LineJoin::LineJoinRound);

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
	graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintClearTypeGridFit);

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
	int margin = this->highlightMargin;
	for (int i = 0; i < this->reelCount; i++)
		for (int j = 0; j < this->rowCount; j++)
		{
			if (WinGlobal::game->highlighted(i, j))
			{
				graphics.DrawRectangle((i+j) % 2? this->penHighlightOdd : this->penHighlightEven
				, this->offsetX + i * this->symbolW + margin
				, this->offsetY + j * this->symbolH + margin
				, this->symbolW - 2*margin
				, this->symbolH - 2*margin);
			}
		}

	// Draw number values
	wchar_t txtWin[50];
	swprintf(txtWin, L"Kredit: %d", Settings::startingCredit + WinGlobal::game->getCredit());
	TextOut(hdc, this->offsetX, 350, txtWin, wcslen(txtWin));
	swprintf(txtWin, L"V�hra: %d", WinGlobal::game->getLastWinAmount());
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
	, bgColor1(150, 150, 50)
	, bgColor2(255, 255, 198)
	, layoutRect(this->xpos, this->ypos, this->width, this->height/2)
	, bgBrush(Gdiplus::Point(xpos, ypos+2*this->height/2)
			, Gdiplus::Point(xpos+this->width, ypos-3*this->height/2)
			, this->bgColor1, this->bgColor2)
	, linePen(this->bgColor1, 2.0)
	, fontBrush(Gdiplus::Color::Black)
	, font(L"Arial", 10)
{
	this->stringFormat.SetAlignment(Gdiplus::StringAlignmentCenter);
	this->stringFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter);
}

void ValueWidget::paint(Gdiplus::Graphics& graphics)
{
	graphics.DrawLine(&this->linePen, xpos, ypos+height, xpos+width, ypos+height);
	graphics.FillRectangle(&this->bgBrush, this->layoutRect);
	graphics.DrawString(this->caption.c_str(), this->caption.length(), &this->font, this->layoutRect, &this->stringFormat, &this->fontBrush);

}