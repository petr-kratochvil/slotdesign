#include "SizzlingHot.h"
#include "Game.h"
#include "Reel.h"
#include <vector>

int WinCalcSizzlingHot::paylineWin7(const Window& window, const Payline& payline, Window* highlight) const
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
	level = 5; // 5 is average
	switch (level)
	{
	case 1:
		switch (sevenCount)
		{
		case 3:
			return 20;
		case 4:
			return 100;
		case 5:
			return 500;
		}
	case 2:
		switch (sevenCount)
		{
		case 3:
			return 40;
		case 4:
			return 200;
		case 5:
			return 1000;
		}
	case 3:
		switch (sevenCount)
		{
		case 3:
			return 60;
		case 4:
			return 300;
		case 5:
			return 1500;
		}
	case 4:
		switch (sevenCount)
		{
		case 3:
			return 100;
		case 4:
			return 1000;
		case 5:
			return 5000;
		}
	case 5:
		switch (sevenCount)
		{
		case 3:
			return 55;
		case 4:
			return 400;
		case 5:
			return 2000;
		}
	}
}

int WinCalcSizzlingHot::scatterWinStar(const Window& window, Window* highlight) const
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

void WinCalcSizzlingHot::loadPaytable(Input* input)
{
	WinCalculator::loadPaytable(input);
	this->payTableBasic[0][1] = 5;
}

int WinCalcSizzlingHot::leftWin(const Window& window, std::vector<Payline*>& paylines, Window* highlight) const
{
	int partialWin = 0;
	for (int i=0; i<this->paylineCount; i++)
	{
		partialWin += this->paylineWin(window, *paylines[i], highlight);
	}
	return partialWin;
}

int WinCalcSizzlingHot::leftWin7(const Window& window, std::vector<Payline*>& paylines, Window* highlight) const
{
	int partialWin = 0;
	for (int i=0; i<this->paylineCount; i++)
	{
		partialWin += this->paylineWin7(window, *paylines[i], highlight);
	}
	return partialWin;
}


GameSizzlingHot::GameSizzlingHot()
	: Game(8, 5, 3, "Sizzling Hot", "2.1")
	, winCalc(8, 5, 3, 5)
	, paylineCount(5)
	, reelSetMain(5, 3)
	, reelSetZero(5, 3)
	, paylines(5)
{
	for (int i=0; i<5; i++)
		this->paylines[i] = new Payline(5);
}
void GameSizzlingHot::load()
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

std::string GameSizzlingHot::getRSVersion() const
{
	char buff[200];
	sprintf(buff, "M%d, Z%d", this->reelSetMain.getVersion(), this->reelSetZero.getVersion());
	std::string result(buff);
	return result;
}

void GameSizzlingHot::loadPaylines(Input* input)
{
	for (int i = 0; i < this->paylineCount; i++)
		this->paylines[i]->load(input);
}

void GameSizzlingHot::updateStats()
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
	assert(this->lastWinAmount <= 5000);
	
	this->stats.statWin7.addData(win7);
	this->stats.statWinStar.addData(winStar);

	this->stats.addWinFromOneSpin(winBasic, this->lastWinAmount);
	
	this->stats.statReel0.addData((this->reelSetUsed == 0)?1:0);
}

void GameSizzlingHot::spin()
{
	//this->reelSetMain.shuffleReels();
	//this->reelSetZero.shuffleReels();
	if (true)
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
