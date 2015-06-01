#include <stdio.h>
#include "Settings.hpp"
#include "Reel.hpp"
#include "InputLoader.hpp"

#ifndef WINCALCULATOR_HPP
#define WINCALCULATOR_HPP
class Payline
{
	const int reelCount;
	int* line;
public:
	Payline(int reelCount)
		: reelCount(reelCount)
	{
		this->line = new int[this->reelCount];
	}
	~Payline()
	{
		delete [] this->line;
		this->line = NULL;
	}
	void load(Input* input)
	{
		for (int j = 0; j < this->reelCount; j++)
			this->line[j] = input->getInt();
	}
	// position on payline
	int linePos(int reel) const
	{
		return this->line[reel];
	}
};

// Calculates the win of one spin (given as a Window of symbols)
class WinCalculator
{
protected:
	const int symbolCount;
	const int reelCount;
	const int rowCount;
	
	int **payTableBasic;

	int paylineWin(const Window& window, const Payline& payline, Window* highlight = NULL) const
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
	int payLeftN(int symbol, int N) const
	{
		if (N == 0)
			return 0;
		assert((N <= this->reelCount) && (N > 0));
		return this->payTableBasic[symbol][N-1];
	}
	int crissCrossWin(const Window& window, Window* highlight = NULL) const
	{
		int win = 0;
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
			int partialWin = this->payLeftN(i, symbolsInRow) * paylineCount;
			win += partialWin;
			if ((highlight != NULL) && (partialWin > 0))
			{
				for (int j = 0; j < symbolsInRow; j++)
					for (int k = 0; k < this->rowCount; k++)
						if (window.getSymbol(j, k) == i)
							highlight->setSymbol(j, k, 1);
			}
		}
		return win;
	}

public:
	virtual void loadPaytable(Input* input)
	{
		for (int i = 0 ; i < this->symbolCount; i++)
			for (int j = 3; j <= 5; j++)
				this->payTableBasic[i][j-1] = input->getInt();
	}

	WinCalculator(int symbolCount, int reelCount, int rowCount)
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


};

#endif