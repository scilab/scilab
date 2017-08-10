/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Allan CORNET
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