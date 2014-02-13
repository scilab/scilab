/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include "Scierror.h"
#include "localization.h"
#include "scilabmode.h"
/*--------------------------------------------------------------------------*/
int gw_action_binding()
{
    if (getScilabMode() == SCILAB_NWNI)
    {
        Scierror(999, _("Scilab '%s' module disabled in -nogui or -nwni mode.\n"), "action_binding");
    }
    else
    {
        Scierror(999, _("Scilab '%s' module not installed.\n"), "action_binding");
    }
    return 0;
}
