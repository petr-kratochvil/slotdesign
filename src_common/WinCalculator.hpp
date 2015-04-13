#include <stdio.h>
#include "Settings.hpp"
#include "Reel.hpp"

#ifndef WINCALCULATOR_HPP
#define WINCALCULATOR_HPP
class Payline
{
	int line[Settings::reelCount];
public:
	void load(FILE* fr)
	{
		for (int j = 0; j < Settings::reelCount; j++)
			fscanf(fr, "%d", &this->line[j]);
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
	int paylineWin(const Window& window, const Payline& payline) const
	{
		int symbol = window.getSymbol(0, payline.linePos(0));
		for (int i = 1; i<= Settings::reelCount; i++)
		{
			if (symbol == window.getSymbol(i, payline.linePos(i)))
				continue;
			else
				return 0;
		}
	}
	int payLeftN(int symbol, int N)
	{
		assert((N <= 5) && (N >= 1));
		return this->payTableBasic[symbol][N-1];
	}

public:
	void loadPaytable(char* fileName)
	{
		char filePath[250];
		strcpy(filePath, Settings::pathInputs);
		strcat(filePath, fileName);
		FILE* fr = fopen(filePath, "r");
		assert(fr != NULL);
		for (int i = 0 ; i < Settings::symbolCount; i++)
			for (int j = 3; j <= 5; j++)
				fscanf(fr, "%d", &this->payTableBasic[i][j]);
		fclose(fr);
		this->payTableBasic[0][1] = 5;
	}

	WinCalculator()
	{
		for (int i = 0; i < Settings::symbolCount; i++)
			for (int j = 0 ; j < 5; j++)
				this->payTableBasic[i][j] = 0;
	}

	int win(const Window& window, const Payline* paylines) const
	{
		int partialWin = 0;
		for (int i=0; i<Settings::paylineCount; i++)
		{
			partialWin += this->paylineWin(window, paylines[i]);
		}
		return partialWin;
	}
};

#endif