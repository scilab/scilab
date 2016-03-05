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
#ifndef __FINDSCILAB_H__
#define __FINDSCILAB_H__

#include "dynlib_scilab_windows.h"
#include "BOOL.h" /* BOOL */

/**
* Check if exists another scilab window
* @return TRUE or FALSE
*/
SCILAB_WINDOWS_IMPEXP BOOL HaveAnotherWindowScilab(void);

/**
* returns name of the first Scilab found
* @return name
*/
SCILAB_WINDOWS_IMPEXP char * getFirstScilabFound(void);

/**
* returns name of the last Scilab found
* @return name
*/
SCILAB_WINDOWS_IMPEXP char * getLastScilabFound(void);

#endif /*__FINDSCILAB_H__ */
/*--------------------------------------------------------------------------*/
