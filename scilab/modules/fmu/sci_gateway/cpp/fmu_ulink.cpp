//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2013 - Scilab Enterprises - Clement David
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//

#include <stdlib.h>

extern "C" {
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "gw_fmu.h"
}

#include "fmu_wrapper.hxx"

int fmu_ulink(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int *piAddressInstance;
    FmuWrapper* pInstance;

    if (!checkOutputArgument(pvApiCtx, 0, 1))
    {
        return -1;
    }

    /*
     * Retrieve 1 arguments: pointer to an instance
     */
    if (!checkInputArgument(pvApiCtx, 1, 1))
    {
        return -1;
    }
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressInstance);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    sciErr =  getPointer(pvApiCtx, piAddressInstance, (void**) &pInstance);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    // virtual destructor will call the right sub-class one.
    delete pInstance;

    return 0;
}
