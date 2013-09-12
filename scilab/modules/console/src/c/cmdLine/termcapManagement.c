/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Karim Mamode
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*/

#include <term.h>
#include <curses.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "termcapManagement.h"
/* Simplify termcap activation */
void setStringCapacities(const char *capacity)
{
    char *stringCap;

    stringCap = tgetstr(capacity, NULL);
    if (stringCap != NULL)
    {
        tputs(stringCap, 1, putchar);
    }
}

/* Move cursor to the column _col and the line _li */
void capGoto(int col, int li)
{
    char *stringCap;

    stringCap = tgetstr("cm", NULL);
    if (stringCap != NULL)
    {
        stringCap = tgoto(stringCap, col, li);
    }
    if (stringCap != NULL)
    {
        tputs(stringCap, 1, putchar);
    }
}
