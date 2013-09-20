/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef __ISDIR_H__
#define __ISDIR_H__

#include <wchar.h>
#include "dynlib_fileio.h"
#include "BOOL.h" /* BOOL */

/**
* Check if a directory exists or not
* @param path the directory
* @return if the directory exists (TRUE) or not (FALSE)
*/
FILEIO_IMPEXP BOOL isdir(const char *path);

/**
* Check if a directory exists or not
* @param path the directory (wide string)
* @return if the directory exists (TRUE) or not (FALSE)
*/
FILEIO_IMPEXP BOOL isdirW(const wchar_t *wcpath);
#endif /* __ISDIR_H__ */
/*--------------------------------------------------------------------------*/
