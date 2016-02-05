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

#define  _WIN32_WINNT 0x0500 // GetConsoleWindow
#include <Windows.h>
#include <shlwapi.h>
#include <stdio.h>
#pragma comment(lib, "shlwapi.lib")

#include "WinConsole.h"
#include "WndThread.h"
#include "console.h"
#include "version.h"
#include "os_string.h"
/*--------------------------------------------------------------------------*/
#define NameConsole "Console"
/*--------------------------------------------------------------------------*/
static CONSOLE_SCREEN_BUFFER_INFO csbiInfoSave;
static char ScilexConsoleName[MAX_PATH];
/*--------------------------------------------------------------------------*/
void UpdateConsoleColors(void)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD Coord;
    DWORD cWritten;

    Coord.X = 0;
    Coord.Y = 0;

    FillConsoleOutputAttribute( hConsole,
                                BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY,
                                csbiInfoSave.dwSize.X * csbiInfoSave.dwSize.Y,
                                Coord,
                                &cWritten);

    SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY);

}
/*--------------------------------------------------------------------------*/
void SaveConsoleColors(void)
{
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiInfoSave);
}
/*--------------------------------------------------------------------------*/
void RestoreConsoleColors(void)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD Coord;
    DWORD cWritten;

    Coord.X = 0;
    Coord.Y = 0;

    FillConsoleOutputAttribute( hConsole,
                                csbiInfoSave.wAttributes,
                                csbiInfoSave.dwSize.X * csbiInfoSave.dwSize.Y,
                                Coord,
                                &cWritten);
    SetConsoleTextAttribute(hConsole, csbiInfoSave.wAttributes);
}
/*--------------------------------------------------------------------------*/
void RenameConsole(void)
{
    HWND hScilex = NULL;
    char CurrentConsoleName[MAX_PATH];
    char CurrentConsoleNameTmp[MAX_PATH];

    GetConsoleTitle(CurrentConsoleName, MAX_PATH);
    strncpy(CurrentConsoleNameTmp, CurrentConsoleName, strlen(NameConsole));
    CurrentConsoleNameTmp[strlen(NameConsole)] = '\0';

    if ( strcmp(CurrentConsoleNameTmp, NameConsole) != 0)
    {
        wsprintf(ScilexConsoleName, "%s %s", NameConsole, SCI_VERSION_STRING);
        SetConsoleTitle(ScilexConsoleName);
    }

    hScilex = GetConsoleWindow();
    if (hScilex)
    {
        HMENU hmenuConsole = NULL;
        // Desactive croix dans la console
        hmenuConsole = GetSystemMenu(hScilex, FALSE);
        DeleteMenu(hmenuConsole, SC_CLOSE, MF_BYCOMMAND);
    }
}
/*--------------------------------------------------------------------------*/
void RestoreExitButton(void)
{
    HWND hScilex = NULL;
    hScilex = GetConsoleWindow();
    if (hScilex)
    {
        HMENU hmenuConsole = NULL;
        // Active croix dans la console
        hmenuConsole = GetSystemMenu(hScilex, FALSE);
        AppendMenu( hmenuConsole, MF_BYCOMMAND, SC_CLOSE, "&Close Alt+F4" );
    }
}
/*--------------------------------------------------------------------------*/
void CreateScilabConsole(int ShowBanner)
{
    HWND hScilex = NULL;

    SetConsoleState(0);  /* Console DOS Cachée par défaut */
    AllocConsole();

    wsprintf(ScilexConsoleName, "%s %s (%d)", NameConsole, SCI_VERSION_STRING, getCurrentScilabId());
    SetConsoleTitle(ScilexConsoleName);

    CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    freopen("CONOUT$", "wb", stdout); /* redirect stdout --> CONOUT$*/
    freopen("CONOUT$", "wb", stderr); /* redirect stderr --> CONOUT$*/

    if (ShowBanner)
    {
        char line[80];

        strcpy(line, "        ___________________________________________\n");
        printf(line);
        wsprintf(line, "                         %s\n\n", SCI_VERSION_STRING);
        printf(line);
        strcpy(line, "                     Scilab Enterprises\n");
        printf(line);
        strcpy(line, "         Copyright (c) 2011-2016 (Scilab Enterprises)\n");
        printf(line);
        strcpy(line, "               Copyright (c) 1989-2012 (INRIA)\n");
        printf(line);
        strcpy(line, "               Copyright (c) 1989-2007 (ENPC)\n");
        printf(line);
        strcpy(line, "        ___________________________________________\n\n");
        printf(line);
    }

    hScilex = GetConsoleWindow();
    if (hScilex)
    {
        HMENU hmenuConsole = NULL;
        // Desactive croix dans la console
        hmenuConsole = GetSystemMenu(hScilex, FALSE);
        DeleteMenu(hmenuConsole, SC_CLOSE, MF_BYCOMMAND);

        /* Cache la fenetre Console */
        ShowWindow(hScilex, SW_HIDE);
    }
}
/*--------------------------------------------------------------------------*/
void CloseScilabConsole(void)
{
    fclose(stdout);
    fclose(stderr);
    FreeConsole();
}
/*--------------------------------------------------------------------------*/
char *getScilexConsoleName(void)
{
    char *retName = NULL;

    if (strlen(ScilexConsoleName) > 0)
    {
        retName = os_strdup(ScilexConsoleName);
    }
    return retName;
}
/*--------------------------------------------------------------------------*/
int getXConsoleScreenSize(void)
{
    return (csbiInfoSave.srWindow.Right - csbiInfoSave.srWindow.Left);
}
/*--------------------------------------------------------------------------*/
int getYConsoleScreenSize(void)
{
    return (csbiInfoSave.srWindow.Bottom - csbiInfoSave.srWindow.Top);
}
/*--------------------------------------------------------------------------*/
