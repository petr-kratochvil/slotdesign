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

public:
	GameCocktail();
	void load();
	std::string getRSVersion() const;
	int getTemperature() const { return this->temperature; }

private:
	void updateStats();
	void spin();
};

#endif