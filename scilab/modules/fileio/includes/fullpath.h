/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
#ifndef __FULLPATH_H__
#define __FULLPATH_H__

#include "dynlib_fileio.h"

/**
* Creates an absolute or full path name for the specified relative path name.
* @param[out] absPath Pointer to a buffer containing the absolute or full path name, or NULL.
* @param[in] relPath Relative path name.
* @param[in] maxLength Maximum length of the absolute path name buffer (absPath).
* @return returns a pointer to a buffer containing the absolute path name (absPath).
*/

FILEIO_IMPEXP char * get_full_path(char * _FullPath, const char * _Path, size_t _SizeInBytes);

/**
* Creates an absolute or full path name for the specified relative path name.
* @param[out] absPath Pointer to a buffer containing the absolute or full path name, or NULL.
* @param[in] relPath Relative path name.
* @param[in] maxLength Maximum length of the absolute path name buffer (absPath).
* @return returns a pointer to a buffer containing the absolute path name (absPath).
* "localized" on windows (Wide char)
*/
FILEIO_IMPEXP wchar_t * get_full_pathW(wchar_t * _wcFullPath, const wchar_t * _wcPath, size_t _SizeInBytes);

#endif
/*--------------------------------------------------------------------------*/
