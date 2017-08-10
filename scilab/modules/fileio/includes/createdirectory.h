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
#ifndef __CREATEDIRECTORY_H__
#define __CREATEDIRECTORY_H__

#include <wchar.h>

#include "dynlib_fileio.h"
#include "BOOL.h" /* BOOL */

/**
* Create a directory
* @param path the path of the futur directory
* @return the result of the operation TRUE or FALSE
*/
FILEIO_IMPEXP BOOL createdirectory(const char *path);

/**
* Create a directory (wide string)
* @param path the path of the futur directory
* @return the result of the operation TRUE or FALSE
*/
FILEIO_IMPEXP BOOL createdirectoryW(const wchar_t *path);


#endif /* __CREATEDIRECTORY_H__ */
/*--------------------------------------------------------------------------*/
