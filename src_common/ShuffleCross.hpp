#include "Game.hpp"
#include "Reel.hpp"

#ifndef SHUFFLECROSS_HPP
#define SHUFFLECROSS_HPP

class WinCalcShuffleCross : public WinCalculator
{
public:
	WinCalcShuffleCross(int symbolCount, int reelCount, int rowCount)
		: WinCalculator(symbolCount, reelCount, rowCount)
	{}
	int basicWin(const Window& window, Window* highlight = NULL) const
	{
		int partialWin = 0;
		partialWin += this->crissCrossWin(window, highlight);
		return partialWin;
	}
};

class GameShuffleCross : public Game
{
	ReelSet reelSetMain;
	WinCalcShuffleCross winCalc;

public:
	GameShuffleCross()
		: Game(9, 5, 3)
		, reelSetMain(5, 3)
		, winCalc(9, 5, 3)
	{}
	void load()
	{
		Input* rsMain = InputLoader::open(INPUT(SC_REELSET0));
		Input* ptable = InputLoader::open(INPUT(SC_PAYTABLE));
		this->reelSetMain.load(rsMain);
		this->winCalc.loadPaytable(ptable);
		InputLoader::close(rsMain);
		InputLoader::close(ptable);
	}

	std::string getRSVersion() const
	{
		std::string result("0");
		return result;
	}

private:
	void updateStats()
	{
		Window* pHighlight = NULL;
		if (this->isHighlighting)
		{
			this->highlightReset();
			pHighlight = &this->highlight;
		}
		int winBasic = this->winCalc.basicWin(this->window, pHighlight);
		this->lastWinAmount = winBasic;
		this->stats.statWin.addData(this->lastWinAmount);
		this->stats.statWinBasic.addData(winBasic);
		if (lastWinAmount == 0)
			this->stats.statWin0.addData();
		else if (lastWinAmount <= 100)
			this->stats.statWinU100.addData();
		else if (lastWinAmount <= 200)
			this->stats.statWinU200.addData();
		else
			this->stats.statWinO200.addData();
		if (this->lastWinAmount > this->stats.maxWin)
			this->stats.maxWin = this->lastWinAmount;
	}

	void spin()
	{
		this->reelSetMain.shuffleReels();
		this->reelSetMain.spin(&this->window);
	
		this->windowReady = true;
	}
};
#endif