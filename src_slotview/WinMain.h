#include <windows.h>

#ifndef WINMAIN_H
#define WINMAIN_H

class Win32Graphics;

namespace WinGlobal
{
	HINSTANCE hInst;
	Win32Graphics* win32Graphics;

	// Main window dimensions
	const int width = 700;
	const int height = 500;
	HWND hWndMain;

	namespace Controls
	{
		HWND buttonStart;
	}

	bool MyRegisterClass(HINSTANCE hInstance);
	bool InitInstance(HINSTANCE, int);
	LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	WNDPROC OldButtonProc;
	LRESULT CALLBACK ButtonProc(HWND, UINT, WPARAM, LPARAM);
	INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
	void InitGraphics();
	void InitControls();
}

#endif