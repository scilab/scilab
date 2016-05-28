/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Sylvestre LEDRU
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
#include <string.h>
#include <stdio.h>
#include "gw_core.h"
#include "api_scilab.h"
#include "version.h"
#include "sci_malloc.h"
#include "freeArrayOfString.h"
#ifdef _MSC_VER
#include "getdynamicDebugInfo_Windows.h"
#include "getstaticDebugInfo_Windows.h"
#else
#include "getstaticdebuginfo.h"
#include "getdynamicdebuginfo.h"
#endif
/*--------------------------------------------------------------------------*/
int sci_getdebuginfo(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    char **outputDynamicList = NULL;
    char **outputStaticList = NULL;
    static int n1 = 1, m1 = 0;
    static int n2 = 1, m2 = 0;

    CheckRhs(0, 0);
    CheckLhs(0, 2);

#ifdef _MSC_VER
    outputDynamicList = getDynamicDebugInfo_Windows(&m1);
    outputStaticList = getStaticDebugInfo_Windows(&m2);
#else
    outputDynamicList = getDynamicDebugInfo(&m1, pvApiCtx);
    outputStaticList = getStaticDebugInfo(&m2);
#endif

    sciErr = createMatrixOfString(pvApiCtx, Rhs + 1, m1, n1, (char const * const*) outputDynamicList);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        freeArrayOfString(outputDynamicList, m1);
        freeArrayOfString(outputStaticList, m2);
        return 0;
    }

    LhsVar(1) = Rhs + 1;

    if (Lhs == 2)
    {
        sciErr = createMatrixOfString(pvApiCtx, Rhs + 2, m2, n2, (char const * const*) outputStaticList);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            freeArrayOfString(outputDynamicList, m1);
            freeArrayOfString(outputStaticList, m2);
            return 0;
        }

        LhsVar(2) = Rhs + 2;
    }
    freeArrayOfString(outputDynamicList, m1);
    freeArrayOfString(outputStaticList, m2);

    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
