/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2014 - Scilab Enterprises - Cedric Delamarre
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
#include "gw_cacsd.h"
#include "api_scilab.h"
#include "MALLOC.h"
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
