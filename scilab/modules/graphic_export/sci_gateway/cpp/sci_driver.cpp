/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include <string.h>

#include "ScilabView.hxx"
#include "SwingView.hxx"
#include "Driver.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "localization.h"
#include "deleteGraphicObject.h"

#include "gw_graphic_export.h"
}


#if _MSC_VER
#define strcasecmp _stricmp
#endif

/*--------------------------------------------------------------------------*/
int sci_driver(char * fname, unsigned long fname_len )
{
    SciErr err;
    int * addr = NULL;
    char * driver = NULL;
    char * previous_driver = NULL;
    bool ok = true;

    CheckRhs(0, 1);

    // Get current driver
    previous_driver = org_scilab_modules_graphic_export::Driver::getDriver(getScilabJavaVM());
    createSingleString(pvApiCtx, Rhs + 1, previous_driver);
    freeAllocatedSingleString(previous_driver);

    if (Rhs == 1) // Change driver if applicable
    {
        err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
        if (err.iErr)
        {
            printError(&err, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
            return 0;
        }

        if (!isStringType(pvApiCtx, addr) || !checkVarDimension(pvApiCtx, addr, 1, 1))
        {
            Scierror(999, gettext("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
            return 0;
        }

        if (getAllocatedSingleString(pvApiCtx, addr, &driver) != 0)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }

        ok = org_scilab_modules_graphic_export::Driver::setDriver(getScilabJavaVM(), driver);
        if (!ok)
        {
            Scierror(999, _("%s: Invalid driver: %s.\n"), fname, driver);
            freeAllocatedSingleString(driver);
            return 0;
        }

        if (strcasecmp(driver, "X11") && strcasecmp(driver, "Rec"))
        {
            org_scilab_modules_gui::SwingView::setHeadless(getScilabJavaVM(), true);
        }
        else if (org_scilab_modules_gui::SwingView::isHeadless(getScilabJavaVM()))
        {
            org_scilab_modules_gui::SwingView::setHeadless(getScilabJavaVM(), false);
        }

        freeAllocatedSingleString(driver);
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
