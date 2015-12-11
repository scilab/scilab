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
