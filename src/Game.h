#include "Settings.h"
#include "Statistics.h"
#include "Reel.h"
#include "WinCalculator.h"
#include "InputLoader.h"

#ifndef GAME_HPP
#define GAME_HPP

class Game
{
	// how big was the win in the last spin
	int lastWinAmount;
	int credit;
	int spinCount;

protected:
	std::vector<StatItem*> stats;
	Window window;
	Window highlight;
	// window is filled with symbols (at least one spin was made)
	bool windowReady;
	void highlightReset();
	void addNewWin(int winAmount, bool respin = false);
	void Game::chargeBet();

public:
	const int symbolCount;
	const int reelCount;
	const int rowCount;
	const std::string gameName;
	const std::string gameVersion;
	bool isInteractive;
	bool highlighted(int reel, int row);
	void printFinalFormattedStats(const char* fileName);

	Game(int symbolCount, int reelCount, int rowCount, const std::string& name, const std::string& version);

	virtual void load() = 0;

protected:
	// this function should change this->window
	virtual void spin() = 0;

public:
	// add values from the last spin to stats
	virtual void updateStats() {} // TODO maybe delete

	// version of used reelsets (optional)
	virtual std::string getRSVersion() const;

	const Window& getWindow() const;

	bool isWindowReady() const;

	int getLastWinAmount() const;
	int getSpinCount() const;
	int getCredit() const;
	double getRTP() const;
	virtual bool isFreeSpinMode() {	return false; }
	virtual int getFreeSpinsRemaining() { return 0; }

	// a player has joyfully pressed the big START! button
	virtual void start();
};

#endif