/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Allan CORNET
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
#ifndef __MPUTL_H__
#define __MPUTL_H__

#include "dynlib_fileio.h"
#include "BOOL.h"

typedef enum
{
    MPUTL_NO_ERROR = 0,
    MPUTL_ERROR = 1,
    MPUTL_INVALID_FILE_DESCRIPTOR = 2,
    MPUTL_NO_WRITE_RIGHT = 3
} mputlError;

/* scilab file descriptor id for stdout (see %io in scilab) */
#define STDOUT_ID 6

/* scilab file descriptor id for stdin (see %io in scilab) */
#define STDIN_ID 5

/**
* mputl function
* @param[in] file descriptor from mopen
* @param[in] strings to write
* @param[in] numbers of strings to write
* @return enum error
*/
FILEIO_IMPEXP mputlError mputl(int _iFileId, wchar_t **pstStrings, int _iSizeStrings, BOOL _CR = TRUE);

#endif /* __MPUTL_H__ */
/*--------------------------------------------------------------------------*/

