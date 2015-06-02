
#include "Game.hpp"
#include "Reel.hpp"
#include <vector>

#ifndef SIZZLINGHOT_HPP
#define SIZZLINGHOT_HPP

class WinCalcSizzlingHot : public WinCalculator
{
	const int paylineCount;
	int paylineWin7(const Window& window, const Payline& payline, Window* highlight = NULL) const
	{
		if (window.getSymbol(0, payline.linePos(0)) != 6)
			return 0;
		int sevenCount = 1;
		for (int i = 1; i < this->reelCount; i++)
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

public:
	int scatterWinStar(const Window& window, Window* highlight = NULL) const
	{
		int starCount = 0;
		for (int i = 0; i < this->reelCount; i++)
			for (int j = 0; j < this->rowCount; j++)
				if (window.getSymbol(i, j) == 7)
					starCount++;
		if (starCount < 3)
			return 0;

		if (highlight != NULL)
		{
			for (int i = 0; i < this->reelCount; i++)
				for (int j = 0; j < this->rowCount; j++)
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

	void loadPaytable(Input* input)
	{
		WinCalculator::loadPaytable(input);
		this->payTableBasic[0][1] = 5;
	}

	WinCalcSizzlingHot(int symbolCount, int reelCount, int rowCount, int paylineCount)
		: WinCalculator(symbolCount, reelCount, rowCount)
		, paylineCount(paylineCount)
	{}

	int leftWin(const Window& window, std::vector<Payline>& paylines, Window* highlight = NULL) const
	{
		int partialWin = 0;
		for (int i=0; i<this->paylineCount; i++)
		{
			partialWin += this->paylineWin(window, paylines[i], highlight);
		}
		return partialWin;
	}

	int leftWin7(const Window& window, std::vector<Payline>& paylines, Window* highlight = NULL) const
	{
		int partialWin = 0;
		for (int i=0; i<this->paylineCount; i++)
		{
			partialWin += this->paylineWin7(window, paylines[i], highlight);
		}
		return partialWin;
	}
};

class GameSizzlingHot : public Game
{
	ReelSet reelSetMain;
	ReelSet reelSetZero;
	WinCalcSizzlingHot winCalc;
	const int paylineCount;
	std::vector<Payline> paylines;
	int reelSetUsed;

public:
	GameSizzlingHot()
		: Game(8, 5, 3)
		, winCalc(8, 5, 3, 5)
		, paylineCount(5)
		, reelSetMain(5, 3)
		, reelSetZero(5, 3)
		, paylines(5, Payline(5))
	{}
	void load()
	{
		Input* rsMain = InputLoader::open(INPUT(SH_REELSET0));
		Input* rsZero = InputLoader::open(INPUT(SH_REELSET1));
		Input* plines = InputLoader::open(INPUT(SH_PAYLINES));
		Input* ptable = InputLoader::open(INPUT(SH_PAYTABLE));
		this->reelSetMain.load(rsMain);
		this->reelSetZero.load(rsZero);
		this->loadPaylines(plines);
		this->winCalc.loadPaytable(ptable);
		InputLoader::close(rsMain);
		InputLoader::close(rsZero);
		InputLoader::close(plines);
		InputLoader::close(ptable);
	}

	std::string getRSVersion() const
	{
		char buff[200];
		sprintf(buff, "M%d, Z%d", this->reelSetMain.getVersion(), this->reelSetZero.getVersion());
		std::string result(buff);
		return result;
	}

private:
	void loadPaylines(Input* input)
	{
		for (int i = 0; i < this->paylineCount; i++)
			this->paylines[i].load(input);
	}

	void updateStats()
	{
		Window* pHighlight = NULL;
		if (this->isInteractive)
		{
			this->highlightReset();
			pHighlight = &this->highlight;
		}
		int winBasic = this->winCalc.leftWin(this->window, this->paylines, pHighlight);
		int win7 = this->winCalc.leftWin7(this->window, this->paylines, pHighlight);
		int winStar = this->winCalc.scatterWinStar(this->window, pHighlight);
		this->lastWinAmount = winBasic + win7 + winStar;
		this->stats.statWin.addData(this->lastWinAmount);
		this->stats.statWinBasic.addData(winBasic);
		this->stats.statWin7.addData(win7);
		this->stats.statWinStar.addData(winStar);
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
		this->stats.statReel0.addData((this->reelSetUsed == 0)?1:0);
	}

	void spin()
	{
		this->reelSetMain.shuffleReels();
		this->reelSetZero.shuffleReels();
		if (Random::genPct(76))
		{
			this->reelSetUsed = 0;
			this->reelSetMain.spin(&this->window);
		}
		else
		{
			this->reelSetUsed = 1;
			this->reelSetZero.spin(&this->window);
		}
		this->windowReady = true;
	}
};
#endif