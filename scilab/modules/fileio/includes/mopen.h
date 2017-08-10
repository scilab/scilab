/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA
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
#ifndef __MOPEN_H__
#define __MOPEN_H__

#include "dynlib_fileio.h"
#include "machine.h"
#include "charEncoding.h"

/**
* Opens file given by file and return it's id
* @param[out] fd (id)
* @param filename
* @param status "r","w","a" or "rb","wb","ab"
* @param swap
* @param[out] res
* @param[out] error
*/

typedef enum
{
    MOPEN_NO_ERROR = 0,
    MOPEN_NO_MORE_LOGICAL_UNIT = 1,
    MOPEN_CAN_NOT_OPEN_FILE = 2,
    MOPEN_NO_MORE_MEMORY = 3,
    MOPEN_INVALID_FILENAME = 4,
    MOPEN_INVALID_STATUS = 5
} mopenError;

FILEIO_IMPEXP int mopen(const wchar_t* _pstFilename, const wchar_t* _pstMode, int _iSwap, int* _piID);

#endif  /* __MOPEN_H__ */
/*--------------------------------------------------------------------------*/
