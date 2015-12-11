/* ==================================================================== */
/* Template toolbox_skeleton */
/* This file is released under the 3-clause BSD license. See COPYING-BSD. */
/* ==================================================================== */
#include "api_scilab.h"
#include "Scierror.h"
#include "sci_malloc.h"
#include "csub.h"
#include <localization.h>

/* ==================================================================== */
int sci_csub(char *fname, void* pvApiCtx)
{
    SciErr sciErr;

    int m1 = 0, n1 = 0;
    int *piAddressVarOne = NULL;
    double *pdVarOne = NULL;
    int iType1 = 0;

    int m2 = 0, n2 = 0;
    int *piAddressVarTwo = NULL;
    double *pdVarTwo = NULL;
    int iType2 = 0;

    int m_out = 0, n_out = 0;
    double dOut = 0.0;

    /* --> result = csub(3,8)
    /* check that we have only 2 input arguments */
    /* check that we have only 1 output argument */
    CheckInputArgument(pvApiCtx, 2, 2);
    CheckOutputArgument(pvApiCtx, 1, 1);

    /* get Address of inputs */
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    /* check input type */
    sciErr = getVarType(pvApiCtx, piAddressVarOne, &iType1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if ( iType1 != sci_matrix )
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), fname, 1);
        return 0;
    }

    sciErr = getVarType(pvApiCtx, piAddressVarTwo, &iType2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if ( iType2 != sci_matrix )
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), fname, 2);
        return 0;
    }

    /* get matrix */
    sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarOne, &m1, &n1, &pdVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarTwo, &m2, &n2, &pdVarTwo);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    /* check size */
    if ( (m1 != n1) && (n1 != 1) )
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), fname, 1);
        return 0;
    }
    if ( (m2 != n2) && (n2 != 1) )
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), fname, 2);
        return 0;
    }

    /* call c function csub */
    csub(&pdVarOne[0], &pdVarTwo[0], &dOut);

    /* create result on stack */
    m_out = 1;
    n_out = 1;
    createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, m_out, n_out, &dOut);

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;

    ReturnArguments(pvApiCtx);

    return 0;
}
/* ==================================================================== */

