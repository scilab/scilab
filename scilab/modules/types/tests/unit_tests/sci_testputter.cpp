/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "TestS2JJ2S.hxx"
#include "GiwsException.hxx"
#include "ScilabToJava.hxx"

extern "C"
{
#include "api_scilab.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "freeArrayOfString.h"
#include "getScilabJavaVM.h"

    int sci_testputter(char *fname, unsigned long fname_len);
}

/*--------------------------------------------------------------------------*/
int sci_testputter(char *fname, unsigned long fname_len)
{
    static int handlerId = TestS2JJ2S::getId(getScilabJavaVM());

    SciErr err;
    const int nbIn = nbInputArgument(pvApiCtx);
    int * addr = 0;

    CheckOutputArgument(pvApiCtx, 1, 1);

    err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    if (err.iErr)
    {
        Scierror(999, "%s: Can not read input argument #%d.\n", fname, 1);
        return 0;
    }

    org_modules_types::ScilabToJava::sendVariable("", addr, false, handlerId, pvApiCtx);

    try
    {
        org_scilab_modules_types::TestS2JJ2S::put(getScilabJavaVM(), nbIn + 1);
    }
    catch (const GiwsException::JniException & e)
    {
        Scierror(999, "%s: Java exception arisen:\n%s\n", fname, e.what());
    }

    AssignOutputVariable(pvApiCtx, 1) = nbIn + 1;
    ReturnArguments(pvApiCtx);

    return 0;
}

/*--------------------------------------------------------------------------*/
