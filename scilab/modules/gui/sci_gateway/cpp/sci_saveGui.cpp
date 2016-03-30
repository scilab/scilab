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
#include "BOOL.h"
#include "expandPathVariable.h"
}

int sci_saveGui(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int* piAddr1 = NULL;
    long long hFig = 0;
    int iFig = 0;

    int iType = 0;
    int* piType = &iType;
    int* piAddr2 = NULL;
    char* pstFile = NULL;
    char* pstFullFile = NULL;

    int* piAddr3 = NULL;
    int bReserve = 0;

    int iRhs = nbInputArgument(pvApiCtx);

    CheckInputArgument(pvApiCtx, 2, 2);
    CheckOutputArgument(pvApiCtx, 1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (isHandleType(pvApiCtx, piAddr1) == 0 || isScalar(pvApiCtx, piAddr1) == 0)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A single handle expected.\n"), fname, 1);
        return 1;
    }

    if (getScalarHandle(pvApiCtx, piAddr1, &hFig))
    {
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 1;
    }

    iFig = getObjectFromHandle((long)hFig);
    if (iFig == 0)
    {
        Scierror(999, _("%s: The handle is not or no more valid.\n"), fname);
        return 0;
    }

    getGraphicObjectProperty(iFig, __GO_TYPE__, jni_int, (void**)&piType);
    if (piType == NULL || iType != __GO_FIGURE__)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Figure handle expected.\n"), fname, 1);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (isStringType(pvApiCtx, piAddr2) == 0 || isScalar(pvApiCtx, piAddr2) == 0)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: string expected.\n"), fname, 2);
        return 1;
    }

    if (getAllocatedSingleString(pvApiCtx, piAddr2, &pstFile))
    {
        if (pstFile)
        {
            freeAllocatedSingleString(pstFile);
        }

        Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 2);
        return 1;
    }

    //reverse flag
    if (iRhs == 3)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        if (isBooleanType(pvApiCtx, piAddr3) == 0 || isScalar(pvApiCtx, piAddr3) == 0)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A boolean expected.\n"), fname, 3);
            return 1;
        }

        if (getScalarBoolean(pvApiCtx, piAddr3, &bReserve))
        {
            Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 3);
            return 1;
        }
    }

    pstFullFile = expandPathVariable(pstFile);

    char* ret = xmlsave(iFig, pstFullFile, (BOOL) bReserve);
    freeAllocatedSingleString(pstFile);
    freeAllocatedSingleString(pstFullFile);

    if (ret[0] != '\0')
    {
        Scierror(202, _("%s: %s.\n"), fname, ret);
        free(ret);
        return 1;
    }

    free(ret);
    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);
    return 0;
}
