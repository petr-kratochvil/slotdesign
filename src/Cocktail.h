#include "Game.h"
#include "Reel.h"
#include <vector>

#ifndef COCKTAIL_HPP
#define COCKTAIL_HPP

class GameCocktail : public Game
{
	ReelSet reelSetMain;
	WinCalculator winCalc;
	StatItem statBasic, statTotal;
	int temperature;
	bool modeFS;	// next spin is part of Freespin series
	bool modeSwing;	// next spin is a respin caused by the swing

public:
	GameCocktail();
	void load();
	std::string getRSVersion() const;
	int getTemperature() const { return this->temperature; }
	bool isSwingMode() const { return this->modeSwing; }

private:
	void updateStats();
	void spin();
	void temperatureUp();
	void temperatureReset();
};

#endif