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
	this->paintBasic(hdc);

	wchar_t txtOut[500];
	if (dynamic_cast<GameShuffleCross*>(WinGlobal::game)->isFreeSpinMode())
	{
		swprintf(txtOut, L"Zbývají free spiny: %d", dynamic_cast<GameShuffleCross*>(WinGlobal::game)->getFreeSpinsRemaining());
		TextOut(hdc, this->width - 1.5*this->offsetX, 370, txtOut, wcslen(txtOut));
	}

	// write win description to the edit box
	swprintf(txtOut, L"%S", dynamic_cast<GameShuffleCross*>(WinGlobal::game)->getWinDescription().c_str());
	SetWindowText(WinGlobal::Controls::editInfo, txtOut);
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