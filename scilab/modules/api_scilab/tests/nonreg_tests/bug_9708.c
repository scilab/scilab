// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

#include <stdlib.h>
#include <api_scilab.h>
#include <Scierror.h>
#include <localization.h>

int sci_bug_9708(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    const char* varname = "iputhereavariabletoolongforscilabandthereisnoerror";

    sciErr = createNamedMatrixOfString(pvApiCtx, varname, 1, 1, &varname);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Invalid variable name.\n"), fname);
        return 0;
    }

    return 0;
}
// =============================================================================
