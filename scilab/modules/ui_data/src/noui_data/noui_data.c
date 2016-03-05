/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
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
#include "noui_data.h"
#include "Scierror.h"
#include "localization.h"
#include "BrowseVarManager.h"
#include "FileBrowserChDir.h"
#include "BOOL.h"
#ifndef _MSC_VER
#include "configvariable_interface.h"
#endif
/*--------------------------------------------------------------------------*/
int gw_ui_data(void)
{
#ifndef _MSC_VER
    if (getScilabMode() == SCILAB_NWNI)
    {
        Scierror(999, _("Scilab '%s' module disabled in -nogui or -nwni mode.\n"), "ui_data");
    }
    else
#endif
    {
        Scierror(999, _("Scilab '%s' module not installed.\n"), "ui_data");
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
void UpdateBrowseVar()
{
    return;
}
/*--------------------------------------------------------------------------*/
void FileBrowserChDir(char * baseDir)
{
    return;
}
/*--------------------------------------------------------------------------*/
