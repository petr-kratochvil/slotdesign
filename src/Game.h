#include "Settings.h"
#include "Statistics.h"
#include "Reel.h"
#include "WinCalculator.h"
#include "InputLoader.h"

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
	void highlightReset();

public:
	const int symbolCount;
	const int reelCount;
	const int rowCount;
	const std::string gameName;
	const std::string gameVersion;
	bool isInteractive;
	bool highlighted(int reel, int row);

	Game(int symbolCount, int reelCount, int rowCount, const std::string& name, const std::string& version);

	virtual void load() = 0;

protected:
	// this function should change this->window
	virtual void spin() = 0;

public:
	// add values from the last spin to stats
	virtual void updateStats() {}

	// version of used reelsets (optional)
	virtual std::string getRSVersion() const;

	const Statistics& getStats() const;

	const Window& getWindow() const;

	bool isWindowReady() const;

	int getLastWinAmount() const;

	virtual int getCredit() const;

	// a player has joyfully pressed the big START! button
	virtual void start();
};

#endif