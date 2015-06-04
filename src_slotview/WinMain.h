#include <windows.h>

#ifndef WINMAIN_H
#define WINMAIN_H

class Win32Graphics;
class Game;

namespace WinGlobal
{
	// The Game which is played
	Game* game;

	// Windows application instance
	HINSTANCE hInst;
	// Window drawing
	Win32Graphics* win32Graphics;

	// Main window dimensions
	const int width = 700;
	const int height = 500;
	// Main window handle
	HWND hWndMain;

	namespace Controls
	{
		HWND buttonStart;
		HWND editInfo;
	}

	bool MyRegisterClass(HINSTANCE hInstance);
	bool InitInstance(HINSTANCE, int);
	LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	WNDPROC OldButtonProc;
	LRESULT CALLBACK ButtonProc(HWND, UINT, WPARAM, LPARAM);
	INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
	void InitGraphics();
	void InitControls();
	void InitGame();
}


#endif