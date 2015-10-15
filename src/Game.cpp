#include <stdio.h>
#include <assert.h>
#include "Game.h"
#include "Settings.h"
#include "Statistics.h"
#include "Reel.h"
#include "WinCalculator.h"
#include "InputLoader.h"

void Game::highlightReset()
{
	for (int i = 0; i < this->reelCount; i++)
		for (int j = 0; j < this->rowCount; j++)
			this->highlight.setSymbol(i, j, 0);
}

bool Game::highlighted(int reel, int row)
{
	return this->highlight.getSymbol(reel, row);
}

Game::Game(int symbolCount, int reelCount, int rowCount, const std::string& name, const std::string& version)
	: window(reelCount, rowCount)
	, highlight(reelCount, rowCount)
	, credit(0)
	, windowReady(false)
	, lastWinAmount(0)
	, spinCount(0)
	, isInteractive(false)
	, symbolCount(symbolCount)
	, reelCount(reelCount)
	, rowCount(rowCount)
	, gameName(name)
	, gameVersion(version)
{
	this->highlightReset();
}


// version of used reelsets (optional)
std::string Game::getRSVersion() const
{
	return "(Abstract Game class)";
}

const Window& Game::getWindow() const
{
	return this->window;
}

bool Game::isWindowReady() const
{
	return this->windowReady;
}

void Game::addNewWin(int winAmount, bool respin)
{
	this->credit += winAmount;
	if (!respin)
	{
		this->spinCount++;
		this->credit -= Settings::bet;
	}
	this->lastWinAmount = winAmount;
}

int Game::getLastWinAmount() const
{
	return this->lastWinAmount;
}

int Game::getCredit() const
{
	return this->credit;	
}

int Game::getSpinCount() const
{
	return this->spinCount;
}

double Game::getRTP() const
{
	if (this->spinCount > 0)
		return double(this->credit + this->spinCount * Settings::bet)/double(this->spinCount * Settings::bet);
	else
		return 0;
}

// a player has joyfully pressed the big START! button
void Game::start()
{
	this->spin();
	this->updateStats();
	// this should be dependent on some on/off option
	// this->stats.writeToFile();
}

void Game::printFinalFormattedStats(const char* fileName)
{
	StatItem::printFinalFormattedBegin(fileName);
	for (int i = 0; i < this->stats.size(); i++)
		this->stats[i]->printFinalFormatted();
	StatItem::printFinalFormattedEnd();
}