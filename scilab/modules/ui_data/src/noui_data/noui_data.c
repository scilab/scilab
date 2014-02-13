/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
#include "scilabmode.h"
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
void UpdateBrowseVar(BOOL b)
{
    return;
}
/*--------------------------------------------------------------------------*/
void FileBrowserChDir(char * baseDir)
{
    return;
}
/*--------------------------------------------------------------------------*/
