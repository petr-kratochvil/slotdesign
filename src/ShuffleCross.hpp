#include "Game.hpp"
#include "Reel.hpp"

#ifndef SHUFFLECROSS_HPP
#define SHUFFLECROSS_HPP

#undef min

class WinCalcShuffleCross : public WinCalculator
{
	static const int PicnicBonusSymbol = 8;
	std::string winDescription;
public:
	std::string getWinDescription() { return this->winDescription; }
	WinCalcShuffleCross(int symbolCount, int reelCount, int rowCount)
		: WinCalculator(symbolCount, reelCount, rowCount)
	{}
	int basicWin(const Window& window, Window* highlight = NULL)
	{
		int partialWin = 0;
		partialWin += this->crissCrossWin(window, highlight, &this->winDescription);
		return partialWin;
	}
	int picnicBonus(const Window& window, Window* highlight = NULL) const
	{
		int picnicCount = 0;
		for (int i = 0; i < this->reelCount; i++)
			for (int j = 0; j < this->rowCount; j++)
				if (window.getSymbol(i, j) == PicnicBonusSymbol)
				{
					picnicCount++;
					if (highlight != NULL)
						highlight->setSymbol(i, j, 1);
				}
		return picnicCount;
	}

	void gatherBonus(Window* w)
	{
		static int* counts = new int[this->symbolCount];
		static int* pays = new int[this->symbolCount];

		for (int s = 0; s < symbolCount; s++)
		{
			counts[s] = 0;
			pays[s] = 0;
		}

		for (int i = 0; i < this->reelCount; i++)
			for (int j = 0; j < this->rowCount; j++)
			{
				counts[w->getSymbol(i, j)]++;
			}

		for (int s = 0; s < this->symbolCount; s++)
		{
			pays[s] = this->payLeftN(s, counts[s]);
		}

		Window doneWindow(this->reelCount, this->rowCount);
		
		for (int i = 0; i < this->reelCount; i++)
				for (int j = 0; j < this->rowCount; j++)
					doneWindow.setSymbol(i, j, 0);

		for (int j = 0; j < 3; j++)
		{
			int maxPay = 0;
			int maxSymbol;
			for (int s = 0; s < symbolCount; s++)
			{
				if (pays[s] > maxPay)
				{
					maxPay = pays[s];
					maxSymbol = s;
				}
			}
			if (maxPay > 0)
			{
				for (int i = 0; i < std::min(counts[maxSymbol], 5); i++)
				{
					w->setSymbol(i, j, maxSymbol);
					doneWindow.setSymbol(i, j, 1);
				}
				counts[maxSymbol] -= std::min(counts[maxSymbol], 5);
				pays[maxSymbol] = this->payLeftN(maxSymbol, counts[maxSymbol]);
			}
		}
		
		for (int s = this->symbolCount-1; s >= 0; s--)
		{
			for (int j = 0; j < this->rowCount; j++)
				for (int i = 0; i < this->reelCount; i++)
				{
					if ((doneWindow.getSymbol(i, j) == 0) && (counts[s] > 0))
					{
						counts[s]--;
						doneWindow.setSymbol(i, j, 1);
						w->setSymbol(i, j, s);
					}
				}
		}
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
		ModeFreeSpin,
		ModeGatherBonus
	};

private:
	// interactive mode
	InteractiveMode interactiveMode;

	// logical mode
	bool freeSpinMode;
	int freeSpinsRemaining;
	int spinCount;
	int lastPicnicCount;

public:
	GameShuffleCross()
		: Game(9, 5, 3, "Shuffle Cross", "0")
		, reelSetMain(5, 3)
		, winCalc(9, 5, 3)
		, interactiveMode(ModeNewSpin)
		, freeSpinMode(false)
		, freeSpinsRemaining(0)
		, spinCount(0)
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

	int getCredit() const
	{
		return Settings::startingCredit
				- Settings::bet * this->spinCount
				+ this->stats.statWin.getTotal();
	}


	InteractiveMode getInteractiveMode() { return this->interactiveMode; }
	bool isFreeSpinMode() {	return this->freeSpinMode; }
	int getFreeSpinsRemaining() { return this->freeSpinsRemaining; }
	std::string getWinDescription()
	{
		if (this->interactiveMode == ModeGatherBonus)
			return "";
		std::ostringstream stringStream;
		stringStream << this->winCalc.getWinDescription();
		if (this->lastPicnicCount > 0)
			stringStream << this->lastPicnicCount << "x piknikovy kosik\r\n";
		return stringStream.str();
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
		this->lastPicnicCount = this->winCalc.picnicBonus(this->window, pHighlight);
		this->freeSpinsRemaining += this->lastPicnicCount;
		if (this->freeSpinsRemaining > 0)
		{
			this->freeSpinMode = true;
			this->interactiveMode = ModeFreeSpin;
		}
		else
		{
			this->freeSpinMode = false;
			this->interactiveMode = ModeNewSpin;
		}
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
		switch (this->interactiveMode)
		{
		case ModeNewSpin:
			this->spin();
			this->spinCount++;
			this->updateStats();
			break;
		case ModeFreeSpin:
			this->spin();
			if (this->interactiveMode)
				this->highlightReset();
			this->freeSpinsRemaining--;
			this->lastWinAmount = 0;
			if (this->freeSpinsRemaining <= 0)
				this->freeSpinMode = false;
			this->interactiveMode = ModeGatherBonus;
			break;
		case ModeGatherBonus:
			this->winCalc.gatherBonus(&this->window);
			this->windowReady = true;
			if (this->freeSpinMode)
				this->interactiveMode = ModeFreeSpin;
			else
				this->interactiveMode = ModeNewSpin;
			this->updateStats();
			break;
		}
	}
};
#endif