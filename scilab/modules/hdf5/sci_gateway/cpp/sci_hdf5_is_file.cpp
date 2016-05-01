/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2012 - DIGITEO - Antoine ELIAS
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
#include "sci_malloc.h"
}


int sci_is_hdf5_file(char *fname, int* pvApiCtx)
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
        Scierror(999, _("%s: Wrong size for input argument #%d: string expected.\n"), fname, 1);
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
