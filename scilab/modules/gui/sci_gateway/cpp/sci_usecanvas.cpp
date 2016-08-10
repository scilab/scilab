/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
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

#include "CallScilabBridge.hxx"
#include "GiwsException.hxx"

extern "C"
{
#include "api_scilab.h"
#include "getScilabJavaVM.h"
#include "localization.h"
#include "Scierror.h"
#include "gw_gui.h"
#include "BOOL.h"
#include"sci_malloc.h"
}
/*--------------------------------------------------------------------------*/
using namespace org_scilab_modules_gui_bridge;

/*--------------------------------------------------------------------------*/
int sci_usecanvas(char *fname, void* pvApiCtx)
{
    SciErr sciErr;

    int m1 = 0, n1 = 0, l1 = 0;
    int status = 0;
    int* piAddr1 = NULL;

    CheckInputArgument(pvApiCtx, 0, 1);
    CheckOutputArgument(pvApiCtx, 0, 1);

    if (nbInputArgument(pvApiCtx) == 1) /* Sets the status of usecanvas */
    {
        if (checkInputArgumentType(pvApiCtx, 1, sci_boolean) == FALSE)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A boolean expected.\n"), fname, 1);
            return 1;
        }

        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        if (getScalarBoolean(pvApiCtx, piAddr1, &l1))
        {
            printError(&sciErr, 0);
            return 1;
        }

        try
        {
            CallScilabBridge::useCanvasForDisplay(getScilabJavaVM(), BOOLtobool(l1));
        }
        catch (const GiwsException::JniException & e)
        {
            Scierror(999, _("%s: A Java exception arisen:\n%s"), fname, e.whatStr().c_str());
            return 1;
        }
    }

    /* Returns current status */
    try
    {
        status = booltoBOOL(CallScilabBridge::useCanvasForDisplay(getScilabJavaVM()));
    }
    catch (const GiwsException::JniException & e)
    {
        Scierror(999, _("%s: A Java exception arisen:\n%s"), fname, e.whatStr().c_str());
        return 1;
    }

    if (createScalarDouble(pvApiCtx, 1, status))
    {
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    AssignOutputVariable(pvApiCtx, 1) = 1;
    ReturnArguments(pvApiCtx);
    return 0;
}

/*--------------------------------------------------------------------------*/
