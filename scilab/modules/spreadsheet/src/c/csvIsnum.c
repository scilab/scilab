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
#include "csvIsnum.h"
#include "csvDefault.h"
#include "stringToComplex.h"
#include "MALLOC.h"
// =============================================================================
BOOL csv_isNum(const char *pStr)
{
    if (pStr)
    {
        stringToComplexError ierr = STRINGTOCOMPLEX_NO_ERROR;
        stringToComplex(pStr, getCsvDefaultDecimal(), FALSE, &ierr);
        return (BOOL)(ierr == STRINGTOCOMPLEX_NO_ERROR);
    }
    return FALSE;
}
// =============================================================================
BOOL *csv_isNumMatrix(const char **pStrs, int nbRows, int nbCols)
{
    BOOL *pResults = NULL;
    if (pStrs)
    {
        pResults = (BOOL*)MALLOC(sizeof(BOOL) * (nbCols * nbRows));
        if (pResults)
        {
            int i = 0;
            for (i = 0; i < nbRows * nbCols; i++)
            {
                pResults[i] = csv_isNum(pStrs[i]);
            }
        }
    }
    return pResults;
}
// =============================================================================


