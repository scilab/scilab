/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2008 - Allan CORNET
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
#ifndef __MUTEXCLOSINGSCILAB_H__
#define __MUTEXCLOSINGSCILAB_H__

#include "dynlib_scilab_windows.h"
#include "BOOL.h"

#define CLOSING_SCILAB_MUTEX_NAME "Closing_Scilab"

/**
* Create a named Mutex used to known if we close scilab
*/
SCILAB_WINDOWS_IMPEXP void createMutexClosingScilab(void);

/**
* Close named Mutex used by Innosetup
*/
SCILAB_WINDOWS_IMPEXP void terminateMutexClosingScilab(void);

/**
* check if Mutex about Closing scilab is opening
* @return TRUE or FALSE
*/
SCILAB_WINDOWS_IMPEXP BOOL haveMutexClosingScilab(void);
/*--------------------------------------------------------------------------*/

#endif /* __MUTEXCLOSINGSCILAB_H__ */
