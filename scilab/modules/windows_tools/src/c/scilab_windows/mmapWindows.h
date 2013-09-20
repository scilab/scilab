/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2010 - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
