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
#include "basename.h"
#include "Scierror.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
int sci_basename(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    BOOL flagexpand = TRUE; /* default */

    int *piAddressVarOne = NULL;
    wchar_t **pStVarOne = NULL;
    int m1 = 0;
    int n1 = 0;
    int i = 0;

    wchar_t **pStResult = NULL;

    /* Check Input & Output parameters */
    CheckRhs(1, 3);
    CheckLhs(1, 1);

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
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
            return 0;
        }
    }

    //do not care of 2nd param ? Oo
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
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: String array expected.\n"), fname, 1);
            return 0;
        }

        LhsVar(1) = Rhs + 1;
        PutLhsVar();
        return 0;
    }

    if (isStringType(pvApiCtx, piAddressVarOne) == 0)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String array expected.\n"), fname, 1);
        return 0;
    }

    if (getAllocatedMatrixOfWideString(pvApiCtx, piAddressVarOne, &m1, &n1, &pStVarOne))
    {
        freeAllocatedMatrixOfWideString(m1, n1, pStVarOne);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    pStResult = (wchar_t**)MALLOC(sizeof(wchar_t*) * (m1 * n1));
    if (pStResult == NULL)
    {
        freeAllocatedMatrixOfWideString(m1, n1, pStVarOne);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    for (i = 0; i < m1 * n1; i++)
    {
        pStResult[i] = basenameW(pStVarOne[i], flagexpand);
    }

    freeAllocatedMatrixOfWideString(m1, n1, pStVarOne);
    sciErr = createMatrixOfWideString(pvApiCtx, Rhs + 1, m1, n1, pStResult);
    freeArrayOfWideString(pStResult, m1 * n1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
