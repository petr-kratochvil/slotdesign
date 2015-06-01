#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "Settings.hpp"
#include "Random.hpp"
#include "InputLoader.hpp"

#ifndef REEL_HPP
#define REEL_HPP

// Visible part of reels. A matrix containing symbols.
class Window
{
	const int reelCount;
	const int rowCount;
	int** symbols;
public:
	Window(int reelCount, int rowCount)
		: reelCount(reelCount)
		, rowCount(rowCount)
	{
		this->symbols = new int* [this->reelCount];
		for (int i = 0; i < this->reelCount; i++)
			this->symbols[i] = new int [this->rowCount];
	}
	~Window()
	{
		for (int i = 0; i < this->reelCount; i++)
			delete [] this->symbols[i];
		delete [] this->symbols;
	}
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
	const int rowCount;
	int* symbols;
	ReelSpinResult(int rowCount)
		: rowCount(rowCount)
	{
		this->symbols = new int [this->rowCount];
	}
	~ReelSpinResult()
	{
		delete [] this->symbols;
	}
};

// Reel is a cyclic list of symbols
class Reel
{
	const int symbolCount;
	const int rowCount;
	int* symbols;
	int lastPosition;

public:
	Reel(int symbolCount, int rowCount)
		: symbolCount(symbolCount)
		, rowCount(rowCount)
	{
		this->symbols = new int [symbolCount];
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
	void load(Input* input)
	{
		for (int j = 0; j < this->symbolCount ; j++)
		{
			this->symbols[j] = input->getInt();
		}
	}
	void spin(ReelSpinResult* res)
	{
		this->lastPosition = Random::gen(0, this->symbolCount - 1);
		for (int i=0; i< this->rowCount; i++)
		{
			res->symbols[i] = this->symbols[(this->lastPosition + i) % this->symbolCount];
		}
	}
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
	int getVersion() const
	{
		return this->reelSetVersion;
	}
	void load(Input* input)
	{
		this->reelSetVersion = input->getInt();
		for (int i = 0; i < this->reelCount; i++)
		{
			int reelLength = input->getInt();
			this->reels[i] = new Reel(reelLength, this->rowCount);
		}
		for (int i = 0; i < this->reelCount; i++)
			this->reels[i]->load(input);
	}

	ReelSet(int reelCount, int rowCount)
		: reelCount(reelCount)
		, rowCount(rowCount)
	{
		this->reelShuffle = new int [this->reelCount];
		this->reels = new Reel* [this->reelCount];
		for (int i=0; i<this->reelCount; i++)
			this->reelShuffle[i] = i;
	}
	// generate a random permutation of reels
	void shuffleReels()
	{
		for (int i = 0; i < this->reelCount; i++)
		{
			int j = Random::gen(0, this->reelCount-1);
			int swap = this->reelShuffle[i];
			this->reelShuffle[i] = this->reelShuffle[j];
			this->reelShuffle[j] = swap;
		}
	}

	void spin(Window* w)
	{
		ReelSpinResult res(this->rowCount);
		for (int i = 0; i < this->reelCount; i++)
		{
			this->reels[this->reelShuffle[i]]->spin(&res);
			for (int j = 0; j < this->rowCount; j++)
				w->setSymbol(i, j, res.symbols[j]);
		}
	}
};

#endif