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

#ifndef	INIT_TC_SHELL_H_
#define	INIT_TC_SHELL_H_

/* Indicate to setAttr and initConsoleMode what mode to set */

/* set Canonic mode */
#define         CANON 1

/* set Raw mode */
#define         RAW 0

/* set Shell mode */
#define         ATTR_RESET -1

/**
 * Initialise the command line. Console mode and Termcaps
 * @param Mode that will be set (Canonic or Raw)
 * @return -1 in case of error, else 0.
 */
int initConsoleMode(int);

/**
 * Set only console mode.
 * @param Mode that will be set (Canonic or Raw)
 * @return -1 in case of error, else 0.
 */
int setAttr(int bin);

#endif /* !INIT_TC_SHELL_H_ */
