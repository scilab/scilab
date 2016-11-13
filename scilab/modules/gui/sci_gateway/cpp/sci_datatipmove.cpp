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

int sci_datatipmove(char *fname, void* pvApiCtx)
{
    int iDatatipUID         = 0;
    int* piAddr             = NULL;
    int mouseButtonNumber   = -1;
    int pixelCoords[2]      = {0, 0};
    int iErr                = 0;
    long long llHandle      = 0;

    int iType = 0;
    int* piType = &iType;

    SciErr sciErr;
    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 0, 1);

    if (nbInputArgument(pvApiCtx) == 1)
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
                while (mouseButtonNumber == -1)
                {
                    CallJxgetmouseWithOptions(1, 1);
                    mouseButtonNumber = getJxgetmouseMouseButtonNumber();
                    pixelCoords[0] = (int) getJxgetmouseXCoordinate();
                    pixelCoords[1] = (int) getJxgetmouseYCoordinate();
                    DatatipDrag::dragDatatip(getScilabJavaVM(), iDatatipUID, pixelCoords[0], pixelCoords[1]);
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
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), fname, 1);
        return 1;
    }

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);
    return 0;
}
