#include "Random.h"

namespace {
	int getFirstSeed()
	{
		std::random_device rd;
		srand(rd());
		return rand();
	}
}

std::mt19937 Random::generator(getFirstSeed());