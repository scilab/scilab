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

#ifndef AUTO_COMPLETION_CLI_H_
#define AUTO_COMPLETION_CLI_H_

#define WK_BUF_SIZE 520
#define GET_MAX(x,y) (x) > (y) ? (x) : (y)

/**
 * Manage auto completion in NW/NWNI console.
 * @param pointer to Command line currently edited.
 * @param Cursor position in command line.
 */
void autoCompletionInConsoleMode(wchar_t ** commandLine, unsigned int *cursorLocation);

#endif /* !AUTO_COMPLETION_CLI_H_ */
