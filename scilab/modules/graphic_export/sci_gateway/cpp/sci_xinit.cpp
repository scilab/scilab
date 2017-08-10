/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
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


#include "Driver.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "localization.h"
#include "expandPathVariable.h"
#include "sci_malloc.h"

#include "gw_graphic_export.h"
}

/*--------------------------------------------------------------------------*/
int sci_xinit(char * fname, void *pvApiCtx)
{
    SciErr err;
    int * addr = 0;
    char * path = 0;
    char * realPath = 0;

    CheckInputArgument(pvApiCtx, 1, 1);

    err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (!isStringType(pvApiCtx, addr) || !checkVarDimension(pvApiCtx, addr, 1, 1))
    {
        Scierror(999, gettext("%s: Wrong type for input argument #%d: string expected.\n"), fname, 1);
        return 0;
    }

    if (getAllocatedSingleString(pvApiCtx, addr, &path) != 0)
    {
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 0;
    }

    realPath = expandPathVariable(path);

    if (realPath)
    {
        org_scilab_modules_graphic_export::Driver::setPath(getScilabJavaVM(), realPath);
        FREE(realPath);
    }
    else
    {
        Scierror(999, _("%s: Invalid path: %s.\n"), fname, path);
        freeAllocatedSingleString(path);
        return 0;
    }

    freeAllocatedSingleString(path);

    LhsVar(1) = 0;
    PutLhsVar();

    return 0;
}
/*--------------------------------------------------------------------------*/
