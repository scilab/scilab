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
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "freeArrayOfString.h"
#include "MALLOC.h"
#include "getScilabJavaVM.h"
}

using namespace org_scilab_modules_xcos_palette;

int sci_xcosPalGenerateAllIcons(char *fname, unsigned long fname_len)
{
    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 0, 1);

    char **path = NULL;
    int pathLength = 0;

    /* path setup */
    if (readVectorString(pvApiCtx, 1, &path, &pathLength, fname))
    {
        return 0;
    }

    /* Call the java implementation */
    try
    {
        Palette::generateAllImages(getScilabJavaVM(), path, pathLength);
    }
    catch (GiwsException::JniCallMethodException &exception)
    {
        Scierror(999, "%s: %s\n", fname, exception.getJavaDescription().c_str());
    }
    catch (GiwsException::JniException &exception)
    {
        Scierror(999, "%s: %s\n", fname, exception.whatStr().c_str());
    }

    freeArrayOfString(path, pathLength);

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);

    return 0;
}

