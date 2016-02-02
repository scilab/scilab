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
