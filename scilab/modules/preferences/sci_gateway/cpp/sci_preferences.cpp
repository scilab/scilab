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
int sci_preferences(char * fname, unsigned long fname_len)
{
    CheckRhs(0, 0);
    CheckLhs(0, 1);

    try
    {
        ScilabPreferences::openPreferences(getScilabJavaVM());
    }
    catch (const GiwsException::JniException & e)
    {
        Scierror(999, _("An error occurred while opening Preferences window:"), fname, e.getJavaDescription().c_str());
    }

    LhsVar(1) = 0;
    PutLhsVar();

    return 0;
}
