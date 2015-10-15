#include "Win32Graphics.h"

#ifndef SHUFFLECROSSGRAPHICS_HPP
#define SHUFFLECROSSGRAPHICS_HPP

class ShuffleCrossGraphics : public Win32Graphics
{
	void setStartButtonCaption();
public:
	ShuffleCrossGraphics(int width, int height)
		: Win32Graphics(width, height)
	{}

	void paint(HDC hdc);
private:
	void loadSymbols();
};


#endif