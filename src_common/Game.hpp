#include <stdio.h>
#include <assert.h>
#include "Settings.hpp"
#include "Statistics.hpp"
#include "Reel.hpp"
#include "WinCalculator.hpp"

#ifndef GAME_HPP
#define GAME_HPP

class Game
{
	Statistics stats;
	Window window;
	Payline paylines[Settings::paylineCount];
	WinCalculator winCalc;
	// window is filled with symbols (at least one spin was made)
	bool windowReady;
	// how big was the win in the last spin
	int lastWinAmount;
	// Game's descendant will probably contain some Reelsets.
	ReelSet reelSetMain;
public:
	Game()
		: windowReady(false)
		, lastWinAmount(0)
	{}

	void loadSizzlingHot()
	{
		this->reelSetMain.load("reelset0.txt");
		this->loadPaylines("paylines.txt");
		this->winCalc.loadPaytable("paytable.txt");
	}

private:
	void loadPaylines(char* fileName)
	{
		char filePath[250];
		strcpy(filePath, Settings::pathInputs);
		strcat(filePath, fileName);
		FILE* fr = fopen(filePath, "r");
		assert(fr != NULL);
		for (int i = 0; i < Settings::paylineCount; i++)
			this->paylines[i].load(fr);
		fclose(fr);
	}

public:
	const Statistics& getStats() const
	{
		return this->stats;
	}

	const Window& getWindow() const
	{
		return this->window;
	}

	bool isWindowReady() const
	{
		return this->windowReady;
	}

	int getLastWinAmount() const
	{
		return this->lastWinAmount;
	}

	int getCredit() const
	{
		return Settings::startingCredit
				- Settings::priceOfSpin * this->stats.statWin.getCount()
				+ this->stats.statWin.getTotal();
	}

	// add values form the last spin to stats
	void updateStats()
	{
		this->lastWinAmount = this->winCalc.win(this->window, this->paylines);
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

	// a player has joyfully pressed the big START! button
	void start()
	{
		this->spin();
		this->updateStats();
		// this should be dependent on some on/off option
		// this->stats.writeToFile();
	}

private:
	// this function should change this->window
	virtual void spin()
	{
		this->reelSetMain.shuffleReels();
		this->reelSetMain.spin(&this->window);
		this->windowReady = true;
	}

};

#endif