/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Karim Mamode
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
