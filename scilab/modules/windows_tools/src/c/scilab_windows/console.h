/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/*--------------------------------------------------------------------------*/ 
#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include "BOOL.h"

/**
* Update Colors of console
*/
void UpdateConsoleColors(void);

/**
* Save colors before  creation
*/
void SaveConsoleColors(void);

/**
* Restore colors of console
*/
void RestoreConsoleColors(void);

/**
* Restore Exit button 
*/
void RestoreExitButton(void);

/**
* Rename Scilab Console  with correct name
*/
void RenameConsole(void);

/**
* Create Scilab Console
*/
void CreateScilabConsole(BOOL ShowBanner);

/**
* Close Scilab Console
*/
void CloseScilabConsole(void);

/**
* get scilex console name
* @return a name
*/
char *getScilexConsoleName(void);

/* return colums size of screen console */
int getXConsoleScreenSize(void);

/* return lines size of screen console */
int getYConsoleScreenSize(void);

#endif /* __CONSOLE_H__ */
/*--------------------------------------------------------------------------*/ 
