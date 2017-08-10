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
#ifndef __FSCANFMAT_H__
#define __FSCANFMAT_H__

#include "BOOL.h"
#include "dynlib_fileio.h"

typedef enum
{
    FSCANFMAT_NO_ERROR = 0,
    FSCANFMAT_MOPEN_ERROR = 1,
    FSCANFMAT_FORMAT_ERROR = 2,
    FSCANFMAT_ERROR = 3,
    FSCANFMAT_MEMORY_ALLOCATION = 4,
    FSCANFMAT_READLINES_ERROR = 5
} fscanfMatError;


typedef struct
{
    char **text;
    int sizeText;
    double *values;
    int m;
    int n;
    fscanfMatError err;
} fscanfMatResult;


#define DEFAULT_FSCANFMAT_FORMAT "%lg"
#define DEFAULT_FSCANFMAT_SEPARATOR " "

/**
*
*/
FILEIO_IMPEXP fscanfMatResult *fscanfMat(char *filename, char *format, char *separator);

FILEIO_IMPEXP void freeFscanfMatResult(fscanfMatResult *resultStruct);

#endif /* __FSCANFMAT_H__ */
/*--------------------------------------------------------------------------*/
