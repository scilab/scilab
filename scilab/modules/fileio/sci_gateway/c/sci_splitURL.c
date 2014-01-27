/*
 * Copyright (C) 2011 - DIGITEO - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include <stdio.h>
#include <libxml/uri.h>
#include "sciprint.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "localization.h"

/* ==================================================================== */
int sci_splitURL(char *fname, int fname_len)
{
    SciErr sciErr;
    int length = 0;
    int *piAddr = NULL;
    char *url = NULL;
    char *dest = NULL;

    int iRows = 0, iCols = 0;
    int iType = 0;
    int *piAddressVarOne = NULL;
    char *emptyString = "";
    int ret = 0;
    xmlURIPtr c = NULL;
    int zero = 0;

    int iRhs = nbInputArgument(pvApiCtx);

    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 0, 7);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if (getAllocatedSingleString(pvApiCtx, piAddressVarOne, &url))
    {
        freeAllocatedSingleString(url);
        Scierror(999, _("%s: Wrong type for argument #%d: A string expected.\n"), fname, 1);
        return 0;
    }

    c = xmlParseURI(url);
    freeAllocatedSingleString(url);
    if (c == NULL)
    {
        Scierror(999, "Could not parse the URL.\n");
        return 0;
    }

    // protocol (http, ftp...)
    if (c->scheme != NULL)
    {
        ret = createSingleString(pvApiCtx, iRhs + 1, c->scheme);
    }
    else
    {
        ret = createSingleString(pvApiCtx, iRhs + 1, emptyString);
    }

    if (ret)
    {
        goto err;
    }

    AssignOutputVariable(pvApiCtx, 1) = iRhs + 1;

    // server
    if (c->server != NULL)
    {
        ret =  createSingleString(pvApiCtx, iRhs + 2, c->server);
    }
    else
    {
        ret = createSingleString(pvApiCtx, iRhs + 2, emptyString);
    }

    if (ret)
    {
        goto err;
    }

    AssignOutputVariable(pvApiCtx, 2) = iRhs + 2;

    // path
    if (c->path != NULL)
    {
        ret = createSingleString(pvApiCtx, iRhs + 3, c->path);
    }
    else
    {
        ret = createSingleString(pvApiCtx, iRhs + 3, emptyString);
    }

    if (ret)
    {
        goto err;
    }

    AssignOutputVariable(pvApiCtx, 3) = iRhs + 3;

    // query
    if (c->query != NULL)
    {
        ret = createSingleString(pvApiCtx, iRhs + 4, c->query);
    }
    else
    {
        ret = createSingleString(pvApiCtx, iRhs + 4, emptyString);
    }

    if (ret)
    {
        goto err;
    }

    AssignOutputVariable(pvApiCtx, 4) = iRhs + 4;

    // user
    if (c->user != NULL)
    {
        ret = createSingleString(pvApiCtx, iRhs + 5, c->user);
    }
    else
    {
        ret = createSingleString(pvApiCtx, iRhs + 5, emptyString);
    }

    if (ret)
    {
        goto err;
    }

    AssignOutputVariable(pvApiCtx, 5) = iRhs + 5;

    // port
    if (createScalarInteger32(pvApiCtx, iRhs + 6, c->port))
    {
        goto err;
    }

    AssignOutputVariable(pvApiCtx, 6) = iRhs + 6;

    // Fragment
    if (c->fragment != NULL)
    {
        ret = createSingleString(pvApiCtx, iRhs + 7, c->fragment);
    }
    else
    {
        ret = createSingleString(pvApiCtx, iRhs + 7, emptyString);
    }

    if (ret)
    {
        goto err;
    }

    xmlFreeURI(c);
    AssignOutputVariable(pvApiCtx, 7) = iRhs + 7;
    ReturnArguments(pvApiCtx);
    return 0;

err:
    xmlFreeURI(c);
    printError(&sciErr, 0);
    return 0;

}

/*
probably useless or internal to libxml
// opaque
if (c->opaque != NULL)
{
sciErr = createMatrixOfString(pvApiCtx, iRhs + 5, 1, 1, &c->opaque);
}
else
{
sciErr = createMatrixOfString(pvApiCtx, iRhs + 5, 1, 1, &emptyString);
}
if (sciErr.iErr)
{
printError(&sciErr, 0);
return 0;
}
AssignOutputVariable(pvApiCtx, 5) = iRhs + 5;

// authority
if (c->authority != NULL)
{
sciErr = createMatrixOfString(pvApiCtx, iRhs + 6, 1, 1, &c->authority);
}
else
{
sciErr = createMatrixOfString(pvApiCtx, iRhs + 6, 1, 1, &emptyString);
}
if (sciErr.iErr)
{
printError(&sciErr, 0);
return 0;
}
AssignOutputVariable(pvApiCtx, 6) = iRhs + 6;

// Clean up
sciErr = createMatrixOfInteger32(pvApiCtx, iRhs + 10, 1, 1, &c->cleanup);
if (sciErr.iErr)
{
printError(&sciErr, 0);
return 0;
}
AssignOutputVariable(pvApiCtx, 10) = iRhs + 10;

*/
