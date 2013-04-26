/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2011 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef __CSV_ISNUM_H__
#define __CSV_ISNUM_H__

#include "BOOL.h"
#include <wchar.h>
#include "dynlib_string.h"

#ifdef __cplusplus
extern "C" {
#endif

    BOOL isNum(const char *pStr);

    STRING_IMPEXP BOOL *isNumMatrix(const char **pStrs, int nbRows, int nbCols);

    BOOL isNumW(const wchar_t *pStr);

    STRING_IMPEXP BOOL *isNumMatrixW(const wchar_t **pStrs, int nbRows, int nbCols);

#ifdef __cplusplus
}
#endif


#endif /* __CSV_ISNUM_H__ */
// =============================================================================
