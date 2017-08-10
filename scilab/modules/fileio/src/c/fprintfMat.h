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
#ifndef __FPRINTFMAT_H__
#define __FPRINTFMAT_H__

#include "dynlib_fileio.h"
typedef enum
{
    FPRINTFMAT_NO_ERROR = 0,
    FPRINTFMAT_FOPEN_ERROR = 1,
    FPRINTMAT_FORMAT_ERROR = 2,
    FPRINTMAT_NULL_POINTER_ERROR = 3,
    FPRINTFMAT_ERROR = 4
} fprintfMatError;

#define DEFAULT_FPRINTFMAT_FORMAT "%lf"
#define DEFAULT_FPRINTFMAT_SEPARATOR " "

FILEIO_IMPEXP fprintfMatError fprintfMat(char *filename, char *format, char *separator,
        double *MatrixValues, int m, int n,
        char **textAdded, int sizeTextAdded);

#endif /* __FPRINTFMAT_H__ */
/*--------------------------------------------------------------------------*/