#include "Win32Graphics.h"

#ifndef SIZZLINGHOTGRAPHICS_HPP
#define SIZZLINGHOTGRAPHICS_HPP

class SizzlingHotGraphics : public Win32Graphics
{
public:
	SizzlingHotGraphics(int width, int height)
		: Win32Graphics(width, height)
	{}

	void paint(HDC hdc);
private:
	void loadSymbols();
};


#endif