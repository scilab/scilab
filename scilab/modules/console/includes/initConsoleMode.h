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
