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

#ifndef	GOTO_FUNC_H_
#define	GOTO_FUNC_H_

#include <wchar.h>

/**
 * Move the cursor to the right.
 * @param Current edited line
 * @param cursor location in the command line.
 * @return user Input
 */
int gotoRight(wchar_t * CommandLine, unsigned int *cursorLocation);

/**
 * Move the cursor to the left.
 * @param Current edited line
 * @param cursor location in the command line.
 * @return user Input
 */
int gotoLeft(wchar_t * CommandLine, unsigned int *cursorLocation);

/**
 * Move the cursor to the end of the current edited line.
 * @param Current edited line
 * @param cursor location in the command line.
 * @return user Input
 */
int endLine(wchar_t * CommandLine, unsigned int *cursorLocation);

/**
 * Move the cursor to the beginning of the current edited line.
 * @param Current edited line
 * @param cursor location in the command line.
 * @return user Input
 */
int begLine(wchar_t * CommandLine, unsigned int *cursorLocation);

/**
 * Move cursor to the beginning of the next word or to the end of the string if none were found.
 * @param current edited line
 * @param cursor location in the command line.
 * @return user Input
 */
int nextWord(wchar_t * CommandLine, unsigned int *cursorLocation);

/**
 * Move cursor to the beginning of the prev word or to the beginning of the string if none were found.
 * @param current edited line
 * @param cursor location in the command line.
 * @return user Input
 */
int previousWord(wchar_t * CommandLine, unsigned int *cursorLocation);

#endif /* !GOTO_FUNC_H_ */
