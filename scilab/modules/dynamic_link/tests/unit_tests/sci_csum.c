#include "api_scilab.h"

extern void c_sum(double *a, double *b, double *sum);

int sci_csum(char *fname, void *pvApiCtx)
{
    int rows1 = 0, cols1 = 0;
    int rows2 = 0, cols2 = 0;
    int rows3 = 0, cols3 = 0;
    SciErr sciErr;
    int* piAddr1 = NULL;
    int* piAddr2 = NULL;
    double* pdblReal2	= NULL;
    double* pdblReal1	= NULL;
    double* pdblOut	= NULL;

    CheckRhs(1, 2);
    CheckLhs(1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getVarDimension(pvApiCtx, piAddr1, &rows1, &cols1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr1, &rows1, &cols1, &pdblReal1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getVarDimension(pvApiCtx, piAddr2, &rows2, &cols2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &rows2, &cols2, &pdblReal2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    rows3 = 1;
    cols3 = 1;
    sciErr = allocMatrixOfDouble(pvApiCtx, Rhs + 1, rows3, cols3, &pdblOut);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    c_sum(pdblReal1, pdblReal2, pdblOut);

    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}

void c_sum(double *a, double *b, double *sum)
{
    *sum = *a + *b;
}
