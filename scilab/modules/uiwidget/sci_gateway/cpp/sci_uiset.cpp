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

#include "UIWidget.hxx"
#include "UIWidgetTools.hxx"
#include "UserDataHandler.hxx"
#include "ScilabToJava.hxx"

#include <vector>

extern "C"
{
#include "gw_uiwidget.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "freeArrayOfString.h"
#include "expandPathVariable.h"
#include "getScilabJavaVM.h"
#include "stricmp.h"
}

using namespace org_scilab_modules_uiwidget;

/*--------------------------------------------------------------------------*/
int sci_uiset(char *fname, unsigned long fname_len)
{
    static int handlerId = UIWidget::getUIWidgetHandler(getScilabJavaVM());

    SciErr err;
    const int nbIn = nbInputArgument(pvApiCtx);
    int * addr = 0;
    char * str = 0;
    char * path = 0;
    int uid = -1;
    std::vector<int> indexes;
    bool varSentToJava = false;

    CheckOutputArgument(pvApiCtx, 1, 1);

    err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    if (err.iErr)
    {
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (UIWidgetTools::isUIWidget(addr, pvApiCtx))
    {
        uid = UIWidgetTools::getUIWidgetId(addr, pvApiCtx);
    }
    else
    {
        if (!isStringType(pvApiCtx, addr) || !checkVarDimension(pvApiCtx, addr, 1, 1))
        {
            Scierror(999, gettext("%s: Wrong type for input argument #%d: A string or a UIWidget expected.\n"), fname, 1);
            return 0;
        }

        if (getAllocatedSingleString(pvApiCtx, addr, &path) != 0)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }
    }

    for (int i = 2; i < nbIn; i += 2)
    {
        int * addr = 0;
        err = getVarAddressFromPosition(pvApiCtx, i, &addr);
        if (err.iErr)
        {
            if (path)
            {
                freeAllocatedSingleString(path);
            }
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, i);
            return 0;
        }
        if (!isStringType(pvApiCtx, addr) || !checkVarDimension(pvApiCtx, addr, 1, 1))
        {
            if (path)
            {
                freeAllocatedSingleString(path);
            }
            Scierror(999, gettext("%s: Wrong type for input argument #%d: A string expected.\n"), fname, i);
            return 0;
        }
        if (getAllocatedSingleString(pvApiCtx, addr, &str) != 0)
        {
            if (path)
            {
                freeAllocatedSingleString(path);
            }
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }
        if (!strcmp(str, "userdata") || !strcmp(str, "user_data"))
        {
            UserDataHandler::put(uid, i + 1);
        }
        else
        {
            org_modules_types::ScilabToJava::sendStringVariable("", indexes, 1, 1, &str, false, false, handlerId);
            err = getVarAddressFromPosition(pvApiCtx, i + 1, &addr);
            if (err.iErr)
            {
                if (path)
                {
                    freeAllocatedSingleString(path);
                }
                freeAllocatedSingleString(str);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, i + 1);
                return 0;
            }
            org_modules_types::ScilabToJava::sendVariable("", addr, false, handlerId, pvApiCtx);
            varSentToJava = true;
        }
        freeAllocatedSingleString(str);
    }

    if (varSentToJava)
    {
        try
        {
            if (path)
            {
                UIWidget::uiset(getScilabJavaVM(), path);
            }
            else
            {
                UIWidget::uiset(getScilabJavaVM(), uid);
            }
        }
        catch (const GiwsException::JniException & e)
        {
            Scierror(999, _("%s: %s\n"), fname, e.getJavaDescription().c_str());
        }
    }

    if (path)
    {
        freeAllocatedSingleString(path);
    }

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);

    return 0;
}

/*--------------------------------------------------------------------------*/
