#include "stdafx.h"
#include "resource.h"

#define MAX_LOADSTRING 100

HINSTANCE hInst;
HMODULE hResDLL;

TCHAR szTitle[MAX_LOADSTRING];
TCHAR szWindowClass[MAX_LOADSTRING];

ATOM RegisterWindowClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
					   LPTSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	TCHAR szDLLName[MAX_LOADSTRING];

	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDS_DLL_FILENAME, szDLLName, MAX_LOADSTRING);

	hResDLL = LoadLibraryEx(szDLLName, NULL, LOAD_LIBRARY_AS_DATAFILE);

	if (hResDLL == NULL)
	{
		TCHAR szDLLFail[MAX_LOADSTRING];
		LoadString(hInstance, IDS_DLL_LOADFAIL, szDLLFail, MAX_LOADSTRING);

		MessageBox(NULL, szDLLFail, szTitle, MB_OK | MB_ICONERROR);
		return 1;
	}
	
	LoadString(hResDLL, IDC_DISCORD, szWindowClass, MAX_LOADSTRING);

	RegisterWindowClass(hInstance);

	if (!InitInstance(hInstance, nCmdShow))
		return FALSE;

	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int) msg.wParam;
}

ATOM RegisterWindowClass(HINSTANCE hInstance)
{
	WNDCLASS wcex;

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DISCORD));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= NULL;
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_DISCORD);
	wcex.lpszClassName	= szWindowClass;

	return RegisterClass(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance;

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
      CW_USEDEFAULT, CW_USEDEFAULT, 383, 693, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

HBITMAP hLoginBkg;
BITMAP loginBkg;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc, bkgHdc;
	
	switch (message)
	{
	case WM_DESTROY:
		DeleteObject(hLoginBkg);
		FreeLibrary(hResDLL);

		PostQuitMessage(0);

		return 0;

	case WM_CREATE:
		hLoginBkg = LoadBitmap(hResDLL, MAKEINTRESOURCE(IDB_LOGIN_BKG));
		GetObject(hLoginBkg, sizeof(BITMAP), &loginBkg);

		return 0;

	case WM_ERASEBKGND:
		hdc = (HDC)wParam;

		bkgHdc = CreateCompatibleDC(hdc);

		SelectObject(bkgHdc, hLoginBkg);
		BitBlt(hdc, 0, 0, loginBkg.bmWidth, loginBkg.bmHeight, bkgHdc, 0, 0, SRCCOPY);

		DeleteDC(bkgHdc);

		return 1;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps); 
		EndPaint(hWnd, &ps); 

		return 0;

	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}
