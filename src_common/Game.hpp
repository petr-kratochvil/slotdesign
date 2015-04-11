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
	bool windowReady;
	// how big was the win in the last spin
	int lastWinAmount;
	// Game's descendant will probably contain some Reelsets.
public:
	Game()
		: windowReady(false)
		, lastWinAmount(0)
	{}

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
	}

	// a player has joyfully pressed the big START! button
	void start()
	{
		this->spin();
		this->updateStats();
		// this should be dependent on some on/off option
		// this->stats.writeToFile();
	}

	// this function should change this->window
	virtual void spin()
	{
		for (int i = 0; i < Settings::reelCount; i++)
			for (int j = 0; j < Settings::windowSize; j++)
				this->window.setSymbol(i, j, i+j);
		this->windowReady = true;
	}

};

#endif