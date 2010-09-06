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
#ifndef __FPRINTFMAT_H__
#define __FPRINTFMAT_H__

typedef enum { 
    FPRINTFMAT_NO_ERROR = 0,
    FPRINTFMAT_FOPEN_ERROR = 1,
    FPRINTMAT_FORMAT_ERROR = 2,
    FPRINTMAT_NULL_POINTER_ERROR = 3,
    FPRINTFMAT_ERROR = 4
} fprintfMatError;

#define DEFAULT_FPRINTFMAT_FORMAT "%lf"
#define DEFAULT_FPRINTFMAT_SEPARATOR " "

fprintfMatError fprintfMat(char *filename, char *format, char *separator,
                           double *MatrixValues, int m, int n,
                           char **textAdded, int sizeTextAdded);

#endif /* __FPRINTFMAT_H__ */
/*--------------------------------------------------------------------------*/