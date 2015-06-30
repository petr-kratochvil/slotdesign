#include "Game.h"
#include "Reel.h"

#ifndef SHUFFLECROSS_HPP
#define SHUFFLECROSS_HPP

#undef min

class WinCalcShuffleCross : public WinCalculator
{
	static const int PicnicBonusSymbol = 8;
	std::string winDescription;
public:
	std::string getWinDescription() { return this->winDescription; }
	WinCalcShuffleCross(int symbolCount, int reelCount, int rowCount)
		: WinCalculator(symbolCount, reelCount, rowCount)
	{}
	int basicWin(const Window& window, Window* highlight = NULL);
	int picnicBonus(const Window& window, Window* highlight = NULL) const;

	void gatherBonus(Window* w);
};

class GameShuffleCross : public Game
{
	ReelSet reelSetMain;
	WinCalcShuffleCross winCalc;

public:
	enum InteractiveMode
	{
		ModeNewSpin,
		ModeFreeSpin,
		ModeGatherBonus
	};

private:
	// interactive mode
	InteractiveMode interactiveMode;

	// logical mode
	bool freeSpinMode;
	int freeSpinsRemaining;
	int spinCount;
	int lastPicnicCount;

public:
	GameShuffleCross()
		: Game(9, 5, 3, "Shuffle Cross", "0")
		, reelSetMain(5, 3)
		, winCalc(9, 5, 3)
		, interactiveMode(ModeNewSpin)
		, freeSpinMode(false)
		, freeSpinsRemaining(0)
		, spinCount(0)
	{}
	void load();

	int getCredit() const;

	InteractiveMode getInteractiveMode() { return this->interactiveMode; }
	bool isFreeSpinMode() {	return this->freeSpinMode; }
	int getFreeSpinsRemaining() { return this->freeSpinsRemaining; }
	std::string getWinDescription();
	
private:
	void updateStats();
	
	void spin();

	// overriden from Game - needed in interactive mode for special controls
	// (possibility to change the meaning of Start! button)
	void start();
};
#endif