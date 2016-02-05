/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) Scilab Enterprises - 2013 - Clément DAVID
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

