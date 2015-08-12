/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "MALLOC.h"
#include "api_scilab.h"
#include "gw_io.h"
#include "stack-c.h"
#include "systemc.h"
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
int sci_host(char *fname, unsigned long fname_len)
{
    SciErr sciErr;

    int *piAddr1;
    char *Str = NULL;
    int stat = 0;

    int iRhs = nbInputArgument(pvApiCtx);

    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 1, 1);

    //get variable address
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (isStringType(pvApiCtx, piAddr1) == 0 || isScalar(pvApiCtx, piAddr1) == 0)
    {
        Scierror(55, _("%s: Wrong type for input argument #%d: String expected.\n"), fname, 1);
        return 0;
    }


    //fisrt call to retrieve dimensions
    if (getAllocatedSingleString(pvApiCtx, piAddr1, &Str))
    {
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 1;
    }

    C2F(systemc)(Str, &stat);
    freeAllocatedSingleString(Str);

    /* Create the matrix as return of the function */
    if (createScalarDouble(pvApiCtx, iRhs + 1, stat))
    {
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    AssignOutputVariable(pvApiCtx, 1) = iRhs + 1;
    ReturnArguments(pvApiCtx);

    return 0;
}
/*--------------------------------------------------------------------------*/
