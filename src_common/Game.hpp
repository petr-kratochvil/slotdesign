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
	const Payline paylines[Settings::paylineCount];
	const WinCalculator winCalc;
	bool windowReady;
	// Game's descendant will probably contain some Reelsets.
public:
	Game()
		: windowReady(false)
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

	// add values form the last spin to stats
	void updateStats()
	{
		int win = this->winCalc.win(this->window, this->paylines);
		this->stats.statWin.addData(win);
	}

	// a player has joyfully pressed the big START! button
	void start()
	{
		this->spin();
		this->updateStats();
		this->stats.writeToFile();
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