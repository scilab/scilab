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
