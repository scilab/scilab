/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include "gw_fileio.h"
#include "MALLOC.h"
#include "localization.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "pathconvert.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
int sci_pathconvert(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int *piAddressVarOne = NULL;
    wchar_t **pStVarOne = NULL;
    int m1 = 0, n1 = 0;

    wchar_t **results = NULL;
    int i = 0;

    BOOL flagtrail = TRUE;
    BOOL flagexpand = TRUE;
    PathConvertType PType = AUTO_STYLE;

    /* Check Input & Output parameters */
    CheckRhs(1, 4);
    CheckLhs(1, 1);

    if (Rhs > 3)
    {
        int *piAddressVarFour = NULL;
        wchar_t *pStVarFour = NULL;

        sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddressVarFour);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 4);
            return 0;
        }

        if (isStringType(pvApiCtx, piAddressVarFour) == 0 || isScalar(pvApiCtx, piAddressVarFour) == 0)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 4);
            return 0;
        }

        if (getAllocatedSingleWideString(pvApiCtx, piAddressVarFour, &pStVarFour))
        {
            if (pStVarFour)
            {
                freeAllocatedSingleWideString(pStVarFour);
            }

            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 4);
            return 0;
        }

        if ((wcscmp(pStVarFour, L"w") == 0) || (wcscmp(pStVarFour, L"u") == 0))
        {
            if (wcscmp(pStVarFour, L"w") == 0)
            {
                PType = WINDOWS_STYLE;
            }
            else
            {
                PType = UNIX_STYLE;
            }

            freeAllocatedSingleWideString(pStVarFour);
        }
        else
        {
            freeAllocatedSingleWideString(pStVarFour);
            Scierror(999, _("%s: Wrong value for input argument #%d: 'w' or 'u' string expected.\n"), fname, 4);
            return 0;
        }
    }

    if (Rhs > 2)
    {
        int *piAddressVarThree = NULL;
        sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddressVarThree);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
            return 0;
        }

        if (isBooleanType(pvApiCtx, piAddressVarThree) == 0 || isScalar(pvApiCtx, piAddressVarThree) == 0)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A boolean expected.\n"), fname, 3);
            return 0;
        }

        if (getScalarBoolean(pvApiCtx, piAddressVarThree, &flagexpand))
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A boolean expected.\n"), fname, 3);
            return 0;
        }
    }

    if (Rhs > 1)
    {
        int *piAddressVarTwo = NULL;
        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
            return 0;
        }

        if (isBooleanType(pvApiCtx, piAddressVarTwo) == 0 || isScalar(pvApiCtx, piAddressVarTwo) == 0)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A boolean expected.\n"), fname, 2);
            return 0;
        }

        if (getScalarBoolean(pvApiCtx, piAddressVarTwo, &flagtrail))
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A boolean expected.\n"), fname, 2);
            return 0;
        }
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (isDoubleType(pvApiCtx, piAddressVarOne))
    {
        if (isEmptyMatrix(pvApiCtx, piAddressVarOne))
        {
            if (createEmptyMatrix(pvApiCtx, Rhs + 1))
            {
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }

            LhsVar(1) = Rhs + 1;
            PutLhsVar();
            return 0;
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: String array expected.\n"), fname, 1);
            return 0;
        }
    }
    else if (isStringType(pvApiCtx, piAddressVarOne))
    {
        if (getAllocatedMatrixOfWideString(pvApiCtx, piAddressVarOne, &m1, &n1, &pStVarOne))
        {
            if (pStVarOne)
            {
                freeAllocatedMatrixOfWideString(m1, n1, pStVarOne);
            }

            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
            return 0;
        }

        results = (wchar_t **) MALLOC(sizeof(wchar_t *) * (m1 * n1));
        if (results == NULL)
        {
            freeAllocatedMatrixOfWideString(m1, n1, pStVarOne);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }

        for (i = 0; i < m1 * n1; i++)
        {
            results[i] = pathconvertW(pStVarOne[i], flagtrail, flagexpand, PType);
        }

        freeAllocatedMatrixOfWideString(m1, n1, pStVarOne);

        sciErr = createMatrixOfWideString(pvApiCtx, Rhs + 1, m1, n1, results);
        freeArrayOfWideString(results, m1 * n1);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
            return 0;
        }

        LhsVar(1) = Rhs + 1;
        PutLhsVar();
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
        return 0;
    }
    return 0;
}

/*--------------------------------------------------------------------------*/
