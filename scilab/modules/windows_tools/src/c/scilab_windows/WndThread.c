/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
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
#include <string.h>
#include <Windows.h>
#include <windowsx.h>
#include "WndThread.h"
#include "sci_malloc.h"
#include "version.h"
#include "wmcopydata.h"
#include "storeCommand.h"
#include "os_string.h"
#include "charEncoding.h"
/*--------------------------------------------------------------------------*/
static HWND hWndScilab = NULL;
HANDLE HandleThreadWnd = NULL;
static char titleHiddenScilabWindow[MAX_PATH] = "";
static int ScilabId = -1;
/*--------------------------------------------------------------------------*/
static DWORD WINAPI WndThread(LPVOID lpParam);
static void RegisterWindowClass(void);
static void GetFreeTitleOfWindowHidden(void);
static BOOL ON_WND_HIDDEN_WM_COPYDATA(HWND hwnd, HWND hWndSend, PCOPYDATASTRUCT MyCopyDataStruct);
/*--------------------------------------------------------------------------*/
BOOL CreateScilabHiddenWndThread(void)
{
    BOOL bOK = FALSE;

    if (!HandleThreadWnd)
    {
        HandleThreadWnd = CreateThread( NULL, 0, WndThread, NULL, 0, NULL);
    }
    if (HandleThreadWnd)
    {
        bOK = TRUE;
    }

    return bOK;
}
/*--------------------------------------------------------------------------*/
static DWORD WINAPI WndThread(LPVOID lpParam)
{
    HANDLE hWndScilab = NULL;
    HINSTANCE hInstanceThisDll = (HINSTANCE)GetModuleHandle("scilab_windows");

    RegisterWindowClass();

    GetFreeTitleOfWindowHidden();

    hWndScilab = CreateWindow(HiddenWindowClassName,
                              titleHiddenScilabWindow, WS_OVERLAPPEDWINDOW,
                              0, 0, 350, 0,
                              NULL, NULL, hInstanceThisDll, NULL);

    ShowWindow (hWndScilab, SW_HIDE);
    UpdateWindow (hWndScilab);

    if (hWndScilab)
    {
        MSG Msg;
        while (GetMessage(&Msg, NULL, 0, 0))
        {
            TranslateMessage(&Msg);
            DispatchMessage(&Msg);
        }
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
LRESULT CALLBACK HiddenWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    switch (Msg)
    {
            HANDLE_MSG(hWnd, WM_COPYDATA, ON_WND_HIDDEN_WM_COPYDATA);
    }
    return DefWindowProc(hWnd, Msg, wParam, lParam);
}
/*--------------------------------------------------------------------------*/
static void RegisterWindowClass(void)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style			= CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc	= HiddenWndProc;
    wcex.cbClsExtra		= 0;
    wcex.cbWndExtra		= 0;
    wcex.hInstance		= (HINSTANCE)GetModuleHandle("scilab_windows");
    wcex.hIcon			= 0;
    wcex.hCursor		= 0;
    wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName	= 0;
    wcex.lpszClassName	= HiddenWindowClassName;
    wcex.hIconSm		= 0;

    RegisterClassEx(&wcex);
}
/*--------------------------------------------------------------------------*/
static void GetFreeTitleOfWindowHidden(void)
{
    HWND hWndScilab = NULL;
    int Number_of_Scilab = 0;

    char SearchedScilabWindow[MAX_PATH];

    wsprintf(SearchedScilabWindow, FORMAT_TITLE_HIDDEN_WINDOWS, SCI_VERSION_STRING, Number_of_Scilab);

    hWndScilab = FindWindow(NULL, SearchedScilabWindow);

    while ( hWndScilab )
    {
        Number_of_Scilab++;
        wsprintf(SearchedScilabWindow, FORMAT_TITLE_HIDDEN_WINDOWS, SCI_VERSION_STRING, Number_of_Scilab);
        hWndScilab = FindWindow(NULL, SearchedScilabWindow);
    }

    strcpy(titleHiddenScilabWindow, SearchedScilabWindow);
    ScilabId = Number_of_Scilab;
}
/*--------------------------------------------------------------------------*/
static BOOL ON_WND_HIDDEN_WM_COPYDATA(HWND hwnd, HWND hWndSend, PCOPYDATASTRUCT MyCopyDataStruct)
{
    char Command[MAX_PATH];
    char TitleWndSend[MAX_PATH];

    ReceiveFromAnotherScilab(hWndSend, MyCopyDataStruct);

    if ( GetCommandFromAnotherScilab(TitleWndSend, Command) )
    {
        StoreCommand(Command);
    }

    return TRUE;
}
/*--------------------------------------------------------------------------*/
char *getCurrentTitleScilabHiddenWindow(void)
{
    char *currentTitle = NULL;
    if ( strcmp(titleHiddenScilabWindow, "") )
    {
        currentTitle = os_strdup(titleHiddenScilabWindow);
    }
    return currentTitle;
}
/*--------------------------------------------------------------------------*/
int getCurrentScilabId(void)
{
    return ScilabId;
}
/*--------------------------------------------------------------------------*/
