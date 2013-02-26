/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#ifndef __FSCANFMAT_H__
#define __FSCANFMAT_H__

#include "BOOL.h"

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
fscanfMatResult *fscanfMat(char *filename, char *format, char *separator);

void freeFscanfMatResult(fscanfMatResult *resultStruct);

#endif /* __FSCANFMAT_H__ */
/*--------------------------------------------------------------------------*/
