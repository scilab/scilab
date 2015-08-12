/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) Scilab Enterprises - 2013 - Simon MARCHETTO
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

#include <stdio.h>
#include "api_scilab.h"
#include "api_int.h"

/*--------------------------------------------------------------------------*/
int sci_addErrorMessage(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int *piAddressVarOne = NULL;
    int errorMessageCount = 0;
    sciErr.iMsgCount = 0;

    CheckInputArgument(pvApiCtx, 1, 1);
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (!sciErr.iErr)
    {
        if (isIntegerType(pvApiCtx, piAddressVarOne))
        {
            if (!getScalarInteger32(pvApiCtx, piAddressVarOne,
                                    &errorMessageCount))
            {
                ;
            }
            {
                SciErr sciErr;
                char errMsg[100];
                int i = 0;
                sciErr.iMsgCount = 0;

                for (i = 1; i <= errorMessageCount; i++)
                {
                    sprintf(errMsg, "Error %d", i);
                    addErrorMessage(&sciErr, 999, errMsg);
                }
                printError(&sciErr, 0);

                return -1;
            }
        }
    }
    return -1;
}
/*--------------------------------------------------------------------------*/
