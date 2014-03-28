/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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

int sci_loadGui(char *fname, unsigned long fname_len)
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
        Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 1);
        return 1;
    }


    if (getAllocatedSingleString(pvApiCtx, piAddr, &pstFile))
    {
        if (pstFile)
        {
            freeAllocatedSingleString(pstFile);
        }

        Scierror(202, _("%s: Wrong type for argument #%d: A string expected.\n"), fname, 1);
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