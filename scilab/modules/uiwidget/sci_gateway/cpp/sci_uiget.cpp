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
int sci_uiget(char *fname, unsigned long fname_len)
{
    static int handlerId = UIWidget::getUIWidgetHandler(getScilabJavaVM());

    SciErr err;
    int * addr = 0;
    char * str = 0;
    int uid;
    const int nbIn = nbInputArgument(pvApiCtx);

    CheckInputArgument(pvApiCtx, 1, 2);
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

        if (getAllocatedSingleString(pvApiCtx, addr, &str) != 0)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }

        if (nbIn == 1 && !strcmp(str, "/"))
        {
            // we get all the root components
            freeAllocatedSingleString(str);
            UIWidget::getRoots(getScilabJavaVM(), nbIn + 1);

            AssignOutputVariable(pvApiCtx, 1) = nbIn + 1;
            ReturnArguments(pvApiCtx);

            return 0;
        }

        uid = UIWidget::getUidFromPath(getScilabJavaVM(), str);
        freeAllocatedSingleString(str);
    }

    if (uid == -1)
    {
        createEmptyMatrix(pvApiCtx, nbIn + 1);

        AssignOutputVariable(pvApiCtx, 1) = nbIn + 1;
        ReturnArguments(pvApiCtx);

        return 0;
    }

    if (nbIn == 1)
    {
        UIWidgetTools::createOnScilabStack(uid, nbIn + 1, pvApiCtx);
    }
    else
    {
        err = getVarAddressFromPosition(pvApiCtx, 2, &addr);
        if (err.iErr)
        {
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
            return 0;
        }
        if (!isStringType(pvApiCtx, addr) || !checkVarDimension(pvApiCtx, addr, 1, 1))
        {
            Scierror(999, gettext("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 2);
            return 0;
        }
        if (getAllocatedSingleString(pvApiCtx, addr, &str) != 0)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }
        if (!stricmp(str, "userdata") || !stricmp(str, "user_data"))
        {
            UserDataHandler::get(uid, pvApiCtx, nbIn + 1);
        }
        else
        {
            try
            {
                UIWidget::uiget(getScilabJavaVM(), uid, str, nbIn + 1);
            }
            catch (const GiwsException::JniException & e)
            {
                freeAllocatedSingleString(str);
                Scierror(999, _("%s: %s\n"), fname, e.getJavaDescription().c_str());
                return 0;
            }
        }
        freeAllocatedSingleString(str);
    }

    AssignOutputVariable(pvApiCtx, 1) = nbIn + 1;
    ReturnArguments(pvApiCtx);

    return 0;
}

/*--------------------------------------------------------------------------*/
