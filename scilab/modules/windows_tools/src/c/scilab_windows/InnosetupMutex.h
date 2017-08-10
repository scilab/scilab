/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - 2008 - Allan CORNET
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
#ifndef __INNOSETUPMUTEX_H__
#define __INNOSETUPMUTEX_H__

#include "dynlib_scilab_windows.h"

/*--------------------------------------------------------------------------*/
/**
* Create a named Mutex used by Innosetup
* http://www.vincenzo.net/isxkb/index.php?title=Application_considerations
*/
SCILAB_WINDOWS_IMPEXP void createInnosetupMutex(void);

/**
* Close named Mutex used by Innosetup
*/
SCILAB_WINDOWS_IMPEXP void closeInnosetupMutex(void);

/**
* Check if named Mutex used by Innosetup exists
*/
SCILAB_WINDOWS_IMPEXP BOOL haveInnosetupMutex(void);

/*--------------------------------------------------------------------------*/

#endif /* __INNOSETUPMUTEX_H__ */
