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
#include "stack-c.h"
#include "api_scilab.h"
#include "scilines.h"
#include "Scierror.h"
#include "localization.h"
#include "MALLOC.h"
#include "gw_console.h"
#include "ScilabLinesUpdate.h"
#include "sci_mode.h"
/*--------------------------------------------------------------------------*/
static int sci_lines_no_rhs(char *fname, int *_piKey);
static int sci_lines_one_rhs(char *fname, int *_piKey);
static int sci_lines_two_rhs(char *fname, int *_piKey);
/*--------------------------------------------------------------------------*/
int sci_lines(char *fname, int *_piKey)
{
    CheckRhs(0,2);
    CheckLhs(1,1);

    switch(Rhs)
    {
    case 0:
        sci_lines_no_rhs(fname, _piKey);
        break;
    case 1:
        sci_lines_one_rhs(fname, _piKey);
        break;
    case 2:
        sci_lines_two_rhs(fname, _piKey);
        break;
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
static int sci_lines_no_rhs(char *fname, int *_piKey)
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
    sciErr = createMatrixOfDouble(_piKey, Rhs + 1, n1, m1, returnedDouble);

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();

    return 0;
}
/*--------------------------------------------------------------------------*/
static int sci_lines_one_rhs(char *fname, int *_piKey)
{
    /* lines(nb_lines) */
    /* nb_lines == -1 auto */
    /* nb_lines == 0 no stop on display */
    /* nb_lines > 0 set value */
    SciErr sciErr;
    int *piAddressVarOne = NULL;
    int iParam1 = 0;

    sciErr = getVarAddressFromPosition(_piKey, 1, &piAddressVarOne);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    /* compatibility with previous version manages int32 and double */
    if (!(isDoubleType(_piKey, piAddressVarOne) || isIntegerType(_piKey, piAddressVarOne)))
    {
        Scierror(999,_("%s: Wrong type for input argument #%d: A scalar expected.\n"),fname, 1);
        return 0;
    }

    if (!isScalar(_piKey, piAddressVarOne))
    {
        Scierror(999,_("%s: Wrong size for input argument #%d: A scalar expected.\n"),fname, 1);
        return 0;
    }

    if (isIntegerType(_piKey, piAddressVarOne))
    {
        if (getScalarInteger32(_piKey, piAddressVarOne, &iParam1) != 0)
        {
            Scierror(999,_("%s: Wrong type for input argument #%d: A scalar expected.\n"),fname, 1);
            return 0;
        }
    }
    else // double
    {
        double dParam1 = 0.;
        if (getScalarDouble(_piKey,piAddressVarOne, &dParam1) == 0)
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
            Scierror(999,_("%s: Wrong value for input argument #%d: A scalar (>= 0) expected.\n"),fname, 1);
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
static int sci_lines_two_rhs(char *fname, int *_piKey)
{
    /* lines (nl, nc) */
    SciErr sciErr;
    int *piAddressVarOne = NULL;
    int *piAddressVarTwo = NULL;

    int iParam1 = 0;
    int iParam2 = 0;

    sciErr = getVarAddressFromPosition(_piKey, 2, &piAddressVarTwo);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = getVarAddressFromPosition(_piKey, 1, &piAddressVarOne);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    /* compatibility with previous version manages int32 and double */
    if (!(isDoubleType(_piKey, piAddressVarOne) || isIntegerType(_piKey, piAddressVarOne)))
    {
        Scierror(999,_("%s: Wrong type for input argument #%d: A scalar expected.\n"),fname, 1);
        return 0;
    }

    if (!(isDoubleType(_piKey, piAddressVarTwo) || isIntegerType(_piKey, piAddressVarTwo)))
    {
        Scierror(999,_("%s: Wrong type for input argument #%d: A scalar expected.\n"),fname, 2);
        return 0;
    }

    if (!isScalar(_piKey, piAddressVarOne))
    {
        Scierror(999,_("%s: Wrong size for input argument #%d: A scalar expected.\n"),fname, 1);
        return 0;
    }

    if (!isScalar(_piKey, piAddressVarTwo))
    {
        Scierror(999,_("%s: Wrong size for input argument #%d: A scalar expected.\n"),fname, 2);
        return 0;
    }

    if (isIntegerType(_piKey, piAddressVarOne))
    {
        if (getScalarInteger32(_piKey, piAddressVarOne, &iParam1) != 0)
        {
            Scierror(999,_("%s: Wrong type for input argument #%d: A scalar expected.\n"),fname, 1);
            return 0;
        }
    }
    else // double
    {
        double dParam1 = 0.;
        if (getScalarDouble(_piKey,piAddressVarOne, &dParam1) == 0)
        {
            iParam1 = (int)dParam1;
        }
    }

    if (isIntegerType(_piKey, piAddressVarTwo))
    {
        if (getScalarInteger32(_piKey, piAddressVarTwo, &iParam2) != 0)
        {
            Scierror(999,_("%s: Wrong type for input argument #%d: A scalar expected.\n"),fname, 2);
            return 0;
        }
    }
    else // double
    {
        double dParam2 = 0.;
        if (getScalarDouble(_piKey,piAddressVarTwo, &dParam2) == 0)
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
