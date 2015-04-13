#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "Settings.hpp"
#include "Random.hpp"

#ifndef REEL_HPP
#define REEL_HPP

// Visible part of reels. A matrix containing symbols.
class Window
{
	int symbols[Settings::reelCount][Settings::windowSize];
public:
	int getSymbol(int reel, int row) const
	{
		return this->symbols[reel][row];
	}
	void setSymbol(int reel, int row, int symbol)
	{
		this->symbols[reel][row] = symbol;
	}
};

// Symbols spun of one reel
struct ReelSpinResult
{
	int symbols[Settings::windowSize];
};

// Reel is a cyclic list of symbols
class Reel
{
	int symbolCount;
	int* symbols;
	int lastPosition;

public:
	Reel(int symbolCount)
		: symbolCount(symbolCount)
	{
		this->symbols = (int*)malloc(symbolCount * sizeof(int));
	}
	~Reel()
	{
		delete this->symbols;
	}
	// Length of the reel is the number of symbols on it
	int getLen()
	{
		return this->symbolCount;
	}
	void load(FILE* fr)
	{
		for (int j = 0; j < this->symbolCount ; j++)
		{
			fscanf(fr, "%d", &this->symbols[j]);
		}
	}
	ReelSpinResult spin()
	{
		ReelSpinResult res;
		this->lastPosition = Random::gen(0, this->symbolCount);
		for (int i=0; i< Settings::windowSize; i++)
		{
			res.symbols[i] = this->symbols[(this->lastPosition+i) % this->symbolCount];
		}
		return res;
	}
};

// Set of reels (Game usually has at least 2 sets, one for wins and one for spins win zero win)
class ReelSet
{
	Reel* reels[Settings::reelCount];
	// Reels need to be shuffled, because they are not identical, so they should appear at random position
	int reelShuffle[Settings::reelCount];

public:
	void load(char* fileName)
	{
		char filePath[250];
		strcpy(filePath, Settings::pathInputs);
		strcat(filePath, fileName);
		FILE* fr = fopen(filePath, "r");
		assert(fr != NULL);
		for (int i = 0; i < Settings::reelCount; i++)
		{
			int reelLength;
			fscanf(fr, "%d", &reelLength);
			this->reels[i] = new Reel(reelLength);
		}
		for (int i = 0; i < Settings::reelCount; i++)
			this->reels[i]->load(fr);
		fclose(fr);
	}

	ReelSet()
	{
		for (int i=0; i<Settings::reelCount; i++)
		{
			this->reelShuffle[i] = i;
		}
	}
	// generate a random permutation of reels
	void shuffleReels()
	{
		for (int i=0; i<Settings::reelCount; i++)
		{
			int j = Random::gen(0, Settings::reelCount-1);
			int swap = this->reelShuffle[i];
			this->reelShuffle[i] = this->reelShuffle[j];
			this->reelShuffle[j] = swap;
		}
	}

	void spin(Window* w)
	{
		ReelSpinResult res;
		for (int i = 0; i < Settings::reelCount; i++)
		{
			res = this->reels[i]->spin();
			for (int j = 0; j < Settings::windowSize; j++)
				w->setSymbol(i, j, res.symbols[j]);
		}
	}
};

#endif