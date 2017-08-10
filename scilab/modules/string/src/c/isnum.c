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
#include "isnum.h"
#include "stringToComplex.h"
#include "sci_malloc.h"
// =============================================================================
BOOL isNum(const char *pStr)
{
    if (pStr)
    {
        stringToComplexError ierr = STRINGTOCOMPLEX_NO_ERROR;
        stringToComplex(pStr, ".", FALSE, &ierr);
        return (BOOL)(ierr == STRINGTOCOMPLEX_NO_ERROR);
    }
    return FALSE;
}
// =============================================================================
BOOL *isNumMatrix(const char **pStrs, int nbRows, int nbCols)
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
                pResults[i] = isNum(pStrs[i]);
            }
        }
    }
    return pResults;
}
// =============================================================================
BOOL isNumW(const wchar_t* pStr)
{
    if (pStr)
    {
        stringToComplexError ierr = STRINGTOCOMPLEX_NO_ERROR;
        stringToComplexW(pStr, L".", FALSE, &ierr);
        return (BOOL)(ierr == STRINGTOCOMPLEX_NO_ERROR);
    }
    return FALSE;
}
// =============================================================================
BOOL *isNumMatrixW(const wchar_t** pStrs, int nbRows, int nbCols)
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
                pResults[i] = isNumW(pStrs[i]);
            }
        }
    }
    return pResults;
}
// =============================================================================


