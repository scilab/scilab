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
#ifndef __WINCONSOLE_H__
#define __WINCONSOLE_H__
/*--------------------------------------------------------------------------*/ 
#include "machine.h"
/*--------------------------------------------------------------------------*/ 
/* Theses functions are used to manipulate console 'dos' added to GUI */
/* only for windows */
/*--------------------------------------------------------------------------*/ 

/**
* hide scilex console
*/
void HideScilex(void);

/**
* show scilex console
*/
void ShowScilex(void);

/**
* switch between hide and show
*/
void SwitchConsole(void);

/**
* Get console state
*@return state 0 hide , 1 show
*/
int GetConsoleState(void);

/**
* Set console state
* @param[in] 0 hide , 1 show
*/
void SetConsoleState(int state);

#endif /* __WINCONSOLE_H__ */
/*--------------------------------------------------------------------------*/ 
