#include "Cocktail.h"

GameCocktail::GameCocktail()
	: Game(9, 5, 3, "Cocktail", "0")
	, winCalc(9, 5, 3)
	, reelSetMain(5, 3)
	, statBasic("statBasic", L"Základní symboly")
	, statTotal("statTotal", L"Celkem")
	, temperature(0)
{
	this->stats.push_back(&this->statBasic);
	this->stats.push_back(&this->statTotal);
}

void GameCocktail::load()
{
	Input* rsMain = InputLoader::open(INPUT(SC_REELSET0));
	Input* ptable = InputLoader::open(INPUT(SH_PAYTABLE));
	this->reelSetMain.load(rsMain);
	this->winCalc.loadPaytable(ptable);
	InputLoader::close(rsMain);
	InputLoader::close(ptable);
}

std::string GameCocktail::getRSVersion() const
{
	char buff[200];
	sprintf(buff, "M%d", this->reelSetMain.getVersion());
	std::string result(buff);
	return result;
}

void GameCocktail::updateStats()
{
	Window* pHighlight = NULL;
	if (this->isInteractive)
	{
		this->highlightReset();
		pHighlight = &this->highlight;
	}
	int winBasic = this->winCalc.crissCrossWin(this->window, pHighlight);
	int lastWinAmount = winBasic;
	
	this->statBasic.addData(winBasic);
	this->statTotal.addData(lastWinAmount);
	this->addNewWin(lastWinAmount);
}

void GameCocktail::spin()
{
	this->reelSetMain.spin(&this->window);
	this->windowReady = true;
}
