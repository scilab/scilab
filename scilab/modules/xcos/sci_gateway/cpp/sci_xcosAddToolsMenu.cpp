/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) Scilab Enterprises - 2011 - Clément DAVID <clement.david@scilab-enterprises.com>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "Xcos.hxx"
#include "GiwsException.hxx"
#include "xcosUtilities.hxx"

extern "C"
{
#include "gw_xcos.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "getScilabJavaVM.h"
}

using namespace org_scilab_modules_xcos;

int sci_xcosAddToolsMenu(char *fname, unsigned long fname_len)
{
    CheckRhs(2, 2);
    CheckLhs(0, 1);

    char *label;
    char *callback;

    if (readSingleString(pvApiCtx, 1, &label, fname))
    {
        return 0;
    }
    if (readSingleString(pvApiCtx, 2, &callback, fname))
    {
        FREE(label);
        return 0;
    }

    /* Call the java implementation */
    try
    {
        Xcos::addToolsMenu(getScilabJavaVM(), label, callback);

        FREE(label);
        FREE(callback);
    }
    catch (GiwsException::JniCallMethodException &exception)
    {
        Scierror(999, "%s: %s\n", fname, exception.getJavaDescription().c_str());

        FREE(label);
        FREE(callback);
        return 0;
    }
    catch (GiwsException::JniException &exception)
    {
        Scierror(999, "%s: %s\n", fname, exception.whatStr().c_str());

        FREE(label);
        FREE(callback);
        return 0;
    }

    PutLhsVar();
    return 0;
}
