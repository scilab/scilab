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
#ifndef __ICONIFYMAINWINDOW_H__
#define __ICONIFYMAINWINDOW_H__

#include "dynlib_gui.h"

/**
* Deiconify Main Window
*/
GUI_IMPEXP void deiconifyMainWindow(void);

/**
* Iconify Main Window
*/
GUI_IMPEXP void iconifyMainWindow(void);

/**
* Maximize Main Window
*/
GUI_IMPEXP void maximizeMainWindow(void);

/**
* Main Window is in the "normal" state.
*/
GUI_IMPEXP void normalMainWindow(void);

#endif /* __ICONIFYMAINWINDOW_H__*/
/*--------------------------------------------------------------------------*/
