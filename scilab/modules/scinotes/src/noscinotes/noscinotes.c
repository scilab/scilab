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
#include "noscinotes.h"
#include "Scierror.h"
#include "localization.h"
#include "scilabmode.h"
/*--------------------------------------------------------------------------*/
int gw_scinotes(void)
{
    if (getScilabMode() == SCILAB_NWNI)
    {
        Scierror(999, _("Scilab '%s' module disabled in -nogui or -nwni mode.\n"), "scinotes");
    }
    else
    {
        Scierror(999, _("Scilab '%s' module not installed.\n"), "scinotes");
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
