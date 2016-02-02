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

#ifndef SETCHARDISPLAY_H_
#define SETCHARDISPLAY_H_

#include <stdlib.h>
#include "BOOL.h"

/*
 * Here you find some defines related to character display.
 * To change character, the next string must be print :
 * \033[#*m
 * # correspond to the type of setting (bold, italic, etc...)
 * If # is equal to 3 (foreground color) or 4 (background color),
 * * correspond to the color you want to set.
 * Those colors are defined in curses.h and must be used with
 * setForegroundColor and setBackgroundColor.
 */

/* Characteristic to use with setCharDisplay */
#define DISP_DEFAULT "0"
#define DISP_BRIGHT "1"
#define DISP_FAINT "2"
#define DISP_ITALIC "3"
#define DISP_NEGATIVE "7"
#define DISP_LAST_SET NULL
#define DISP_RESET "0"

#define SET_CONSOLE_FOREGROUND 3
#define SET_CONSOLE_BACKGROUND 4

/**
 * Set character display characteristic.
 * @param Characteristic to set using previous define.
 */
void setCharDisplay(char *characteristic);

/**
 * Change Characters color using nCurses colors define.
 * @param color define in (n)curses.h
 */
void setForegroundColor(int color);

/**
 * Change Background color using nCurses colors define.
 * @param color define in (n)curses.h
 */
void setBackgroundColor(int color);

/**
 * Return the color mode for the CLI (true with color, false otherwise)
 *
 * @return TRUE if color should be printed
 */
BOOL getCLIColor(void);

/**
 * Enable or disable color in the term
 *
 * @param CLIColor True for color, False without
 */
void setCLIColor(BOOL CLIColor);


#endif /* SETCHARDISPLAY_H_ */
