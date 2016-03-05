/*
 * Copyright (C) 2011 - DIGITEO - Sylvestre LEDRU
 * Copyright (C) 2013 - Scilab Enterprises - Sylvestre LEDRU
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
#include <stdio.h>
#include <string.h>
#include "sciprint.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "sci_malloc.h"
#include "dlManager.h"
#include "localization.h"
/* ==================================================================== */
static void freeAllocatedStrings(char** url, char** dest, char** username, char** password, char** content, char** filePath);
/* ==================================================================== */
int sci_getURL(char *fname, void* pvApiCtx)
{
    SciErr sciErr;

    char *url = NULL;
    char *dest = NULL;
    char *username = NULL;
    char *password = NULL;
    char *content = NULL;

    int *piAddressVarOne = NULL;
    int ret = 0;
    char *filePath = NULL;

    int iRhs = nbInputArgument(pvApiCtx);

    CheckInputArgument(pvApiCtx, 1, 4);
    CheckOutputArgument(pvApiCtx, 0, 2);


    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    ret = getAllocatedSingleString(pvApiCtx, piAddressVarOne, &url);
    if (ret)
    {
        Scierror(999, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 1);
        freeAllocatedStrings(&url, &dest, &username, &password, &content, &filePath);
        return 0;
    }

    if (iRhs > 1)
    {
        //Specify the destination
        int *piAddressVarTwo = NULL;

        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            freeAllocatedStrings(&url, &dest, &username, &password, &content, &filePath);
            return 0;
        }

        ret = getAllocatedSingleString(pvApiCtx, piAddressVarTwo, &dest);
        if (ret)
        {
            Scierror(999, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 2);
            freeAllocatedStrings(&url, &dest, &username, &password, &content, &filePath);
            return 0;
        }

    }
    if (iRhs > 2)
    {
        //Specify the username
        int *piAddressVarThree = NULL;

        sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddressVarThree);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            freeAllocatedStrings(&url, &dest, &username, &password, &content, &filePath);
            return 0;
        }

        ret = getAllocatedSingleString(pvApiCtx, piAddressVarThree, &username);
        if (ret)
        {
            Scierror(999, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 3);
            freeAllocatedStrings(&url, &dest, &username, &password, &content, &filePath);
            return 0;
        }

    }

    if (iRhs > 3)
    {
        //Specify the password
        int *piAddressVarFour = NULL;

        sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddressVarFour);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            freeAllocatedStrings(&url, &dest, &username, &password, &content, &filePath);
            return 0;
        }

        ret = getAllocatedSingleString(pvApiCtx, piAddressVarFour, &password);
        if (ret)
        {
            Scierror(999, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 4);
            freeAllocatedStrings(&url, &dest, &username, &password, &content, &filePath);
            return 0;
        }

    }

    //Set to NULL if 0 length strings
    if (url != NULL && strlen(url) == 0)
    {
        FREE(url);
        url = NULL;
    }

    if (dest != NULL && strlen(dest) == 0)
    {
        FREE(dest);
        dest = NULL;
    }

    if (username != NULL && strlen(username) == 0)
    {
        FREE(username);
        username = NULL;
    }

    if (password != NULL && strlen(password) == 0)
    {
        FREE(password);
        password = NULL;
    }

    // call function
    filePath = downloadFile(url, dest, username, password, &content);
    if (filePath)
    {
        //Return the second argument which si the file name
        ret = createSingleString(pvApiCtx, iRhs + 1, filePath);
        FREE(filePath);
        filePath = NULL;
        if (ret)
        {
            Scierror(999, _("%s: Could not create the output argument.\n"));
            freeAllocatedStrings(&url, &dest, &username, &password, &content, &filePath);
            return 0;
        }
    }

    //Return the second argument which is the content
    if (content && nbOutputArgument(pvApiCtx) == 2)
    {
        //create new variable with the content
        int res = createSingleString(pvApiCtx, iRhs + 2, content);
        if (res)
        {
            Scierror(999, _("%s: Could not create the output argument.\n"));
            freeAllocatedStrings(&url, &dest, &username, &password, &content, &filePath);
            return 0;
        }
    }

    freeAllocatedStrings(&url, &dest, &username, &password, &content, &filePath);

    AssignOutputVariable(pvApiCtx, 1) = iRhs + 1;
    AssignOutputVariable(pvApiCtx, 2) = iRhs + 2;
    ReturnArguments(pvApiCtx);
    return 0;
}
/* ==================================================================== */
static void freeAllocatedStrings(char** url, char** dest, char** username, char** password, char** content, char** filePath)
{
    if (url && *url)
    {
        freeAllocatedSingleString(*url);
        *url = NULL;
    }

    if (dest && *dest)
    {
        freeAllocatedSingleString(*dest);
        *dest = NULL;
    }

    if (username && *username)
    {
        freeAllocatedSingleString(*username);
        *username = NULL;
    }

    if (password && *password)
    {
        freeAllocatedSingleString(*password);
        *password = NULL;
    }

    if (content && *content)
    {
        FREE(*content);
        *content = NULL;
    }

    if (filePath && *filePath)
    {
        FREE(*filePath);
        *filePath = NULL;
    }
}

