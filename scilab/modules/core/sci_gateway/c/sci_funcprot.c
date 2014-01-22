/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2009-2011 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "gw_core.h"
#include "funcprot.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_funcprot)(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    CheckLhs(1, 1);
    CheckRhs(0, 1);

    if (Rhs == 0)
    {
        double dOut = (double) getfuncprot();

        if (createScalarDouble(pvApiCtx, Rhs + 1, dOut) != 0)
        {
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }

        LhsVar(1) = Rhs + 1;
        PutLhsVar();
    }
    else if (Rhs == 1)
    {
        int ilevel = 0;
        int *piAddressVarOne = NULL;
        double dVarOne = 0.;
        double dPreviousValue = (double) getfuncprot();

        /* get Address of inputs */
        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
            return 0;
        }

        /* check input type */
        if (isDoubleType(pvApiCtx, piAddressVarOne) != 1)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), fname, 1);
            return 0;
        }

        if (isScalar(pvApiCtx, piAddressVarOne) != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), fname, 1);
            return 0;
        }

        if (getScalarDouble(pvApiCtx, piAddressVarOne, &dVarOne) != 0)
        {
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
            return 0;
        }

        ilevel = (int) dVarOne;

        if (dVarOne != (double)ilevel)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: An integer value expected.\n"), fname, 1);
            return 0;
        }

        if (!setfuncprot(ilevel))
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: %d, %d or %d expected.\n"), fname, 1, 0, 1, 2);
            return 0;
        }
        else
        {
            if (createScalarDouble(pvApiCtx, Rhs + 1, dPreviousValue) != 0)
            {
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }

            LhsVar(1) = Rhs + 1;
            PutLhsVar();
        }
    }
    return 0;
}
/*--------------------------------------------------------------------------*/

