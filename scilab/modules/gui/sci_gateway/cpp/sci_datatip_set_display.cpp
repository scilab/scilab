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

int sci_datatip_set_display(char *fname, unsigned long fname_len)
{

    SciErr sciErr;
    CheckInputArgument(pvApiCtx, 1, 2);
    CheckOutputArgument(pvApiCtx, 1, 1);

    int stkAdr = 0;
    int nbRow = 0, nbCol = 0;
    char* datatipUID = NULL;
    int iType = 0;
    int *piType = &iType;
    int* piAddr = NULL;
    int iRet = 0;
    char* pstData = NULL;

    GetRhsVar(1, GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &stkAdr);
    datatipUID = (char *)getObjectFromHandle((unsigned long) * (hstk(stkAdr)));

    if (checkInputArgumentType(pvApiCtx, 1, sci_handles))
    {

        getGraphicObjectProperty(datatipUID, __GO_TYPE__, jni_int, (void**) &piType);
        if (iType == __GO_DATATIP__)
        {

            if (nbInputArgument(pvApiCtx) == 1)
            {

                setGraphicObjectProperty(datatipUID, __GO_DATATIP_DISPLAY_FNC__, "", jni_string, 1);
                LhsVar(1) = 0;
                PutLhsVar();
                return FALSE;

            }
            else if (nbInputArgument(pvApiCtx) == 2)
            {

                sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
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

                        setGraphicObjectProperty(datatipUID, __GO_DATATIP_DISPLAY_FNC__, pstData, jni_string, 1);

                        freeAllocatedSingleString(pstData);
                        LhsVar(1) = 0;
                        PutLhsVar();
                        return FALSE;
                    }
                    else
                    {

                        Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 2);
                        return FALSE;

                    }
                }
                else
                {

                    Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 2);
                    return FALSE;

                }

            }
            else
            {

                Scierror(999, _("%s: Wrong number of input arguments: %d or %d expected.\n"), fname, 1, 2);
                return FALSE;

            }

        }
        else
        {

            Scierror(999, _("%s: Wrong type for input argument #%d: A '%s' handle expected.\n"), fname, 1, "Datatip");
            return FALSE;

        }

    }
    else
    {

        Scierror(999, _("%s: Wrong type for input argument #%d: A '%s' handle expected.\n"), fname, 1, "Datatip");
        return FALSE;

    }

}