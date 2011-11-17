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

#ifndef CMD_FUNC_H_
#define	CMD_FUNC_H_

#include <wchar.h>

/**
 * Save a command if listCmd is not NULL, else, display it.
 * @param list of command.
 */
void memCmd(wchar_t * listCmd, unsigned int cursorLocation);

/**
 * Catch the key pressed and call the godd function.
 * @param command line currently edited.
 * @param cursor location in the command line.
 * @return token to indicated the end of the edition of the line.
 */
int getKey(wchar_t ** commandLine, unsigned int *cursorLocation);

#endif /* !CMD_FUNC_H_ */
