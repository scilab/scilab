/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2013 - Gustavo Barbosa Libotte <gustavolibotte@gmail.com>
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

extern "C"
{
#include "api_scilab.h"
#include "getScilabJavaVM.h"
#include "localization.h"
#include "sci_malloc.h"
#include "Scierror.h"
#include "gw_gui.h"
#include "HandleManagement.h"
#include "CurrentFigure.h"
#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
}

int sci_datatip_set_display(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    CheckInputArgument(pvApiCtx, 1, 2);
    CheckOutputArgument(pvApiCtx, 1, 1);

    int iDatatipUID     = 0;
    int* piAddr         = NULL;
    char* pstData       = NULL;
    int iRet            = 0;
    int iErr            = 0;
    long long llHandle  = 0;

    int iType = 0;
    int *piType = &iType;

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

    iDatatipUID = getObjectFromHandle((unsigned long) llHandle);

    if (checkInputArgumentType(pvApiCtx, 1, sci_handles))
    {
        getGraphicObjectProperty(iDatatipUID, __GO_TYPE__, jni_int, (void**) &piType);
        if (iType == __GO_DATATIP__ || iType == __GO_POLYLINE__)
        {
            if (nbInputArgument(pvApiCtx) == 1)
            {
                setGraphicObjectProperty(iDatatipUID, __GO_DATATIP_DISPLAY_FNC__, "", jni_string, 1);
                AssignOutputVariable(pvApiCtx, 1) = 0;
                ReturnArguments(pvApiCtx);
                return 0;
            }
            else if (nbInputArgument(pvApiCtx) == 2)
            {
                sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 1;
                }

                if (isStringType(pvApiCtx, piAddr))
                {
                    if (isScalar(pvApiCtx, piAddr))
                    {

                        iRet = getAllocatedSingleString(pvApiCtx, piAddr, &pstData);
                        if (iRet)
                        {
                            freeAllocatedSingleString(pstData);
                            return iRet;
                        }

                        setGraphicObjectProperty(iDatatipUID, __GO_DATATIP_DISPLAY_FNC__, pstData, jni_string, 1);

                        freeAllocatedSingleString(pstData);
                        AssignOutputVariable(pvApiCtx, 1) = 0;
                        ReturnArguments(pvApiCtx);
                        return 0;
                    }
                    else
                    {
                        Scierror(999, _("%s: Wrong size for input argument #%d: string or macro name expected.\n"), fname, 2);
                        return 1;
                    }
                }
                else
                {
                    sciErr = getVarType(pvApiCtx, piAddr, &iType);
                    if (iType == sci_c_function || iType == sci_u_function)
                    {
                        char *pstFunctionName = (char*) MALLOC(sizeof(char) * (200));
                        sciErr = getVarNameFromPosition(pvApiCtx, 2, pstFunctionName);
                        if (sciErr.iErr)
                        {
                            Scierror(999, _("%s: Wrong type for input argument #%d: string or macro name expected.\n"), fname, 2);
                            return 1;
                        }
                        setGraphicObjectProperty(iDatatipUID, __GO_DATATIP_DISPLAY_FNC__, pstFunctionName, jni_string, 1);
                        FREE(pstFunctionName);
                        AssignOutputVariable(pvApiCtx, 1) = 0;
                        ReturnArguments(pvApiCtx);
                        return 0;
                    }
                    else
                    {
                        Scierror(999, _("%s: Wrong type for input argument #%d: string or macro name expected.\n"), fname, 2);
                        return 1;
                    }
                }
            }
            else
            {
                Scierror(999, _("%s: Wrong number of input arguments: %d or %d expected.\n"), fname, 1, 2);
                return 1;
            }

        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A '%s' or '%s' handle expected.\n"), fname, 1, "Polyline", "Datatip");
            return 1;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A '%s' or '%s' handle expected.\n"), fname, 1, "Polyline", "Datatip");
        return 1;
    }
}
