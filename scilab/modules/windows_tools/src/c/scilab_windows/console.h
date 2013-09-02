/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#ifndef __CONSOLE_H__
#define __CONSOLE_H__
#include "dynlib_scilab_windows.h"
#include "BOOL.h"

/**
* Update Colors of console
*/
SCILAB_WINDOWS_IMPEXP void UpdateConsoleColors(void);

/**
* Save colors before  creation
*/
SCILAB_WINDOWS_IMPEXP void SaveConsoleColors(void);

/**
* Restore colors of console
*/
SCILAB_WINDOWS_IMPEXP void RestoreConsoleColors(void);

/**
* Restore Exit button
*/
SCILAB_WINDOWS_IMPEXP void RestoreExitButton(void);

/**
* Rename Scilab Console  with correct name
*/
SCILAB_WINDOWS_IMPEXP void RenameConsole(void);

/**
* Create Scilab Console
*/
SCILAB_WINDOWS_IMPEXP void CreateScilabConsole(BOOL ShowBanner);

/**
* Close Scilab Console
*/
SCILAB_WINDOWS_IMPEXP void CloseScilabConsole(void);

/**
* get scilex console name
* @return a name
*/
SCILAB_WINDOWS_IMPEXP char *getScilexConsoleName(void);

/* return colums size of screen console */
SCILAB_WINDOWS_IMPEXP int getXConsoleScreenSize(void);

/* return lines size of screen console */
SCILAB_WINDOWS_IMPEXP int getYConsoleScreenSize(void);

#endif /* __CONSOLE_H__ */
/*--------------------------------------------------------------------------*/
