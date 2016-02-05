/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) Scilab Enterprises - 2013 - Simon MARCHETTO
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
*
*/

#include <stdio.h>
#include "api_scilab.h"
#include "api_int.h"

/*--------------------------------------------------------------------------*/
int sci_addErrorMessage(char *fname, void* pvApiCtx)
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
