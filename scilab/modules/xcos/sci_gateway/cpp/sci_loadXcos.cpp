/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2012 - Scilal Enterprises - Clément DAVID
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
#include "gw_xcos.h"
#include "api_scilab.h"
}

/*
 * Stub function to load the gateway
 */
int sci_loadXcos(char *fname, unsigned long fname_len)
{
    CheckRhs(0, 1);
    LhsVar(1) = 0;
    PutLhsVar();
    return 0;
}

