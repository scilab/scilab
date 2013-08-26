/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2012 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include "gw_integer.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "machine.h"
#include "sci_types.h"
/*--------------------------------------------------------------------------*/
extern int C2F(stackconvert)(int *iTypeToConvert); /* FORTRAN subroutine */
/*--------------------------------------------------------------------------*/
int sci_iconvert(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int *piAddressVarOne = NULL;
    int *piAddressVarTwo = NULL;
    double dValue = 0.;
    int iValue = 0;

    CheckRhs(2, 2);
    CheckLhs(0, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (isVarComplex(pvApiCtx, piAddressVarOne))
    {
        Scierror(202, _("%s: Wrong type for argument #%d: Real matrix expected.\n"), fname, 1);
        return 0;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return 0;
    }

    if (!isDoubleType(pvApiCtx, piAddressVarTwo))
    {
        Scierror(202, _("%s: Wrong type for argument #%d: Real scalar expected.\n"), fname, 2);
        return 0;
    }

    if (isVarComplex(pvApiCtx, piAddressVarTwo))
    {
        Scierror(202, _("%s: Wrong type for argument #%d: Real scalar expected.\n"), fname, 2);
        return 0;
    }

    if (!isScalar(pvApiCtx, piAddressVarTwo))
    {
        Scierror(202, _("%s: Wrong size for argument #%d: Real scalar expected.\n"), fname, 2);
        return 0;
    }

    if (getScalarDouble(pvApiCtx, piAddressVarTwo, &dValue) != 0)
    {
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 0;
    }

    iValue = (int)dValue;
    if (dValue != (double)iValue)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: An integer value expected.\n"), fname, 2);
        return 0;
    }

    switch (iValue)
    {
        case 0: // double
        case sci_int8:
        case sci_uint8:
        case sci_int16:
        case sci_uint16:
        case sci_int32:
        case sci_uint32:
        {
            --Top;
            --Rhs;
            C2F(stackconvert)(&iValue);
        }
        break;
        default:
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"), fname, 2, "0, 1, 2, 4, 11, 12, 14");
        }
        break;
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
