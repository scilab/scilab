/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) DIGITEO - 2010-2010 - Clément DAVID <clement.david@scilab.org>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "Palette.hxx"
#include "GiwsException.hxx"
#include "xcosUtilities.hxx"

extern "C"
{
#include "gw_xcos.h"
#include "stack-c.h"
#include "callxcos.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "getScilabJavaVM.h"
}

using namespace org_scilab_modules_xcos_palette;

int
sci_xcosPalLoad(char *fname, unsigned long fname_len)
{
    CheckRhs(1, 2);
    CheckLhs(0, 1);

    char* path = NULL;

    int lenCategory = 0;
    char** category = NULL;

    /* path setup */
    if(readSingleString(pvApiCtx, 1, &path, fname))
    {
        return 0;
    }

    /* category setup */
    if (Rhs == 2 && readVectorString(pvApiCtx, 2, &category, &lenCategory, fname))
    {
        FREE(path);
        return 0;
    }

    /* Call the java implementation */
    try
    {
        // FIXME #7266 workaround
        // check category emptyness
        if (category == NULL || (lenCategory == 1 && *category == '\0'))
        {
            Palette::loadPal(getScilabJavaVM(), path);
        }
        else
        {
            Palette::loadPal(getScilabJavaVM(), path, category, lenCategory);
        }
    }
    catch (GiwsException::JniCallMethodException exception)
    {
        Scierror(999, "%s: %s\n", fname, exception.getJavaDescription().c_str());
        return 0;
    }
    catch (GiwsException::JniException exception)
    {
        Scierror(999, "%s: %s\n", fname, exception.what());
        return 0;
    }

    PutLhsVar();
    return 0;
}



