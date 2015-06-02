/* ============================================== */
#include "api_scilab.h"
#include "sciprint.h"
/* ============================================== */
/*  Interface for c_fun fonction */
/* ============================================== */
void c_fun(double *b, double *c, double *a);
/* ============================================== */
int c_intfun(char *fname, void * pvApiCtx)
{
    int rows1 = 0, cols1 = 0, rows2 = 0, cols2 = 0, rows3 = 1, cols3 = 1;
    int minlhs = 1, maxlhs = 1, minrhs = 2, maxrhs = 2;

    double *pdbl1 = NULL, *pdbl2 = NULL, *pdblOut = NULL;
    int* piAddr1 = NULL;
    int* piAddr2 = NULL;
    int* piAddr3 = NULL;

    SciErr sciErr;


    /* Check number of inputs (rhs=2) and outputs (lhs=1) */
    CheckRhs(minrhs, maxrhs) ;
    CheckLhs(minlhs, maxlhs) ;

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
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr1, &rows1, &cols1, &pdbl1);
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
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &rows2, &cols2, &pdbl2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }


    sciErr = allocMatrixOfDouble(pvApiCtx, Rhs + 1, rows3, cols3, &pdblOut);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }


    /* Check dimensions  */
    if (!(rows1 == 1) | !(cols1 == 1))
    {
        sciprint("%s: Wrong inputs \r\n", "c_fun");
        SciError(999);
        return 0;
    }
    if (!(rows2 == 1) | !(cols2 == 1))
    {
        sciprint("%s: Wrong inputs \r\n", "c_fun");
        SciError(999);
        return 0;
    }

    /* Call c_fun */
    c_fun(pdbl1, pdbl2, pdblOut);

    /*  Return result  */
    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}
/* ============================================== */
void c_fun(double *b, double *c, double *a)
{
    *a = *b + *c;
}
/* ============================================== */
