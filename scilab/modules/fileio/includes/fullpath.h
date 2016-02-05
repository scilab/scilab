/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
