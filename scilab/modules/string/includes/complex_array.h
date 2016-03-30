/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2011 - DIGITEO - Allan CORNET
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

