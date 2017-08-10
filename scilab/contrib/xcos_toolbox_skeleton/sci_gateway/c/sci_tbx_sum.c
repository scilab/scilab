/* This file is released under the 3-clause BSD license. See COPYING-BSD. */

#include "business.h"
#include "api_scilab.h"
#include "Scierror.h"

int sci_tbx_sum(char *fname, void* pvApiCtx)
{
    SciErr sciErr;

    int *piAddressVarOne = NULL;
    double dVarOne = 0.0;

    int *piAddressVarTwo = NULL;
    double dVarTwo = 0.0;

    double dOut = 0.0;

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
    if ( !isDoubleType(pvApiCtx, piAddressVarOne) )
    {
        Scierror(999, "%s: Wrong type for input argument #%d: A scalar expected.\n", fname, 1);
        return 0;
    }

    if ( !isDoubleType(pvApiCtx, piAddressVarTwo) )
    {
        Scierror(999, "%s: Wrong type for input argument #%d: A scalar expected.\n", fname, 2);
        return 0;
    }

    if ( getScalarDouble(pvApiCtx, piAddressVarOne, &dVarOne) )
    {
        Scierror(999, "%s: Wrong size for input argument #%d: A scalar expected.\n", fname, 1);
        return 0;
    }

    if ( getScalarDouble(pvApiCtx, piAddressVarTwo, &dVarTwo) )
    {
        Scierror(999, "%s: Wrong size for input argument #%d: A scalar expected.\n", fname, 2);
        return 0;
    }

    /* call c business function */
    dOut = business_sum(dVarOne, dVarTwo);

    createScalarDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, dOut);
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
}

