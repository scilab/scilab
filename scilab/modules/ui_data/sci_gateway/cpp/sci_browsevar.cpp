/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

extern "C"
{
#include "gw_ui_data.h"
#include "api_scilab.h"
#include "BrowseVarManager.h"
}
/*--------------------------------------------------------------------------*/
int sci_browsevar(char *fname, unsigned long fname_len)
{
    CheckRhs(0, 0);
    CheckLhs(0, 1);

    OpenBrowseVar();

    LhsVar(1) = 0;
    PutLhsVar();

    return 0;
}
