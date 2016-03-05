/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
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
#include <stdio.h>
#include <stdlib.h> /* _MAX_ENV */
#include "gw_io.h"
#include "sci_malloc.h"
#include "freeArrayOfString.h"
#include "localization.h"
#include "Scierror.h"
#include "getenvc.h"
#include "PATH_MAX.h"
#include "api_scilab.h"
/*--------------------------------------------------------------------------*/
int sci_getenv(char *fname, void* pvApiCtx)
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

    int iRhs = nbInputArgument(pvApiCtx);

    CheckRhs(1, 2);
    CheckLhs(1, 1);

    if (iRhs == 2)
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
            Scierror(999, _("%s: Wrong size for input argument #%d: string expected.\n"), fname, 2);
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
        if (pStVarTwo)
        {
            freeAllocatedSingleString(pStVarTwo);
        }
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (isStringType(pvApiCtx, piAddressVarOne) == 0 || isScalar(pvApiCtx, piAddressVarOne) == 0)
    {
        if (pStVarTwo)
        {
            freeAllocatedSingleString(pStVarTwo);
        }
        Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 2);
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

    getenvc(&ierr, pStVarOne, NULL, &length_env, &iflag);
    if (ierr)
    {
        if (pStVarTwo)
        {
            if (createSingleString(pvApiCtx, iRhs + 1, pStVarTwo))
            {
                freeAllocatedSingleString(pStVarOne);
                freeAllocatedSingleString(pStVarTwo);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }
            else
            {
                freeAllocatedSingleString(pStVarOne);
                freeAllocatedSingleString(pStVarTwo);
                LhsVar(1) = iRhs + 1;
                ReturnArguments(pvApiCtx);
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

    getenvc(&ierr, pStVarOne, env_value, &length_env, &iflag);
    freeAllocatedSingleString(pStVarOne);

    //create variable on stack and return it.
    if (createSingleString(pvApiCtx, iRhs + 1, env_value))
    {
        FREE(env_value);
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    FREE(env_value);
    AssignOutputVariable(pvApiCtx, 1) = iRhs + 1;
    ReturnArguments(pvApiCtx);


    return 0;
}
/*--------------------------------------------------------------------------*/
