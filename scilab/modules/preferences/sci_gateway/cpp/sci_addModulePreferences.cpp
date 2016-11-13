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
#include "expandPathVariable.h"
#include "sci_malloc.h"
}

using namespace org_scilab_modules_preferences;

/*--------------------------------------------------------------------------*/
int sci_addModulePreferences(char * fname, void *pvApiCtx)
{
    SciErr err;
    int * addr = 0;
    char * tbxName = 0;
    char * tbxPath = 0;
    char * expTbxPath = 0;
    char * tbxPrefFile = 0;
    char * expTbxPrefFile = 0;
    char ** array[] = {&tbxName, &tbxPath, &tbxPrefFile};
    bool error = false;

    CheckLhs(1, 1);
    CheckRhs(3, 3);

    for (int i = 0; i < Rhs; i++)
    {
        err = getVarAddressFromPosition(pvApiCtx, i + 1, &addr);
        if (err.iErr)
        {
            printError(&err, 0);
            return 0;
        }

        if (!isStringType(pvApiCtx, addr))
        {
            Scierror(999, gettext("%s: Wrong type for input argument #%i: string expected.\n"), fname, i + 1);
            for (int j = 0; j < i; j++)
            {
                if (array[j])
                {
                    freeAllocatedSingleString(*(array[j]));
                }
            }
            return 0;
        }

        if (getAllocatedSingleString(pvApiCtx, addr, array[i]))
        {
            return 0;
        }
    }

    expTbxPath = expandPathVariable(const_cast<char *>(tbxPath));
    expTbxPrefFile = expandPathVariable(const_cast<char *>(tbxPrefFile));

    try
    {
        ScilabPreferences::addToolboxInfos(getScilabJavaVM(), tbxName, expTbxPath, expTbxPrefFile);
    }
    catch (const GiwsException::JniException & e)
    {
        Scierror(999, "%s: %s", fname, e.getJavaDescription().c_str());
        for (int i = 0; i < Rhs; i++)
        {
            if (array[i])
            {
                freeAllocatedSingleString(*(array[i]));
            }
        }
        FREE(expTbxPath);
        FREE(expTbxPrefFile);

        return 0;
    }

    for (int i = 0; i < Rhs; i++)
    {
        if (array[i])
        {
            freeAllocatedSingleString(*(array[i]));
        }
    }
    FREE(expTbxPath);
    FREE(expTbxPrefFile);

    LhsVar(1) = 0;
    PutLhsVar();

    return 0;
}
/*--------------------------------------------------------------------------*/
