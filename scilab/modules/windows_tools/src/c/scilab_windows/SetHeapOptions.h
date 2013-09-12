/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __SETHEAPOPTIONS_H__
#define __SETHEAPOPTIONS_H__

#include "dynlib_scilab_windows.h"
#include "BOOL.h"

/**
 * Heap corruption detection is the ability to fail an application if
 * the heap manager detects that the application has corrupted the heap.
 * Only on Vista and debug mode.
 * @return TRUE if it is enabled.
 */

SCILAB_WINDOWS_IMPEXP BOOL SetHeapOptions(void);

#endif /* __SETHEAPOPTIONS_H__ */
/*--------------------------------------------------------------------------*/