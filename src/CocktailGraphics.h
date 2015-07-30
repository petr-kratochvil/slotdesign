#include "Win32Graphics.h"

#ifndef COCKTAILGRAPHICS_HPP
#define COCKTAILGRAPHICS_HPP

class CocktailGraphics : public Win32Graphics
{
public:
	CocktailGraphics(int width, int height)
		: Win32Graphics(width, height)
	{}

	~CocktailGraphics()
	{
		delete this->swingBrush;
		delete this->swingPen;
	}
	void paint(HDC hdc);
	void init();
private:
	void loadSymbols();
	void initValueWidgets();
	void updateValueWidgets();
	Gdiplus::LinearGradientBrush* swingBrush;
	Gdiplus::Pen* swingPen;
};


#endif