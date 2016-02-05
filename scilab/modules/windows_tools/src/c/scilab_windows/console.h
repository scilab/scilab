/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
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
