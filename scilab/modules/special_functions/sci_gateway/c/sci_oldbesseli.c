/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA - Allan CORNET <allan.cornet@inria.fr>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string.h>
#include "gw_special_functions.h"
#include "machine.h"
#include "warningmode.h"
#include "localization.h"
#include "sciprint.h"
/*--------------------------------------------------------------------------*/
extern int C2F(intsbesseli)(char *id,unsigned long fname_len);
/*--------------------------------------------------------------------------*/
int sci_oldbesseli(char *fname,unsigned long fname_len)
{
    if (getWarningMode())
    {
        sciprint(_("Warning: "));
        sciprint(_("Function %s is obsolete."), fname);
        sciprint("\n");
        sciprint(_("Warning: "));
        sciprint(_("Please use %s instead."), "besseli");
        sciprint("\n");
        sciprint(_("Warning: "));
        sciprint(_("This function will be permanently removed in Scilab %s"), "5.3.1");
        sciprint("\n");
    }

    C2F(intsbesseli)(fname,fname_len);
    return 0;
}
/*--------------------------------------------------------------------------*/
