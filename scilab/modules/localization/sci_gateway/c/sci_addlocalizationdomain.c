/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2013 - Scilab Enterprises - Antoine ELIAS
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
#include <stdlib.h>
#include "sci_malloc.h"
#include "localization.h"
#include "gw_localization.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "strsubst.h"
#include "freeArrayOfString.h"
#include "expandPathVariable.h"

int sci_addlocalizationdomain(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int* piAddr1 = NULL;
    char* pstDomain = NULL;

    int* piAddr2 = NULL;
    char* pstPath = NULL;
    char* expandedPath = NULL;

    char* pstRet = NULL;

    int iRhs = nbInputArgument(pvApiCtx);
    int iLhs = nbOutputArgument(pvApiCtx);


    CheckInputArgument(pvApiCtx, 2, 2);
    CheckOutputArgument(pvApiCtx, 1, 1);

    //get domain name
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), "addlocalizationdomain", 1);
        return 0;
    }

    if (isStringType(pvApiCtx, piAddr1) == 0 || isScalar(pvApiCtx, piAddr1) == 0)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: String expected.\n"), "addlocalizationdomain", 1);
        return 0;
    }

    if (getAllocatedSingleString(pvApiCtx, piAddr1, &pstDomain))
    {
        Scierror(999, _("%s: Memory allocation error.\n"), "addlocalizationdomain");
        return 0;
    }

    //get domain path
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), "addlocalizationdomain", 2);
        freeAllocatedSingleString(pstDomain);
        return 0;
    }

    if (isStringType(pvApiCtx, piAddr2) == 0 || isScalar(pvApiCtx, piAddr2) == 0)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: String expected.\n"), "addlocalizationdomain", 2);
        freeAllocatedSingleString(pstDomain);
        return 0;
    }

    if (getAllocatedSingleString(pvApiCtx, piAddr2, &pstPath))
    {
        Scierror(999, _("%s: Memory allocation error.\n"), "addlocalizationdomain");
        freeAllocatedSingleString(pstDomain);
        return 0;
    }

    expandedPath = expandPathVariable(pstPath);
    pstRet = bindtextdomain(pstDomain, expandedPath);

    freeAllocatedSingleString(pstPath);
    FREE(expandedPath);

    if (pstRet == NULL)
    {
        Scierror(999, _("%s: Unable to add new domain %s.\n"), "addlocalizationdomain", pstDomain);
        freeAllocatedSingleString(pstDomain);
        return 0;
    }

    bind_textdomain_codeset (pstDomain, "UTF-8"); /*such that gettext and dgettext return UTF8 string*/

    if (createScalarBoolean(pvApiCtx, iRhs + 1, 1))
    {
        Scierror(999, _("%s: Unable to add new domain %s.\n"), "addlocalizationdomain", pstDomain);
        freeAllocatedSingleString(pstDomain);
        return 0;
    }

    freeAllocatedSingleString(pstDomain);
    AssignOutputVariable(pvApiCtx, 1) = iRhs + 1;
    ReturnArguments(pvApiCtx);
    return 0;
}
