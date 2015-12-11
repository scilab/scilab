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

#ifndef CHAR_CTL_H_
#define CHAR_CTL_H_

#include <wchar.h>

/*
 * Note:
 * The command line must be a multiple of 1024.
 * Then, each time the buffer size is reached, 1024 * sizeof(wchar_t) is added.
 */

/**
 * Add a character in the command line at the cursor position.
 * @param current edited line.
 * @param user input.
 * @param cursor position in the string.
 * @return user input.
 */
int addChar(wchar_t ** CommandLine, int key, unsigned int *cursorLocation);

/**
 * Remove a character in the command line at the cursor position if SCI_DELETE is passed as key.
 * Remove a character in the command line at the cursor position if SCI_BACKSPACE is passed as key.
 * @param current edited line.
 * @param user input.
 * @param cursor position in the string.
 * @return user input.
 */
int rmChar(wchar_t * CommandLine, int key, unsigned int *cursorLocation);

/**
 * Delete all characters from the cursor position to the end of the line.
 * @param current edited line.
 * @param cursor position in the string.
 * @return user input.
 */
int deleteFromCursToEndLine(wchar_t * CommandLine, unsigned int *cursorLocation);

/**
 * Delete all characters from the cursor position to the beginning of the line.
 * @param current edited line.
 * @param cursor position in the string.
 * @return user input.
 */
int deleteFromCursToBeginningLine(wchar_t * CommandLine, unsigned int *cursorLocation);

/**
 * Delete previous word from cursor.
 * @param command line.
 * @param cursor location.
 * @return user input.
 */
int deletePreviousWordFromCurs(wchar_t * CommandLine, unsigned int *cursorLocation);
#endif /* !CHAR_CTL_H_ */
