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

#ifndef	GOTO_FUNC_H_
#define	GOTO_FUNC_H_

#include	"reader.h"

/**
 * Move the cursor to the right.
 * @param Current edited line
 * @param user Input
 * @return user Input
 */
int gotoRight(t_list_cmd ** cmd, int cursorLocation);

/**
 * Move the cursor to the left.
 * @param Current edited line
 * @param user Input
 * @return user Input
 */
int gotoLeft(t_list_cmd ** cmd, int cursorLocation);

/**
 * Move the cursor to the end of the current edited line.
 * @param Current edited line
 * @param user Input
 * @return user Input
 */
int endLine(t_list_cmd ** cmd, int cursorLocation);

/**
 * Move the cursor to the beginning of the current edited line.
 * @param Current edited line
 * @param user Input
 * @return user Input
 */
int begLine(t_list_cmd ** cmd, int cursorLocation);

/**
 * Move cursor to the beginning of the next word or to the end of the string if none were found.
 * @param current edited line
 * @param user Input
 * @return user Input
 */
int nextWord(t_list_cmd ** cmd, int cursorLocation);

/**
 * Move cursor to the beginning of the previous word or to the beginning of the string if none were found.
 * @param current edited line
 * @param user Input
 * @return user Input
 */
int previousWord(t_list_cmd ** cmd, int cursorLocation);

#endif /* !GOTO_FUNC_H_ */
