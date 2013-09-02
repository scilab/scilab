/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "ScilabPreferences.hxx"
#include "GiwsException.hxx"

extern "C"
{
#include "gw_preferences.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "localization.h"
#include "getScilabJavaVM.h"
}

using namespace org_scilab_modules_preferences;

/*--------------------------------------------------------------------------*/
int sci_removeModulePreferences(char * fname, unsigned long fname_len)
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
            Scierror(999, gettext("%s: Wrong type for input argument #%i: A string expected.\n"), fname, 1);
        }
        getAllocatedSingleString(pvApiCtx, addr, &tbxName);
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
