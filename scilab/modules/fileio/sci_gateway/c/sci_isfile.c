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
#include "FileExist.h"
#include "isdir.h"
#include "expandPathVariable.h"
#include "freeArrayOfString.h"
#include "BOOL.h"
/*--------------------------------------------------------------------------*/
int sci_isfile(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int *piAddressVarOne = NULL;
    wchar_t **pStVarOne = NULL;
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

    if (getAllocatedMatrixOfWideString(pvApiCtx, piAddressVarOne, &m1, &n1, &pStVarOne))
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        FREE(results);
        return 0;
    }

    sciErr = allocMatrixOfBoolean(pvApiCtx, Rhs + 1, m1, n1, &results);
    if (sciErr.iErr)
    {
        freeAllocatedMatrixOfWideString(m1, n1, pStVarOne);
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    for (i = 0; i < m1 * n1; i++)
    {
        wchar_t *expandedPath = expandPathVariableW(pStVarOne[i]);
        if (expandedPath)
        {
            results[i] = !isdirW(expandedPath) && FileExistW(expandedPath);
            FREE(expandedPath);
            expandedPath = NULL;
        }
        else
        {
            results[i] = FALSE;
        }
    }

    freeAllocatedMatrixOfWideString(m1, n1, pStVarOne);

    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
