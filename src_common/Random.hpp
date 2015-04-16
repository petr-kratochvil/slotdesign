#include <stdlib.h>
#include <time.h>
#ifndef RANDOM_HPP
#define RANDOM_HPP

// All randomness comes from this class
// Should implement a better generation algorithm, to give a uniform distribution
class Random
{
public:
	static void init()
	{
		srand((unsigned int)time(NULL));
	}
	static int gen(int min, int max)
	{
		return (rand() % (max-min+1)) + min;
	}
	static bool genPct(int pct)
	{
		return (Random::gen(0, 99) < pct);
	}
	static bool genPml(int pml)
	{
		return (Random::gen(0, 999) < pml);
	}
};

#endif