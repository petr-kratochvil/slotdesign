#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#undef max

// Global settings of constants
// This should probably became part of class Game,
// but as long as we are simulating one game at a time, it can stay simly here.
class Settings
{
public:
	static const int symbolCount = 8;
	static const int windowSize = 3;
	static const int reelCount = 5;
	static const int paylineCount = 5;
	static const int priceOfSpin = 5;
	static const int startingCredit = 1000;
	static const char* pathInputs;
	static const char* pathOutputs;
};

const char* Settings::pathInputs = "load/";
const char* Settings::pathOutputs = "outputs/";

#endif