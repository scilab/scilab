/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#include <stdio.h>
#include <stdlib.h> /* _MAX_ENV */
#include "gw_io.h"
#include "MALLOC.h"
#include "freeArrayOfString.h"
#include "localization.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "getenvc.h"
#include "PATH_MAX.h"
/*--------------------------------------------------------------------------*/
int sci_getenv(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int ierr = 0;
    char *env_value = NULL;
    int length_env = 0;

    int *piAddressVarOne = NULL;
    char *pStVarOne = NULL;

    int *piAddressVarTwo = NULL;
    char *pStVarTwo = NULL;

    int iflag = 0;

    Rhs = Max(Rhs, 0);

    CheckRhs(1, 2);
    CheckLhs(1, 1);

    if (Rhs == 2)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
            return 0;
        }

        if (isStringType(pvApiCtx, piAddressVarTwo) == 0 || isScalar(pvApiCtx, piAddressVarTwo) == 0)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 2);
            return 0;
        }

        if (getAllocatedSingleString(pvApiCtx, piAddressVarTwo, &pStVarTwo))
        {
            if (pStVarTwo)
            {
                freeAllocatedSingleString(pStVarTwo);
            }

            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
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

    if (isStringType(pvApiCtx, piAddressVarOne) == 0 || isScalar(pvApiCtx, piAddressVarOne) == 0)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 2);
        return 0;
    }

    if (getAllocatedSingleString(pvApiCtx, piAddressVarOne, &pStVarOne))
    {
        if (pStVarOne)
        {
            freeAllocatedSingleString(pStVarOne);
        }

        if (pStVarTwo)
        {
            freeAllocatedSingleString(pStVarTwo);
        }

        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }


    C2F(getenvc)(&ierr, pStVarOne, NULL, &length_env, &iflag);

    if (ierr)
    {
        if (pStVarTwo)
        {
            if (createSingleString(pvApiCtx, Rhs + 1, pStVarTwo))
            {
                freeAllocatedSingleString(pStVarOne);
                freeAllocatedSingleString(pStVarTwo);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }
            else
            {
                LhsVar(1) = Rhs + 1;
                PutLhsVar();
                return 0;
            }
        }
        else
        {
            Scierror(999, _("%s: Undefined environment variable %s.\n"), fname, pStVarOne);
            freeAllocatedSingleString(pStVarOne);
            return 0;
        }
    }

    //variable exists in env, we don't need default any more
    freeAllocatedSingleString(pStVarTwo);

    env_value = (char*)MALLOC(sizeof(char) * (length_env + 1));
    if (env_value == NULL)
    {
        freeAllocatedSingleString(pStVarOne);
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 0;
    }

    C2F(getenvc)(&ierr, pStVarOne, env_value, &length_env, &iflag);

    //create variable on stack and return it.
    if (createSingleString(pvApiCtx, Rhs + 1, env_value))
    {
        FREE(env_value);
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    FREE(env_value);
    LhsVar(1) = Rhs + 1;
    PutLhsVar();


    return 0;
}
/*--------------------------------------------------------------------------*/
