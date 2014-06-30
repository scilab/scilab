/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Clément DAVID
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

#include "gw_scicos.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"

/*
 * Stub function to load the gateway
 */
int sci_loadScicos(char *fname, void* pvApiCtx)
{
    CheckInputArgument(pvApiCtx, 0, 0);
    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);
    return 0;
}

