/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
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
#ifndef __FINDFILES_H__
#define __FINDFILES_H__

#include <wchar.h>

#include "dynlib_fileio.h"
#include "BOOL.h"

/**
* Finding all files in a directory with a given filespec
* @param path example : "c:\"
* @param filespec example : "*.sce"
* @param[out] sizeListReturned the size of the files list
* @param[in] if TRUE display a warning if path does not exist
* @return all the files except '.' & '..'
*/

FILEIO_IMPEXP char** findfiles(const char* path, const char* filespec, int *sizeListReturned, BOOL warning);
FILEIO_IMPEXP wchar_t** findfilesW(const wchar_t* path, const wchar_t* filespec, int *sizeListReturned, BOOL warning);

#endif /* __FINDFILES_H__ */
/*--------------------------------------------------------------------------*/
