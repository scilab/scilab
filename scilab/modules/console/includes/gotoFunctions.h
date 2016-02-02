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
