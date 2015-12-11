//===================================================
// Allan CORNET - INRIA - 2008
// dummy test
//===================================================
#include <math.h>
#include "api_scilab.h"
//===================================================
int interf_template_A0(char *fname, void * pvApiCtx)
{
    int rows = 1, cols = 1;
    int iValue = 0;
    int *piInt = NULL;

    int* piAddr = NULL;
    SciErr sciErr;

    CheckRhs(0, 0);
    CheckLhs(1, 1);


    sciErr = allocMatrixOfInteger32(pvApiCtx, 1, rows, cols, &piInt);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    *piInt = iValue;
    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}
//===================================================
int interf_template_B0(char *fname, void * pvApiCtx)
{
    int rows = 1, cols = 1;
    int iValue = 0;
    int *piInt = NULL;

    int* piAddr = NULL;
    SciErr sciErr;

    CheckRhs(0, 0);
    CheckLhs(1, 1);

    sciErr = allocMatrixOfInteger32(pvApiCtx, Rhs + 1, rows, cols, &piInt);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    *piInt = iValue;
    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}
//===================================================

