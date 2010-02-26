/*--------------------------------------------------------------------------*/ 
/*
* ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2009 - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
