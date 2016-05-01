/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005 - INRIA - Allan CORNET
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
#ifndef __FILEEXIST_H__
#define __FILEEXIST_H__

#include <wchar.h>
#include "dynlib_fileio.h"
#include "BOOL.h" /* BOOL */

/**
* verify if filename is a valid file
* @param filename a string
* @return TRUE or FALSE
*/
FILEIO_IMPEXP BOOL FileExist(const char *filename);

/**
* verify if filename is a valid file
* @param filename a wide string
* @return TRUE or FALSE
*/
FILEIO_IMPEXP BOOL FileExistW(const wchar_t *wcfilename);
/*--------------------------------------------------------------------------*/
#endif /* __FILEEXIST_H__ */
