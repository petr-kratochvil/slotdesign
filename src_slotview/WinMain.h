#include <windows.h>

#ifndef WINMAIN_H
#define WINMAIN_H

class Win32Graphics;

namespace WinMainGlobal
{
	HINSTANCE hInst;
	Win32Graphics* win32Graphics;
	HWND hWndMain;

	bool				MyRegisterClass(HINSTANCE hInstance);
	bool				InitInstance(HINSTANCE, int);
	LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
	INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
	void InitGraphics();
}

#endif