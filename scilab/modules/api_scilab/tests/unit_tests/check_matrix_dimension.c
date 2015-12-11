#include "api_scilab.h"
#include "sciprint.h"

int check_matrix_dimension(char * fname, void *pvApiCtx)
{
    SciErr sciErr;
    int* piAddr = NULL;
    int iRows = 0;
    int iCols = 0;

    CheckInputArgument(pvApiCtx, 1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    sciErr = getVarDimension(pvApiCtx, piAddr, &iRows, &iCols);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (checkVarDimension(pvApiCtx, piAddr, iCols, iRows))
    {
        sciprint("checkVarDimension returns false\n");
    }
    else
    {
        sciprint("checkVarDimension returns true\n");
    }

    if (isRowVector(pvApiCtx, piAddr))
    {
        sciprint("isRowVector\n");
    }

    if (isColumnVector(pvApiCtx, piAddr))
    {
        sciprint("isColumnVector\n");
    }

    if (isVector(pvApiCtx, piAddr))
    {
        sciprint("isVector\n");
    }

    if (isScalar(pvApiCtx, piAddr))
    {
        sciprint("isScalar\n");
    }

    if (isSquareMatrix(pvApiCtx, piAddr))
    {
        sciprint("isSquareMatrix\n");
    }

    if (isEmptyMatrix(pvApiCtx, piAddr))
    {
        sciprint("isEmptyMatrix\n");
    }

    //no return value
    AssignOutputVariable(pvApiCtx, 0) = 0;
    return 0;
}
