/* ==================================================================== */
/* Template toolbox_skeleton */
/* Example detail in "API_scilab getting started" help page */
/* This file is released under the 3-clause BSD license. See COPYING-BSD. */
/* ==================================================================== */
#include "api_scilab.h"
#include "BOOL.h"
#include <localization.h>

/* ==================================================================== */
int sci_foo(char *fname, void* pvApiCtx)
{
    // Error management variable
    SciErr sciErr;

    ////////// Variables declaration //////////
    int m1 = 0, n1 = 0;
    int *piAddressVarOne = NULL;
    double *matrixOfDouble = NULL;
    double *newMatrixOfDouble = NULL;

    int m2 = 0, n2 = 0;
    int *piAddressVarTwo = NULL;
    int *matrixOfBoolean = NULL;
    int *newMatrixOfBoolean = NULL;
    int i = 0;


    ////////// Check the number of input and output arguments //////////
    /* --> [c, d] = foo(a, b) */
    /* check that we have only 2 input arguments */
    /* check that we have only 2 output argument */
    CheckInputArgument(pvApiCtx, 2, 2) ;
    CheckOutputArgument(pvApiCtx, 2, 2) ;


    ////////// Manage the first input argument (double) //////////
    /* get Address of inputs */
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    /* Check that the first input argument is a real matrix (and not complex) */
    if ( !isDoubleType(pvApiCtx, piAddressVarOne) ||  isVarComplex(pvApiCtx, piAddressVarOne) )
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), fname, 1);
        return 0;
    }

    /* get matrix */
    sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarOne, &m1, &n1, &matrixOfDouble);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    ////////// Manage the second input argument (boolean) //////////

    /* get Address of inputs */
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if ( !isBooleanType(pvApiCtx, piAddressVarTwo) )
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A boolean matrix expected.\n"), fname, 2);
        return 0;
    }

    /* get matrix */
    sciErr = getMatrixOfBoolean(pvApiCtx, piAddressVarTwo, &m2, &n2, &matrixOfBoolean);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    ////////// Check the consistency of the two input arguments //////////

    if ((m1 != m2) | - (n1 != n2))
    {
        Scierror(999, _("%s: Wrong size for input arguments: Same size expected.\n"), fname, 1);
        return 0;
    }


    newMatrixOfDouble = (double*)malloc(sizeof(double) * m1 * n1);
    ////////// Application code //////////
    // Could be replaced by a call to a library

    for (i = 0; i < m1 * n1; i++)
    {
        /* For each element of the matrix, multiply by 2 */
        newMatrixOfDouble[i] = matrixOfDouble[i] * 2;
    }

    newMatrixOfBoolean = (int*)malloc(sizeof(double) * m2 * n2);
    for (i = 0; i < m2 * n2; i++)
    {
        /* For each element of the matrix, invert the value */
        newMatrixOfBoolean[i] = matrixOfBoolean[i] == TRUE ? FALSE : TRUE;
    }

    ////////// Create the output arguments //////////

    /* Create the matrix as return of the function */
    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, m1, n1, newMatrixOfDouble);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    /* Create the matrix as return of the function */
    sciErr = createMatrixOfBoolean(pvApiCtx, nbInputArgument(pvApiCtx) + 2, m2, n2, newMatrixOfBoolean);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    ////////// Return the output arguments to the Scilab engine //////////

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx) + 2;

    ReturnArguments(pvApiCtx);

    return 0;
}
/* ==================================================================== */

