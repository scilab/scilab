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
#include "freeArrayOfString.h"
#include "getScilabJavaVM.h"
}

using namespace org_scilab_modules_xcos_palette;

int sci_xcosPalCategoryAdd(char *fname, void* pvApiCtx)
{
    CheckRhs(1, 2);
    CheckLhs(0, 1);

    char **name = NULL;
    int nameLength = 0;
    bool visible = false;

    /* name setup */
    if (readVectorString(pvApiCtx, 1, &name, &nameLength, fname))
    {
        return 0;
    }

    /* visible setup */
    if (Rhs > 2)
    {
        if (readSingleBoolean(pvApiCtx, 2, &visible, fname))
        {
            releaseVectorString(name, nameLength);
            return 0;
        }
    }
    else
    {
        visible = true;
    }

    /* Call the java implementation */
    set_loaded_status(XCOS_CALLED);
    try
    {
        Palette::addCategory(getScilabJavaVM(), name, nameLength, visible);
    }
    catch (GiwsException::JniCallMethodException &exception)
    {
        releaseVectorString(name, nameLength);
        Scierror(999, "%s: %s\n", fname, exception.getJavaDescription().c_str());
        return 0;
    }
    catch (GiwsException::JniException &exception)
    {
        releaseVectorString(name, nameLength);
        Scierror(999, "%s: %s\n", fname, exception.whatStr().c_str());
        return 0;
    }

    releaseVectorString(name, nameLength);
    PutLhsVar();
    return 0;
}
