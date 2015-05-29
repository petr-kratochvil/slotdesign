#include <stdio.h>
#include "Settings.hpp"
#include "Reel.hpp"
#include "InputLoader.hpp"

#ifndef WINCALCULATOR_HPP
#define WINCALCULATOR_HPP
class Payline
{
	int line[Settings::reelCount];
public:
	void load(Input* input)
	{
		for (int j = 0; j < Settings::reelCount; j++)
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
	int payTableBasic[Settings::symbolCount][5];
	int paylineWin(const Window& window, const Payline& payline, Window* highlight = NULL) const
	{
		int N = 0;
		int symbol = window.getSymbol(0, payline.linePos(0));
		for (int i = 1; i < Settings::reelCount; i++)
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
			N = Settings::reelCount;
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
		assert((N <= 5) && (N >= 1));
		return this->payTableBasic[symbol][N-1];
	}
	int crissCrossWin(const Window& window, Window* highlight = NULL) const
	{
		return 1;
	}

public:
	void loadPaytable(Input* input)
	{
		for (int i = 0 ; i < Settings::symbolCount; i++)
			for (int j = 3; j <= 5; j++)
				this->payTableBasic[i][j-1] = input->getInt();
		this->payTableBasic[0][1] = 5;
	}

	WinCalculator()
	{
		for (int i = 0; i < Settings::symbolCount; i++)
			for (int j = 0 ; j < 5; j++)
				this->payTableBasic[i][j] = 0;
	}


};

#endif