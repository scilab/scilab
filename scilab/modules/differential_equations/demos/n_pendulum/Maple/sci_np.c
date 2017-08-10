#include "api_scilab.h"
#include "Scierror.h"
#include "sci_malloc.h"
#include <localization.h>

extern int F2C(np)(int *a);

int sci_np(char *fname, void* pvApiCtx)
{
    SciErr sciErr;

    int m_out = 1, n_out = 1;
    int iOut = 0;

    CheckInputArgument(pvApiCtx, 0, 0);
    CheckOutputArgument(pvApiCtx, 0, 1);

    F2C(np)(&iOut);

    sciErr = createMatrixOfInteger32(pvApiCtx, nbInputArgument(pvApiCtx) + 1, m_out, n_out, &iOut);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;

    return 0;
}



