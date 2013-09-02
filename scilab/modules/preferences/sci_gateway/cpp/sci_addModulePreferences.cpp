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
#include "expandPathVariable.h"
#include "MALLOC.h"
}

using namespace org_scilab_modules_preferences;

/*--------------------------------------------------------------------------*/
int sci_addModulePreferences(char * fname, unsigned long fname_len)
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
            Scierror(999, gettext("%s: Wrong type for input argument #%i: A string expected.\n"), fname, i + 1);
            for (int j = 0; j < i; j++)
            {
                if (array[j])
                {
                    freeAllocatedSingleString(*(array[j]));
                }
            }
            return 0;
        }

        getAllocatedSingleString(pvApiCtx, addr, array[i]);
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
