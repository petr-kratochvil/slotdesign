#include <windows.h>
#include "vs_slotview/resource.h"

#include "Win32Graphics.hpp"
#include "WinMain.h"
#include "Game.hpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WinGlobal::MyRegisterClass(hInstance);
	if (!WinGlobal::InitInstance(hInstance, nCmdShow))
		return 1;

	WinGlobal::InitGame();
	WinGlobal::InitGraphics();
	UpdateWindow(WinGlobal::hWndMain);

	// Main message loop:
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

bool WinGlobal::MyRegisterClass(HINSTANCE hInstance)
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

	return (bool)RegisterClassEx(&wcex);
}

bool WinGlobal::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   WinGlobal::hInst = hInstance;

   WinGlobal::hWndMain = CreateWindow(L"MainWindowClass", L"Sizzling Hot", WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
      CW_USEDEFAULT, 0, 700, 500, NULL, NULL, hInstance, NULL);

   if (!WinGlobal::hWndMain)
      return false;

   WinGlobal::win32Graphics = new Win32Graphics(WinGlobal::width, WinGlobal::height);

   ShowWindow(WinGlobal::hWndMain, nCmdShow);

   return true;
}

void WinGlobal::InitGraphics()
{
	WinGlobal::win32Graphics->init();
	WinGlobal::InitControls();
}

void WinGlobal::InitControls()
{
	WinGlobal::Controls::buttonStart = CreateWindowEx(WS_EX_CLIENTEDGE, L"BUTTON", L"Start !"
													  , WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
													  , WinGlobal::width / 2 - 50, 350
													  , 100, 40, WinGlobal::hWndMain, HMENU(NULL), WinGlobal::hInst, NULL);
	WinGlobal::OldButtonProc = (WNDPROC)SetWindowLong(WinGlobal::Controls::buttonStart, GWL_WNDPROC, (LONG)WinGlobal::ButtonProc);
}

void WinGlobal::InitGame()
{
	WinGlobal::game = new Game();
}

LRESULT CALLBACK WinGlobal::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		if (lParam == (LPARAM)WinGlobal::Controls::buttonStart)
		{
			WinGlobal::game->start();
			InvalidateRect(WinGlobal::hWndMain, NULL, true);
			UpdateWindow(WinGlobal::hWndMain);
		}
		else return DefWindowProc(hWnd, message, wParam, lParam);
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		WinGlobal::win32Graphics->paint(hdc);
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

LRESULT CALLBACK WinGlobal::ButtonProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
		if (wParam == VK_RETURN)
		{
			SendMessage(hWnd, BM_CLICK, 0, 0);
			break;
		}
	default:
		return CallWindowProc(WinGlobal::OldButtonProc, hWnd, message, wParam, lParam);
	}
	return 0;
}

