
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA -
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include <string.h>
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "sci_rankqr.h"
#include "gw_slicot.h"
/*--------------------------------------------------------------------------*/
int sci_rankqr(char* fname, unsigned long fname_len)
{
    SciErr sciErr;
    int* piAddr = 0;
    int ret = 0;

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (isVarComplex(pvApiCtx, piAddr))
    {
        ret = sci_zrankqr("rankqr", 6L);
        return ret;
    }
    else
    {
        ret = sci_rrankqr("rankqr", 6L);
        return ret;
    }
}
/*--------------------------------------------------------------------------*/
