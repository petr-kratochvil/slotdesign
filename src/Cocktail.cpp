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
	, statBasicThermo("statBasicThermo", L"Teploměr základ")
	, statSwingThermo("statSwingThermo", L"Teploměr houpačka")
	, temperature(1)
	, modeFS(No)
	, modeSwing(No)
	, remainingFScount(0)
	, partialWin(0)
{
	this->stats.push_back(&this->statBasic);
	this->stats.push_back(&this->statTotal);
	this->stats.push_back(&this->statZero);
	this->stats.push_back(&this->statFS);
	this->stats.push_back(&this->statSwing);
	this->stats.push_back(&this->statBasicThermo);
	this->stats.push_back(&this->statSwingThermo);
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
	
	if ((this->modeSwing != End) && ((this->modeFS == No) || (this->modeFS == BeginsNext)))
	{
		this->statBasic.addData(winBasic / this->temperature);
		this->statBasicThermo.addData(winBasic - winBasic / this->temperature);
	}

	this->partialWin += winBasic;

static int fsStartWin = 0;
	if (this->modeFS == BeginsNext)
		fsStartWin = winBasic;

	int FSwin = (this->partialWin - fsStartWin) * (this->modeFS == End?1:0);	
	int swingwin = winBasic * (((this->modeSwing == End) && (this->modeFS == No))?1:0);

	if (
		((this->modeSwing == End) && (this->modeFS == No)) // End of swing
		|| (this->modeFS == End)	// End of FS series
		|| (((this->modeFS == No)) && (this->modeSwing == No))	// basic spin
		)
	{
		this->statTotal.addData(this->partialWin);
		this->statFS.addData(FSwin);
		this->statSwing.addData(swingwin / this->temperature);
		this->statSwingThermo.addData(swingwin - swingwin / this->temperature);
		if (this->partialWin == 0)
			this->statZero.addData(1);
		else
			this->statZero.addData(0);
		this->partialWin = 0;
	}

	this->addNewWin(winBasic, (this->modeSwing == End) || (this->modeFS == InProcess) || (this->modeFS == End));
}

bool GameCocktail::isSwingApplicable()
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
	return replaceSomeReel;
}

void GameCocktail::applySwing()
{
	Window newWindow(this->window);
	int symbolInSwing = this->window.getSymbol(0,1);

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
}

void GameCocktail::spin()
{
	if (this->modeSwing == End)
		this->modeSwing = No;

	if (this->modeFS == BeginsNext)
		this->modeFS = InProcess;
	if (this->modeFS == End)
		this->modeFS = No;

	// basic or basic-FS game mode
	if (this->modeSwing == No)
	{
		if (this->modeFS == InProcess)
			this->remainingFScount--;

		this->reelSetMain.spin(&this->window);
		this->windowReady = true;

		// Swing
		if ((this->window.getSymbol(0, 1) == this->window.getSymbol(4, 1)) && (this->window.getSymbol(0, 1) != 8))
		{
			if (this->isSwingApplicable())
				this->modeSwing = InProcess;
		}
		else
			this->modeSwing = No;
			
		// Temperature
		if ((this->getLastWinAmount() > 0) && (!this->isFreeSpinMode()))
			this->temperatureUp();
		else
			this->temperatureReset();

		// Free spins
		if ((this->window.getSymbol(0, 1) == 8) && (this->window.getSymbol(4, 1) == 8))
		{
			if (this->modeFS == No)
				this->modeFS = BeginsNext;
			this->remainingFScount += 10;
		}
		if ((this->modeFS == InProcess) && (this->remainingFScount <= 0) && (this->modeSwing != InProcess))
			this->modeFS = End;
	}
	// swing or FS-swing mode
	else
	{
		this->applySwing();
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