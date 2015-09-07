/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) Scilab Enterprises - 2013 - Clément DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "Palette.hxx"
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

using namespace org_scilab_modules_xcos_palette;

int sci_xcosPalGet(char *fname, void* pvApiCtx)
{
    CheckRhs(1, 1);
    CheckLhs(0, 1);

    int lenCategory = 0;
    char **category = NULL;

    /* category setup */
    if (readVectorString(pvApiCtx, 1, &category, &lenCategory, fname))
    {
        return 0;
    }

    /* Call the java implementation */
    set_loaded_status(XCOS_CALLED);
    try
    {
        Palette::get(getScilabJavaVM(), category, lenCategory);
    }
    catch (GiwsException::JniCallMethodException &exception)
    {
        releaseVectorString(category, lenCategory);
        Scierror(999, "%s: %s\n", fname, exception.getJavaDescription().c_str());
        return 0;
    }
    catch (GiwsException::JniException &exception)
    {
        releaseVectorString(category, lenCategory);
        Scierror(999, "%s: %s\n", fname, exception.whatStr().c_str());
        return 0;
    }

    releaseVectorString(category, lenCategory);
    PutLhsVar();
    return 0;
}

