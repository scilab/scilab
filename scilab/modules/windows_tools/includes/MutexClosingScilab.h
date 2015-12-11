/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2008 - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
