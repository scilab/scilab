/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "api_scilab.h"
#include "createtempfilename.h"
#include "gw_fileio.h"
#include "Scierror.h"
#include "localization.h"
#include "sci_malloc.h"
/*--------------------------------------------------------------------------*/
#define DEFAULT_PREFIX "SCI"
/*--------------------------------------------------------------------------*/
int sci_tempname(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    char *wcprefix = NULL;
    char *wcTempFilename = NULL;

    //Rhs = Max(Rhs, 0);
    CheckRhs(0, 1);
    CheckLhs(1, 1);

    if (Rhs == 0)
    {
        wcprefix = (char*)MALLOC(sizeof(char) * (strlen(DEFAULT_PREFIX) + 1));
        strcpy(wcprefix, DEFAULT_PREFIX);
    }
    else
    {
        int *piAddressVarOne = NULL;

        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
            return 0;
        }

        if (!isScalar(pvApiCtx, piAddressVarOne))
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), fname, 1);
            return 0;
        }

        if (isStringType(pvApiCtx, piAddressVarOne))
        {
            if (getAllocatedSingleString(pvApiCtx, piAddressVarOne, &wcprefix))
            {
                if (wcprefix)
                {
                    freeAllocatedSingleString(wcprefix);
                }

                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }

#if _MSC_VER
            if (strlen(wcprefix) > 3)
            {
                freeAllocatedSingleString(wcprefix);
                Scierror(999, _("%s: Wrong size for input argument #%d: A string (3 characters max.) expected.\n"), fname, 1);
                return 0;
            }
#endif
        }
        else
        {
            freeAllocatedSingleString(wcprefix);
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 1);
            return 0;
        }
    }

    wcTempFilename = createtempfilename(wcprefix, TRUE);
    freeAllocatedSingleString(wcprefix);
    if (wcTempFilename == NULL)
    {
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    if (createSingleString(pvApiCtx, Rhs + 1, wcTempFilename))
    {
        FREE(wcTempFilename);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    FREE(wcTempFilename);
    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
