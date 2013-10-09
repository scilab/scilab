/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Sylvestre LEDRU
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
/*
 * Provides is_dir to scilab
 *
 */
/*--------------------------------------------------------------------------*/
#include "gw_fileio.h"
#include "MALLOC.h"
#include "localization.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "expandPathVariable.h"
#include "isdir.h"
#include "freeArrayOfString.h"
#include "BOOL.h"
/*--------------------------------------------------------------------------*/
int sci_isdir(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int *piAddressVarOne = NULL;
    wchar_t **pStVarOne = NULL;
    int iType = 0;
    int *lenStVarOne = NULL;
    int m1 = 0, n1 = 0;

    BOOL *results = NULL;
    int i = 0;

    /* Check Input & Output parameters */
    CheckRhs(1, 1);
    CheckLhs(1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    sciErr = getVarType(pvApiCtx, piAddressVarOne, &iType);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (isStringType(pvApiCtx, piAddressVarOne) == 0)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
        return 0;
    }

    if (getAllocatedMatrixOfWideString(pvApiCtx, piAddressVarOne, &m1, &n1, &pStVarOne))
    {
        if (pStVarOne)
        {
            freeAllocatedMatrixOfWideString(m1, n1, pStVarOne);
        }

        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }


    sciErr = allocMatrixOfBoolean(pvApiCtx, Rhs + 1, m1, n1, &results);
    if (sciErr.iErr)
    {
        FREE(results);
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    for (i = 0; i < m1 * n1; i++)
    {
        wchar_t *expandedPath = expandPathVariableW(pStVarOne[i]);
        if (expandedPath == NULL)
        {
            results[i] = FALSE;
            continue;
        }

        results[i] = isdirW(expandedPath);
        FREE(expandedPath);
        expandedPath = NULL;
    }

    freeAllocatedMatrixOfWideString(m1, n1, pStVarOne);

    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
