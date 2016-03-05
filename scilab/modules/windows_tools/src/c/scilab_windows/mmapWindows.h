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
#ifndef __MMAPWINDOWS_H__
#define __MMAPWINDOWS_H__

#include "dynlib_scilab_windows.h"

#define MMAP_FAILURE (-1)
#define MUNMAP_FAILURE (-1)
#define PROT_READ 0x1
#define PROT_WRITE 0x2
#define MAP_SHARED 1
#define MAP_ANONYMOUS 0x20

/* Emulation of mmap and munmap on Windows */

SCILAB_WINDOWS_IMPEXP void *mmap (void *ptr, long size, long prot, long type, long handle, long arg);

SCILAB_WINDOWS_IMPEXP long munmap (void *ptr, long size);

#endif /* __MMAPWINDOWS_H__ */
/*--------------------------------------------------------------------------*/
