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
#include "Controller.hxx"
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
using namespace org_scilab_modules_scicos;

int sci_xcosPalGenerateIcon(char *fname, void* pvApiCtx)
{
    CheckRhs(1, 1);
    CheckLhs(0, 1);

    char *iconPath = NULL;

    /* iconPath setup */
    if (readSingleString(pvApiCtx, 1, &iconPath, fname))
    {
        return 0;
    }

    /* Call the java implementation */
    set_loaded_status(XCOS_CALLED);
    try
    {
        Controller controller;
        Palette::generatePaletteIcon(getScilabJavaVM(), controller.createObject(DIAGRAM), iconPath);
    }
    catch (GiwsException::JniCallMethodException &exception)
    {
        Scierror(999, "%s: %s\n", fname, exception.getJavaDescription().c_str());
    }
    catch (GiwsException::JniException &exception)
    {
        Scierror(999, "%s: %s\n", fname, exception.whatStr().c_str());
    }

    FREE(iconPath);

    LhsVar(1) = 0;
    PutLhsVar();
    return 0;
}
