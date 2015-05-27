#include <stdio.h>
#include <assert.h>
#include "Settings.hpp"
#include "Statistics.hpp"
#include "Reel.hpp"
#include "WinCalculator.hpp"
#include "InputLoader.hpp"

#ifndef GAME_HPP
#define GAME_HPP

class Game
{
protected:
	Statistics stats;
	Window window;
	Window highlight;

	// window is filled with symbols (at least one spin was made)
	bool windowReady;
	// how big was the win in the last spin
	int lastWinAmount;

	void highlightReset()
	{
		for (int i = 0; i<Settings::reelCount; i++)
			for (int j=0; j<Settings::rowCount; j++)
				this->highlight.setSymbol(i, j, 0);
	}

public:
	bool isHighlighting;
	bool highlighted(int reel, int row)
	{
		return this->highlight.getSymbol(reel, row);
	}

	Game()
		: windowReady(false)
		, lastWinAmount(0)
		, isHighlighting(false)
	{
		this->highlightReset();
	}

	virtual void load() = 0;

protected:
	// this function should change this->window
	virtual void spin() = 0;

public:
	// add values from the last spin to stats
	virtual void updateStats() {}

	// version of used reelsets (optional)
	virtual std::string getRSVersion()
	{
		return "(Abstract Game class)";
	}

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
				- Settings::bet * this->stats.statWin.getCount()
				+ this->stats.statWin.getTotal();
	}

	// a player has joyfully pressed the big START! button
	void start()
	{
		this->spin();
		this->updateStats();
		// this should be dependent on some on/off option
		// this->stats.writeToFile();
	}
};

#endif