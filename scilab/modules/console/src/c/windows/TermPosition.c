/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2008 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#include <Windows.h>
#include "TermPosition.h"
/*--------------------------------------------------------------------------*/
void TermSetPosition(int x, int y)
{
    COORD pt;

    pt.X = (SHORT)x;
    pt.Y = (SHORT)y;
    SetConsoleCursorPosition (GetStdHandle(STD_OUTPUT_HANDLE), pt);
}
/*--------------------------------------------------------------------------*/
void TermGetPosition(int *x, int *y)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    *x = csbi.dwCursorPosition.X;
    *y = csbi.dwCursorPosition.Y;
}
/*--------------------------------------------------------------------------*/
