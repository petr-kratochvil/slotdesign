#include "Statistics.hpp"

#ifndef GAME_HPP
#define GAME_HPP

class Game
{
	Statistics stats;
	Window window;
	const Payline paylines[Settings::paylineCount];
	const WinCalculator winCalc;
	// Game's descendant will probably contain some Reelsets.
public:
	const Statistics& getStats() const
	{
		return this->stats;
	}

	const Window& getWindow() const
	{
		return this->window;
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
	virtual void spin() = 0;

};

#endif