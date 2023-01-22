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

	INITCOMMONCONTROLSEX icc;
	icc.dwSize = sizeof(icc);
	icc.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&icc);
	
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

HBITMAP hLoginBkg, hLoginHlp;
HBRUSH hBkgBrush;
BITMAP loginBkg;

HFONT hmFont;

TCHAR szEmailTitle[MAX_LOADSTRING];
TCHAR szPasswordTitle[MAX_LOADSTRING];
TCHAR szSignInTitle[MAX_LOADSTRING];

TCHAR szForgotPassTitle[MAX_LOADSTRING];
TCHAR szNewTitle[MAX_LOADSTRING];
TCHAR szForgotPassLink[MAX_LOADSTRING];
TCHAR szNewLink[MAX_LOADSTRING];

TCHAR szCopyrightTitle[MAX_LOADSTRING];

HWND hEmailTitle, hEmailBox;
HWND hPasswordTitle, hPasswordBox;
HWND hSignInBtn;
HWND hForgotPassLink, hNewLink;
HWND hCopyrightTitle;
HWND hHelpBtn;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, bkgHdc;

	long lhMain;
	
	switch (message)
	{
	case WM_DESTROY:
		// free resources as not to make leaks happen
		DeleteObject(hLoginBkg);
		DeleteObject(hLoginHlp);
		DeleteObject(hmFont);
		DeleteObject(hBkgBrush);

		// free the resource DLL
		FreeLibrary(hResDLL);

		// farewell!
		PostQuitMessage(0);

		return 0;

	case WM_CREATE:
		hLoginBkg = LoadBitmap(hResDLL, MAKEINTRESOURCE(IDB_LOGIN_BKG));
		hLoginHlp = LoadBitmap(hResDLL, MAKEINTRESOURCE(IDB_LOGIN_HELP));

		GetObject(hLoginBkg, sizeof(BITMAP), &loginBkg);
		
		hBkgBrush = CreatePatternBrush(hLoginBkg);

		hdc = GetDC(NULL);
		lhMain = -MulDiv(10, GetDeviceCaps(hdc, LOGPIXELSY), 72);
		ReleaseDC(NULL, hdc);

		hmFont = CreateFont(lhMain, 0, 0, 0, 0, FALSE, FALSE, FALSE, 0, 0, 0, 0, 0, _T("Tahoma"));

		LoadString(hResDLL, IDS_EMAIL, szEmailTitle, MAX_LOADSTRING);
		LoadString(hResDLL, IDS_PASSWORD, szPasswordTitle, MAX_LOADSTRING);
		LoadString(hResDLL, IDS_SIGNIN, szSignInTitle, MAX_LOADSTRING);
		LoadString(hResDLL, IDS_FORGOTPASS, szForgotPassTitle, MAX_LOADSTRING);
		LoadString(hResDLL, IDS_NEW, szNewTitle, MAX_LOADSTRING);
		LoadString(hResDLL, IDS_COPYRIGHT, szCopyrightTitle, MAX_LOADSTRING);

		_stprintf_s(szForgotPassLink, MAX_LOADSTRING, _T("<a>%s</a>"), szForgotPassTitle);
		_stprintf_s(szNewLink, MAX_LOADSTRING, _T("<a>%s</a>"), szNewTitle);

		// CreateWindow(lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam)

		// E-mail address
		hEmailTitle = CreateWindow(_T("STATIC"), szEmailTitle, WS_VISIBLE | WS_CHILD | SS_LEFT, 60, 175, 275, 20, hWnd, NULL, hInst, NULL);
		SendMessage(hEmailTitle, WM_SETFONT, (WPARAM)hmFont, FALSE);
		
		// E-mail box
		hEmailBox = CreateWindow(_T("EDIT"), NULL, WS_BORDER | WS_CHILD | WS_VISIBLE | ES_LEFT | ES_AUTOHSCROLL, 60, 195, 275, 20, hWnd, NULL, hInst, NULL); 
		SendMessage(hEmailBox, WM_SETFONT, (WPARAM)hmFont, FALSE);

		// Password
		hPasswordTitle = CreateWindow(_T("STATIC"), szPasswordTitle, WS_VISIBLE | WS_CHILD | SS_LEFT, 60, 235, 275, 20, hWnd, NULL, hInst, NULL);
		SendMessage(hPasswordTitle, WM_SETFONT, (WPARAM)hmFont, FALSE);

		// Password box
		hPasswordBox = CreateWindow(_T("EDIT"), NULL, WS_BORDER | WS_CHILD | WS_VISIBLE | ES_LEFT | ES_AUTOHSCROLL | ES_PASSWORD, 60, 255, 275, 20, hWnd, NULL, hInst, NULL);
		SendMessage(hPasswordBox, WM_SETFONT, (WPARAM)hmFont, FALSE);
		
		// Sign in button
		hSignInBtn = CreateWindow(_T("BUTTON"), szSignInTitle, WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 145, 305, 85, 25, hWnd, NULL, hInst, NULL);
		SendMessage(hSignInBtn, WM_SETFONT, (WPARAM)hmFont, FALSE);

		// Forgot password link
		hForgotPassLink = CreateWindow(WC_LINK, szForgotPassLink, WS_VISIBLE | WS_CHILD | LWS_TRANSPARENT, 135, 350, 110, 20, hWnd, NULL, hInst, NULL);
		SendMessage(hForgotPassLink, WM_SETFONT, (WPARAM)hmFont, FALSE);

		// New to discord link
		hNewLink = CreateWindow(WC_LINK, szNewLink, WS_VISIBLE | WS_CHILD | LWS_TRANSPARENT, 140, 375, 100, 20, hWnd, NULL, hInst, NULL);
		SendMessage(hNewLink, WM_SETFONT, (WPARAM)hmFont, FALSE);

		// Copyright 
		hCopyrightTitle = CreateWindow(_T("STATIC"), szCopyrightTitle, WS_VISIBLE | WS_CHILD | SS_LEFT, 15, 635, 285, 20, hWnd, NULL, hInst, NULL);
		SendMessage(hCopyrightTitle, WM_SETFONT, (WPARAM)hmFont, FALSE);

		// Help
		hHelpBtn = CreateWindow(_T("STATIC"), NULL, WS_VISIBLE | WS_CHILD | SS_NOTIFY | SS_BITMAP, 330, 630, 29, 29, hWnd, NULL, hInst, NULL);
		SendMessage(hHelpBtn, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hLoginHlp);

		return 0;

	case WM_CTLCOLORSTATIC:
		if (hEmailTitle == (HWND)lParam 
		 || hPasswordTitle == (HWND)lParam 
		 || hCopyrightTitle == (HWND)lParam)
		{
			hdc = (HDC)wParam;
			SetBkMode(hdc, TRANSPARENT);

			if (hCopyrightTitle == (HWND)lParam)
				SetTextColor(hdc, RGB(128, 128, 128));

			return (INT_PTR)GetStockObject(HOLLOW_BRUSH);
		}

		break;

	case WM_ERASEBKGND:
		hdc = (HDC)wParam;

		bkgHdc = CreateCompatibleDC(hdc);

		SelectObject(bkgHdc, hLoginBkg);
		BitBlt(hdc, 0, 0, loginBkg.bmWidth, loginBkg.bmHeight, bkgHdc, 0, 0, SRCCOPY);

		DeleteDC(bkgHdc);

		return 1;

	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}
