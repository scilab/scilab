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

#ifndef	CHAR_CTL_H_
#define	CHAR_CTL_H_

#include	"reader.h"

/**
 * Add a character in the command line at the cursor position.
 * @param current edited line.
 * @param user input.
 * @return user input.
 */
int addChar(t_list_cmd ** cmd, int key);

/**
 * Remove a character in the command line at the cursor position if SCI_DELETE is passed as key.
 * Remove a character in the command line at the cursor position if SCI_BACKSPACE is passed as key.
 * @param current edited line.
 * @param user input.
 * @return user input.
 */
int rmChar(t_list_cmd ** cmd, int key);

/**
 * Add a character in the command line at the cursor position.
 * @param current edited line.
 * @param user input.
 * @return user input.
 */
int deleteLineFromCurs(t_list_cmd ** cmd, int key);

#endif /* !CHAR_CTL_H_ */
