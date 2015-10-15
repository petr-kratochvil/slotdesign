#include <windows.h>
#include <vector>

#ifndef WINGLOBAL_H
#define WINGLOBAL_H

class Game;
class Win32Graphics;
class ValueWidget;

namespace WinGlobal
{
	bool MyRegisterClass(HINSTANCE hInstance);
	bool InitInstance(HINSTANCE, int);
	LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	LRESULT CALLBACK ButtonProc(HWND, UINT, WPARAM, LPARAM);
	INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
	void InitGraphics();
	void InitControls();
	void InitGame();

	extern WNDPROC OldButtonProc;

	// The Game which is played
	extern Game* game;

	// Windows application instance
	extern HINSTANCE hInst;
	// Window drawing
	extern Win32Graphics* win32Graphics;

	// Main window dimensions
	extern int width;
	extern int height;
	// Main window handle
	extern HWND hWndMain;

	namespace Controls
	{
		extern HWND buttonStart;
		extern HWND editInfo;
		extern std::vector<ValueWidget*> values;
	}
}


#endif