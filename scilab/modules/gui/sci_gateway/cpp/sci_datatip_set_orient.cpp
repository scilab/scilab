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
#include "string.h"
}

using namespace org_scilab_modules_gui_datatip;

int sci_datatip_set_orient(char *fname, unsigned long fname_len)
{

    SciErr sciErr;
    CheckInputArgument(pvApiCtx, 2, 2);
    CheckOutputArgument(pvApiCtx, 0, 1);

    int nbRow = 0, nbCol = 0, stkAdr = 0;
    char* datatipUID = NULL;
    int iType = 0;
    int *piType = &iType;
    int* piAddr = NULL;
    int iRet = 0;
    int compVar = 0;

    if (nbInputArgument(pvApiCtx) == 2)
    {

        GetRhsVar(1, GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &stkAdr);
        datatipUID = (char *)getObjectFromHandle((unsigned long) * (hstk(stkAdr)));

        if (checkInputArgumentType(pvApiCtx, 1, sci_handles))
        {

            getGraphicObjectProperty(datatipUID, __GO_TYPE__, jni_int, (void**) &piType);
            if (iType == __GO_DATATIP__)
            {

                if (checkInputArgumentType(pvApiCtx, 2, sci_strings))
                {

                    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return 0;
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

                        compVar = strcmp(pstData, "upper left");
                        if (compVar == 0)
                        {
                            DatatipOrientation::datatipSetOrientation(getScilabJavaVM(), (char*)datatipUID, (char*)pstData, 0);
                            freeAllocatedSingleString(pstData);
                            LhsVar(1) = 0;
                            PutLhsVar();
                            return TRUE;
                        }

                        compVar = strcmp(pstData, "upper right");
                        if (compVar == 0)
                        {
                            DatatipOrientation::datatipSetOrientation(getScilabJavaVM(), (char*)datatipUID, (char*)pstData, 1);
                            freeAllocatedSingleString(pstData);
                            LhsVar(1) = 0;
                            PutLhsVar();
                            return TRUE;
                        }

                        compVar = strcmp(pstData, "lower left");
                        if (compVar == 0)
                        {
                            DatatipOrientation::datatipSetOrientation(getScilabJavaVM(), (char*)datatipUID, (char*)pstData, 2);
                            freeAllocatedSingleString(pstData);
                            LhsVar(1) = 0;
                            PutLhsVar();
                            return TRUE;
                        }

                        compVar = strcmp(pstData, "lower right");
                        if (compVar == 0)
                        {
                            DatatipOrientation::datatipSetOrientation(getScilabJavaVM(), (char*)datatipUID, (char*)pstData, 3);
                            freeAllocatedSingleString(pstData);
                            LhsVar(1) = 0;
                            PutLhsVar();
                            return TRUE;
                        }

                        compVar = strcmp(pstData, "automatic");
                        if (compVar == 0)
                        {
                            DatatipOrientation::datatipSetOrientation(getScilabJavaVM(), (char*)datatipUID, (char*)pstData, 4);
                            freeAllocatedSingleString(pstData);
                            LhsVar(1) = 0;
                            PutLhsVar();
                            return TRUE;
                        }


                        DatatipOrientation::datatipSetOrientation(getScilabJavaVM(), (char*)datatipUID, (char*)pstData, -1);
                        freeAllocatedSingleString(pstData);
                        LhsVar(1) = 0;
                        PutLhsVar();
                        return TRUE;

                    }
                    else
                    {

                        Scierror(999, _("%s: Wrong dimension for input argument #%d: Single dimension string expected.\n"), fname, 2);
                        return FALSE;

                    }

                }
                else
                {

                    Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), fname, 2);
                    return FALSE;

                }

            }
            else
            {

                Scierror(999, _("%s: Wrong type for input argument #%d: Datatip handler expected.\n"), fname, 1);
                return FALSE;

            }

        }
        else
        {

            Scierror(999, _("%s: Wrong type for input argument #%d: Datatip handler expected.\n"), fname, 1);
            return FALSE;

        }

    }
    else
    {

        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), fname, 2);
        return FALSE;

    }

}