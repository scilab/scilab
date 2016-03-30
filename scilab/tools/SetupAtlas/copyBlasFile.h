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

#ifndef __COPYBLASFILE_H__
#define __COPYBLASFILE_H__

#include <wchar.h>

typedef enum
{
    COPY_NO_RIGHT_TO_WRITE = 0,
    COPY_OK = 1,
    COPY_FILE_SRC_NOT_EXISTS = 2,
    COPY_DESTINATION_NOT_EXISTS = 3,
    COPY_FILE_FAILED = 4
} copy_blas_error;

copy_blas_error copyBlasFile(wchar_t *blasDllName);
#endif /* __COPYBLASFILE_H__ */
/*--------------------------------------------------------------------------*/
