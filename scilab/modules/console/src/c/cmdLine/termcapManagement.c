/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Karim Mamode
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
