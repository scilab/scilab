/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2014 - Scilab Enterprises - Cedric Delamarre
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
#include "sci_malloc.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int checkParam(void* _pvCtx, int _iPos, char* fname)
{
    SciErr sciErr;
    int* piAddr = NULL;

    //get var address
    sciErr = getVarAddressFromPosition(_pvCtx, _iPos, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, _iPos);
        return 1;
    }

    //check is real scalar double
    if ( isScalar(_pvCtx, piAddr) == 0 ||
            isDoubleType(_pvCtx, piAddr) == 0 ||
            isVarComplex(_pvCtx, piAddr) == 1)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: An integer value expected.\n"), fname, _iPos);
        return 1;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
char getIntegerValue(void* _pvCtx, int _iPos)
{
    SciErr sciErr;
    int* piAddr = NULL;
    double dblVal;

    //get var address
    sciErr = getVarAddressFromPosition(_pvCtx, _iPos, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), "sorder", _iPos);
        return 1;
    }

    getScalarDouble(_pvCtx, piAddr, &dblVal);
    return (char)dblVal;
}
/*--------------------------------------------------------------------------*/
