/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET 
 * Copyright (C) 2011-2012 - DIGITEO - Allan CORNET 
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
#include "scilabmode.h"
/*--------------------------------------------------------------------------*/
static int sci_lines_no_rhs(char *fname);
static int sci_lines_one_rhs(char *fname);
static int sci_lines_two_rhs(char *fname);
/*--------------------------------------------------------------------------*/
int sci_lines(char *fname,unsigned long fname_len)
{
    CheckRhs(0,2);
    CheckLhs(1,1);

    switch(Rhs)
    {
    case 0:
        sci_lines_no_rhs(fname);
        break;
    case 1:
        sci_lines_one_rhs(fname);
        break;
    case 2:
        sci_lines_two_rhs(fname);
        break;
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
static int sci_lines_no_rhs(char *fname)
{
    /* ncl = lines() */
    /* with ncl a 1x2 vector [nc , nl] */
    /* for compatibility BUT order should be [nl, nc] */
    /* input/output not coherents */
    SciErr sciErr;
    double returnedDouble[2];
    int n1 = 0,m1 = 0;

    returnedDouble[0] = (double)getColumnsSize();
    returnedDouble[1] = (double)getLinesSize();

    n1 = 1; m1 = 2;
    sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, n1, m1, returnedDouble);

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999,_("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();

    return 0;
}
/*--------------------------------------------------------------------------*/
static int sci_lines_one_rhs(char *fname)
{
    /* lines(nb_lines) */
    /* nb_lines == -1 auto */
    /* nb_lines == 0 no stop on display */
    /* nb_lines > 0 set value */
    SciErr sciErr;
    int *piAddressVarOne = NULL;

    int isScalarInput = 0;

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    /* compatibility with previous version manages int32 and double */
    if (!(isDoubleType(pvApiCtx, piAddressVarOne) || isIntegerType(pvApiCtx, piAddressVarOne)))
    {
        Scierror(999,_("%s: Wrong type for input argument #%d: A scalar expected.\n"),fname, 1);
        return 0;
    }

    isScalarInput = isScalar(pvApiCtx, piAddressVarOne);

    if (!isScalarInput && !checkVarDimension(pvApiCtx, piAddressVarOne, 1, 2))
    {
        Scierror(999,_("%s: Wrong size for input argument #%d: A scalar expected.\n"),fname, 1);
        return 0;
    }

    if (isScalarInput)
    {
        int iParam1 = 0;
        if (isIntegerType(pvApiCtx, piAddressVarOne))
        {
            if (getScalarInteger32(pvApiCtx, piAddressVarOne, &iParam1) != 0)
            {
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                return 0;
            }
        }
        else // double
        {
            double dParam1 = 0.;
            if (getScalarDouble(pvApiCtx,piAddressVarOne, &dParam1) == 0)
            {
                iParam1 = (int)dParam1;
                if (dParam1 != (double)iParam1)
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d: An integer value expected.\n"), fname, 1);
                    return 0;
                }
            }
            else
            {
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                return 0;
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
                Scierror(999,_("%s: Wrong value for input argument #%d: A scalar (>= 0) expected.\n"),fname, 1);
                return 0;
            }
            else
            {
                setLinesSize(iParam1);
            }
        }
    }
    else
    {
        int iLinesValues[2];

        if (isIntegerType(pvApiCtx, piAddressVarOne))
        {
            int *iParams1 = NULL;
            int m = 0, n = 0;
            sciErr = getMatrixOfInteger32(pvApiCtx,piAddressVarOne, &m, &n, &iParams1);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                return 0;
            }
            iLinesValues[0] = iParams1[0];
            iLinesValues[1] = iParams1[1];
        }
        else // double
        {
            double *dParams1 = NULL;
            int m = 0, n = 0;

            sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarOne, &m, &n, &dParams1);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                return 0;
            }

            iLinesValues[0] = (int)dParams1[0];
            iLinesValues[1] = (int)dParams1[1];

            if ((dParams1[0] != (double)iLinesValues[0]) ||
                (dParams1[1] != (double)iLinesValues[1]))
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: An integer value expected.\n"), fname, 1);
                return 0;
            }
        }

        if ((iLinesValues[0] < 0) || (iLinesValues[1] < 0))
        {
            Scierror(999,_("%s: Wrong value for input argument #%d.\n"), fname, 1);
            return 0;
        }

        scilines(iLinesValues[1], iLinesValues[0]);
    }

    LhsVar(1) = 0;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
static int sci_lines_two_rhs(char *fname)
{
    /* lines (nl, nc) */
    SciErr sciErr;
    int *piAddressVarOne = NULL;
    int *piAddressVarTwo = NULL;

    int iParam1 = 0;
    int iParam2 = 0;

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return 0;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    /* compatibility with previous version manages int32 and double */
    if (!(isDoubleType(pvApiCtx, piAddressVarOne) || isIntegerType(pvApiCtx, piAddressVarOne)))
    {
        Scierror(999,_("%s: Wrong type for input argument #%d: A scalar expected.\n"),fname, 1);
        return 0;
    }

    if (!(isDoubleType(pvApiCtx, piAddressVarTwo) || isIntegerType(pvApiCtx, piAddressVarTwo)))
    {
        Scierror(999,_("%s: Wrong type for input argument #%d: A scalar expected.\n"),fname, 2);
        return 0;
    }

    if (!isScalar(pvApiCtx, piAddressVarOne))
    {
        Scierror(999,_("%s: Wrong size for input argument #%d: A scalar expected.\n"),fname, 1);
        return 0;
    }

    if (!isScalar(pvApiCtx, piAddressVarTwo))
    {
        Scierror(999,_("%s: Wrong size for input argument #%d: A scalar expected.\n"),fname, 2);
        return 0;
    }

    if (isIntegerType(pvApiCtx, piAddressVarOne))
    {
        if (getScalarInteger32(pvApiCtx, piAddressVarOne, &iParam1) != 0)
        {
            Scierror(999,_("%s: Wrong type for input argument #%d: A scalar expected.\n"),fname, 1);
            return 0;
        }
    }
    else // double
    {
        double dParam1 = 0.;
        if (getScalarDouble(pvApiCtx,piAddressVarOne, &dParam1) == 0)
        {
            iParam1 = (int)dParam1;
        }
    }

    if (isIntegerType(pvApiCtx, piAddressVarTwo))
    {
        if (getScalarInteger32(pvApiCtx, piAddressVarTwo, &iParam2) != 0)
        {
            Scierror(999,_("%s: Wrong type for input argument #%d: A scalar expected.\n"),fname, 2);
            return 0;
        }
    }
    else // double
    {
        double dParam2 = 0.;
        if (getScalarDouble(pvApiCtx,piAddressVarTwo, &dParam2) == 0)
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
            Scierror(999,_("%s: Wrong value for input argument #%d: A scalar (>= 0) expected.\n"),fname, 1);
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
        Scierror(999,_("%s: Wrong value for input argument #%d: A scalar (>= 0) expected.\n"),fname, 2);
        return 0;
    }

    LhsVar(1) = 0;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
