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
#ifndef __GETLONGPATHNAME_H__
#define __GETLONGPATHNAME_H__

#include <wchar.h>
#include "BOOL.h" /* BOOL */
#include "dynlib_fileio.h"

/**
* Retrieves the long path form of the specified path
* @param[in] short path name
* @param[out] TRUE if conversion is ok
* @return long path name
* on Linux returns same path name
* used for Windows
*/
FILEIO_IMPEXP char *getlongpathname(char *shortpathname, BOOL *convertok);

/**
* Retrieves the long path form of the specified path (wide string)
* @param[in] short path name
* @param[out] TRUE if conversion is ok
* @return long path name
* on Linux returns same path name
* used for Windows
*/
FILEIO_IMPEXP wchar_t *getlongpathnameW(wchar_t *wcshortpathname, BOOL *convertok);


#endif /* __GETLONGPATHNAME_H__ */
/*--------------------------------------------------------------------------*/
