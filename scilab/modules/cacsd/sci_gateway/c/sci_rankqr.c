
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA -
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
#include <string.h>
#include "gw_cacsd.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "sci_rankqr.h"
/*--------------------------------------------------------------------------*/
int sci_rankqr(char* fname, void* pvApiCtx)
{
    SciErr sciErr;
    int* piAddr = 0;
    int ret = 0;

    CheckInputArgument(pvApiCtx, 1, 3);
    CheckOutputArgument(pvApiCtx, 1, 5);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (isVarComplex(pvApiCtx, piAddr))
    {
        ret = sci_zrankqr("rankqr", pvApiCtx);
        return ret;
    }
    else
    {
        ret = sci_rrankqr("rankqr", pvApiCtx);
        return ret;
    }
}
/*--------------------------------------------------------------------------*/
