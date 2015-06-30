#include "ShuffleCrossGraphics.h"
#include "ShuffleCross.h"
#include "Win32Graphics.h"


void ShuffleCrossGraphics::setStartButtonCaption()
{
	switch (dynamic_cast<GameShuffleCross*>(WinGlobal::game)->getInteractiveMode())
	{
	case GameShuffleCross::ModeNewSpin:
		SetWindowText(WinGlobal::Controls::buttonStart, L"Start !");
		break;
	case GameShuffleCross::ModeFreeSpin:
		SetWindowText(WinGlobal::Controls::buttonStart, L"Free spin");
		break;
	case GameShuffleCross::ModeGatherBonus:
		SetWindowText(WinGlobal::Controls::buttonStart, L"Seber bonus !	");
		break;
	}
}

void ShuffleCrossGraphics::paint(HDC hdc)
{
	if (!this->wasInitialized)
		return;

	this->setStartButtonCaption();

	Gdiplus::Graphics graphics(hdc);
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

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
	wchar_t txtWin[500];
	swprintf(txtWin, L"Kredit: %d", WinGlobal::game->getCredit());
	TextOut(hdc, this->offsetX, 350, txtWin, wcslen(txtWin));
	swprintf(txtWin, L"Výhra: %d", WinGlobal::game->getLastWinAmount());
	TextOut(hdc, this->width - 1.5*this->offsetX, 350, txtWin, wcslen(txtWin));
	if (dynamic_cast<GameShuffleCross*>(WinGlobal::game)->isFreeSpinMode())
	{
		swprintf(txtWin, L"Zbývají free spiny: %d", dynamic_cast<GameShuffleCross*>(WinGlobal::game)->getFreeSpinsRemaining());
		TextOut(hdc, this->width - 1.5*this->offsetX, 370, txtWin, wcslen(txtWin));
	}

	// write win description to the edit box
	swprintf(txtWin, L"%S", dynamic_cast<GameShuffleCross*>(WinGlobal::game)->getWinDescription().c_str());
	SetWindowText(WinGlobal::Controls::editInfo, txtWin);
}

void ShuffleCrossGraphics::loadSymbols()
{
	this->bmpSymbol[0] = LoadBitmap(WinGlobal::hInst, MAKEINTRESOURCE(SC_SYMBOL1));
	this->bmpSymbol[1] = LoadBitmap(WinGlobal::hInst, MAKEINTRESOURCE(SC_SYMBOL2));
	this->bmpSymbol[2] = LoadBitmap(WinGlobal::hInst, MAKEINTRESOURCE(SC_SYMBOL3));
	this->bmpSymbol[3] = LoadBitmap(WinGlobal::hInst, MAKEINTRESOURCE(SC_SYMBOL4));
	this->bmpSymbol[4] = LoadBitmap(WinGlobal::hInst, MAKEINTRESOURCE(SC_SYMBOL5));
	this->bmpSymbol[5] = LoadBitmap(WinGlobal::hInst, MAKEINTRESOURCE(SC_SYMBOL6));
	this->bmpSymbol[6] = LoadBitmap(WinGlobal::hInst, MAKEINTRESOURCE(SC_SYMBOL7));
	this->bmpSymbol[7] = LoadBitmap(WinGlobal::hInst, MAKEINTRESOURCE(SC_SYMBOL8));
	this->bmpSymbol[8] = LoadBitmap(WinGlobal::hInst, MAKEINTRESOURCE(SC_SYMBOL9));
}