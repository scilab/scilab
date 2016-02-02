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

#include <wchar.h>
#include <wctype.h>
#include <term.h>
#include <curses.h>
#include "cliPrompt.h"
#include "getNbrLine.h"

/* Get the number of line the string passed take in the window */
int getNbrLine(wchar_t * string)
{
    int promptSize = 0;

    int sizeString = 0;

    int nbrLine = 0;

    promptSize = printPrompt(NOWRITE_PROMPT);
    sizeString = wcslen(string);
    /*
     * Number of line is equal to :
     * the size of the command line plus the size of the prompt
     * divided by the number of column in the window
     * Plus one.
     */
    nbrLine = 1 + (sizeString + promptSize + 1) / tgetnum("co");
    return nbrLine;
}
