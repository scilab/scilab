/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "gw_core.h"
#include "api_scilab.h"
#include "stack-c.h"
#include "lasterror.h"
#include "BOOL.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_lasterror)(char *fname,unsigned long fname_len)
{
    SciErr sciErr;
    BOOL bClearLastError = TRUE;
    int NbLines = 0;
    const char **errorMessage = NULL;

    Rhs = Max(0, Rhs);
    CheckRhs(0,1);
    CheckLhs(1,4);

    if (Rhs == 1)
    {
        int iType = 0;
        int *piAddressVarOne = NULL;

        /* get Address of inputs */
        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        if (isBooleanType(pvApiCtx, piAddressVarOne))
        {
            if (isScalar(pvApiCtx, piAddressVarOne))
            {
                getScalarBoolean(pvApiCtx, piAddressVarOne, &bClearLastError);
            }
            else
            {
                Scierror(999,_("%s: Wrong size for input argument #%d: A boolean expected.\n"), fname, 1);
            }
        }
        else
        {
            Scierror(999,_("%s: Wrong type for input argument #%d: A boolean expected.\n"), fname, 1);
        }
    }

    errorMessage = getLastErrorMessage(&NbLines);

    if ((NbLines <= 0) || (errorMessage == NULL))
    {
        createEmptyMatrix(pvApiCtx, Rhs + 1);
    }
    else
    {
        sciErr = createMatrixOfString(pvApiCtx, Rhs + 1, NbLines, 1, (char**)errorMessage);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
    }
    LhsVar(1) = Rhs + 1;

    if (Lhs >= 2)
    {
        double dLastErrorValue = (double) getLastErrorValue();
        createScalarDouble(pvApiCtx, Rhs + 2, dLastErrorValue);
        LhsVar(2) = Rhs + 2;
    }

    if (Lhs >= 3)
    {
        double dLinePosition = (double)  getLastErrorLinePosition();
        createScalarDouble(pvApiCtx, Rhs + 3, dLinePosition);
        LhsVar(3) = Rhs + 3;
    }

    if (Lhs == 4)
    {
        createSingleString(pvApiCtx, Rhs + 4, (char*)getLastErrorFunctionName());
        LhsVar(4) = Rhs + 4;
    }

    if (bClearLastError)
    {
        clearLastError();
    }

    C2F(putlhsvar)();

    return 0;
}
/*--------------------------------------------------------------------------*/
