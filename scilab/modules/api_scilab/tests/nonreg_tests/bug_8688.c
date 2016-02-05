// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

#include <stdlib.h>
#include <api_scilab.h>
#include <Scierror.h>
#include <sci_malloc.h>
#include <localization.h>
#include <os_string.h>

int sci_bug_8688(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int N = 20000000;
    char **ptrStrs = (char **)MALLOC(sizeof(char*) * N);
    int i = 0;

    if (ptrStrs == NULL)
    {
        Scierror(999, "memory allocation error");
    }

    for (i = 0; i < N; i++)
    {
        ptrStrs[i] = os_strdup("TEST BUG 8688.");
    }

    sciErr = createMatrixOfString(pvApiCtx, Rhs + 1, 1, N, ptrStrs);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    for (i = 0; i < N; i++)
    {
        free(ptrStrs[i]);
    }

    FREE(ptrStrs);
    LhsVar(1) = 1;
    return 0;
}
// =============================================================================
