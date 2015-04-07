#ifndef SETTINGS_HPP
#define SETTINGS_HPP

// Global settings of constants
// This should probably became part of class Game,
// but as long as we are simulating one game at a time, it can stay simly here.
class Settings
{
public:
	static const int symbolCount = 9;
	static const int windowSize = 3;
	static const int reelCount = 5;
	static const int paylineCount = 5;
};

#endif