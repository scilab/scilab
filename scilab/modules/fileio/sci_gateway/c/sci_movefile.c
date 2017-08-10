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
/*--------------------------------------------------------------------------*/
static wchar_t *getFilenameWithExtensionForMove(wchar_t * wcFullFilename);
static int returnMoveFileResultOnStack(int ierr, char *fname, void* pvApiCtx);
/*--------------------------------------------------------------------------*/
int sci_movefile(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int *piAddressVarOne = NULL;
    wchar_t *pStVarOne = NULL;
    wchar_t *pStVarOneExpanded = NULL;

    int *piAddressVarTwo = NULL;
    wchar_t *pStVarTwo = NULL;
    wchar_t *pStVarTwoExpanded = NULL;

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

    if (getAllocatedSingleWideString(pvApiCtx, piAddressVarOne, &pStVarOne))
    {
        if (pStVarOne)
        {
            freeAllocatedSingleWideString(pStVarOne);
        }

        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    if (getAllocatedSingleWideString(pvApiCtx, piAddressVarTwo, &pStVarTwo))
    {
        freeAllocatedSingleWideString(pStVarOne);
        freeAllocatedSingleWideString(pStVarTwo);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    pStVarOneExpanded = expandPathVariableW(pStVarOne);
    pStVarTwoExpanded = expandPathVariableW(pStVarTwo);

    freeAllocatedSingleWideString(pStVarOne);
    freeAllocatedSingleWideString(pStVarTwo);

    if (isdirW(pStVarOneExpanded) || FileExistW(pStVarOneExpanded))
    {
        int ierrMove = 0;

        if (isdirW(pStVarOneExpanded))
        {
            /* move a directory into a directory */
            ierrMove = MoveDirectoryFunction(pStVarTwoExpanded, pStVarOneExpanded);
        }
        else if (FileExistW(pStVarOneExpanded))
        {
            if (isdirW(pStVarTwoExpanded))
            {
                /* move file into a existing directory */
                /* copy file into a existing directory */
                wchar_t *filename = getFilenameWithExtensionForMove(pStVarOneExpanded);

                if (filename)
                {
#define FORMAT_FULLFILENAME "%s/%s"
                    wchar_t *destFullFilename = NULL;

                    /* remove last file separator if it exists */
                    if ((pStVarTwoExpanded[wcslen(pStVarTwoExpanded) - 1] == L'\\') || (pStVarTwoExpanded[wcslen(pStVarTwoExpanded) - 1] == L'/'))
                    {
                        pStVarTwoExpanded[wcslen(pStVarTwoExpanded) - 1] = L'\0';
                    }

                    destFullFilename = (wchar_t *) MALLOC(sizeof(wchar_t) * ((int)wcslen(pStVarTwoExpanded) + (int)wcslen(filename) + (int)wcslen(L"/") + 1));
                    wcscpy(destFullFilename, pStVarTwoExpanded);
                    wcscat(destFullFilename, L"/");
                    wcscat(destFullFilename, filename);

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
static wchar_t *getFilenameWithExtensionForMove(wchar_t * wcFullFilename)
{
    wchar_t *wcfilename = NULL;

    if (wcFullFilename)
    {
        wchar_t *wcdrv = (wchar_t *) MALLOC(sizeof(wchar_t) * ((int)wcslen(wcFullFilename) + 1));
        wchar_t *wcdir = (wchar_t *) MALLOC(sizeof(wchar_t) * ((int)wcslen(wcFullFilename) + 1));
        wchar_t *wcname = (wchar_t *) MALLOC(sizeof(wchar_t) * ((int)wcslen(wcFullFilename) + 1));
        wchar_t *wcext = (wchar_t *) MALLOC(sizeof(wchar_t) * ((int)wcslen(wcFullFilename) + 1));

        wcfilename = (wchar_t *) MALLOC(sizeof(wchar_t) * ((int)wcslen(wcFullFilename) + 1));

        if (wcdrv == NULL || wcdir == NULL || wcname == NULL || wcext == NULL || wcfilename == NULL)
        {
            FREE(wcdrv);
            FREE(wcdir);
            FREE(wcname);
            FREE(wcext);
            FREE(wcfilename);
            return NULL;
        }

        splitpathW(wcFullFilename, FALSE, wcdrv, wcdir, wcname, wcext);

        wcscpy(wcfilename, wcname);
        wcscat(wcfilename, wcext);

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
    wchar_t *sciError = NULL;

#ifdef _MSC_VER
    if (ierr)
    {
#define BUFFER_SIZE 1024
        DWORD dw = GetLastError();
        wchar_t buffer[BUFFER_SIZE];

        if (FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL,
                           dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buffer, BUFFER_SIZE, NULL) == 0)
        {
            wcscpy(buffer, L"Unknown Error");
        }

        // for compatibilty with copyfile, we return 0 (error)
        //dError = (double) dw;
        dError = (double)0;

        sciError = (wchar_t *) MALLOC(sizeof(wchar_t) * ((int)wcslen(buffer) + 1));
        if (sciError == NULL)
        {
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }

        wcscpy(sciError, buffer);
    }
    else
    {
        dError = 1.;
        sciError = (wchar_t *) MALLOC(sizeof(wchar_t) * 1);
        if (sciError == NULL)
        {
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }
        wcscpy(sciError, L"");
    }
#else
    if (ierr)
    {
        // for compatibilty with copyfile, we return 0 (error)
        //dError = (double) ierr;
        dError = (double)0.;

        sciError = to_wide_string(strerror(errno));
        if (sciError == NULL)
        {
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }
    }
    else
    {
        dError = 1.;
        sciError = (wchar_t *) MALLOC(sizeof(wchar_t) * 1);
        if (sciError == NULL)
        {
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }
        wcscpy(sciError, L"");
    }
#endif

    createScalarDouble(pvApiCtx, Rhs + 1, dError);
    LhsVar(1) = Rhs + 1;

    if (Lhs == 2)
    {
        createSingleWideString(pvApiCtx, Rhs + 2, sciError);
        LhsVar(2) = Rhs + 2;
    }

    FREE(sciError);

    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
