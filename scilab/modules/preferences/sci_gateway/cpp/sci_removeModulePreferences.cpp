/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
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

#include "ScilabPreferences.hxx"
#include "GiwsException.hxx"
#include "preferences_gw.hxx"

extern "C"
{
#include "Scierror.h"
#include "api_scilab.h"
#include "localization.h"
#include "getScilabJavaVM.h"
}

using namespace org_scilab_modules_preferences;

/*--------------------------------------------------------------------------*/
int sci_removeModulePreferences(char * fname, void *pvApiCtx)
{
    SciErr err;
    int * addr = 0;
    char * tbxName = 0;
    bool error = false;

    CheckLhs(1, 1);
    CheckRhs(1, 1);

    err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        return 0;
    }

    if (!isEmptyMatrix(pvApiCtx, addr))
    {
        if (!isStringType(pvApiCtx, addr))
        {
            Scierror(999, gettext("%s: Wrong type for input argument #%i: string expected.\n"), fname, 1);
        }
        if (getAllocatedSingleString(pvApiCtx, addr, &tbxName))
        {
            return 0;
        }
    }

    try
    {
        ScilabPreferences::removeToolboxInfos(getScilabJavaVM(), tbxName);
    }
    catch (const GiwsException::JniException & e)
    {
        Scierror(999, _("%s: A Java exception arised:\n%s"), fname, e.what());
        error = true;
    }

    if (tbxName)
    {
        freeAllocatedSingleString(tbxName);
    }

    if (!error)
    {
        LhsVar(1) = 0;
        PutLhsVar();
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
