/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
#ifndef __SETVISIBLEMAINWINDOW_H__
#define __SETVISIBLEMAINWINDOW_H__

#include "BOOL.h" /* BOOL */
#include "dynlib_gui.h"


/**
* set Main Window visibility
* @param[in] new state
*/
GUI_IMPEXP void setVisibleMainWindow(BOOL newVisibleState);

/**
* isVisibleMainWindow
* @return current state
*/
GUI_IMPEXP BOOL isVisibleMainWindow(void);

#endif /* __SETVISIBLEMAINWINDOW_H__ */
/*--------------------------------------------------------------------------*/
