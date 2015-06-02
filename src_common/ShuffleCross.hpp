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
	enum InteractiveMode
	{
		ModeNewSpin,
		ModeGatherBonus
	};

private:
	// interactive mode
	InteractiveMode interactiveMode;

	// logical mode
	bool freeSpinMode;
	int freeSpinsRemaining;

public:
	GameShuffleCross()
		: Game(9, 5, 3)
		, reelSetMain(5, 3)
		, winCalc(9, 5, 3)
		, interactiveMode(GameShuffleCross::ModeNewSpin)
		, freeSpinMode(false)
		, freeSpinsRemaining(0)
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

	InteractiveMode getInteractiveMode()
	{
		return this->interactiveMode;
	}

private:
	void updateStats()
	{
		Window* pHighlight = NULL;
		if (this->isInteractive)
		{
			this->highlightReset();
			pHighlight = &this->highlight;
		}
		int winBasic = this->winCalc.basicWin(this->window, pHighlight);
		this->lastWinAmount = winBasic;
		this->stats.addWinFromOneSpin(winBasic, this->lastWinAmount);
	}

	void spin()
	{
		this->reelSetMain.shuffleReels();
		this->reelSetMain.spin(&this->window);
	
		this->windowReady = true;
	}

	// overriden from Game - needed in interactive mode for special controls
	// (possibility to change the meaning of Start! button)
	void start()
	{
		this->spin();
		this->updateStats();
	}
};
#endif