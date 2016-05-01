/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
* Copyright (C) DIGITEO - 2012 - Allan CORNET
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
*
*/
/*--------------------------------------------------------------------------*/
#pragma comment (lib, "gdiplus.lib")
/*--------------------------------------------------------------------------*/
#include <Windows.h>
#include <stdio.h>
#include <string>
#include <CommCtrl.h>
#include <GdiPlus.h>
extern "C"
{
#include "splashScreen.h"
#include "localization.h"
#include "resource.h"
#include "version.h"
#include "WndThread.h"
#include "charEncoding.h"
#include "getScilabDirectory.h"
#include "InnosetupMutex.h"
#include "sci_malloc.h"
};
/*--------------------------------------------------------------------------*/
#define SPLASH_WINDOW_CLASSNAME "Scilab splashscreen"
#define FORMAT_FULLPATH_SPLASH_IMAGE "%s/modules/gui/images/icons/aboutScilab.png"
/*--------------------------------------------------------------------------*/
static DWORD WINAPI ThreadSplashScreen(LPVOID lpParam);
static BOOL stopSplashScreen(UINT _time, UINT _timeMax);
static BOOL haveConsoleWindow(void);
/*--------------------------------------------------------------------------*/
static UINT timeSplashScreen = 0;
static Gdiplus::Image* pImage = NULL;
/*--------------------------------------------------------------------------*/
void splashScreen(void)
{
    DWORD dwThreadId, dwThrdParam = 0;

    if (!haveConsoleWindow() && !haveInnosetupMutex())
    {
        HANDLE hThreadSplashScreen = CreateThread(NULL, 0, ThreadSplashScreen, &dwThrdParam, 0, &dwThreadId);
    }
}
/*--------------------------------------------------------------------------*/
LRESULT CALLBACK SplashWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_PAINT:
        {
            if (pImage)
            {
                Gdiplus::Graphics gdip(hwnd);
                gdip.DrawImage(pImage, 0, 0, pImage->GetWidth(), pImage->GetHeight());
            }
            ValidateRect(hwnd, NULL);
            return 0;
        }
        break;
        case WM_DESTROY:
        {
            if (pImage)
            {
                delete pImage;
                pImage = NULL;
            }
        }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
/*--------------------------------------------------------------------------*/
static DWORD WINAPI ThreadSplashScreen(LPVOID lpParam)
{
    char *ScilabDirectory = NULL;
    char *ImageFilename = NULL;
    wchar_t *wImageFilename = NULL;

    size_t len = 0;
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;

    ULONG_PTR gdiplusToken = NULL;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    HINSTANCE hInstanceThisDll = (HINSTANCE)GetModuleHandle("scilab_windows");

    ScilabDirectory = getScilabDirectory(TRUE);
    if (ScilabDirectory == NULL)
    {
        return 0;
    }

    len = strlen(FORMAT_FULLPATH_SPLASH_IMAGE) + strlen(ScilabDirectory) + 1;
    ImageFilename = (char*)MALLOC(sizeof(char) * len);
    if (ImageFilename == NULL)
    {
        return 0;
    }

    sprintf(ImageFilename, FORMAT_FULLPATH_SPLASH_IMAGE, ScilabDirectory);
    FREE(ScilabDirectory);
    ScilabDirectory = NULL;

    wImageFilename = to_wide_string(ImageFilename);
    FREE(ImageFilename);
    ImageFilename = NULL;
    if (wImageFilename == NULL)
    {
        return 0;
    }

    pImage = Gdiplus::Image::FromFile((const WCHAR *)wImageFilename);
    FREE(wImageFilename);
    wImageFilename = NULL;
    if (pImage == NULL)
    {
        return 0;
    }

    WNDCLASS wndcls = {0};

    wndcls.style = CS_HREDRAW | CS_VREDRAW;
    wndcls.lpfnWndProc = SplashWndProc;
    wndcls.hInstance = GetModuleHandle(NULL);
    wndcls.hCursor = LoadCursor(NULL, IDC_APPSTARTING);
    wndcls.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wndcls.lpszClassName = SPLASH_WINDOW_CLASSNAME;
    wndcls.hIcon = LoadIcon(wndcls.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

    if (!RegisterClass(&wndcls))
    {
        if (GetLastError() != 0x00000582) // already registered
        {
            return 0;
        }
    }

    // try to find monitor where mouse was last time
    POINT point = {0};
    MONITORINFO mi = {sizeof(MONITORINFO), 0};
    HMONITOR hMonitor = 0;
    RECT rcArea = {0};

    ::GetCursorPos(&point);
    hMonitor = ::MonitorFromPoint(point, MONITOR_DEFAULTTONEAREST);
    if (::GetMonitorInfo(hMonitor, &mi))
    {
        rcArea.left = (mi.rcMonitor.right + mi.rcMonitor.left - pImage->GetWidth()) / 2;
        rcArea.top = (mi.rcMonitor.top + mi.rcMonitor.bottom - pImage->GetHeight()) / 2;
    }
    else
    {
        SystemParametersInfo(SPI_GETWORKAREA, NULL, &rcArea, NULL);
        rcArea.left = (rcArea.right + rcArea.left - pImage->GetWidth()) / 2;
        rcArea.top = (rcArea.top + rcArea.bottom - pImage->GetHeight()) / 2;
    }

    HWND hdlg = CreateWindowEx(WS_EX_TOOLWINDOW,
                               SPLASH_WINDOW_CLASSNAME,
                               SPLASH_WINDOW_CLASSNAME,
                               WS_CLIPCHILDREN | WS_POPUP,
                               rcArea.left,
                               rcArea.top,
                               pImage->GetWidth(),
                               pImage->GetHeight(),
                               NULL,
                               NULL,
                               wndcls.hInstance,
                               NULL);

    if (hdlg == NULL)
    {
        return 0;
    }

    ShowWindow(hdlg, SW_SHOWNORMAL);
    UpdateWindow(hdlg);
    SetWindowPos(hdlg, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

    while (!stopSplashScreen(20, 1000))
    {
        Sleep(20);
    }

    DestroyWindow(hdlg);
    return 0;
}
/*--------------------------------------------------------------------------*/
static BOOL stopSplashScreen(UINT _time, UINT _timeMax)
{
    if (haveConsoleWindow() || (timeSplashScreen >= _timeMax))
    {
        return TRUE;
    }
    else
    {
        if (timeSplashScreen < _timeMax)
        {
            timeSplashScreen = timeSplashScreen + _time;
        }
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/
static BOOL haveConsoleWindow(void)
{
    HWND hWndMainScilab = NULL;
    char titleMainWindow[MAX_PATH];
    int id = getCurrentScilabId();

    wsprintf(titleMainWindow, "%s (%d)", SCI_VERSION_STRING, id);
    hWndMainScilab = FindWindow(NULL, titleMainWindow);

    if (hWndMainScilab == NULL)
    {
        wsprintf(titleMainWindow, _("Scilab %s Console"),  std::string(SCI_VERSION_STRING).substr(strlen("scilab-"), std::string::npos).c_str());
        hWndMainScilab = FindWindow(NULL, titleMainWindow);
    }

    return (hWndMainScilab == NULL) ? FALSE : TRUE;
}
/*--------------------------------------------------------------------------*/
