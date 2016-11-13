/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
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

extern "C" {
#include "gw_gui.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "createGraphicObject.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "HandleManagement.h"
#include "FileExist.h"
#include "expandPathVariable.h"
}

int sci_loadGui(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int* piAddr = NULL;
    char* pstFile = NULL;
    char* pstFullFile = NULL;

    int iRhs = nbInputArgument(pvApiCtx);

    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (isStringType(pvApiCtx, piAddr) == 0 || isScalar(pvApiCtx, piAddr) == 0)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: string expected.\n"), fname, 1);
        return 1;
    }


    if (getAllocatedSingleString(pvApiCtx, piAddr, &pstFile))
    {
        if (pstFile)
        {
            freeAllocatedSingleString(pstFile);
        }

        Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 1);
        return 1;
    }

    pstFullFile = expandPathVariable(pstFile);
    if (!FileExist(pstFullFile))
    {
        Scierror(999, _("%s: This file %s does not exist.\n"), fname, pstFile);
        freeAllocatedSingleString(pstFile);
        freeAllocatedSingleString(pstFullFile);
        return 0;
    }

    int iFig = xmldomload(pstFullFile);
    if (iFig < 1)
    {
        Scierror(999, _("%s: can not read file %s.\n"), fname, pstFile);
        freeAllocatedSingleString(pstFile);
        freeAllocatedSingleString(pstFullFile);
        return 0;
    }

    freeAllocatedSingleString(pstFile);

    long long h = (long long)getHandle(iFig);
    createScalarHandle(pvApiCtx, iRhs + 1, h);
    AssignOutputVariable(pvApiCtx, 1) = iRhs + 1;
    ReturnArguments(pvApiCtx);
    return 0;
}
