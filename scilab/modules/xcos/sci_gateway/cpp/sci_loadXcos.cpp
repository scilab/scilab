/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2012 - Scilal Enterprises - Clement DAVID
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

extern "C"
{
#include "gw_xcos.h"
#include "api_scilab.h"
}

/*
 * Stub function to load the gateway
 */
int sci_loadXcos(char *fname, void *pvApiCtx)
{
    CheckRhs(0, 1);
    LhsVar(1) = 0;
    PutLhsVar();
    return 0;
}
