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
#ifndef __WNDTHREAD_H__
#define __WNDTHREAD_H__

#include "BOOL.h" /* BOOL */

/* format for title of hidden windows */
#define FORMAT_TITLE_HIDDEN_WINDOWS "%s hidden window (%d)"

/* window class name */
#define HiddenWindowClassName "Scilab hidden window"

/**
* Create a hidden window for Scilab (in a separate thread)
* This window (only windows) is used for some features as WM_COPYDATA
* disabled on -NWNI mode
* @return TRUE or FALSE
*/
BOOL CreateScilabHiddenWndThread(void);

/**
* get current title for scilab hidden window
* example : scilab-5.0 hidden window (0)
*/
char *getCurrentTitleScilabHiddenWindow(void);

/**
* get current scilab id
* return a Id : must be (>= 0)
* -1 if we have a problem
*/
int getCurrentScilabId(void);
#endif /* __WNDTHREAD_H__ */
/*--------------------------------------------------------------------------*/ 
