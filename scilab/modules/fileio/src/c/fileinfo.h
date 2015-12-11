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

#ifndef __FILEINFO_H__
#define __FILEINFO_H__

#include "dynlib_fileio.h"
#include <wchar.h>
#define FILEINFO_ARRAY_SIZE 13

#define FILEINFO_TOTAL_SIZE_INDICE 0
#define FILEINFO_MODE_INDICE       1
#define FILEINFO_UID_INDICE        2
#define FILEINFO_GID_INDICE        3
#define FILEINFO_DEV_INDICE        4
#define FILEINFO_MTIME_INDICE      5
#define FILEINFO_CTIME_INDICE      6
#define FILEINFO_ATIME_INDICE      7
#define FILEINFO_RDEV_INDICE       8
#define FILEINFO_BLKSIZE_INDICE    9
#define FILEINFO_BLOCKS_INDICE    10
#define FILEINFO_INO_INDICE       11
#define FILEINFO_NLINK_INDICE     12

#define FILEINFO_DEFAULT_ERROR    -1

FILEIO_IMPEXP double* filesinfoW(wchar_t** _pwstFilename, int _iSize, int* _piErr);

#endif /* __FILEINFO_H__ */
/*--------------------------------------------------------------------------*/
