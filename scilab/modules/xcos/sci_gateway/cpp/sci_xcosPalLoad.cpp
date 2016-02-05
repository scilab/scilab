/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) DIGITEO - 2010-2010 - Clément DAVID <clement.david@scilab.org>
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

int sci_xcosPalLoad(char *fname, void* pvApiCtx)
{
    CheckRhs(1, 2);
    CheckLhs(0, 1);

    char *name = NULL;

    int lenCategory = 0;
    char **category = NULL;

    /* name setup */
    if (readSingleString(pvApiCtx, 1, &name, fname))
    {
        return 0;
    }

    /* category setup */
    if (Rhs == 2 && readVectorString(pvApiCtx, 2, &category, &lenCategory, fname))
    {
        FREE(name);
        return 0;
    }

    /* Call the java implementation */
    set_loaded_status(XCOS_CALLED);
    try
    {
        // FIXME #7266 workaround
        // check category emptyness
        if (category == NULL || (lenCategory == 1 && **category == '\0'))
        {
            Palette::loadPal(getScilabJavaVM(), name);
        }
        else
        {
            Palette::loadPal(getScilabJavaVM(), name, category, lenCategory);
        }
    }
    catch (GiwsException::JniCallMethodException &exception)
    {
        if (category != NULL)
        {
            releaseVectorString(category, lenCategory);
        }
        FREE(name);
        Scierror(999, "%s: %s\n", fname, exception.getJavaDescription().c_str());
        return 0;
    }
    catch (GiwsException::JniException &exception)
    {
        if (category != NULL)
        {
            releaseVectorString(category, lenCategory);
        }
        FREE(name);
        Scierror(999, "%s: %s\n", fname, exception.whatStr().c_str());
        return 0;
    }

    if (category != NULL)
    {
        releaseVectorString(category, lenCategory);
    }
    FREE(name);
    PutLhsVar();
    return 0;
}
