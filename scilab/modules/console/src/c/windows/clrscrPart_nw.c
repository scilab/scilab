/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
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
#include "clrscrPart_nw.h"
#include "TermPosition.h"
/*--------------------------------------------------------------------------*/
void clrscrPart_nw(int nblines)
{
    COORD coord;
    DWORD written;
    CONSOLE_SCREEN_BUFFER_INFO info;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);

    coord.X = 0;
    coord.Y = (SHORT)(info.dwCursorPosition.Y - nblines - 1) ;

    TermSetPosition(0, coord.Y - 1);

    FillConsoleOutputCharacter (GetStdHandle(STD_OUTPUT_HANDLE), ' ',
                                info.dwSize.X * (info.dwSize.Y - coord.Y - 1), coord, &written);

    TermSetPosition(0, coord.Y - 1);
}
/*--------------------------------------------------------------------------*/
