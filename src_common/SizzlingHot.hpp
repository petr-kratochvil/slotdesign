#include "Game.hpp"
#include "Reel.hpp"

#ifndef SIZZLINGHOT_HPP
#define SIZZLINGHOT_HPP

class GameSizzlingHot : public Game
{
	ReelSet reelSetMain;
	ReelSet reelSetZero;
	WinCalculator winCalc;
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