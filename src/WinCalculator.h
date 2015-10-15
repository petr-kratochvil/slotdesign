#include "Settings.h"
#include "Reel.h"
#include "InputLoader.h"

#ifndef WINCALCULATOR_HPP
#define WINCALCULATOR_HPP
class Payline
{
	const int reelCount;
	int* line;
public:
	Payline(int reelCount);
	~Payline();
	void load(Input* input);
	// position on payline
	int linePos(int reel) const;
};

// Calculates the win of one spin (given as a Window of symbols)
class WinCalculator
{
protected:
	const int symbolCount;
	const int reelCount;
	const int rowCount;
	
	int **payTableBasic;

public:
	int paylineWin(const Window& window, const Payline& payline, Window* highlight = NULL) const;
	int payLeftN(int symbol, int N) const;
	int crissCrossWin(const Window& window, Window* highlight = NULL, std::string* desc = NULL) const;
	virtual void loadPaytable(Input* input);

	WinCalculator(int symbolCount, int reelCount, int rowCount);
};

#endif