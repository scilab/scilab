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