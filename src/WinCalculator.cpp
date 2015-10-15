#include <stdio.h>
#include <sstream>
#include "WinCalculator.h"
#include "Settings.h"
#include "Reel.h"
#include "InputLoader.h"

Payline::Payline(int reelCount)
	: reelCount(reelCount)
{
	this->line = new int[this->reelCount];
}
Payline::~Payline()
{
	delete [] this->line;
	this->line = NULL;
}
void Payline::load(Input* input)
{
	for (int j = 0; j < this->reelCount; j++)
		this->line[j] = input->getInt();
}
// position on payline
int Payline::linePos(int reel) const
{
	return this->line[reel];
}

int WinCalculator::paylineWin(const Window& window, const Payline& payline, Window* highlight) const
{
	int N = 0;
	int symbol = window.getSymbol(0, payline.linePos(0));
	for (int i = 1; i < this->reelCount; i++)
	{
		if (symbol == window.getSymbol(i, payline.linePos(i)))
			continue;
		else
		{
			N = i;
			break;
		}
	}
	if (N == 0)
		N = this->reelCount;
	int pay = this->payLeftN(symbol, N);
	if ((highlight != NULL)  && (pay > 0))
	{
		for (int i = 0; i < N; i++)
			highlight->setSymbol(i, payline.linePos(i), 1);
	}
	return pay;
}
int WinCalculator::payLeftN(int symbol, int N) const
{
	if (N == 0)
		return 0;
	if (N > this->reelCount)
		N = this->reelCount;
	return this->payTableBasic[symbol][N-1];
}
int WinCalculator::crissCrossWin(const Window& window, Window* highlight, std::string* desc) const
{
	int win = 0;
	std::ostringstream stringStream;
	if (desc != NULL)
		desc->erase();
	for (int i = 0; i < this->symbolCount; i++)
	{
		int paylineCount = 1;
		int symbolsInRow = 0;
		for (int j = 0; j < this->reelCount; j++)
		{
			int symbolsFound = 0;
			for (int k = 0; k < this->rowCount; k++)
			{
				if (window.getSymbol(j, k) == i)
					symbolsFound++;
			}
			if (symbolsFound == 0)
				break;
			paylineCount *= symbolsFound;
			symbolsInRow = j+1;
		}
		int partialWin = this->payLeftN(i, symbolsInRow);
		if ((desc != NULL) && (partialWin > 0))
		{
			stringStream << paylineCount << "x vyhra " << partialWin << " za radu "
				<< symbolsInRow << " ";
			switch (i)
			{
			case 0:
				stringStream << "tresni";
				break;
			case 1:
				stringStream << "citronu";
				break;
			case 2:
				stringStream << "pomerancu";
				break;
			case 3:
				stringStream << "svestek";
				break;
			case 4:
				stringStream << "hroznu";
				break;
			case 5:
				stringStream << "melounu";
				break;
			case 6:
				stringStream << "sedmicek";
				break;
			case 7:
				stringStream << "hvezd";
				break;
			}
			stringStream << "\r\n";
		}
		win += partialWin * paylineCount;
		if ((highlight != NULL) && (partialWin > 0))
		{
			for (int j = 0; j < symbolsInRow; j++)
				for (int k = 0; k < this->rowCount; k++)
					if (window.getSymbol(j, k) == i)
						highlight->setSymbol(j, k, 1);
		}
	}
	if (desc != NULL)
		*desc += stringStream.str();

	return win;
}

void WinCalculator::loadPaytable(Input* input)
{
	for (int i = 0 ; i < this->symbolCount; i++)
		for (int j = 3; j <= 5; j++)
			this->payTableBasic[i][j-1] = input->getInt();
}

WinCalculator::WinCalculator(int symbolCount, int reelCount, int rowCount)
	: symbolCount(symbolCount)
	, reelCount(reelCount)
	, rowCount(rowCount)
{
	this->payTableBasic = new int*[this->symbolCount];
	for (int i = 0; i < this->symbolCount; i++)
	{
		this->payTableBasic[i] = new int[this->reelCount];
		for (int j = 0 ; j < 5; j++)
			this->payTableBasic[i][j] = 0;
	}
}
