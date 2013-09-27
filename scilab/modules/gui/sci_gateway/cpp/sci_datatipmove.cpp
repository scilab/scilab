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

#include "DatatipDrag.hxx"

extern "C"
{
#include "api_scilab.h"
#include "getScilabJavaVM.h"
#include "localization.h"
#include "Scierror.h"
#include "gw_gui.h"
#include "HandleManagement.h"
#include "CallJxgetmouse.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
}

using namespace org_scilab_modules_gui_datatip;

int sci_datatipmove(char *fname, unsigned long fname_len)
{
    double* l1 = NULL;
    int mouseButtonNumber = -1;
    int pixelCoords[2];
    int iType = 0;
    int *piType = &iType;
    char* datatipUID = NULL;
    int nbRow = 0, nbCol = 0, stkAdr = 0;

    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 0, 1);

    if (nbInputArgument(pvApiCtx) == 1)
    {
        GetRhsVar(1, GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &stkAdr);
        datatipUID = (char *)getObjectFromHandle((unsigned long) * (hstk(stkAdr)));
        if (checkInputArgumentType(pvApiCtx, 1, sci_handles))
        {
            getGraphicObjectProperty(datatipUID, __GO_TYPE__, jni_int, (void**) &piType);
            if (iType == __GO_DATATIP__)
            {
                while (mouseButtonNumber == -1)
                {
                    CallJxgetmouseWithOptions(1, 1);
                    mouseButtonNumber = getJxgetmouseMouseButtonNumber();
                    pixelCoords[0] = (int) getJxgetmouseXCoordinate();
                    pixelCoords[1] = (int) getJxgetmouseYCoordinate();
                    DatatipDrag::dragDatatip(getScilabJavaVM(), (char*)datatipUID, pixelCoords[0], pixelCoords[1]);
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
    else
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), fname, 1);
        return FALSE;
    }

    LhsVar(1) = 0;
    PutLhsVar();

    return TRUE;
}