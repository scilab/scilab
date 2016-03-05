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
#ifndef __WNDTHREAD_H__
#define __WNDTHREAD_H__

#include "BOOL.h" /* BOOL */
#include "dynlib_scilab_windows.h"

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
SCILAB_WINDOWS_IMPEXP BOOL CreateScilabHiddenWndThread(void);

/**
* get current title for scilab hidden window
* example : scilab-5.0 hidden window (0)
*/
SCILAB_WINDOWS_IMPEXP char *getCurrentTitleScilabHiddenWindow(void);

/**
* get current scilab id
* return a Id : must be (>= 0)
* -1 if we have a problem
*/
SCILAB_WINDOWS_IMPEXP int getCurrentScilabId(void);

#endif /* __WNDTHREAD_H__ */
/*--------------------------------------------------------------------------*/
