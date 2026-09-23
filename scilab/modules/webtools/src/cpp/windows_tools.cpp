/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *
 * Copyright (C) 2024-2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#ifdef _MSC_VER

#include <windows.h>
#include <stdio.h>
#include "version.h"

#define FORMAT_TITLE_HIDDEN_WINDOWS "%s hidden window (%d)"

void SetWindowLspDapTitle()
{
    HWND CurrenthWnd = NULL;
    char BeginningHiddenScilabWindow[255];

    sprintf(BeginningHiddenScilabWindow, FORMAT_TITLE_HIDDEN_WINDOWS, SCI_VERSION, 0);

    /* scilab-5.0 hidden window */
    BeginningHiddenScilabWindow[strlen(BeginningHiddenScilabWindow) - 4] = '\0';

    CurrenthWnd = GetWindow(GetDesktopWindow(), GW_CHILD);
    CurrenthWnd = GetWindow(CurrenthWnd, GW_HWNDFIRST);

    while (CurrenthWnd != NULL)
    {
        char Title[255];

        GetWindowTextA(CurrenthWnd, Title, (int)strlen(BeginningHiddenScilabWindow) + 1);
        if (strcmp(Title, BeginningHiddenScilabWindow) == 0)
        {
            DWORD processID = 0;
            GetWindowThreadProcessId(CurrenthWnd, &processID);
            if (GetCurrentProcessId() == processID)
            {
                SetWindowTextA(CurrenthWnd, "Scilab LSP/DAP");
                return;
            }
        }
        CurrenthWnd = GetWindow(CurrenthWnd, GW_HWNDNEXT);
    }
}

#endif
