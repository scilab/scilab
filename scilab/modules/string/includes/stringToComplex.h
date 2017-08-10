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
 * This code is also published under the GPL v3 license.
 *
 */
#ifndef __STRINGTOCOMPLEX_H__
#define __STRINGTOCOMPLEX_H__

#include <wchar.h>
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

doublecomplex stringToComplexW(const wchar_t *pSTR,
                               const wchar_t *decimal,
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
