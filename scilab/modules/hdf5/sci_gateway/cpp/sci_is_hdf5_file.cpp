/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2012 - DIGITEO - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

#include <hdf5.h>

extern "C"
{
#include "gw_hdf5.h"
#include "Scierror.h"
#include "localization.h"
#include "api_scilab.h"
#include "../../../call_scilab/includes/call_scilab.h"
#include "h5_fileManagement.h"
#include "sciprint.h"
#include "expandPathVariable.h"
#include "MALLOC.h"
}


int sci_is_hdf5_file(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int *piAddr     = NULL;
    char* pstFile   = NULL;
    const int nbIn = nbInputArgument(pvApiCtx);

    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 1, 4);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (getAllocatedSingleString(pvApiCtx, piAddr, &pstFile))
    {
        freeAllocatedSingleString(pstFile);
        Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 1);
        return 1;
    }

    char* pstFileName = expandPathVariable(pstFile);
    if (isHDF5File(pstFileName))
    {
        createScalarBoolean(pvApiCtx, nbIn + 1, 1);
    }
    else
    {
        createScalarBoolean(pvApiCtx, nbIn + 1, 0);
    }

    FREE(pstFileName);
    freeAllocatedSingleString(pstFile);
    AssignOutputVariable(pvApiCtx, 1) = nbIn + 1;
    ReturnArguments(pvApiCtx);
    return 0;
}
