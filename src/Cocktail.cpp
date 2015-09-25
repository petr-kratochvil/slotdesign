#include "Cocktail.h"

GameCocktail::GameCocktail()
	: Game(9, 5, 3, "Cocktail", "0")
	, winCalc(9, 5, 3)
	, reelSetMain(5, 3)
	, statBasic("statBasic", L"Základní symboly")
	, statTotal("statTotal", L"Celkem")
	, statZero("statZero", L"Nulové otáčky")
	, statFS("statFS", L"Free spiny")
	, statSwing("statSwing", L"Houpačka")
	, temperature(1)
	, modeFS(Basic)
	, modeSwing(Basic)
	, remainingFScount(0)
	, partialWin(0)
{
	this->stats.push_back(&this->statBasic);
	this->stats.push_back(&this->statTotal);
	this->stats.push_back(&this->statZero);
	this->stats.push_back(&this->statFS);
	this->stats.push_back(&this->statSwing);
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
	
	if ((this->modeSwing != End) && (this->modeFS != End) && (this->modeFS != InProcess))
		this->statBasic.addData(winBasic);

	int FSwin = this->partialWin * (this->modeFS == End?1:0);	
	int swingwin = this->partialWin * ((this->modeSwing == End) && (this->modeFS == Basic)?1:0);

	this->partialWin += lastWinAmount;

	if (
		((this->modeSwing == End) && (this->modeFS == Basic))
		|| ((this->modeFS == End))
		|| (((this->modeFS == Basic)) && (this->modeSwing == Basic))
		)
	{
		this->statTotal.addData(this->partialWin);
		this->statFS.addData(FSwin);
		this->statSwing.addData(swingwin);
		if (this->partialWin == 0)
			this->statZero.addData(1);
		else
			this->statZero.addData(0);
		this->partialWin = 0;
	}

	this->addNewWin(lastWinAmount, (this->modeSwing == End) || (this->modeFS == InProcess) || (this->modeFS == End));
}

void GameCocktail::spin()
{
	if (this->modeSwing == End)
		this->modeSwing = Basic;

	if (this->modeFS == End)
		this->modeFS = Basic;
	if (this->modeFS == Begin)
		this->modeFS = InProcess;

	if (this->modeSwing == Basic)
	{
		if (this->modeFS != Basic)
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
				this->modeSwing = Begin;
		}
		else
			this->modeSwing = Basic;
		if ((this->getLastWinAmount() > 0) && (!this->isFreeSpinMode()))
			this->temperatureUp();
		else
			this->temperatureReset();
		if ((this->window.getSymbol(0, 1) == 8) && (this->window.getSymbol(4, 1) == 8))
		{
			if (this->modeFS == Basic)
				this->modeFS = Begin;
			this->remainingFScount += 10;
		}
		if ((this->modeFS == InProcess) && (this->remainingFScount <= 0) && (this->modeSwing != Begin))
			this->modeFS = End;
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
		this->modeSwing = End;
		if ((this->modeFS == InProcess) && (this->remainingFScount <= 0))
			this->modeFS = End;
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