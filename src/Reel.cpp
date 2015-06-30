#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "Reel.h"
#include "Settings.h"
#include "Random.h"
#include "InputLoader.h"

Window::Window(int reelCount, int rowCount)
	: reelCount(reelCount)
	, rowCount(rowCount)
{
	this->symbols = new int* [this->reelCount];
	for (int i = 0; i < this->reelCount; i++)
		this->symbols[i] = new int [this->rowCount];
}
Window::~Window()
{
	for (int i = 0; i < this->reelCount; i++)
		delete [] this->symbols[i];
	delete [] this->symbols;
}
int Window::getSymbol(int reel, int row) const
{
	return this->symbols[reel][row];
}
void Window::setSymbol(int reel, int row, int symbol)
{
	this->symbols[reel][row] = symbol;
}


ReelSpinResult::ReelSpinResult(int rowCount)
	: rowCount(rowCount)
{
	this->symbols = new int [this->rowCount];
}

ReelSpinResult::~ReelSpinResult()
{
	delete [] this->symbols;
}


Reel::Reel(int symbolCount, int rowCount)
	: symbolCount(symbolCount)
	, rowCount(rowCount)
{
	this->symbols = new int [symbolCount];
}
Reel::~Reel()
{
	delete this->symbols;
}
// Length of the reel is the number of symbols on it
int Reel::getLen()
{
	return this->symbolCount;
}
void Reel::load(Input* input)
{
	for (int j = 0; j < this->symbolCount ; j++)
	{
		this->symbols[j] = input->getInt();
	}
}
void Reel::spin(ReelSpinResult* res)
{
	this->lastPosition = Random::gen(0, this->symbolCount - 1);
	for (int i=0; i< this->rowCount; i++)
	{
		res->symbols[i] = this->symbols[(this->lastPosition + i) % this->symbolCount];
	}
}

int ReelSet::getVersion() const
{
	return this->reelSetVersion;
}
void ReelSet::load(Input* input)
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

ReelSet::ReelSet(int reelCount, int rowCount)
	: reelCount(reelCount)
	, rowCount(rowCount)
{
	this->reelShuffle = new int [this->reelCount];
	this->reels = new Reel* [this->reelCount];
	for (int i=0; i<this->reelCount; i++)
		this->reelShuffle[i] = i;
}
// generate a random permutation of reels
void ReelSet::shuffleReels()
{
	for (int i = 0; i < this->reelCount; i++)
	{
		int j = Random::gen(0, this->reelCount-1);
		int swap = this->reelShuffle[i];
		this->reelShuffle[i] = this->reelShuffle[j];
		this->reelShuffle[j] = swap;
	}
}

void ReelSet::spin(Window* w)
{
	ReelSpinResult res(this->rowCount);
	for (int i = 0; i < this->reelCount; i++)
	{
		this->reels[this->reelShuffle[i]]->spin(&res);
		for (int j = 0; j < this->rowCount; j++)
			w->setSymbol(i, j, res.symbols[j]);
	}
}