﻿#include "CocktailGraphics.h"
#include "Cocktail.h"

void CocktailGraphics::init()
{
	Win32Graphics::init();
	this->swingBrush = new Gdiplus::LinearGradientBrush(Gdiplus::Point(0, 2*(this->symbolH)), Gdiplus::Point(this->symbolW, this->symbolH)
							, Gdiplus::Color(0x0,0x0, 0xff), Gdiplus::Color(0xcc, 0xff, 0xcc));
	this->swingBrush->TranslateTransform(this->offsetX-this->highlightMargin/2, this->offsetY-this->highlightMargin);
	this->swingPen = new Gdiplus::Pen(this->swingBrush, 16);
	this->swingPen->SetLineJoin(Gdiplus::LineJoinRound);
}

void CocktailGraphics::paint(HDC hdc)
{
	Gdiplus::Graphics* graphics = this->initGdiplusGraphics(hdc);
	this->paintBasic(hdc, *graphics);
	int margin = 6;
	graphics->DrawRectangle(this->swingPen
				, this->offsetX + 0 * this->symbolW + margin
				, this->offsetY + 1 * this->symbolH + margin
				, this->symbolW - 2*margin
				, this->symbolH - 2*margin);
	graphics->DrawRectangle(this->swingPen
				, this->offsetX + 4 * this->symbolW + margin
				, this->offsetY + 1 * this->symbolH + margin
				, this->symbolW - 2*margin
				, this->symbolH - 2*margin);
	this->drawSymbolHighlights(*graphics);
	this->endGdiplusGraphics(graphics);
}

void CocktailGraphics::loadSymbols()
{
	this->bmpSymbol[0] = LoadBitmap(WinGlobal::hInst, MAKEINTRESOURCE(C_SYMBOL1));
	this->bmpSymbol[1] = LoadBitmap(WinGlobal::hInst, MAKEINTRESOURCE(C_SYMBOL2));
	this->bmpSymbol[2] = LoadBitmap(WinGlobal::hInst, MAKEINTRESOURCE(C_SYMBOL3));
	this->bmpSymbol[3] = LoadBitmap(WinGlobal::hInst, MAKEINTRESOURCE(C_SYMBOL4));
	this->bmpSymbol[4] = LoadBitmap(WinGlobal::hInst, MAKEINTRESOURCE(C_SYMBOL5));
	this->bmpSymbol[5] = LoadBitmap(WinGlobal::hInst, MAKEINTRESOURCE(C_SYMBOL6));
	this->bmpSymbol[6] = LoadBitmap(WinGlobal::hInst, MAKEINTRESOURCE(C_SYMBOL7));
	this->bmpSymbol[7] = LoadBitmap(WinGlobal::hInst, MAKEINTRESOURCE(C_SYMBOL8));
	this->bmpSymbol[8] = LoadBitmap(WinGlobal::hInst, MAKEINTRESOURCE(C_SYMBOL9));
}

void CocktailGraphics::initValueWidgets()
{
	WinGlobal::Controls::values.push_back(new ValueWidget(L"Kredit", this->offsetX - 20, 360, 120, 60));
	WinGlobal::Controls::values.push_back(new ValueWidget(L"Výhra", this->width - this->offsetX - 120 + 20, 360, 120, 60));
	WinGlobal::Controls::values.back()->setHighlight(ValueWidget::HighlightOnPositive);
	WinGlobal::Controls::values.push_back(new ValueWidget(L"Teploměr", 110, 450, 120, 60));
	WinGlobal::Controls::values.push_back(new ValueWidget(L"Počet freespinů", 230, 450, 120, 60));
	WinGlobal::Controls::values.back()->setHighlight(ValueWidget::HighlightOnPositive);
	WinGlobal::Controls::values.push_back(new ValueWidget(L"Počet otáček", 350, 450, 120, 60));
	WinGlobal::Controls::values.push_back(new ValueWidget(L"Výhernost", 470, 450, 120, 60));
	WinGlobal::Controls::values.back()->setFormat(ValueWidget::FormatPercent);
}

void CocktailGraphics::updateValueWidgets()
{
	WinGlobal::Controls::values[0]->setValue(WinGlobal::game->getCredit() + Settings::startingCredit);
	WinGlobal::Controls::values[1]->setValue(WinGlobal::game->getLastWinAmount());
	WinGlobal::Controls::values[2]->setValue(dynamic_cast<GameCocktail*>(WinGlobal::game)->getTemperature());
	WinGlobal::Controls::values[3]->setValue(WinGlobal::game->getFreeSpinsRemaining());
	WinGlobal::Controls::values[4]->setValue(WinGlobal::game->getSpinCount());
	WinGlobal::Controls::values[5]->setValue(WinGlobal::game->getRTP());
}
