#include "Game.hpp"
#include "Reel.hpp"

#ifndef SIZZLINGHOT_HPP
#define SIZZLINGHOT_HPP

class WinCalcSizzlingHot : public WinCalculator
{
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

}

class GameSizzlingHot : public Game
{
	ReelSet reelSetMain;
	ReelSet reelSetZero;
	WinCalcSizzlingHot winCalc;
	Payline paylines[Settings::paylineCount];

public:
	void load()
	{
		Input* rsMain = InputLoader::open(INPUT(res_reelset0));
		Input* rsZero = InputLoader::open(INPUT(res_reelset1));
		Input* plines = InputLoader::open(INPUT(res_paylines));
		Input* ptable = InputLoader::open(INPUT(res_paytable));
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
		for (int i = 0; i < Settings::paylineCount; i++)
			this->paylines[i].load(input);
	}

	void updateStats()
	{
		Window* pHighlight = NULL;
		if (this->isHighlighting)
		{
			this->highlightReset();
			pHighlight = &this->highlight;
		}
		this->lastWinAmount = this->winCalc.win(this->window, this->paylines, pHighlight);
		this->stats.statWin.addData(this->lastWinAmount);
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
		this->reelSetZero.shuffleReels();
		if (Random::genPml(760))
			this->reelSetMain.spin(&this->window);
		else
			this->reelSetZero.spin(&this->window);
		this->windowReady = true;
	}
};
#endif