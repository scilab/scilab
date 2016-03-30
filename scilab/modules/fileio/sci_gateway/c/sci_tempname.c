/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Allan CORNET
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
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "api_scilab.h"
#include "createtempfilename.h"
#include "gw_fileio.h"
#include "Scierror.h"
#include "localization.h"
#include "sci_malloc.h"
/*--------------------------------------------------------------------------*/
#define DEFAULT_PREFIX L"SCI"
/*--------------------------------------------------------------------------*/
int sci_tempname(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    wchar_t *wcprefix = NULL;
    wchar_t *wcTempFilename = NULL;

    //Rhs = Max(Rhs, 0);
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
                freeAllocatedSingleWideString(wcprefix);
                Scierror(999, _("%s: Wrong size for input argument #%d: A string (3 characters max.) expected.\n"), fname, 1);
                return 0;
            }
#endif
        }
        else
        {
            freeAllocatedSingleWideString(wcprefix);
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 1);
            return 0;
        }
    }

    wcTempFilename = createtempfilenameW(wcprefix, TRUE);
    freeAllocatedSingleWideString(wcprefix);
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
