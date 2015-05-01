#include <stdlib.h>
#include <time.h>
#include <random>
#ifndef RANDOM_HPP
#define RANDOM_HPP

// All randomness comes from this class
// Uses Mersenne twister with period of 2^19937-1
class Random
{
	static std::mt19937 generator;
public:
	static void init()
	{
	}
	static int gen(int min, int max)
	{
		std::uniform_int_distribution<int> distribution(min, max);
		return distribution(Random::generator);

		int g;
		int mod = max - min +1;
		do
		{
			g = rand();
		} while (RAND_MAX / mod == g / mod);
		return (g % mod) + min;
	}
	static bool genPct(int pct)
	{
		return (Random::gen(0, 99) < pct);
	}
	static bool genPml(int pml)
	{
		return (Random::gen(0, 999) < pml);
	}
	static bool genP4(int p4)
	{
		return (Random::gen(0, 9999) < p4);
	}
};

namespace {
	int getFirstSeed()
	{
		srand((unsigned int)time(NULL));
		return rand();
	}
}

std::mt19937 Random::generator(getFirstSeed());

#endif