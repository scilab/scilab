/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009-2012 - DIGITEO - Allan CORNET
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

#ifndef _MSC_VER
#include <errno.h>
#include <string.h>
#else
#include <windows.h>
#endif
#include "gw_fileio.h"
#include "sci_malloc.h"
#include "localization.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "FileExist.h"
#include "movefile.h"
#include "isdir.h"
#include "splitpath.h"
#include "freeArrayOfString.h"
#include "charEncoding.h"
#include "expandPathVariable.h"
#include "os_string.h"
/*--------------------------------------------------------------------------*/
static char* getFilenameWithExtensionForMove(char*  fullFilename);
static int returnMoveFileResultOnStack(int ierr, char *fname, void* pvApiCtx);
/*--------------------------------------------------------------------------*/
int sci_movefile(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int *piAddressVarOne = NULL;
    char* pStVarOne = NULL;
    char* pStVarOneExpanded = NULL;

    int *piAddressVarTwo = NULL;
    char* pStVarTwo = NULL;
    char* pStVarTwoExpanded = NULL;

    /* Check Input & Output parameters */
    CheckRhs(2, 2);
    CheckLhs(1, 2);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (isStringType(pvApiCtx, piAddressVarOne) == 0 || isScalar(pvApiCtx, piAddressVarOne) == 0)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 1);
        return 0;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return 0;
    }

    if (isStringType(pvApiCtx, piAddressVarTwo) == 0 || isScalar(pvApiCtx, piAddressVarTwo) == 0)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 2);
        return 0;
    }

    if (getAllocatedSingleString(pvApiCtx, piAddressVarOne, &pStVarOne))
    {
        if (pStVarOne)
        {
            freeAllocatedSingleString(pStVarOne);
        }

        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    if (getAllocatedSingleString(pvApiCtx, piAddressVarTwo, &pStVarTwo))
    {
        freeAllocatedSingleString(pStVarOne);
        freeAllocatedSingleString(pStVarTwo);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    pStVarOneExpanded = expandPathVariable(pStVarOne);
    pStVarTwoExpanded = expandPathVariable(pStVarTwo);

    freeAllocatedSingleString(pStVarOne);
    freeAllocatedSingleString(pStVarTwo);

    if (isdir(pStVarOneExpanded) || FileExist(pStVarOneExpanded))
    {
        int ierrMove = 0;

        if (isdir(pStVarOneExpanded))
        {
            /* move a directory into a directory */
            ierrMove = MoveDirectoryFunction(pStVarTwoExpanded, pStVarOneExpanded);
        }
        else if (FileExist(pStVarOneExpanded))
        {
            if (isdir(pStVarTwoExpanded))
            {
                /* move file into a existing directory */
                /* copy file into a existing directory */
                char* filename = getFilenameWithExtensionForMove(pStVarOneExpanded);

                if (filename)
                {
#define FORMAT_FULLFILENAME "%s/%s"
                    char* destFullFilename = NULL;

                    /* remove last file separator if it exists */
                    if ((pStVarTwoExpanded[strlen(pStVarTwoExpanded) - 1] == '\\') || (pStVarTwoExpanded[strlen(pStVarTwoExpanded) - 1] == '/'))
                    {
                        pStVarTwoExpanded[strlen(pStVarTwoExpanded) - 1] = '\0';
                    }

                    destFullFilename = (char*)MALLOC(sizeof(char) * ((int)strlen(pStVarTwoExpanded) + (int)strlen(filename) + (int)strlen("/") + 1));
                    strcpy(destFullFilename, pStVarTwoExpanded);
                    strcat(destFullFilename, "/");
                    strcat(destFullFilename, filename);

                    ierrMove = MoveFileFunction(destFullFilename, pStVarOneExpanded);

                    FREE(filename);
                    filename = NULL;
                    FREE(destFullFilename);
                    destFullFilename = NULL;
                }
                else
                {
                    FREE(pStVarOneExpanded);
                    FREE(pStVarTwoExpanded);
                    Scierror(999, _("%s: Memory allocation error.\n"), fname);
                    return 0;
                }
            }
            else
            {
                /* move a file into a file */
                ierrMove = MoveFileFunction(pStVarTwoExpanded, pStVarOneExpanded);
            }
        }
        else
        {
            FREE(pStVarOneExpanded);
            FREE(pStVarTwoExpanded);
            Scierror(999, _("%s: Wrong value for input argument #%d: A valid filename or directory expected.\n"), fname, 1);
            return 0;
        }

        returnMoveFileResultOnStack(ierrMove, fname, pvApiCtx);
    }
    else
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: A valid filename or directory expected.\n"), fname, 1);
    }

    FREE(pStVarOneExpanded);
    FREE(pStVarTwoExpanded);

    return 0;
}

/*--------------------------------------------------------------------------*/
static char* getFilenameWithExtensionForMove(char*  fullFilename)
{
    char* wcfilename = NULL;

    if (fullFilename)
    {
        int len = (int)strlen(fullFilename);
        char* wcdrv = (char*)MALLOC(sizeof(char) * (len + 1));
        char* wcdir = (char*)MALLOC(sizeof(char) * (len + 1));
        char* wcname = (char*)MALLOC(sizeof(char) * (len + 1));
        char* wcext = (char*)MALLOC(sizeof(char) * (len + 1));

        wcfilename = (char*)MALLOC(sizeof(char) * (len + 1));

        if (wcdrv == NULL || wcdir == NULL || wcname == NULL || wcext == NULL || wcfilename == NULL)
        {
            FREE(wcdrv);
            FREE(wcdir);
            FREE(wcname);
            FREE(wcext);
            FREE(wcfilename);
            return NULL;
        }

        splitpath(fullFilename, FALSE, wcdrv, wcdir, wcname, wcext);

        strcpy(wcfilename, wcname);
        strcat(wcfilename, wcext);

        FREE(wcdrv);
        FREE(wcdir);
        FREE(wcname);
        FREE(wcext);
    }
    return wcfilename;
}
/*--------------------------------------------------------------------------*/
static int returnMoveFileResultOnStack(int ierr, char *fname, void* pvApiCtx)
{
    double dError = 0.;
    char* sciError = NULL;

#ifdef _MSC_VER
    if (ierr)
    {
#define BUFFER_SIZE 1024
        DWORD dw = GetLastError();
        char buffer[BUFFER_SIZE];

        if (FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL,
                           dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buffer, BUFFER_SIZE, NULL) == 0)
        {
            strcpy(buffer, "Unknown Error");
        }

        // for compatibilty with copyfile, we return 0 (error)
        //dError = (double) dw;
        dError = (double)0;

        sciError = (char* ) MALLOC(sizeof(char) * ((int)strlen(buffer) + 1));
        if (sciError == NULL)
        {
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }

        strcpy(sciError, buffer);
    }
    else
    {
        dError = 1.;
        sciError = (char* ) MALLOC(sizeof(char) * 1);
        if (sciError == NULL)
        {
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }

        strcpy(sciError, "");
    }
#else
    if (ierr)
    {
        // for compatibilty with copyfile, we return 0 (error)
        //dError = (double) ierr;
        dError = (double)0.;
        sciError = strerror(errno);
    }
    else
    {
        dError = 1.;
        sciError = os_strdup("");
    }
#endif

    createScalarDouble(pvApiCtx, Rhs + 1, dError);
    LhsVar(1) = Rhs + 1;

    if (Lhs == 2)
    {
        createSingleString(pvApiCtx, Rhs + 2, sciError);
        LhsVar(2) = Rhs + 2;
    }

    FREE(sciError);

    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
