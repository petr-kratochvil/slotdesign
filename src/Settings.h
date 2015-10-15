#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#undef max

// Global settings of constants
// This should probably became part of class Game,
// but as long as we are simulating one game at a time, it can stay simly here.
// test
class Settings
{
public:
	static const int bet = 5;
	static const int startingCredit = 1000;
	static const char* pathInputs;
	static const char* pathOutputs;
};

#endif