#include "Settings.h"
#include "Random.h"
#include "InputLoader.h"

#ifndef REEL_HPP
#define REEL_HPP

// Visible part of reels. A matrix containing symbols.
class Window
{
	const int reelCount;
	const int rowCount;
	int** symbols;
public:
	Window(int reelCount, int rowCount);
	Window(const Window& w);
	~Window();
	int getSymbol(int reel, int row) const;
	void setSymbol(int reel, int row, int symbol);
};

// Symbols spun of one reel
struct ReelSpinResult
{
	const int rowCount;
	int* symbols;
	ReelSpinResult(int rowCount);
	~ReelSpinResult();
};

// Reel is a cyclic list of symbols
class Reel
{
	const int symbolCount;
	const int rowCount;
	int* symbols;
	int lastPosition;

public:
	Reel(int symbolCount, int rowCount);
	~Reel();
	// Length of the reel is the number of symbols on it
	int getLen();
	void load(Input* input);
	void spin(ReelSpinResult* res);
	// used in game Cocktail; finds the nearest (in upwards direction)
	// position with given symbol in the first spot
	void spinToSymbol_Cocktail(ReelSpinResult *res, int symbol);
};

// Set of reels (Game usually has at least 2 sets, one for wins and one for spins win zero win)
class ReelSet
{
	const int reelCount;
	const int rowCount;
	Reel** reels;
	// Reels need to be shuffled, because they are not identical, so they should appear at random position
	int* reelShuffle;
	int reelSetVersion;

public:
	int getVersion() const;
	void load(Input* input);

	ReelSet(int reelCount, int rowCount);
	// generate a random permutation of reels
	void shuffleReels();

	void spin(Window* w);
	// used in game Cocktail; see Reel::spinToSymbol_Cocktail
	void spinToSymbol_Cocktail(Window* w, int symbol);
};

#endif