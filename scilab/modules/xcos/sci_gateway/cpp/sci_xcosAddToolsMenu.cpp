/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) Scilab Enterprises - 2011 - Clément DAVID <clement.david@scilab-enterprises.com>
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

#include "Xcos.hxx"
#include "GiwsException.hxx"
#include "xcosUtilities.hxx"
#include "loadStatus.hxx"

extern "C"
{
#include "gw_xcos.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "sci_malloc.h"
#include "getScilabJavaVM.h"
}

using namespace org_scilab_modules_xcos;

int sci_xcosAddToolsMenu(char *fname, void *pvApiCtx)
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
    set_loaded_status(XCOS_CALLED);
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
