/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2008 - DIGITEO - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
