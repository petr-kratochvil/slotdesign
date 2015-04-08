#include <windows.h>
#include "Win32Graphics.hpp"
#include "WinMain.h"
#include "vs_slotview/resource.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WinMainGlobal::MyRegisterClass(hInstance);
	if (!WinMainGlobal::InitInstance(hInstance, nCmdShow))
		return 1;

	WinMainGlobal::InitGraphics();
	UpdateWindow(WinMainGlobal::hWndMain);

	// Main message loop:
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

bool WinMainGlobal::MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= L"MainWindowClass";
	wcex.hIconSm		= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));

	return RegisterClassEx(&wcex);
}

bool WinMainGlobal::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   WinMainGlobal::hInst = hInstance;

   WinMainGlobal::hWndMain = CreateWindow(L"MainWindowClass", L"Sizzling Hot", WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
      CW_USEDEFAULT, 0, 700, 500, NULL, NULL, hInstance, NULL);

   if (!WinMainGlobal::hWndMain)
      return false;

   WinMainGlobal::win32Graphics = new Win32Graphics(700, 500);

   ShowWindow(WinMainGlobal::hWndMain, nCmdShow);

   return true;
}

void WinMainGlobal::InitGraphics()
{
	WinMainGlobal::win32Graphics->init();
}

LRESULT CALLBACK WinMainGlobal::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		WinMainGlobal::win32Graphics->paint(hdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

