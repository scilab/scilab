/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Allan CORNET
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
