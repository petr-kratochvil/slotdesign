#include "Win32Graphics.h"

#ifndef COCKTAILGRAPHICS_HPP
#define COCKTAILGRAPHICS_HPP

class CocktailGraphics : public Win32Graphics
{
public:
	CocktailGraphics(int width, int height)
		: Win32Graphics(width, height)
	{}

	void paint(HDC hdc);
private:
	void loadSymbols();
	void initValueWidgets();
	void updateValueWidgets();
};


#endif