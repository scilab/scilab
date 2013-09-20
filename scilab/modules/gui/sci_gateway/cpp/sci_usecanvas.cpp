/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
#include"MALLOC.h"
}
/*--------------------------------------------------------------------------*/
using namespace org_scilab_modules_gui_bridge;

/*--------------------------------------------------------------------------*/
int sci_usecanvas(char *fname, unsigned long fname_len)
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
        printError(&sciErr, 0);
        return 1;
    }

    AssignOutputVariable(pvApiCtx, 1) = 1;
    returnArguments(pvApiCtx);
    return 0;
}

/*--------------------------------------------------------------------------*/
