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
