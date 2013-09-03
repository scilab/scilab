/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) DIGITEO - 2010-2010 - Clément DAVID <clement.david@scilab.org>
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

extern "C"
{
#include "gw_xcos.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "getScilabJavaVM.h"
}

using namespace org_scilab_modules_xcos_palette;

int sci_xcosPalDelete(char *fname, unsigned long fname_len)
{
    CheckRhs(1, 1);
    CheckLhs(0, 1);

    char **name = NULL;
    int nameLength = 0;

    /* name setup */
    if (readVectorString(pvApiCtx, 1, &name, &nameLength, fname))
    {
        return 0;
    }

    /* Call the java implementation */
    try
    {
        Palette::remove(getScilabJavaVM(), name, nameLength);
    }
    catch (GiwsException::JniCallMethodException &exception)
    {
        Scierror(999, "%s: %s\n", fname, exception.getJavaDescription().c_str());
        return 0;
    }
    catch (GiwsException::JniException &exception)
    {
        Scierror(999, "%s: %s\n", fname, exception.whatStr().c_str());
        return 0;
    }

    PutLhsVar();
    return 0;
}
