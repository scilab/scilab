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
// =============================================================================
int sci_bug_9707_1(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    const char varname[] = "1";


    sciErr = createNamedMatrixOfString(pvApiCtx, varname, 1, 1, &fname);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Invalid variable name.\n"), fname);
    }
    return 0;
}
// =============================================================================
int sci_bug_9707_2(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    const char varname[] = "";

    sciErr = createNamedMatrixOfString(pvApiCtx, varname, 1, 1, &fname);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Invalid variable name.\n"), fname);
    }
    return 0;
}
// =============================================================================
int sci_bug_9707_3(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    const char varname[] = " ";

    sciErr = createNamedMatrixOfString(pvApiCtx, varname, 1, 1, &fname);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Invalid variable name.\n"), fname);
    }
    return 0;
}
// =============================================================================
int sci_bug_9707_4(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    const char varname[] = "Scilab Variable";

    sciErr = createNamedMatrixOfString(pvApiCtx, varname, 1, 1, &fname);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Invalid variable name.\n"), fname);
    }
    return 0;
}
// =============================================================================
int sci_bug_9707_OK(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    const char varname[] = "VAR_9707";

    sciErr = createNamedMatrixOfString(pvApiCtx, varname, 1, 1, &fname);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Invalid variable name.\n"), fname);
    }
    return 0;
}
// =============================================================================
