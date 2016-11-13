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
int sci_preferences(char * fname, void *pvApiCtx)
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
