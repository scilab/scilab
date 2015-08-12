/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
* Copyright (C) DIGITEO - 2012 - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

#include <string.h>
#include "gw_boolean.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "vect_and.h"
/*--------------------------------------------------------------------------*/
/* SCILAB function : and */
/*--------------------------------------------------------------------------*/
int sci_and(char *fname, unsigned long l)
{
    int m1 = 0, n1 = 0;
    int opt = 0;
    int *pBoolValuesOne = NULL;
    int *pBoolResult = NULL;

    SciErr sciErr;
    int *piAddressVarOne = NULL;

    CheckRhs(1, 2);
    CheckLhs(1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (!isBooleanType(pvApiCtx, piAddressVarOne))
    {
        callOverloadFunction(pvApiCtx, 1, fname, l);
        return 0;
    }

    if (Rhs == 2)
    {
        int *piAddressVarTwo = NULL;

        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
            return 0;
        }

        if (!isScalar(pvApiCtx, piAddressVarTwo))
        {
            Scierror(999, _("%s: Wrong size for input argument #%d.\n"), fname, 2);
            return 0;
        }

        if (isStringType(pvApiCtx, piAddressVarTwo))
        {
            char *pStr = NULL;
            if (getAllocatedSingleString(pvApiCtx, piAddressVarTwo, &pStr) == 0)
            {
                if (pStr)
                {
                    size_t len = strlen(pStr);
                    switch (pStr[0])
                    {
                        case 'r':
                        {
                            opt = AND_BY_ROWS;
                        }
                        break;
                        case '*':
                        {
                            opt = AND_ON_ALL;
                        }
                        break;
                        case 'c':
                        {
                            opt = AND_BY_COLUMNS;
                        }
                        break;
                        default:
                        {
                            Scierror(44, _("%s: Wrong value for input argument #%d.\n"), fname, 2);
                            return 0;
                        }
                        break;
                    }

                    freeAllocatedSingleString(pStr);
                    pStr = NULL;

                    if (len != 1)
                    {
                        Scierror(44, _("%s: Wrong value for input argument #%d.\n"), fname, 2);
                        return 0;
                    }
                }
                else
                {
                    Scierror(999, _("%s: Memory allocation error.\n"), fname);
                    return 0;
                }
            }
            else
            {
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }
        }
        else
        {
            if (isDoubleType(pvApiCtx, piAddressVarTwo))
            {
                double dValue = 0.;
                if (isVarComplex(pvApiCtx, piAddressVarTwo))
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d.\n"), fname, 2);
                    return 0;
                }

                if (getScalarDouble(pvApiCtx, piAddressVarTwo, &dValue) != 0)
                {
                    Scierror(999, _("%s: No more memory.\n"), fname);
                    return 0;
                }

                opt = (int)dValue;
                if (dValue != (double)opt)
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d: An integer value expected.\n"), fname, 2);
                    return 0;
                }

                if ((opt > AND_BY_COLUMNS) || (opt < AND_BY_ROWS))
                {
                    Scierror(44, _("%s: Wrong value for input argument #%d.\n"), fname, 2);
                    return 0;
                }
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d.\n"), fname, 2);
                return 0;
            }
        }
    }

    if ((Rhs == 2) && isEmptyMatrix(pvApiCtx, piAddressVarOne))
    {
        createEmptyMatrix(pvApiCtx, Rhs + 1);
        LhsVar(1) = Rhs + 1;
        PutLhsVar();
        return 0;
    }

    sciErr = getMatrixOfBoolean(pvApiCtx, piAddressVarOne, &m1, &n1, &pBoolValuesOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    /* cross variable size checking */
    if (opt == AND_ON_ALL)
    {
        sciErr = allocMatrixOfBoolean(pvApiCtx, Rhs + 1, 1, 1, &pBoolResult);
    }
    else if (opt == AND_BY_ROWS)
    {
        sciErr = allocMatrixOfBoolean(pvApiCtx, Rhs + 1, 1, n1, &pBoolResult);
    }
    else if (opt == AND_BY_COLUMNS)
    {
        sciErr = allocMatrixOfBoolean(pvApiCtx, Rhs + 1, m1, 1, &pBoolResult);
    }

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 0;
    }

    vect_and(pBoolValuesOne, m1, n1, pBoolResult, opt);

    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
