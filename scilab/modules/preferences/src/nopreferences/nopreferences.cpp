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
/*--------------------------------------------------------------------------*/
#include "preferences_gw.hxx"
extern "C"
{
#include "Scierror.h"
#include "localization.h"
}
/*--------------------------------------------------------------------------*/
int gw_preferences(void)
{
    Scierror(999, _("Scilab '%s' module not installed.\n"), "preferences");
    return 0;
}
/*--------------------------------------------------------------------------*/
int sci_addModulePreferences(char * fname, void *pvApiCtx)
{
    return gw_preferences();
}
int sci_preferences(char * fname, void *pvApiCtx)
{
    return gw_preferences();
}
int sci_removeModulePreferences(char * fname, void *pvApiCtx)
{
    return gw_preferences();
}
/*--------------------------------------------------------------------------*/
