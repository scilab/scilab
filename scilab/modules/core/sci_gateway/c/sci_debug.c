/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2012 - DIGITEO - Allan CORNET
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
#include "gw_core.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "sciprint.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_debug)(char *fname, void *pvApiCtx)
{
    double dPreviousDebugLevel = 0.;

    Rhs = Max(Rhs, 0);
    CheckLhs(1, 1);
    CheckRhs(0, 1);

    //dPreviousDebugLevel = (double)C2F(iop).ddt;

    if (Rhs == 1)
    {
        int *piAddressVarOne = NULL;

        double dNewDebugLevel = 0.;
        int iNewDebugLevel = 0;
        SciErr sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
            return 0;
        }

        if (isVarComplex(pvApiCtx, piAddressVarOne))
        {
            Scierror(202, _("%s: Wrong type for argument #%d: Real scalar expected.\n"), fname, 1);
            return 0;
        }

        if (!isDoubleType(pvApiCtx, piAddressVarOne))
        {
            Scierror(202, _("%s: Wrong type for argument #%d: Real scalar expected.\n"), fname, 1);
            return 0;
        }

        if (isEmptyMatrix(pvApiCtx, piAddressVarOne))
        {
            Scierror(202, _("%s: Wrong size for argument #%d: Real scalar expected.\n"), fname, 1);
            return 0;
        }

        if (!isScalar(pvApiCtx, piAddressVarOne))
        {
            Scierror(202, _("%s: Wrong size for argument #%d: Real scalar expected.\n"), fname, 1);
            return 0;
        }

        if (getScalarDouble(pvApiCtx, piAddressVarOne, &dNewDebugLevel) != 0)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }

        iNewDebugLevel = (int)dNewDebugLevel;
        if (dNewDebugLevel != (double)iNewDebugLevel)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: An integer value expected.\n"), fname, 1);
            return 0;
        }

        //C2F(iop).ddt = iNewDebugLevel;
        //sciprint(" debug %d\n", C2F(iop).ddt);
    }

    if (createScalarDouble(pvApiCtx, Rhs + 1, dPreviousDebugLevel) != 0)
    {
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }
    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
