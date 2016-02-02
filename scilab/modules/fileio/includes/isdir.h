/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
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
