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

#ifndef HISTORY_H_
#define HISTORY_H_

#include <wchar.h>

/**
 * Change current editing line to prev editing line
 * @param list of all command lines.
 * @param cursor location in the command line.
 * @return user input
 */
int previousCmd(wchar_t ** cmd, unsigned int *cursorLocation);

/**
 * Change current editing line to next editing line
 * @param list of all command lines.
 * @param cursor location in the command line.
 * @return user input
 */
int nextCmd(wchar_t ** cmd, unsigned int *cursorLocation);

#endif /* !HISTORY_H_ */
