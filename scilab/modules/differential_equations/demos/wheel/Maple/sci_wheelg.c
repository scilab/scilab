#include "api_scilab.h"
#include "Scierror.h"
#include "sci_malloc.h"
#include <localization.h>
#include "sciprint.h"

extern int F2C(wheelg)(int *n, int *k, double *uf, double *vf, double *wf, double *xx);

int sci_wheelg(char *fname, void* pvApiCtx)
{
    SciErr sciErr;

    int *piAddressVarOne = NULL;
    int m1 = 0, n1 = 0;
    int *piN = NULL;

    int *piAddressVarTwo = NULL;
    int m2 = 0, n2 = 0;
    int *piK = NULL;

    int *piAddressVarThree = NULL;
    int m3 = 0, n3 = 0;
    double *pdUF = NULL;

    int *piAddressVarFour = NULL;
    int m4 = 0, n4 = 0;
    double *pdVF = NULL;

    int *piAddressVarFive = NULL;
    int m5 = 0, n5 = 0;
    double *pdWF = NULL;

    int *piAddressVarSix = NULL;
    int m6 = 0, n6 = 0;
    double *pdXX = NULL;

    CheckInputArgument(pvApiCtx, 1, 6);
    CheckOutputArgument(pvApiCtx, 0, 3);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddressVarOne, &m1, &n1, &piN);
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

    sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddressVarTwo, &m2, &n2, &piK);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddressVarThree);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarThree, &m3, &n3, &pdUF);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddressVarFour);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarFour, &m4, &n4, &pdVF);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 5, &piAddressVarFive);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarFive, &m5, &n5, &pdWF);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 6, &piAddressVarSix);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarSix, &m6, &n6, &pdXX);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    F2C(wheelg)(piN, piK, pdUF, pdVF, pdWF, pdXX);

    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, m3, n3, pdUF);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 2, m4, n4, pdVF);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 3, m5, n5, pdWF);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx) + 2;
    AssignOutputVariable(pvApiCtx, 3) = nbInputArgument(pvApiCtx) + 3;

    ReturnArguments(pvApiCtx);
    return 0;
}
