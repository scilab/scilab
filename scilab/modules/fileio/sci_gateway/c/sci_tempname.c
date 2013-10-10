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
/*--------------------------------------------------------------------------*/
#define DEFAULT_PREFIX L"SCI"
/*--------------------------------------------------------------------------*/
int sci_tempname(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    wchar_t *wcprefix = NULL;
    wchar_t *wcTempFilename = NULL;

    Rhs = Max(Rhs, 0);
    CheckRhs(0, 1);
    CheckLhs(1, 1);

    if (Rhs == 0)
    {
        wcprefix = (wchar_t *)MALLOC(sizeof(wchar_t) * (wcslen(DEFAULT_PREFIX) + 1));
        wcscpy(wcprefix, DEFAULT_PREFIX);
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
            if (getAllocatedSingleWideString(pvApiCtx, piAddressVarOne, &wcprefix))
            {
                if (wcprefix)
                {
                    freeAllocatedSingleWideString(wcprefix);
                }

                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }

#if _MSC_VER
            if (wcslen(wcprefix) > 3)
            {
                FREE(wcprefix);
                Scierror(999, _("%s: Wrong size for input argument #%d: A string (3 characters max.) expected.\n"), fname, 1);
                return 0;
            }
#endif
        }
        else
        {
            FREE(wcprefix);
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
            return 0;
        }
    }

    wcTempFilename = createtempfilenameW(wcprefix, TRUE);
    FREE(wcprefix);
    if (wcTempFilename == NULL)
    {
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    if (createSingleWideString(pvApiCtx, Rhs + 1, wcTempFilename))
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
