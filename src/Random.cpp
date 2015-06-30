#include "Random.h"

namespace {
	int getFirstSeed()
	{
		srand((unsigned int)time(NULL));
		return rand();
	}
}

std::mt19937 Random::generator(getFirstSeed());