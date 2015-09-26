#include "Game.h"
#include "Reel.h"
#include <vector>

#ifndef COCKTAIL_HPP
#define COCKTAIL_HPP

class GameCocktail : public Game
{
	enum GameMode {
		No,
		BeginsNext,
		InProcess,
		End
	};
	ReelSet reelSetMain;
	WinCalculator winCalc;
	StatItem statBasic, statTotal, statZero, statFS, statSwing, statBasicThermo, statSwingThermo;
	int temperature;
	GameMode modeFS;	// next spin is part of Freespin series
	GameMode modeSwing;	// next spin is a respin caused by the swing
	int remainingFScount; // count of remaining free spins in a FS series
	int partialWin; // part of the one-spin win (sum of subsequent respins)

	// returns true if there is at least one reel to spin because od the swing
	// i.e. the swing symbol does not form a 5-in-a-line win
	bool isSwingApplicable();
	// spins the specific reels to complete the swing
	void GameCocktail::applySwing();

public:
	GameCocktail();
	void load();
	std::string getRSVersion() const;
	int getTemperature() const { return this->temperature; }
	bool isSwingMode() const { return this->modeSwing == InProcess; }
	bool isFreeSpinMode() {	return this->modeFS == InProcess; }
	int getFreeSpinsRemaining() { return this->remainingFScount; }

private:
	void updateStats();
	void spin();
	void temperatureUp();
	void temperatureReset();
};

#endif