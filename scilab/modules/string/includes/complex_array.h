/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2011 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#ifndef __COMPLEX_ARRAY_H__
#define __COMPLEX_ARRAY_H__

#include "dynlib_string.h"

typedef struct
{
    double *realPart;
    double *imagPart;
    int nbElements;
    int isComplex;
} complexArray;

#ifdef __cplusplus
extern "C" {
#endif

    STRING_IMPEXP complexArray *createComplexArray(const double *realpart, const double *imagpart, int nbSize);
    STRING_IMPEXP complexArray *createComplexArrayEmpty(int nbSize);
    STRING_IMPEXP int cleanImagPartComplexArray(complexArray *pComplexArray);
    STRING_IMPEXP void freeComplexArray(complexArray *pComplexArray);

#ifdef __cplusplus
}
#endif


#endif /* __CSV_COMPLEX_H__ */

