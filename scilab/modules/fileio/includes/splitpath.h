/*--------------------------------------------------------------------------*/
/*
* ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2009 - Allan CORNET
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
#ifndef __SPLIPATH_H__
#define __SPLIPATH_H__

#include <wchar.h>
#include "BOOL.h"
#include "dynlib_fileio.h"

/**
* Parts of file name and path
* @param[in] filename or path
* @param[in] BOOL expand filename
* @param[out] drive
* @param[out] directory
* @param[out] filename
* @param[out] file extension
*/
FILEIO_IMPEXP void splitpath(const char* path, BOOL bExpand, char* drv, char* dir, char* name, char* ext);

/**
* Parts of file name and path (Wide string)
* @param[in] filename or path
* @param[in] BOOL expand filename
* @param[out] drive
* @param[out] directory
* @param[out] filename
* @param[out] file extension
*/

FILEIO_IMPEXP void splitpathW(const wchar_t* path, BOOL bExpand, wchar_t* drv, wchar_t* dir, wchar_t* name, wchar_t* ext);

#endif /* __SPLIPATH_H__ */
/*--------------------------------------------------------------------------*/
