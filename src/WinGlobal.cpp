#include "WinGlobal.h"

namespace WinGlobal
{
	WNDPROC OldButtonProc;

	// The Game which is played
	Game* game;

	// Windows application instance
	HINSTANCE hInst;
	// Window drawing
	Win32Graphics* win32Graphics;

	// Main window dimensions
	int width = 700;
	int height = 500;
	// Main window handle
	HWND hWndMain;

	namespace Controls
	{
		HWND buttonStart;
		HWND editInfo;
		std::vector<ValueWidget*> values;
	}
}