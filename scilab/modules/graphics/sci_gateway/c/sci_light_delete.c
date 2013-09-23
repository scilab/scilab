/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Pedro SOUZA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "gw_graphics.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "HandleManagement.h"
#include "Light.h"


int sci_light_delete(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int* piAddr	= NULL;
    int nbRow, nbCol;
    long long lightHandle = 0;
    BOOL result;

    CheckInputArgument(pvApiCtx, 1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (getScalarHandle(pvApiCtx, piAddr, &lightHandle))
    {
        Scierror(999, _("%s: Wrong type for argument %d: A graphic handle expected.\n"), fname, 1);
        return 0;
    }

    deleteLight(lightHandle);

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);
}
