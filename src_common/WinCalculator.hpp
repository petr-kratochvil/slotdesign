#ifndef WINCALCULATOR_HPP
#define WINCALCULATOR_HPP

class Payline
{
	int line[Settings::reelCount];
public:
	void load()
	{
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
	int paylineWin(const Window& window, const Payline& payline) const
	{
	}
public:
	int win(const Window& window, const Payline* paylines) const
	{
		int partialWin = 0;
		for (int i=0; i<Settings::paylineCount; i++)
		{
			partialWin += this->paylineWin(window, paylines[i]);
		}
	}
};

#endif