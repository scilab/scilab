/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Vincent COUVERT
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include "Signal.hxx"
#include "GiwsException.hxx"

extern "C"
{
#include "gw_action_binding.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "freeArrayOfString.h"
#include "getScilabJavaVM.h"
}
/*--------------------------------------------------------------------------*/
using namespace org_scilab_modules_action_binding_utils;

/*--------------------------------------------------------------------------*/
int sci_notify(char *fname, unsigned long fname_len)
{
    CheckRhs(1, 1);
    CheckLhs(0, 1);

    int m1 = 0, n1 = 0;
    int *piAddressVarOne = NULL;
    char **pStVarOne = NULL;
    int *lenStVarOne = NULL;
    int iType = 0;
    SciErr sciErr;

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

    if (iType != sci_strings)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
        return 0;
    }

    /* get dimensions */
    sciErr = getMatrixOfString(pvApiCtx, piAddressVarOne, &m1, &n1, lenStVarOne, pStVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (m1 * n1 != 1)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
        return 0;
    }

    lenStVarOne = (int *)MALLOC(sizeof(int));
    if (lenStVarOne == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 0;
    }

    /* get lengths */
    sciErr = getMatrixOfString(pvApiCtx, piAddressVarOne, &m1, &n1, lenStVarOne, pStVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    pStVarOne = (char **)MALLOC(sizeof(char *));
    if (pStVarOne == NULL)
    {
        FREE(lenStVarOne);
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 0;
    }

    pStVarOne[0] = (char *)MALLOC(sizeof(char *) * (lenStVarOne[0] + 1));

    /* get strings */
    sciErr = getMatrixOfString(pvApiCtx, piAddressVarOne, &m1, &n1, lenStVarOne, pStVarOne);
    if (sciErr.iErr)
    {
        FREE(lenStVarOne);
        FREE(pStVarOne[0]);
        FREE(pStVarOne);
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    try
    {
        Signal::notify(getScilabJavaVM(), pStVarOne[0]);
    }
    catch (const GiwsException::JniException & e)
    {
        Scierror(999, _("%s: A Java exception arisen:\n%s"), fname, e.whatStr().c_str());
        return 0;
    }

    freeArrayOfString(pStVarOne, 1);
    FREE(lenStVarOne);

    LhsVar(1) = 0;
    PutLhsVar();

    return 0;
}

/*--------------------------------------------------------------------------*/
