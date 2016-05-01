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
#ifndef __WINCONSOLE_H__
#define __WINCONSOLE_H__
/*--------------------------------------------------------------------------*/
#include "dynlib_windows_tools.h"
/*--------------------------------------------------------------------------*/
/* Theses functions are used to manipulate console 'dos' added to GUI */
/* only for windows */
/*--------------------------------------------------------------------------*/

/**
* hide scilex console
*/
WINDOWS_TOOLS_IMPEXP void HideScilex(void);

/**
* show scilex console
*/
WINDOWS_TOOLS_IMPEXP void ShowScilex(void);

/**
* switch between hide and show
*/
WINDOWS_TOOLS_IMPEXP void SwitchConsole(void);

/**
* Get console state
*@return state 0 hide , 1 show
*/
WINDOWS_TOOLS_IMPEXP int GetConsoleState(void);

/**
* Set console state
* @param[in] 0 hide , 1 show
*/
WINDOWS_TOOLS_IMPEXP void SetConsoleState(int state);

#endif /* __WINCONSOLE_H__ */
/*--------------------------------------------------------------------------*/
