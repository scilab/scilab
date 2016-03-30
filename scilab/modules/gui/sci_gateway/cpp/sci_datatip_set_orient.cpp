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

#include "DatatipOrientation.hxx"

extern "C"
{
#include "api_scilab.h"
#include "getScilabJavaVM.h"
#include "localization.h"
#include "Scierror.h"
#include "gw_gui.h"
#include "HandleManagement.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "os_string.h"
}

using namespace org_scilab_modules_gui_datatip;

int sci_datatip_set_orient(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    CheckInputArgument(pvApiCtx, 2, 2);
    CheckOutputArgument(pvApiCtx, 0, 1);

    int iDatatipUID     = 0;
    int* piAddr         = NULL;
    int iRet            = 0;
    int iErr            = 0;
    int compVar         = 0;
    long long llHandle  = 0;

    int iType = 0;
    int *piType = &iType;

    if (nbInputArgument(pvApiCtx) == 2)
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

        iDatatipUID = getObjectFromHandle((unsigned long) llHandle);

        if (checkInputArgumentType(pvApiCtx, 1, sci_handles))
        {
            getGraphicObjectProperty(iDatatipUID, __GO_TYPE__, jni_int, (void**) &piType);
            if (iType == __GO_DATATIP__)
            {
                if (checkInputArgumentType(pvApiCtx, 2, sci_strings))
                {
                    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return 1;
                    }

                    if (isScalar(pvApiCtx, piAddr))
                    {
                        char* pstData = NULL;

                        iRet = getAllocatedSingleString(pvApiCtx, piAddr, &pstData);
                        if (iRet)
                        {
                            freeAllocatedSingleString(pstData);
                            return iRet;
                        }

                        compVar = stricmp(pstData, "upper left");
                        if (compVar == 0)
                        {
                            DatatipOrientation::datatipSetOrientation(getScilabJavaVM(), iDatatipUID, (char*)pstData, 0);
                            freeAllocatedSingleString(pstData);
                            AssignOutputVariable(pvApiCtx, 1) = 0;
                            ReturnArguments(pvApiCtx);
                            return 0;
                        }

                        compVar = stricmp(pstData, "upper right");
                        if (compVar == 0)
                        {
                            DatatipOrientation::datatipSetOrientation(getScilabJavaVM(), iDatatipUID, (char*)pstData, 1);
                            freeAllocatedSingleString(pstData);
                            AssignOutputVariable(pvApiCtx, 1) = 0;
                            ReturnArguments(pvApiCtx);
                            return 0;
                        }

                        compVar = stricmp(pstData, "lower left");
                        if (compVar == 0)
                        {
                            DatatipOrientation::datatipSetOrientation(getScilabJavaVM(), iDatatipUID, (char*)pstData, 2);
                            freeAllocatedSingleString(pstData);
                            AssignOutputVariable(pvApiCtx, 1) = 0;
                            ReturnArguments(pvApiCtx);
                            return 0;
                        }

                        compVar = stricmp(pstData, "lower right");
                        if (compVar == 0)
                        {
                            DatatipOrientation::datatipSetOrientation(getScilabJavaVM(), iDatatipUID, (char*)pstData, 3);
                            freeAllocatedSingleString(pstData);
                            AssignOutputVariable(pvApiCtx, 1) = 0;
                            ReturnArguments(pvApiCtx);
                            return 0;
                        }

                        compVar = stricmp(pstData, "left");
                        if (compVar == 0)
                        {
                            DatatipOrientation::datatipSetOrientation(getScilabJavaVM(), iDatatipUID, (char*)pstData, 4);
                            freeAllocatedSingleString(pstData);
                            AssignOutputVariable(pvApiCtx, 1) = 0;
                            ReturnArguments(pvApiCtx);
                            return 0;
                        }

                        compVar = stricmp(pstData, "right");
                        if (compVar == 0)
                        {
                            DatatipOrientation::datatipSetOrientation(getScilabJavaVM(), iDatatipUID, (char*)pstData, 5);
                            freeAllocatedSingleString(pstData);
                            AssignOutputVariable(pvApiCtx, 1) = 0;
                            ReturnArguments(pvApiCtx);
                            return 0;
                        }

                        compVar = stricmp(pstData, "upper");
                        if (compVar == 0)
                        {
                            DatatipOrientation::datatipSetOrientation(getScilabJavaVM(), iDatatipUID, (char*)pstData, 6);
                            freeAllocatedSingleString(pstData);
                            AssignOutputVariable(pvApiCtx, 1) = 0;
                            ReturnArguments(pvApiCtx);
                            return 0;
                        }

                        compVar = stricmp(pstData, "lower");
                        if (compVar == 0)
                        {
                            DatatipOrientation::datatipSetOrientation(getScilabJavaVM(), iDatatipUID, (char*)pstData, 7);
                            freeAllocatedSingleString(pstData);
                            AssignOutputVariable(pvApiCtx, 1) = 0;
                            ReturnArguments(pvApiCtx);
                            return 0;
                        }

                        compVar = stricmp(pstData, "automatic");
                        if (compVar == 0)
                        {
                            DatatipOrientation::datatipSetOrientation(getScilabJavaVM(), iDatatipUID, (char*)pstData, 8);
                            freeAllocatedSingleString(pstData);
                            AssignOutputVariable(pvApiCtx, 1) = 0;
                            ReturnArguments(pvApiCtx);
                            return 0;
                        }

                        DatatipOrientation::datatipSetOrientation(getScilabJavaVM(), iDatatipUID, (char*)pstData, -1);
                        freeAllocatedSingleString(pstData);
                        AssignOutputVariable(pvApiCtx, 1) = 0;
                        ReturnArguments(pvApiCtx);
                        return 0;
                    }
                    else
                    {
                        Scierror(999, _("%s: Wrong dimension for input argument #%d: string expected.\n"), fname, 2);
                        return 1;
                    }
                }
                else
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 2);
                    return 1;
                }
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A '%s' handle expected.\n"), fname, 1, "Datatip");
                return 1;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A '%s' handle expected.\n"), fname, 1, "Datatip");
            return 1;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), fname, 2);
        return 1;
    }
}
