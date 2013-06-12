/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * Copyright (C) 2011 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "api_scilab.h"
#include "scilines.h"
#include "Scierror.h"
#include "localization.h"
#include "MALLOC.h"
#include "gw_console.h"
#include "ScilabLinesUpdate.h"
#include "sci_mode.h"
/*--------------------------------------------------------------------------*/
static int sci_lines_no_rhs(char *fname, void* pvApiCtx);
static int sci_lines_one_rhs(char *fname, void* pvApiCtx);
static int sci_lines_two_rhs(char *fname, void* pvApiCtx);
/*--------------------------------------------------------------------------*/
int sci_lines(char *fname, void* pvApiCtx)
{
    CheckRhs(0, 2);
    CheckLhs(1, 1);

    switch (Rhs)
    {
        case 0:
            sci_lines_no_rhs(fname, pvApiCtx);
            break;
        case 1:
            sci_lines_one_rhs(fname, pvApiCtx);
            break;
        case 2:
            sci_lines_two_rhs(fname, pvApiCtx);
            break;
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
static int sci_lines_no_rhs(char *fname, void* pvApiCtx)
{
    /* ncl = lines() */
    /* with ncl a 1x2 vector [nc , nl] */
    /* for compatibility BUT order should be [nl, nc] */
    /* input/output not coherents */
    SciErr sciErr;
    double returnedDouble[2];
    int n1 = 0, m1 = 0;

    returnedDouble[0] = (double)getColumnsSize();
    returnedDouble[1] = (double)getLinesSize();

    n1 = 1;
    m1 = 2;
    sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, n1, m1, returnedDouble);

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();

    return 0;
}
/*--------------------------------------------------------------------------*/
static int sci_lines_one_rhs(char *fname, void* pvApiCtx)
{
    /* lines(nb_lines) */
    /* nb_lines == -1 auto */
    /* nb_lines == 0 no stop on display */
    /* nb_lines > 0 set value */
    SciErr sciErr;
    int *piAddressVarOne = NULL;
    int iParam1 = 0;

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    /* compatibility with previous version manages int32 and double */
    if (!(isDoubleType(pvApiCtx, piAddressVarOne) || isIntegerType(pvApiCtx, piAddressVarOne)))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), fname, 1);
        return 0;
    }

    if (!isScalar(pvApiCtx, piAddressVarOne))
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), fname, 1);
        return 0;
    }

    if (isIntegerType(pvApiCtx, piAddressVarOne))
    {
        if (getScalarInteger32(pvApiCtx, piAddressVarOne, &iParam1) != 0)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), fname, 1);
            return 0;
        }
    }
    else // double
    {
        double dParam1 = 0.;
        if (getScalarDouble(pvApiCtx, piAddressVarOne, &dParam1) == 0)
        {
            iParam1 = (int)dParam1;
        }
    }

    if (iParam1 == -1)
    {
        scilinesdefault();
        if (getScilabMode() == SCILAB_STD)
        {
            ScilabLinesUpdate();
        }
    }
    else
    {
        if (iParam1 < 0)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: A scalar (>= 0) expected.\n"), fname, 1);
            return 0;
        }
        else
        {
            setLinesSize(iParam1);
        }
    }

    LhsVar(1) = 0;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
static int sci_lines_two_rhs(char *fname, void* pvApiCtx)
{
    /* lines (nl, nc) */
    SciErr sciErr;
    int *piAddressVarOne = NULL;
    int *piAddressVarTwo = NULL;

    int iParam1 = 0;
    int iParam2 = 0;

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return 0;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    /* compatibility with previous version manages int32 and double */
    if (!(isDoubleType(pvApiCtx, piAddressVarOne) || isIntegerType(pvApiCtx, piAddressVarOne)))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), fname, 1);
        return 0;
    }

    if (!(isDoubleType(pvApiCtx, piAddressVarTwo) || isIntegerType(pvApiCtx, piAddressVarTwo)))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), fname, 2);
        return 0;
    }

    if (!isScalar(pvApiCtx, piAddressVarOne))
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), fname, 1);
        return 0;
    }

    if (!isScalar(pvApiCtx, piAddressVarTwo))
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), fname, 2);
        return 0;
    }

    if (isIntegerType(pvApiCtx, piAddressVarOne))
    {
        if (getScalarInteger32(pvApiCtx, piAddressVarOne, &iParam1) != 0)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), fname, 1);
            return 0;
        }
    }
    else // double
    {
        double dParam1 = 0.;
        if (getScalarDouble(pvApiCtx, piAddressVarOne, &dParam1) == 0)
        {
            iParam1 = (int)dParam1;
        }
    }

    if (isIntegerType(pvApiCtx, piAddressVarTwo))
    {
        if (getScalarInteger32(pvApiCtx, piAddressVarTwo, &iParam2) != 0)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), fname, 2);
            return 0;
        }
    }
    else // double
    {
        double dParam2 = 0.;
        if (getScalarDouble(pvApiCtx, piAddressVarTwo, &dParam2) == 0)
        {
            iParam2 = (int)dParam2;
        }
    }


    if (iParam1 == -1)
    {
        scilinesdefault();
        if (getScilabMode() == SCILAB_STD)
        {
            ScilabLinesUpdate();
        }
    }
    else
    {
        if (iParam1 < 0)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: A scalar (>= 0) expected.\n"), fname, 1);
            return 0;
        }
        else
        {
            setLinesSize(iParam1);
        }
    }

    if (iParam2 > 0)
    {
        setColumnsSize(iParam2);
    }
    else
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: A scalar (>= 0) expected.\n"), fname, 2);
        return 0;
    }

    LhsVar(1) = 0;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
