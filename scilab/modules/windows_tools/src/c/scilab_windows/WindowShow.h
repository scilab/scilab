/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2010 - Allan CORNET
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
#ifndef __WINDOWSHOW_H__
#define __WINDOWSHOW_H__

#include "dynlib_scilab_windows.h"

/**
* set Current Window Mode Show
* @param[IN] set Current Window Mode Show
*/
SCILAB_WINDOWS_IMPEXP void setWindowShowMode(int nCmdShow);

/**
* get Current Window Show Mode
* @return current Window Show Mode
*/
SCILAB_WINDOWS_IMPEXP int getWindowShowMode(void);

/**
* update Window show with current Mode
*/
SCILAB_WINDOWS_IMPEXP void WindowShow(void);

#endif /* __WINDOWSHOW_H__*/
/*--------------------------------------------------------------------------*/
