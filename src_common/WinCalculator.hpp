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
	int paylineWin7(const Window& window, const Payline& payline, Window* highlight = NULL) const
	{
		if (window.getSymbol(0, payline.linePos(0)) != 6)
			return 0;
		int sevenCount = 1;
		for (int i = 1; i < Settings::reelCount; i++)
		{
			if (window.getSymbol(i, payline.linePos(i)) == 6)
				sevenCount = i + 1;
			else
				break;
		}

		if (sevenCount < 3)
			return 0;

		if (highlight != NULL)
		{
			for (int i = 0; i < sevenCount; i++)
				highlight->setSymbol(i, payline.linePos(i), 1);
		}
		
		int level = Random::gen(1, 4);
		switch (level)
		{
		case 1:
			switch (sevenCount)
			{
			case 3:
				return 70;
			case 4:
				return 150;
			case 5:
				return 750;
			}
		case 2:
			switch (sevenCount)
			{
			case 3:
				return 100;
			case 4:
				return 200;
			case 5:
				return 1000;
			}
		case 3:
			switch (sevenCount)
			{
			case 3:
				return 150;
			case 4:
				return 300;
			case 5:
				return 1500;
			}
		case 4:
			switch (sevenCount)
			{
			case 3:
				return 300;
			case 4:
				return 1000;
			case 5:
				return 5000;
			}
		}
	}

	int scatterWinStar(const Window& window, Window* highlight = NULL) const
	{
		int starCount = 0;
		for (int i = 0; i < Settings::reelCount; i++)
			for (int j = 0; j < Settings::windowSize; j++)
				if (window.getSymbol(i, j) == 7)
					starCount++;
		if (starCount < 3)
			return 0;

		if (highlight != NULL)
		{
			for (int i = 0; i < Settings::reelCount; i++)
				for (int j = 0; j < Settings::windowSize; j++)
					if (window.getSymbol(i, j) == 7)
						highlight->setSymbol(i, j, 1);
		}

		if (starCount > 5)
			starCount = 5;
		switch (starCount)
		{
		case 3:
			return 10;
		case 4:
			return 50;
		case 5:
			return 250;
		}
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

	int win(const Window& window, const Payline* paylines, Window* highlight = NULL) const
	{
		int partialWin = 0;
		for (int i=0; i<Settings::paylineCount; i++)
		{
			int w = this->paylineWin(window, paylines[i], highlight);
			int w7 = this->paylineWin7(window, paylines[i], highlight);
			partialWin += w + w7;
		}
		int wstar = this->scatterWinStar(window, highlight);
		partialWin += wstar;
		return partialWin;
	}
};

#endif