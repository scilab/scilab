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
