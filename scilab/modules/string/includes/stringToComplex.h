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
 * This code is also published under the GPL v3 license.
 *
 */
#ifndef __STRINGTOCOMPLEX_H__
#define __STRINGTOCOMPLEX_H__

#include "doublecomplex.h"
#include "complex_array.h"
#include "BOOL.h"

#include "dynlib_string.h"

typedef enum
{
    STRINGTOCOMPLEX_NO_ERROR = 0,
    STRINGTOCOMPLEX_MEMORY_ALLOCATION = 1,
    STRINGTOCOMPLEX_NOT_A_NUMBER = 2,
    STRINGTOCOMPLEX_ERROR = 3
} stringToComplexError;


#ifdef __cplusplus
extern "C" {
#endif

    doublecomplex stringToComplex(const char *pSTR,
                                  const char *decimal,
                                  BOOL bConvertByNAN,
                                  stringToComplexError *ierr);

    STRING_IMPEXP  complexArray *stringsToComplexArray(const char **pSTRs, int nbElements,
            const char *decimal,
            BOOL bConvertByNAN,
            stringToComplexError *ierr);

#ifdef __cplusplus
}
#endif


#endif /* __STRINGTOCOMPLEX_H__ */
// =============================================================================
