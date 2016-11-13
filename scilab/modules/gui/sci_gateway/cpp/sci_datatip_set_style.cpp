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

#include "DatatipSetStyle.hxx"

extern "C"
{
#include "api_scilab.h"
#include "getScilabJavaVM.h"
#include "localization.h"
#include "Scierror.h"
#include "gw_gui.h"
#include "HandleManagement.h"
#include "CurrentFigure.h"
#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
}

using namespace org_scilab_modules_gui_datatip;

int sci_datatip_set_style(char *fname, void* pvApiCtx)
{
    int* piAddr                 = NULL;
    int iPolylineUID            = 0;
    double* markStyle           = NULL;
    int* pbBoxed                = NULL;
    int* pbLabeled              = NULL;
    int datatipSetStyleOption   = 0;
    long long llHandle          = 0;
    int nbRow                   = 0;
    int nbCol                   = 0;
    int iErr                    = 0;

    int iType = 0;
    int *piType = &iType;

    SciErr sciErr;
    CheckInputArgument(pvApiCtx, 1, 4);
    CheckOutputArgument(pvApiCtx, 0, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    iErr = getScalarHandle(pvApiCtx, piAddr, &llHandle);
    if (iErr)
    {
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    iPolylineUID = getObjectFromHandle((unsigned long) llHandle);

    if (checkInputArgumentType(pvApiCtx, 1, sci_handles))
    {
        getGraphicObjectProperty(iPolylineUID, __GO_TYPE__, jni_int, (void**) &piType);
        if (iType == __GO_POLYLINE__)
        {
            if (nbInputArgument(pvApiCtx) == 1)
            {
                datatipSetStyleOption = DatatipSetStyle::datatipSetStyleWindow(getScilabJavaVM());
                switch (datatipSetStyleOption)
                {
                    case 0:
                        //Do nothing.
                        AssignOutputVariable(pvApiCtx, 1) = 0;
                        ReturnArguments(pvApiCtx);
                        return 0;
                    case 1:
                        DatatipSetStyle::datatipSetStyle(getScilabJavaVM(), iPolylineUID, 1, true, true);
                        AssignOutputVariable(pvApiCtx, 1) = 0;
                        ReturnArguments(pvApiCtx);
                        return 0;
                    case 2:
                        DatatipSetStyle::datatipSetStyle(getScilabJavaVM(), iPolylineUID, 1, false, true);
                        AssignOutputVariable(pvApiCtx, 1) = 0;
                        ReturnArguments(pvApiCtx);
                        return 0;
                    case 3:
                        DatatipSetStyle::datatipSetStyle(getScilabJavaVM(), iPolylineUID, 1, false, false);
                        AssignOutputVariable(pvApiCtx, 1) = 0;
                        ReturnArguments(pvApiCtx);
                        return 0;
                    case 4:
                        DatatipSetStyle::datatipSetStyle(getScilabJavaVM(), iPolylineUID, 2, true, true);
                        AssignOutputVariable(pvApiCtx, 1) = 0;
                        ReturnArguments(pvApiCtx);
                        return 0;
                    case 5:
                        DatatipSetStyle::datatipSetStyle(getScilabJavaVM(), iPolylineUID, 2, false, true);
                        AssignOutputVariable(pvApiCtx, 1) = 0;
                        ReturnArguments(pvApiCtx);
                        return 0;
                    case 6:
                        DatatipSetStyle::datatipSetStyle(getScilabJavaVM(), iPolylineUID, 2, false, false);
                        AssignOutputVariable(pvApiCtx, 1) = 0;
                        ReturnArguments(pvApiCtx);
                        return 0;
                }
            }
            else if (nbInputArgument(pvApiCtx) == 2)
            {
                if (checkInputArgumentType(pvApiCtx, 2, sci_matrix))
                {
                    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return 1;
                    }
                    sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &nbRow, &nbCol, &markStyle);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return 1;
                    }

                    if ((int)markStyle[0] == 1 || (int)markStyle[0] == 2)
                    {
                        DatatipSetStyle::datatipSetStyle(getScilabJavaVM(), iPolylineUID, (int)markStyle[0], true, true);
                        AssignOutputVariable(pvApiCtx, 1) = 0;
                        ReturnArguments(pvApiCtx);
                        return 0;
                    }
                    else
                    {
                        Scierror(999, _("%s: Wrong value for input argument #%d: %d or %d expected.\n"), fname, 2, 1, 2);
                        return 1;
                    }
                }
                else
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), fname, 2);
                    return 1;
                }
            }
            else if (nbInputArgument(pvApiCtx) == 3)
            {
                if (checkInputArgumentType(pvApiCtx, 2, sci_matrix))
                {
                    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return 1;
                    }
                    sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &nbRow, &nbCol, &markStyle);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return 1;
                    }

                    if ((int)markStyle[0] == 1 || (int)markStyle[0] == 2)
                    {

                        if (checkInputArgumentType(pvApiCtx, 3, sci_boolean))
                        {

                            sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr);
                            if (sciErr.iErr)
                            {
                                printError(&sciErr, 0);
                                return 1;
                            }
                            sciErr = getMatrixOfBoolean(pvApiCtx, piAddr, &nbRow, &nbCol, &pbBoxed);
                            if (sciErr.iErr)
                            {
                                printError(&sciErr, 0);
                                return 1;
                            }

                            DatatipSetStyle::datatipSetStyle(getScilabJavaVM(), iPolylineUID, (int)markStyle[0], pbBoxed[0] != 0, true);
                            AssignOutputVariable(pvApiCtx, 1) = 0;
                            ReturnArguments(pvApiCtx);
                            return 0;
                        }
                        else
                        {
                            Scierror(999, _("%s: Wrong type for input argument #%d: A boolean expected.\n"), fname, 3);
                            return 1;
                        }
                    }
                    else
                    {
                        Scierror(999, _("%s: Wrong value for input argument #%d: %d or %d expected.\n"), fname, 2, 1, 2);
                        return 1;
                    }
                }
                else
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), fname, 2);
                    return 1;
                }
            }
            else if (nbInputArgument(pvApiCtx) == 4)
            {
                if (checkInputArgumentType(pvApiCtx, 2, sci_matrix))
                {
                    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return 1;
                    }
                    sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &nbRow, &nbCol, &markStyle);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return 1;
                    }

                    if ((int)markStyle[0] == 1 || (int)markStyle[0] == 2)
                    {
                        if (checkInputArgumentType(pvApiCtx, 3, sci_boolean))
                        {
                            sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr);
                            if (sciErr.iErr)
                            {
                                printError(&sciErr, 0);
                                return 1;
                            }
                            sciErr = getMatrixOfBoolean(pvApiCtx, piAddr, &nbRow, &nbCol, &pbBoxed);
                            if (sciErr.iErr)
                            {
                                printError(&sciErr, 0);
                                return 1;
                            }

                            if (checkInputArgumentType(pvApiCtx, 4, sci_boolean))
                            {
                                sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddr);
                                if (sciErr.iErr)
                                {
                                    printError(&sciErr, 0);
                                    return 1;
                                }
                                sciErr = getMatrixOfBoolean(pvApiCtx, piAddr, &nbRow, &nbCol, &pbLabeled);
                                if (sciErr.iErr)
                                {
                                    printError(&sciErr, 0);
                                    return 1;
                                }

                                DatatipSetStyle::datatipSetStyle(getScilabJavaVM(), iPolylineUID, (int)markStyle[0], pbBoxed[0] != 0, pbLabeled[0] != 0);
                                AssignOutputVariable(pvApiCtx, 1) = 0;
                                ReturnArguments(pvApiCtx);
                                return 0;
                            }
                            else
                            {
                                Scierror(999, _("%s: Wrong type for input argument #%d: A boolean expected.\n"), fname, 4);
                                return 1;
                            }
                        }
                        else
                        {
                            Scierror(999, _("%s: Wrong type for input argument #%d: A boolean expected.\n"), fname, 3);
                            return 1;
                        }
                    }
                    else
                    {
                        Scierror(999, _("%s: Wrong value for input argument #%d: %d or %d expected.\n"), fname, 2, 1, 2);
                        return 1;
                    }
                }
                else
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), fname, 2);
                    return 1;
                }
            }
            else
            {
                Scierror(999, _("%s: Wrong number of input arguments: %d to %d expected.\n"), fname, 1, 4);
                return 1;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A '%s' handle expected.\n"), fname, 1, "Polyline");
            return 1;
        }
    }

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);
    return 0;
}
