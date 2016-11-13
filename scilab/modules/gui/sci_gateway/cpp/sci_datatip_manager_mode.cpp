/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Pedro Arthur dos S. Souza
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

#include "DatatipManager.hxx"

extern "C"
{
#include "api_scilab.h"
#include "getScilabJavaVM.h"
#include "localization.h"
#include "Scierror.h"
#include "gw_gui.h"
#include "HandleManagement.h"
#include "CurrentFigure.h"
}

using namespace org_scilab_modules_gui_datatip;

int sci_datatip_manager_mode(char *fname, void* pvApiCtx)
{
    int iFigureUID      = NULL;

    int* piAddr         = NULL;
    int* pbValue        = NULL;
    char* pstData       = NULL;
    int iRows           = 0;
    int iCols           = 0;
    int stkAdr          = 0;
    int iType           = 0;
    int iLen            = 0;
    int iErr            = 0;
    bool enabled        = false;
    long long llHandle  = 0;

    SciErr sciErr;
    CheckInputArgument(pvApiCtx, 0, 2);
    CheckOutputArgument(pvApiCtx, 1, 1);

    if (nbInputArgument(pvApiCtx) == 0)
    {
        iFigureUID = getCurrentFigure();
        if (iFigureUID)
        {
            enabled = DatatipManager::isEnabled(getScilabJavaVM(), iFigureUID);
            DatatipManager::setEnabled(getScilabJavaVM(), iFigureUID, (!enabled));
        }
    }
    else if (nbInputArgument(pvApiCtx) == 1)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        sciErr = getVarType(pvApiCtx, piAddr, &iType);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        switch (iType)
        {
            case sci_boolean :
                sciErr = getMatrixOfBoolean(pvApiCtx, piAddr, &iRows, &iCols, &pbValue);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 1;
                }
                if (iRows * iCols != 1)
                {
                    Scierror(999, _("%s: Wrong size for input argument #%d: A boolean expected.\n"), fname, 1);
                    return 1;
                }
                iFigureUID = getCurrentFigure();
                enabled = pbValue[0] == 0 ? false : true;
                break;
            case sci_strings :
                sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, NULL, NULL);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 1;
                }
                if (iRows * iCols != 1)
                {
                    Scierror(999, _("%s: Wrong size for input argument #%d: A boolean expected.\n"), fname, 1);
                    return 1;
                }
                sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, &iLen, NULL);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 1;
                }
                pstData = (char*) malloc(sizeof(char) * (iLen + 1));
                sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, &iLen, &pstData);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    free(pstData);
                    return 1;
                }
                if (strcmp("on", pstData) == 0 || strcmp("T", pstData) == 0 || strcmp("1", pstData) == 0)
                {
                    iFigureUID = getCurrentFigure();
                    enabled = true;
                }
                else if (strcmp("off", pstData) == 0 || strcmp("F", pstData) == 0 || strcmp("0", pstData) == 0)
                {
                    iFigureUID = getCurrentFigure();
                    enabled = false;
                }
                else
                {
                    free(pstData);
                    Scierror(999, _("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"), fname, 1, "on", "off");
                    return 1;
                }
                free(pstData);
                break;
            case sci_handles :
                iErr = getScalarHandle(pvApiCtx, piAddr, &llHandle);
                if (iErr)
                {
                    Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                    return 1;
                }
                iFigureUID = getObjectFromHandle((unsigned long) llHandle);
                if (iFigureUID)
                {
                    enabled = !(DatatipManager::isEnabled(getScilabJavaVM(), iFigureUID));
                }
                break;
            default :
                Scierror(999, _("%s: Wrong type for input argument #%d: A boolean expected.\n"), fname, 1);
                return 1;
        }
    }
    else if (nbInputArgument(pvApiCtx) == 2)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        iErr = getScalarHandle(pvApiCtx, piAddr, &llHandle);
        if (iErr)
        {
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
            return 1;
        }

        iFigureUID = getObjectFromHandle((unsigned long) llHandle);

        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        sciErr = getVarType(pvApiCtx, piAddr, &iType);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        switch (iType)
        {
            case sci_boolean :
                sciErr = getMatrixOfBoolean(pvApiCtx, piAddr, &iRows, &iCols, &pbValue);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 1;
                }
                if (iRows * iCols != 1)
                {
                    Scierror(999, _("%s: Wrong size for input argument #%d: A boolean expected.\n"), fname, 1);
                    return 1;
                }
                iFigureUID = getCurrentFigure();
                enabled = pbValue[0] == 0 ? false : true;
                break;
            case sci_strings :
                sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, NULL, NULL);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 1;
                }
                if (iRows * iCols != 1)
                {
                    Scierror(999, _("%s: Wrong size for input argument #%d: A boolean expected.\n"), fname, 1);
                    return 1;
                }
                sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, &iLen, NULL);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 1;
                }
                pstData = (char*) malloc(sizeof(char) * (iLen + 1));
                sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, &iLen, &pstData);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    free(pstData);
                    return 1;
                }
                if (strcmp("on", pstData) == 0 || strcmp("T", pstData) == 0 || strcmp("1", pstData) == 0)
                {
                    iFigureUID = getCurrentFigure();
                    enabled = true;
                }
                else if (strcmp("off", pstData) == 0 || strcmp("F", pstData) == 0 || strcmp("0", pstData) == 0)
                {
                    iFigureUID = getCurrentFigure();
                    enabled = false;
                }
                else
                {
                    free(pstData);
                    Scierror(999, _("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"), fname, 1, "on", "off");
                    return 1;
                }
                free(pstData);
                break;
            default :
                Scierror(999, _("%s: Wrong type for input argument #%d: A boolean expected.\n"), fname, 2);
                return 1;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong number for input argument: %d expected.\n"), fname, 2);
        return 1;
    }

    if (iFigureUID)
    {
        DatatipManager::setEnabled(getScilabJavaVM(), iFigureUID, enabled);
    }

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);
    return 0;
}

