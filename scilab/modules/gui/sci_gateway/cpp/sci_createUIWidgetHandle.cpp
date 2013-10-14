/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

extern "C"
{
#include "gw_gui.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
}

/*--------------------------------------------------------------------------*/
int sci_createUIWidgetHandle(char * fname, unsigned long fname_len)
{
    SciErr err;
    int * addr = 0;
    int prec;
    int hdl;
    const int nbIn = nbInputArgument(pvApiCtx);

    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 1, 1);

    err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    if (err.iErr)
    {
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (!isIntegerType(pvApiCtx, addr) || !checkVarDimension(pvApiCtx, addr, 1, 1))
    {
        Scierror(999, gettext("%s: Wrong type for input argument #%d: An int32 expected.\n"), fname, 1);
        return 0;
    }

    err = getMatrixOfIntegerPrecision(pvApiCtx, addr, &prec);
    if (err.iErr)
    {
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (prec != SCI_INT32)
    {
        Scierror(999, gettext("%s: Wrong type for input argument #%d: An int32 expected.\n"), fname, 1);
        return 0;
    }

    if (getScalarInteger32(pvApiCtx, addr, &hdl))
    {
        Scierror(999, gettext("%s: Cannot retrieve argument #%d.\n"), fname, 1);
        return 0;
    }

    if (createScalarHandle(pvApiCtx, nbIn + 1, (long long)hdl))
    {
        Scierror(999, gettext("%s: Cannot create handle.\n"), fname);
        return 0;
    }

    AssignOutputVariable(pvApiCtx, 1) = nbIn + 1;
    ReturnArguments(pvApiCtx);

    return 0;
}

/*--------------------------------------------------------------------------*/
