
#include "Game.h"
#include "Reel.h"
#include <vector>

#ifndef SIZZLINGHOT_HPP
#define SIZZLINGHOT_HPP

class WinCalcSizzlingHot : public WinCalculator
{
	const int paylineCount;
	int paylineWin7(const Window& window, const Payline& payline, Window* highlight = NULL) const;
public:
	int scatterWinStar(const Window& window, Window* highlight = NULL) const;

	void loadPaytable(Input* input);

	WinCalcSizzlingHot(int symbolCount, int reelCount, int rowCount, int paylineCount)
		: WinCalculator(symbolCount, reelCount, rowCount)
		, paylineCount(paylineCount)
	{}

	int leftWin(const Window& window, std::vector<Payline*>& paylines, Window* highlight = NULL) const;

	int leftWin7(const Window& window, std::vector<Payline*>& paylines, Window* highlight = NULL) const;
};

class GameSizzlingHot : public Game
{
	ReelSet reelSetMain;
	ReelSet reelSetZero;
	WinCalcSizzlingHot winCalc;
	const int paylineCount;
	std::vector<Payline*> paylines;
	int reelSetUsed;
	StatItem stat7, statStar, statBasic, statTotal;

public:
	GameSizzlingHot();
	void load();

	std::string getRSVersion() const;

private:
	void loadPaylines(Input* input);

	void updateStats();

	void spin();
};
#endif