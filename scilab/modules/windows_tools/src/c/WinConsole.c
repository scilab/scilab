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
#define  _WIN32_WINNT 0x0500
#include <Windows.h>
#include <stdio.h>
#include "WinConsole.h"
#include "version.h"
#include "configvariable_interface.h"
#include "sci_malloc.h"
#include "scilab_windows/console.h"
/*--------------------------------------------------------------------------*/
static int Windows_Console_State = 0;/* 0 Hide 1 Show */
/*--------------------------------------------------------------------------*/
/*Cache la fenetre Scilex(x) de ce processus */
void HideScilex(void)
{
    HWND hScilex = NULL;
    hScilex = GetConsoleWindow();
    if (hScilex)
    {
        ShowWindow(hScilex, SW_HIDE);
        SetConsoleState(0);
    }
}
/*--------------------------------------------------------------------------*/
/*Montre la fenetre Scilex(x) de ce processus */
void ShowScilex(void)
{
    HWND hScilex = NULL;
    hScilex = GetConsoleWindow();
    if (hScilex)
    {
        ShowWindow(hScilex, SW_SHOWNOACTIVATE);
        SetConsoleState(1);
    }
}
/*--------------------------------------------------------------------------*/
void SwitchConsole(void)
{
    char *ConsoleName =  getScilexConsoleName();

    switch (GetConsoleState())
    {
        /* La fenetre etait cachée , on la restaure */
        case 0:
        {
            ShowScilex();
        }
        break;
        /* La fenetre etait apparente , on la cache */
        case 1:
        {
            HideScilex();
        }
        break;
    }

    if (ConsoleName)
    {
        SetConsoleTitle(ConsoleName);
        FREE(ConsoleName);
        ConsoleName = NULL;
    }
}
/*--------------------------------------------------------------------------*/
int GetConsoleState(void)
{
    return Windows_Console_State;
}
/*--------------------------------------------------------------------------*/
void SetConsoleState(int state)
{
    Windows_Console_State = state;
}
/*--------------------------------------------------------------------------*/
