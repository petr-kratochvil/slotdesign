// link the Gdiplus library
#pragma comment(lib,"gdiplus.lib")

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <windows.h>
#include <GdiPlus.h>
#include "WinGlobal.h"
#include "resource.h"
#include "Settings.h"
#include "Game.h"
#include "Reel.h"

#ifndef WIN32GRAPHICS_HPP
#define WIN32GRAPHICS_HPP

class Win32Graphics
{
protected:
	const int symbolCount;
	const int reelCount;
	const int rowCount;
	int width, height;
	int offsetX, offsetY;
	int symbolW, symbolH;
	bool wasInitialized;
	HBITMAP* bmpSymbol;

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::Color mainColor1, mainColor2, gridColor, highlightColor1, highlightColor2;
	Gdiplus::Brush* mainBrush;
	Gdiplus::LinearGradientBrush *highlightBrushEven, *highlightBrushOdd;
	Gdiplus::Pen* penGrid;
	Gdiplus::Pen* penFrame;
	Gdiplus::Pen* penFrameWin;
	Gdiplus::Pen* penHighlightEven, *penHighlightOdd;

	int highlightMargin;
public:
	Win32Graphics(int width, int height);
	~Win32Graphics();
	void init();
public:
	virtual void paint(HDC hdc);
	void paintBasic(HDC hdc);
private:
	virtual void loadSymbols()
	{}
};

class ValueWidget
{
public:
	typedef enum
	{
		FormatInt,
		FormatDouble
	} ValueFormat;

	ValueWidget(std::wstring caption, int xpos, int ypos, int width, int height);
	void setValue(int value);
	void setValue(double value);
	void setFormat(ValueFormat format);
	void paint(Gdiplus::Graphics& graphics);

private:
	int xpos, ypos, width, height;
	std::wstring caption;
	int valueInt;
	double valueDouble;
	ValueFormat format;
};

#endif