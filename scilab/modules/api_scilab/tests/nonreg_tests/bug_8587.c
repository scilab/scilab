/* ========================================================================== */
/* bug 8587 */
/* Allan CORNET - 2010 - DIGITEO */
/* ========================================================================== */
#include <api_scilab.h>
#include <BOOL.h>
#include <Scierror.h>
#include <sci_malloc.h>
/* ========================================================================== */
int sci_readDoubles(char *fname, void* pvApiCtx)
{
#define NAME_MATRIX_TO_READ "REF_TEST_MATRIX"
#define REF_SIZE_COLS 3
#define REF_SIZE_ROWS 2
    SciErr sciErr;
    int nbRow = 0;
    int nbCol = 0;
    double * matrixOfDouble = NULL;

    sciErr = readNamedMatrixOfDouble(pvApiCtx, NAME_MATRIX_TO_READ, &nbRow, &nbCol , NULL);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
    }

    if ((nbRow != REF_SIZE_ROWS) || (nbCol != REF_SIZE_COLS))
    {
        Scierror(999, "%s: Wrong size for input argument #%s.\n", fname, NAME_MATRIX_TO_READ);
        return 0;
    }

    matrixOfDouble = (double*)MALLOC((nbRow * nbCol) * sizeof(double));

    sciErr = readNamedMatrixOfDouble(pvApiCtx, NAME_MATRIX_TO_READ, &nbRow, &nbCol, matrixOfDouble);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
    }

    sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, nbRow, nbCol, matrixOfDouble);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
    }

    FREE(matrixOfDouble);
    LhsVar(1) = Rhs + 1;
    return 0;
}
/* ========================================================================== */
