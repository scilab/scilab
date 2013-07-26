/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2013 - Gustavo Barbosa Libotte <gustavolibotte@gmail.com>
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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

int sci_datatip_set_style(char *fname, unsigned long fname_len)
{

    int stkAdr = 0;
    int* piAddr	= NULL;
    int nbRow = 0, nbCol = 0;
    char* polylineUID = NULL;
    double* markStyle = NULL;
    int* pbBoxed = NULL;
    int* pbLabeled = NULL;
    int datatipSetStyleOption;
    int iType = 0;
    int *piType = &iType;

    SciErr sciErr;
    CheckInputArgument(pvApiCtx, 1, 4);
    CheckOutputArgument(pvApiCtx, 1, 1);

    GetRhsVar(1, GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &stkAdr);
    polylineUID = (char *)getObjectFromHandle((unsigned long) * (hstk(stkAdr)));

    if (checkInputArgumentType(pvApiCtx, 1, sci_handles))
    {

        getGraphicObjectProperty(polylineUID, __GO_TYPE__, jni_int, (void**) &piType);
        if (iType == __GO_POLYLINE__)
        {

            if (nbInputArgument(pvApiCtx) == 1)
            {

                datatipSetStyleOption = DatatipSetStyle::datatipSetStyleWindow(getScilabJavaVM());

                switch (datatipSetStyleOption)
                {
                    case(1):
                        DatatipSetStyle::datatipSetStyle(getScilabJavaVM(), (char*)polylineUID, 1, true, true);
                        LhsVar(1) = 0;
                        PutLhsVar();
                        return TRUE;
                    case(2):
                        DatatipSetStyle::datatipSetStyle(getScilabJavaVM(), (char*)polylineUID, 1, false, true);
                        LhsVar(1) = 0;
                        PutLhsVar();
                        return TRUE;
                    case(3):
                        DatatipSetStyle::datatipSetStyle(getScilabJavaVM(), (char*)polylineUID, 1, false, false);
                        LhsVar(1) = 0;
                        PutLhsVar();
                        return TRUE;
                    case(4):
                        DatatipSetStyle::datatipSetStyle(getScilabJavaVM(), (char*)polylineUID, 2, true, true);
                        LhsVar(1) = 0;
                        PutLhsVar();
                        return TRUE;
                    case(5):
                        DatatipSetStyle::datatipSetStyle(getScilabJavaVM(), (char*)polylineUID, 2, false, true);
                        LhsVar(1) = 0;
                        PutLhsVar();
                        return TRUE;
                    case(6):
                        DatatipSetStyle::datatipSetStyle(getScilabJavaVM(), (char*)polylineUID, 2, false, false);
                        LhsVar(1) = 0;
                        PutLhsVar();
                        return TRUE;
                    case(0):
                        //Do nothing.
                        LhsVar(1) = 0;
                        PutLhsVar();
                        return FALSE;
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
                        return 0;
                    }
                    sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &nbRow, &nbCol, &markStyle);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return 0;
                    }

                    if ((int)markStyle[0] == 1 || (int)markStyle[0] == 2)
                    {

                        DatatipSetStyle::datatipSetStyle(getScilabJavaVM(), (char*)polylineUID, (int)markStyle[0], true, true);
                        LhsVar(1) = 0;
                        PutLhsVar();
                        return TRUE;
                    }
                    else
                    {
                        Scierror(999, _("%s: Wrong value for input argument #%d: %d or %d expected.\n"), fname, 2, 1, 2);
                        return FALSE;
                    }

                }
                else
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: integer expected.\n"), fname, 2);
                    return FALSE;
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
                        return 0;
                    }
                    sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &nbRow, &nbCol, &markStyle);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return 0;
                    }

                    if ((int)markStyle[0] == 1 || (int)markStyle[0] == 2)
                    {

                        if (checkInputArgumentType(pvApiCtx, 3, sci_boolean))
                        {

                            sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr);
                            if (sciErr.iErr)
                            {
                                printError(&sciErr, 0);
                                return FALSE;
                            }
                            sciErr = getMatrixOfBoolean(pvApiCtx, piAddr, &nbRow, &nbCol, &pbBoxed);
                            if (sciErr.iErr)
                            {
                                printError(&sciErr, 0);
                                return FALSE;
                            }

                            DatatipSetStyle::datatipSetStyle(getScilabJavaVM(), (char*)polylineUID, (int)markStyle[0], (bool)pbBoxed[0], true);
                            LhsVar(1) = 0;
                            PutLhsVar();
                            return TRUE;

                        }
                        else
                        {
                            Scierror(999, _("%s: Wrong type for input argument #%d: boolean expected.\n"), fname, 3);
                            return FALSE;
                        }
                    }
                    else
                    {
                        Scierror(999, _("%s: Wrong value for input argument #%d: %d or %d expected.\n"), fname, 2, 1, 2);
                        return FALSE;
                    }

                }
                else
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: integer expected.\n"), fname, 2);
                    return FALSE;
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
                        return 0;
                    }
                    sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &nbRow, &nbCol, &markStyle);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return 0;
                    }

                    if ((int)markStyle[0] == 1 || (int)markStyle[0] == 2)
                    {

                        if (checkInputArgumentType(pvApiCtx, 3, sci_boolean))
                        {

                            sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr);
                            if (sciErr.iErr)
                            {
                                printError(&sciErr, 0);
                                return FALSE;
                            }
                            sciErr = getMatrixOfBoolean(pvApiCtx, piAddr, &nbRow, &nbCol, &pbBoxed);
                            if (sciErr.iErr)
                            {
                                printError(&sciErr, 0);
                                return FALSE;
                            }

                            if (checkInputArgumentType(pvApiCtx, 4, sci_boolean))
                            {

                                sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddr);
                                if (sciErr.iErr)
                                {
                                    printError(&sciErr, 0);
                                    return FALSE;
                                }
                                sciErr = getMatrixOfBoolean(pvApiCtx, piAddr, &nbRow, &nbCol, &pbLabeled);
                                if (sciErr.iErr)
                                {
                                    printError(&sciErr, 0);
                                    return FALSE;
                                }

                                DatatipSetStyle::datatipSetStyle(getScilabJavaVM(), (char*)polylineUID, (int)markStyle[0], (bool)pbBoxed[0], (bool)pbLabeled[0]);
                                LhsVar(1) = 0;
                                PutLhsVar();
                                return TRUE;

                            }
                            else
                            {
                                Scierror(999, _("%s: Wrong type for input argument #%d: boolean expected.\n"), fname, 4);
                                return FALSE;
                            }

                        }
                        else
                        {
                            Scierror(999, _("%s: Wrong type for input argument #%d: boolean expected.\n"), fname, 3);
                            return FALSE;
                        }
                    }
                    else
                    {
                        Scierror(999, _("%s: Wrong value for input argument #%d: %d or %d expected.\n"), fname, 2, 1, 2);
                        return FALSE;
                    }

                }
                else
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: integer expected.\n"), fname, 2);
                    return FALSE;
                }


            }

            else
            {
                Scierror(999, _("%s: Wrong number of input arguments: %d to %d expected.\n"), fname, 1, 4);
                return FALSE;
            }

        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Polyline handler expected.\n"), fname, 1);
            return FALSE;
        }

    }

    LhsVar(1) = 0;
    PutLhsVar();

    return TRUE;

}