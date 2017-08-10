/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2008 - DIGITEO - Allan CORNET
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
