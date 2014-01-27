/*
 * Copyright (C) 2011 - DIGITEO - Sylvestre LEDRU
 * Copyright (C) 2013 - Scilab Enterprises - Sylvestre LEDRU
 * Copyright (C) 2013 - Scilab Enterprises - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#include <stdio.h>
#include "sciprint.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "dlManager.h"
#include "localization.h"
/* ==================================================================== */
static void freeAllocatedStrings(char** url, char** dest, char** username, char** password, char** content, char** filePath);
/* ==================================================================== */
int sci_getURL(char *fname, int fname_len)
{
    SciErr sciErr;
    int length = 0;

    int *piAddr = NULL;
    char *url = NULL;
    char *dest = NULL;
    char *username = NULL;
    char *password = NULL;
    char *content = NULL;

    int iRows = 0, iCols = 0;
    int iType = 0;
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
        Scierror(999, _("%s: Wrong type for argument #%d: A string expected.\n"), fname, 1);
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
            Scierror(999, _("%s: Wrong type for argument #%d: A string expected.\n"), fname, 2);
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
            Scierror(999, _("%s: Wrong type for argument #%d: A string expected.\n"), fname, 3);
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
            Scierror(999, _("%s: Wrong type for argument #%d: A string expected.\n"), fname, 4);
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

