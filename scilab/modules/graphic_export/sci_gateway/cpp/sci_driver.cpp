/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string.h>

extern "C"
{
#include "getScilabJavaVM.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "localization.h"

#include "gw_graphic_export.h"
}

#include "SwingView.hxx"
#include "Driver.hxx"

#if _MSC_VER
#define strcasecmp _stricmp
#endif

/*--------------------------------------------------------------------------*/
int sci_driver(char * fname, unsigned long fname_len )
{
    SciErr err;
    int * addr = 0;
    char * driver = 0;
    bool ok = true;

    CheckRhs(0, 1);

    if (Rhs == 1) // Change driver
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

        org_scilab_modules_gui::SwingView::setHeadless(getScilabJavaVM(), strcasecmp(driver, "X11") && strcasecmp(driver, "Rec"));

        createSingleString(pvApiCtx, Rhs + 1, driver);

        freeAllocatedSingleString(driver);
    }

    if (Rhs == 0) // Get current driver
    {
        driver = org_scilab_modules_graphic_export::Driver::getDriver(getScilabJavaVM());

        createSingleString(pvApiCtx, Rhs + 1, driver);
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
