#include "api_scilab.h"
#include <math.h>

extern double fun2();
extern void fun1 ( double *x, double *y);

int intfun1(char *fname, void* pvApiCtx)
{
    int rows, cols;
    SciErr sciErr;
    int* piAddr1 = NULL;
    double* pdblReal	= NULL;
    double* pdblOut	= NULL;

    CheckRhs(1, 1);
    CheckLhs(1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getVarDimension(pvApiCtx, piAddr1, &rows, &cols);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr1, &rows, &cols, &pdblReal);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = allocMatrixOfDouble(pvApiCtx, Rhs + 1, rows, cols, &pdblOut);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    fun1(pdblReal, pdblOut);

    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}

void fun1(double *x, double *y)
{
    *y = fun2(*x) / (*x);
}

double fun2(double x)
{
    return ( sin(x + 1.));
}
