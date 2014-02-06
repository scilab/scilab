/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA 2007 - Cong WU
 * Copyright (C) INRIA 2008 - Allan CORNET
 * Copyright (C) DIGITEO 2009-2011 - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/* desc : For usual or polynomial matrix  n  is the int equal to
number of rows times number of columns of  M . (Also valid for  M
a boolean matrix)

For matrices made of character strings (and in particular for a
character string)  length  returns in  n  the length of entries of
the matrix of character strings  M .

The length of a list is the number of elements in the list
(also given by  size ).

length('123')  is  3 .  length([1,2;3,4])  is  4 .                     */
/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "gw_string.h"
#include "api_scilab.h"
#include "core_math.h"
#include "MALLOC.h"
#include "localization.h"
#include "Scierror.h"
#include "freeArrayOfString.h"
#include "charEncoding.h"
#include "isScilabFunction.h"
/*----------------------------------------------------------------------------*/
/* get length */
static int lengthStrings(int *piAddressVar);
static int lengthOthers(char *fname);
static int lengthDefault(int *piAddressVar);
static int lengthList(int *piAddressVar);
static int lengthMList(const char *fname, int *piAddressVar);
/* !!! WARNING !!! : Read comments about length on sparse matrix */
static int lengthSparse(int *piAddressVar);
/*----------------------------------------------------------------------------*/
int sci_length(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int *piAddressVarOne = NULL;
    int iScilabType = 0;

    CheckRhs(1, 1);
    CheckLhs(1, 1);

    /* get Address of inputs */
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = getVarType(pvApiCtx, piAddressVarOne, &iScilabType);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    switch ( iScilabType )
    {
        case sci_strings :
        {
            return lengthStrings(piAddressVarOne);
        }
        case sci_sparse :
        {
            return lengthSparse(piAddressVarOne);
        }
        case sci_list :
        case sci_tlist :
        {
            return lengthList(piAddressVarOne);
        }
        break;
        case sci_mlist :
        {
            return lengthMList(fname, piAddressVarOne);
        }
        break;

        case sci_matrix :
        case sci_poly :
        case sci_boolean :
        case sci_boolean_sparse :
        case sci_matlab_sparse :
        case sci_ints :
        case sci_handles :
        {
            return lengthDefault(piAddressVarOne);
        }
        default :
            return lengthOthers(fname);
            break;
    }
}
/*--------------------------------------------------------------------------*/
static int lengthStrings(int *piAddressVar)
{
    SciErr sciErr;
    int m1 = 0, n1 = 0;
    int iType				= 0;
    char **pStVarOne = NULL;
    int *lenStVarOne = NULL;

    int m_out = 0, n_out = 0;
    double *pdOut = NULL;
    int i = 0;

    sciErr = getVarType(pvApiCtx, piAddressVar, &iType);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if ( iType != sci_strings )
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), "length", 1);
        return 0;
    }

    sciErr = getMatrixOfString(pvApiCtx, piAddressVar, &m1, &n1, NULL, NULL);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: impossible to get dimensions of this matrix.\n"), "length");
        return 0;
    }

    lenStVarOne = (int*)MALLOC(sizeof(int) * (m1 * n1));
    if (lenStVarOne == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), "length");
        return 0;
    }

    sciErr = getMatrixOfString(pvApiCtx, piAddressVar, &m1, &n1, lenStVarOne, NULL);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        FREE(lenStVarOne);
        lenStVarOne = NULL;
        Scierror(999, _("%s: impossible to get dimensions of this matrix.\n"), "length");
        return 0;
    }

    pStVarOne = (char**)MALLOC(sizeof(char*) * (m1 * n1));
    if (pStVarOne == NULL)
    {
        FREE(lenStVarOne);
        Scierror(999, _("%s: No more memory.\n"), "length");
        return 0;
    }

    for (i = 0; i < m1 * n1; i++)
    {
        pStVarOne[i] = (char*)MALLOC(sizeof(char) * (lenStVarOne[i] + 1));
        if (pStVarOne[i] == NULL)
        {
            FREE(lenStVarOne);
            lenStVarOne = NULL;
            freeArrayOfString(pStVarOne, i);
            Scierror(999, _("%s: No more memory.\n"), "length");
            return 0;
        }
    }

    sciErr = getMatrixOfString(pvApiCtx, piAddressVar, &m1, &n1, lenStVarOne, pStVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        FREE(lenStVarOne);
        lenStVarOne = NULL;
        freeArrayOfString(pStVarOne, m1 * n1);
        Scierror(999, _("%s: impossible to get dimensions of this matrix.\n"), "length");
        return 0;
    }

    m_out = m1;
    n_out = n1;
    pdOut = (double*)MALLOC(sizeof(double) * (m_out * n_out));

    if (pdOut == NULL)
    {
        FREE(lenStVarOne);
        lenStVarOne = NULL;
        freeArrayOfString(pStVarOne, m1 * n1);
        Scierror(999, _("%s: No more memory.\n"), "length");
        return 0;
    }

    for (i = 0; i < m_out * n_out; i++)
    {
        int clen = (int)strlen(pStVarOne[i]);
        int scilen = lenStVarOne[i];

        int trueLength = scilen;

        wchar_t *wcStr = NULL;

        if (scilen > clen)
        {
            int j = 0;
            /* bug 4727 */
            /* A scilab string is an array of characters */
            /* we can put '\0' in a scilab string */
            for (j = 0; j < lenStVarOne[i]; j++)
            {
                if (pStVarOne[i][j] == 0)
                {
                    pStVarOne[i][j] = ' ';
                }
            }

            wcStr = to_wide_string(pStVarOne[i]);
            if (wcStr)
            {
                trueLength = (int) wcslen(wcStr);
                FREE(wcStr);
                wcStr = NULL;
            }
        }
        else
        {
            wcStr = to_wide_string(pStVarOne[i]);
            if (wcStr)
            {
                char *cStr = wide_string_to_UTF8(wcStr);
                if (cStr)
                {
                    /* Bug 8525: we check if it is a malformed string */
                    if (strlen(cStr) != strlen(pStVarOne[i]))
                    {
                        trueLength = clen;
                    }
                    else
                    {
                        trueLength = (int) wcslen(wcStr);
                    }
                    FREE(cStr);
                    cStr = NULL;
                }
                else
                {
                    trueLength = (int) wcslen(wcStr);
                }
                FREE(wcStr);
                wcStr = NULL;

            }
        }

        pdOut[i] = (double)trueLength;
    }

    freeArrayOfString(pStVarOne,  m_out * n_out);

    FREE(lenStVarOne);
    lenStVarOne = NULL;

    sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, m_out, n_out, pdOut);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        FREE(pdOut);
        Scierror(999, _("%s: Memory allocation error.\n"), "length");
        return 0;
    }

    FREE(pdOut);
    pdOut = NULL;

    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
static int lengthOthers(char *fname)
{
    /* unknown type */
    Scierror(999, _("%s: Wrong type for input argument(s).\n"), fname);
    return 0;
}
/*--------------------------------------------------------------------------*/
/* !!! WARNING !!! */
/* Compatibility with Scilab 4.x */
/* length returned is the max of dimensions of the sparse matrix max(m,n) */
/* and not m * n */
static int lengthSparse(int *piAddressVar)
{
    int m_out = 0, n_out = 0;
    double *pdOut = NULL;

    int m = 0, n = 0;
    SciErr sciErr = getVarDimension(pvApiCtx, piAddressVar, &m, &n);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    m_out = 1;
    n_out = 1;
    pdOut = (double*)MALLOC(sizeof(double) * (m_out * n_out));
    if (pdOut == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), "length");
        return 0;
    }

    pdOut[0] = Max(m, n);

    sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, m_out, n_out, pdOut);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        FREE(pdOut);
        Scierror(999, _("%s: Memory allocation error.\n"), "length");
        return 0;
    }

    FREE(pdOut);
    pdOut = NULL;

    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
static int lengthMList(const char *fname, int *piAddressVar)
{
    int nbItem = 0;
    int* piAddrChild    = NULL;

    SciErr sciErr = getListItemNumber(pvApiCtx, piAddressVar, &nbItem);
    if ((sciErr.iErr) || (nbItem < 1))
    {
        return lengthList(piAddressVar);
    }

    sciErr = getListItemAddress(pvApiCtx, piAddressVar, 1, &piAddrChild);
    if (sciErr.iErr)
    {
        return lengthList(piAddressVar);
    }

    if (isStringType(pvApiCtx, piAddrChild))
    {
        int m = 0, n = 0;
        char **pstrData = NULL;
        if (getAllocatedMatrixOfString(pvApiCtx, piAddrChild, &m, &n, &pstrData) == 0)
        {
            char overloadFunctionName[nlgh];
            int lengthOverloadFunctionName = (int)(strlen("%") + strlen(pstrData[0]) + strlen("_") + strlen(fname) + 1);
            if (lengthOverloadFunctionName > nlgh) // length of function name in S5
            {
                freeAllocatedMatrixOfString(m, n, pstrData);
                pstrData = NULL;
                return lengthList(piAddressVar);
            }
            else
            {
                char key[] = "hm ";
                // Case of an hypermat
                if (strncmp(key, pstrData[0], 2) == 0 && strlen(pstrData[0]) == 2)
                {
                    int* piAddrChild3;
                    int iRows = 0;
                    int iCols = 0;
                    double* pdblReal = NULL;
                    int iType = 0;
                    sciErr = getListItemAddress(pvApiCtx, piAddressVar, 3, &piAddrChild3);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return sciErr.iErr;
                    }
                    sciErr = getVarType(pvApiCtx, piAddrChild3, &iType);
                    if (iType == sci_strings)
                    {
                        return lengthList(piAddressVar);
                    }
                    return lengthDefault(piAddrChild3);
                }
                else
                {
                    strcpy(overloadFunctionName, "%");
                    strcat(overloadFunctionName, pstrData[0]);
                    freeAllocatedMatrixOfString(m, n, pstrData);
                    pstrData = NULL;

                    strcat(overloadFunctionName, "_");
                    strcat(overloadFunctionName, fname);

                    if (isScilabFunction(overloadFunctionName))
                    {
                        int lw = 1 + Top - Rhs;
                        C2F(overload)(&lw, (char*)fname, (unsigned long)strlen(fname));
                        return 0;
                    }
                    else
                    {
                        return lengthList(piAddressVar);
                    }
                }
            }
        }
    }
    return lengthList(piAddressVar);
}
/*--------------------------------------------------------------------------*/
static int lengthList(int *piAddressVar)
{
    int m_out = 0, n_out = 0;
    double *pdOut = NULL;

    int nbItem = 0;
    SciErr sciErr = getListItemNumber(pvApiCtx, piAddressVar, &nbItem);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    m_out = 1;
    n_out = 1;
    pdOut = (double*)MALLOC(sizeof(double) * (m_out * n_out));
    if (pdOut == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), "length");
        return 0;
    }

    pdOut[0] = (double) nbItem;

    sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, m_out, n_out, pdOut);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        FREE(pdOut);
        Scierror(999, _("%s: Memory allocation error.\n"), "length");
        return 0;
    }

    FREE(pdOut);
    pdOut = NULL;

    LhsVar(1) = Rhs + 1;
    PutLhsVar();

    return 0;
}
/*--------------------------------------------------------------------------*/
static int lengthDefault(int *piAddressVar)
{
    int m_out = 0, n_out = 0;
    double *pdOut = NULL;

    int m = 0, n = 0;
    SciErr sciErr = getVarDimension(pvApiCtx, piAddressVar, &m, &n);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    m_out = 1;
    n_out = 1;
    pdOut = (double*)MALLOC(sizeof(double) * (m_out * n_out));
    if (pdOut == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), "length");
        return 0;
    }

    pdOut[0] = m * n;

    sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, m_out, n_out, pdOut);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        FREE(pdOut);
        Scierror(999, _("%s: Memory allocation error.\n"), "length");
        return 0;
    }

    FREE(pdOut);
    pdOut = NULL;

    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
