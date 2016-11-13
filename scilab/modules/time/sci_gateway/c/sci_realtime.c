/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA -
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
#include "gw_time.h"
#include "realtime.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int sci_realtimeinit(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int m1 = 0;
    int n1 = 0;
    int * p1_in_address = NULL;
    int iType = 0;
    double * pDblReal = NULL;
    double zer = 0.0;

    CheckRhs(1, 1);
    CheckLhs(1, 1);

    /*  checking variable scale */
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &p1_in_address);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    sciErr = getVarType(pvApiCtx, p1_in_address, &iType);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (iType != sci_matrix)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real scalar expected.\n"), fname, 1);
        return 1;
    }

    if (isScalar(pvApiCtx, p1_in_address) == 0)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real scalar expected.\n"), fname, 1);
        return 1;
    }

    sciErr = getMatrixOfDouble(pvApiCtx, p1_in_address, &m1, &n1, &pDblReal);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    /* cross variable size checking */
    C2F(realtimeinit)(&zer, pDblReal);

    LhsVar(1) = 0;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
int sci_realtime(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int m1 = 0;
    int n1 = 0;
    int * p1_in_address = NULL;
    int iType = 0;
    double * pDblReal = NULL;

    CheckRhs(1, 1);
    CheckLhs(1, 1);

    /*  checking variable t */
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &p1_in_address);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    sciErr = getVarType(pvApiCtx, p1_in_address, &iType);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (iType != sci_matrix)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real scalar expected.\n"), fname, 1);
        return 1;
    }

    if (isScalar(pvApiCtx, p1_in_address) == 0)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real scalar expected.\n"), fname, 1);
        return 1;
    }

    sciErr = getMatrixOfDouble(pvApiCtx, p1_in_address, &m1, &n1, &pDblReal);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    /* cross variable size checking */
    C2F(realtime)(pDblReal);

    LhsVar(1) = 0;
    PutLhsVar();

    return 0;
}
/*--------------------------------------------------------------------------*/
