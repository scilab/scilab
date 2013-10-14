/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "UIWidgetTools.hxx"
#include "UIWidget.hxx"
#include "UserDataHandler.hxx"
#include "ScilabToJava.hxx"

extern "C"
{
#include "gw_gui.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "freeArrayOfString.h"
#include "expandPathVariable.h"
#include "getScilabJavaVM.h"
#include "stricmp.h"
}

using namespace org_scilab_modules_gui_uiwidget;

/*--------------------------------------------------------------------------*/
int sci_uiwidget(char * fname, unsigned long fname_len)
{
    static int handlerId = UIWidget::getUIWidgetHandler(getScilabJavaVM());

    SciErr err;
    int * addr = 0;
    char * str = 0;
    const int nbIn = nbInputArgument(pvApiCtx);
    std::vector<int> indexes;

    CheckOutputArgument(pvApiCtx, 1, 1);

    if (nbIn == 1)
    {
        char * str = 0;
        char * expandedPath = 0;

        err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
        if (err.iErr)
        {
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
            return 0;
        }

        if (!isStringType(pvApiCtx, addr) || !checkVarDimension(pvApiCtx, addr, 1, 1))
        {
            Scierror(999, gettext("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
            return 0;
        }

        if (getAllocatedSingleString(pvApiCtx, addr, &str) != 0)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }
        expandedPath = expandPathVariable(str);
        freeAllocatedSingleString(str);

        try
        {
            UIWidget::uiwidgetLoad(getScilabJavaVM(), expandedPath);
        }
        catch (const GiwsException::JniException & e)
        {
            Scierror(999, _("%s: %s\n"), fname, e.getJavaDescription().c_str());
            return 0;
        }

        AssignOutputVariable(pvApiCtx, 1) = 0;
        ReturnArguments(pvApiCtx);

        return 0;
    }
    else
    {
        int uid = -1;
        const int start = nbIn % 2 + 1;
        int userDataPos = -1;

        if (start == 2)
        {
            err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
            if (err.iErr)
            {
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                return 0;
            }
            org_modules_types::ScilabToJava::sendVariable("", addr, false, handlerId, pvApiCtx);
        }

        for (int i = start; i <= nbIn; i += 2)
        {
            err = getVarAddressFromPosition(pvApiCtx, i, &addr);
            if (err.iErr)
            {
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, i);
                return 0;
            }
            if (!isStringType(pvApiCtx, addr) || !checkVarDimension(pvApiCtx, addr, 1, 1))
            {
                Scierror(999, gettext("%s: Wrong type for input argument #%d: A string expected.\n"), fname, i);
                return 0;
            }
            if (getAllocatedSingleString(pvApiCtx, addr, &str) != 0)
            {
                Scierror(999, _("%s: No more memory.\n"), fname);
                return 0;
            }
            if (!stricmp(str, "userdata") || !stricmp(str, "user_data"))
            {
                userDataPos = i + 1;
            }
            else
            {
                org_modules_types::ScilabToJava::sendStringVariable("", indexes, 1, 1, &str, false, false, handlerId);
                err = getVarAddressFromPosition(pvApiCtx, i + 1, &addr);
                if (err.iErr)
                {
                    freeAllocatedSingleString(str);
                    Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, i + 1);
                    return 0;
                }
                org_modules_types::ScilabToJava::sendVariable("", addr, false, handlerId, pvApiCtx);
            }
            freeAllocatedSingleString(str);
        }

        try
        {
            uid = UIWidget::uiwidget(getScilabJavaVM());
        }
        catch (const GiwsException::JniException & e)
        {
            Scierror(999, _("%s: %s\n"), fname, e.getJavaDescription().c_str());
            return 0;
        }

        if (uid != -1)
        {
            if (userDataPos != -1)
            {
                UserDataHandler::put(uid, userDataPos);
            }

            UIWidgetTools::createOnScilabStack(uid, nbIn + 1, pvApiCtx);
            AssignOutputVariable(pvApiCtx, 1) = nbIn + 1;
            ReturnArguments(pvApiCtx);
        }
        else
        {
            Scierror(999, _("%s: Invalid object.\n"), fname);
        }

        return 0;
    }
}

/*--------------------------------------------------------------------------*/
