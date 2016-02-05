/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2008 - DIGITEO - Allan CORNET
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
*
*/

#ifndef __TERMCONSOLE_H__
#define __TERMCONSOLE_H__

/**
* initialize Terminal
*/
void InitializeTerminal(void);

/**
* printf in terminal
* @param[in] buffer
* @return number of characters
*/
int TerminalPrintf(char *buffer);

/**
* put a character
* @param[in] character
*/
int TerminalPutc(char ch);

/**
* returns line from terminal
* @param[in] prompt
* @return line from terminal
*/
char *TerminalGetString(char *prompt);

/**
* do a beep
*/
void TerminalBeep(void);

#endif /* __TERMCONSOLE_H__ */
