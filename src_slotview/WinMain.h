#include <windows.h>
#include "Win32Graphics.hpp"

namespace WinMainGlobal
{
	HINSTANCE hInst;
	Win32Graphics win32Graphics;

	bool				MyRegisterClass(HINSTANCE hInstance);
	bool				InitInstance(HINSTANCE, int);
	LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
	INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
}