#include <string>
extern "C" {
#include "api_scilab.h"
#include "Scierror.h"
#include <stdio.h>


    int sci_cppfind(char *fname, void *pvApiCtx)
    {
        char *inputString1 = NULL, *inputString2 = NULL;
        int rowsOut = 0, colsOut = 0, err = 0;
        SciErr sciErr;

        int* piAddr1 = NULL;
        int* piAddr2 = NULL;

        double *position = NULL; /* Where we will store the position */

        CheckRhs(2, 2); /* Check the number of input argument */
        CheckLhs(1, 1); /* Check the number of output argument */

        //get string
        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
        err = getAllocatedSingleString(pvApiCtx, piAddr1, &inputString1);
        if (err != 0)
        {
            SciError(err);
            return 0;
        }

        //get substring to find
        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
        err = getAllocatedSingleString(pvApiCtx, piAddr2, &inputString2);
        if (err != 0)
        {
            SciError(err);
            return 0;
        }

        std::string myMessage (inputString1);
        std::string search (inputString2);

        //allocate result variable
        rowsOut = 1;
        colsOut = 1;
        sciErr = allocMatrixOfDouble(pvApiCtx, Rhs + 1, rowsOut, colsOut, &position);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        //process
        if (myMessage.find(search) != std::string::npos)
        {
            *position = myMessage.find(search); /* The actual operation */
        }
        else
        {
            *position = -1; /* Substring not found */
        }

        LhsVar(1) = Rhs + 1;
        PutLhsVar();
        return 0;
    }
}
