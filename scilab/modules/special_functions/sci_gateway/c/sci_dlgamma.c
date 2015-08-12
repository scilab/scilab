/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA - Allan CORNET
 * Copyright (C) 2012 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#include "gw_special_functions.h"
#include "machine.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
extern double C2F(psi)();
/*--------------------------------------------------------------------------*/
int sci_dlgamma(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    double* lX   = NULL;
    int* piAddrX = NULL;

    int iType1 = 0;
    int MX = 0, NX = 0, i = 0;

    nbInputArgument(pvApiCtx) = Max(0, nbInputArgument(pvApiCtx));

    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrX);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 1;
    }

    sciErr = getVarType(pvApiCtx, piAddrX, &iType1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 1;
    }

    if ((iType1 == sci_list) || (iType1 == sci_tlist) || (iType1 == sci_mlist))
    {
        OverLoad(1);
        return 0;
    }

    if (isVarComplex(pvApiCtx, piAddrX))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real expected.\n"), fname, 1);
        return 1;
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddrX, &MX, &NX, &lX);
    if (sciErr.iErr)
    {
        Scierror(999, _("%s: Wrong type for argument #%d: A matrix expected.\n"), fname, 1);
    }

    for (i = 0; i < MX * NX; i++)
    {
        lX[i] = C2F(psi)(lX + i);
    }

    AssignOutputVariable(pvApiCtx, 1) = 1;
    returnArguments(pvApiCtx);
    return 0;
}
/*--------------------------------------------------------------------------*/
