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
