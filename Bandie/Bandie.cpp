
#include "stdafx.h"
#include "Bandie.h"
#include "Filter.h"
#include <windows.h>
#include <shellapi.h>

#define MAX_LOADSTRING 128
#define WM_TRAY WM_USER + 1

HINSTANCE hInst;                                
WCHAR szTitle[MAX_LOADSTRING];                  
WCHAR szWindowClass[MAX_LOADSTRING];
HWND hAbout;
HICON hIcon;

ATOM				RegisterTrayClass(HINSTANCE hInstance);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	Filter::Start();

	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_BANDIE, szWindowClass, MAX_LOADSTRING);

	HWND hTrayWnd = FindWindow(szWindowClass, NULL);
	MSG stMsg;

	if (hTrayWnd)
	{
		PostMessage(hTrayWnd, WM_TRAY, 0, WM_LBUTTONDBLCLK);
	}
	else
	{
		ATOM aClass = RegisterTrayClass(hInstance);
		if (aClass)
		{
			hInst = hInstance;
			if (hTrayWnd = CreateWindow((LPCTSTR)aClass, _T(""), 0, 0, 0, 0, 0, NULL, NULL, hInstance, NULL))
			{
				while (GetMessage(&stMsg, NULL, 0, 0) > 0)
				{
					TranslateMessage(&stMsg);
					DispatchMessage(&stMsg);
				}

				if (IsWindow(hTrayWnd)) DestroyWindow(hTrayWnd);
			}

			UnregisterClass((LPCTSTR)aClass, hInst);
		}
	}

	return(int)stMsg.wParam;
}

ATOM RegisterTrayClass(HINSTANCE hInstance)
{
	WNDCLASS stWC;
	ZeroMemory(&stWC, sizeof(stWC));
	stWC.lpszClassName = _T("Bandie");
	stWC.hInstance = hInstance;
	stWC.lpfnWndProc = WndProc;
	return RegisterClass(&stWC);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
	{
		NOTIFYICONDATA stData;
		ZeroMemory(&stData, sizeof(stData));
		stData.cbSize = sizeof(stData);
		stData.hWnd = hWnd;
		stData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
		stData.uCallbackMessage = WM_TRAY;
		stData.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(MAINICON));
		hIcon = stData.hIcon;
		LoadStringW(hInst, IDS_APP_TITLE, stData.szTip, MAX_LOADSTRING);
		if (!Shell_NotifyIcon(NIM_ADD, &stData)) return -1;
	}
	return 0;

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
			{
				if (hAbout)
				{
					SetForegroundWindow(hAbout);
					break;
				}

				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;
			}
            case IDM_EXIT:
				Filter::Stop();
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
		NOTIFYICONDATA stData;
		ZeroMemory(&stData, sizeof(stData));
		stData.cbSize = sizeof(stData);
		stData.hWnd = hWnd;
		Shell_NotifyIcon(NIM_DELETE, &stData);
        PostQuitMessage(0);
        break;
	case WM_TRAY:
		switch (lParam)
		{
		case WM_RBUTTONDOWN:
		{
			HMENU hMenu = LoadMenu(hInst, MAKEINTRESOURCE(IDC_MENU));
			if (hMenu)
			{
				HMENU hSubMenu = GetSubMenu(hMenu, 0);
				if (hSubMenu)
				{
					SetForegroundWindow(hWnd);

					POINT stPoint;
					GetCursorPos(&stPoint);

					TrackPopupMenu(hSubMenu, TPM_LEFTALIGN | TPM_BOTTOMALIGN | TPM_RIGHTBUTTON, stPoint.x, stPoint.y, 0, hWnd, NULL);
				}

				DestroyMenu(hMenu);
			}
		}
		break;
		}
		return 0;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
	{
		if (!hAbout)
		{
			hAbout = hDlg;
		}

		if (hIcon)
		{
			SendMessage(hDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
		}
		return (INT_PTR)TRUE;
	}
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hAbout, LOWORD(wParam));
			hAbout = 0;
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
