#include "Cocktail.h"

GameCocktail::GameCocktail()
	: Game(9, 5, 3, "Cocktail", "0")
	, winCalc(9, 5, 3)
	, reelSetMain(5, 3)
	, statBasic("statBasic", L"Základní symboly")
	, statTotal("statTotal", L"Celkem")
	, temperature(1)
	, modeFS(false)
	, modeSwing(false)
	, remainingFScount(0)
{
	this->stats.push_back(&this->statBasic);
	this->stats.push_back(&this->statTotal);
}

void GameCocktail::load()
{
	Input* rsMain = InputLoader::open(INPUT(C_REELSET0));
	Input* ptable = InputLoader::open(INPUT(C_PAYTABLE));
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
	int winBasic = this->winCalc.crissCrossWin(this->window, pHighlight) * this->temperature;
	int lastWinAmount = winBasic;
	
	this->statBasic.addData(winBasic);
	this->statTotal.addData(lastWinAmount);
	this->addNewWin(lastWinAmount, this->modeSwing || this->modeFS);
}

void GameCocktail::spin()
{
	if (!this->modeSwing)
	{
		if (this->modeFS)
			this->remainingFScount--;
		this->reelSetMain.spin(&this->window);
		this->windowReady = true;
		if ((this->window.getSymbol(0, 1) == this->window.getSymbol(4, 1)) && (this->window.getSymbol(0, 1) != 8))
		{
			bool replaceSomeReel = true;
			for (int i = 1; i < this->reelCount - 1; i++)
			{
				bool replaceThisReel = true;
				for (int j = 0; j < this->rowCount; j++)
				{
					if (this->window.getSymbol(i, j) == this->window.getSymbol(0, 1))
					{
						replaceThisReel = false;
						break;
					}
				}
				if (replaceThisReel)
				{
					replaceSomeReel = true;
					break;
				}
			}
			if (replaceSomeReel)
				this->modeSwing = true;
		}
		else
			this->modeSwing = false;
		if ((this->getLastWinAmount() > 0) && (!this->isFreeSpinMode()))
			this->temperatureUp();
		else
			this->temperatureReset();
		if ((this->window.getSymbol(0, 1) == 8) && (this->window.getSymbol(4, 1) == 8))
		{
			this->modeFS = true;
			this->remainingFScount += 10;
		}
		if (this->remainingFScount <= 0)
			this->modeFS = false;
	}
	else
	{
		int symbolInSwing = this->window.getSymbol(0,1);
		Window newWindow(this->window);
		this->reelSetMain.spinToSymbol_Cocktail(&newWindow, symbolInSwing);
		for (int i = 1; i < this->reelCount - 1; i++)
		{
			bool replaceThisReel = true;
			for (int j = 0; j < this->rowCount; j++)
			{
				if (this->window.getSymbol(i, j) == symbolInSwing)
				{
					replaceThisReel = false;
					break;
				}
			}
			if (replaceThisReel)
			{
				for (int j = 0; j < this->rowCount; j++)
					this->window.setSymbol(i, j, newWindow.getSymbol(i, j));
			}
		}
		this->modeSwing = false;
	}
}

void GameCocktail::temperatureUp()
{
	this->temperature++;
	if (this->temperature > 5)
		this->temperature = 1;
}

void GameCocktail::temperatureReset()
{
	this->temperature = 1;
}