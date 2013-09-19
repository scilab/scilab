/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "gw_preferences.h"
#include "api_scilab.h"
#include "callFunctionFromGateway.h"
#include "BOOL.h"
#include "scilabmode.h"
#include "localization.h"
#include "Scierror.h"
#include "BOOL.h"
#include "loadOnUseClassPath.h"
/*--------------------------------------------------------------------------*/
static BOOL loadedDep = FALSE;
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[] =
{
    {sci_addModulePreferences, "addModulePreferences"},
    {sci_removeModulePreferences, "removeModulePreferences"},
    {sci_preferences, "preferences"}
};
/*--------------------------------------------------------------------------*/
int gw_preferences(void)
{
    Rhs = Max(0, Rhs);

    if ( getScilabMode() == SCILAB_NWNI)
    {
        Scierror(999, _("Scilab '%s' module disabled in -nogui or -nwni mode.\n"), "preferences");
        return 0;
    }

    if (!loadedDep)
    {
        loadOnUseClassPath("preferences");
        loadedDep = TRUE;
    }
    callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
    return 0;
}
/*--------------------------------------------------------------------------*/
