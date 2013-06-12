#include <math.h>
#include "api_scilab.h"
#include "api_oldstack.h"

static double fun2(double x);

void fun1(double *x, double *y)
{
    *y = fun2(*x) / (*x);
}

static double fun2(double x)
{
    return( sin(x + 1));
}

int intfun1(char *fname, int* _piKey)
{
    SciErr sciErr;
    int* piAddr = NULL;
    double dblIn = 0;
    double dblOut = 0;
    CheckRhs(1, 1);
    CheckLhs(1, 1);

    sciErr = getVarAddressFromPosition(_piKey, 1, &piAddr);
    if (sciErr.iErr != 0)
    {
        //error
        return 1;
    }

    getScalarDouble(_piKey, piAddr, &dblIn);
    fun1(&dblIn, &dblOut);

    createScalarDouble(_piKey, Rhs + 1, dblOut);
    LhsVar(1) = Rhs + 1;
    return 0;
}


