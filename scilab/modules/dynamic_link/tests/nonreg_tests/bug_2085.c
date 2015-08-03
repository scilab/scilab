/* ============================================== */
/* Allan CORNET - bug 2085 */
/* ============================================== */
#include <string.h>
#include <stdio.h>
#include "api_scilab.h"
/* ============================================== */
static void f99(double *ar, double *ac, int *ita, int *ma, int *na);
/* ============================================== */
int intex2c(char *fname, void * pvApiCtx)
{
    static int cplx, rows, cols;
    int minlhs = 1, minrhs = 1, maxlhs = 1, maxrhs = 1;
    double *pdblReal = NULL, *pdblImag = NULL;
    int* piAddr1 = NULL;

    SciErr sciErr;

    CheckRhs(minrhs, maxrhs) ;
    CheckLhs(minlhs, maxlhs) ;

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
    cplx = isVarComplex(pvApiCtx, piAddr1);
    sciErr = getComplexMatrixOfDouble(pvApiCtx, piAddr1, &rows, &cols, &pdblReal, &pdblImag);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    f99(pdblReal, pdblImag, &cplx, &rows, &cols);

    LhsVar(1) = 1;
    PutLhsVar();
    return 0;
}
/* ============================================== */
static void f99(double *ar, double *ac, int *ita, int *ma, int *na)
{
    int i = 0;
    for ( i = 0 ; i < (*ma) * (*na) ; i++)
    {
        ar[i] = 2 * ar[i] ;
    }
    if ( *ita == 1)
    {
        for ( i = 0 ; i < (*ma) * (*na) ; i++)
        {
            ac[i] = 3 * ac[i] ;
        }
    }
}
/* ============================================== */
