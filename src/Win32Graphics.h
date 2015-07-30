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
	Gdiplus::Color mainFrameColor1, mainFrameColor2, gridColor, highlightColor1, highlightColor2;
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
		FormatDouble,
		FormatPercent
	} ValueFormat;

	typedef enum
	{
		HighlightNo,
		HighlightOnPositive
	} HighlightType;

	ValueWidget(std::wstring caption, int xpos, int ypos, int width, int height);
	void setValue(int value);
	void setValue(double value);
	void setFormat(ValueFormat format);
	void setHighlight(HighlightType type);
	void paint(Gdiplus::Graphics& graphics);

private:
	int xpos, ypos, width, height;
	std::wstring caption;
	int valueInt;
	double valueDouble;
	ValueFormat format;
	HighlightType highlightType;
	bool isHighlighted;

	Gdiplus::Color bgColor1, bgColor2, hiColor1, hiColor2;
	Gdiplus::RectF layoutRectCapt, layoutRectVal;
	Gdiplus::LinearGradientBrush bgBrush, hiBrush;
	Gdiplus::Pen linePen, hiPen;
	Gdiplus::SolidBrush fontBrush;
	Gdiplus::Font font;
	Gdiplus::StringFormat stringFormat;

	wchar_t valueWchar[100];
};

#endif