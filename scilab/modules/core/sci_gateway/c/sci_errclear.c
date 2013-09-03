/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
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
#include "api_scilab.h"
#include "lasterror.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_errclear)(char *fname, unsigned long fname_len)
{
    Rhs = Max(0, Rhs);
    CheckRhs(0, 2);
    CheckLhs(1, 1);

    if (Rhs == 1)
    {
        SciErr sciErr;
        int *piAddressVarOne = NULL;

        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
            return 0;
        }

        if (isDoubleType(pvApiCtx, piAddressVarOne))
        {
            double dValue = 0.;
            int iValue = 0;
            int iLastErrorValue = getInternalLastErrorValue();

            if (!isScalar(pvApiCtx, piAddressVarOne))
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), fname, 1);
                return 0;
            }

            getScalarDouble(pvApiCtx, piAddressVarOne, &dValue);
            iValue = (int)dValue;

            if ((double)iValue != dValue)
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: An integer value expected.\n"), fname, 1);
                return 0;
            }

            if ((iValue == iLastErrorValue) || (iValue <= 0))
            {
                /* clear fortran common error */
                C2F(errgst).err2 = 0;

                /* clear last error buffer (C) */
                clearInternalLastError();
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: An integer value expected.\n"), fname, 1);
            return 0;
        }
    }
    else
    {
        /* clear fortran common error */
        C2F(errgst).err2 = 0;

        /* clear last error buffer (C) */
        clearInternalLastError();
    }
    LhsVar(1) = 0;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
