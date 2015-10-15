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
		delete this->swingBrushBasic;
		delete this->swingBrushWin;
		delete this->swingPenBasic;
		delete this->swingPenWin;
	}
	void paint(HDC hdc);
	void init();

private:
	void loadSymbols();
	void initValueWidgets();
	void updateValueWidgets();
	void setStartButtonCaption();
	Gdiplus::LinearGradientBrush* swingBrushBasic;
	Gdiplus::LinearGradientBrush* swingBrushWin;
	Gdiplus::Pen* swingPenBasic;
	Gdiplus::Pen* swingPenWin;
};


#endif