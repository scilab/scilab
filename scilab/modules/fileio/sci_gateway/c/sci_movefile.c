/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009-2012 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#if defined(__linux__)
#undef _FORTIFY_SOURCE /* Avoid dependency on GLIBC_2.4 (__wcscat_chk/__wcscpy_chk) */
#endif
#ifndef _MSC_VER
#include <errno.h>
#else
#include <windows.h>
#endif
#include "gw_fileio.h"
#include "MALLOC.h"
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
static int returnMoveFileResultOnStack(int ierr, char *fname);
/*--------------------------------------------------------------------------*/
int sci_movefile(char *fname, unsigned long fname_len)
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

    if (!isStringType(pvApiCtx, piAddressVarOne))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
        return 0;
    }

    if (!isScalar(pvApiCtx, piAddressVarOne))
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 1);
        return 0;
    }


    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return 0;
    }

    if (!isStringType(pvApiCtx, piAddressVarTwo))
    {
        if (pStVarOne)
        {
            FREE(pStVarOne);
            pStVarOne = NULL;
        }
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 2);
        return 0;
    }

    if (!isScalar(pvApiCtx, piAddressVarTwo))
    {
        if (pStVarOne)
        {
            FREE(pStVarOne);
            pStVarOne = NULL;
        }
        Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 2);
        return 0;
    }

    if (getAllocatedSingleWideString(pvApiCtx, piAddressVarOne, &pStVarOne))
    {
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    if (getAllocatedSingleWideString(pvApiCtx, piAddressVarTwo, &pStVarTwo))
    {
        if (pStVarOne)
        {
            freeAllocatedSingleWideString(pStVarOne);
            pStVarOne = NULL;
        }
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    pStVarOneExpanded = expandPathVariableW(pStVarOne);
    pStVarTwoExpanded = expandPathVariableW(pStVarTwo);

    freeAllocatedSingleWideString(pStVarTwo);
    pStVarTwo = NULL;

    freeAllocatedSingleWideString(pStVarOne);
    pStVarOne = NULL;

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
                    if (pStVarOneExpanded)
                    {
                        FREE(pStVarOneExpanded);
                        pStVarOneExpanded = NULL;
                    }
                    if (pStVarTwoExpanded)
                    {
                        FREE(pStVarTwoExpanded);
                        pStVarTwoExpanded = NULL;
                    }

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
            if (pStVarOneExpanded)
            {
                FREE(pStVarOneExpanded);
                pStVarOneExpanded = NULL;
            }

            if (pStVarTwo)
            {
                FREE(pStVarTwoExpanded);
                pStVarTwoExpanded = NULL;
            }
            Scierror(999, _("%s: Wrong value for input argument #%d: A valid filename or directory expected.\n"), fname, 1);
            return 0;
        }

        returnMoveFileResultOnStack(ierrMove, fname);
    }
    else
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: A valid filename or directory expected.\n"), fname, 1);
    }

    if (pStVarOneExpanded)
    {
        FREE(pStVarOneExpanded);
        pStVarOneExpanded = NULL;
    }
    if (pStVarTwoExpanded)
    {
        FREE(pStVarTwoExpanded);
        pStVarTwoExpanded = NULL;
    }

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

        splitpathW(wcFullFilename, FALSE, wcdrv, wcdir, wcname, wcext);

        wcscpy(wcfilename, wcname);
        wcscat(wcfilename, wcext);

        if (wcdrv)
        {
            FREE(wcdrv);
            wcdrv = NULL;
        }
        if (wcdir)
        {
            FREE(wcdir);
            wcdir = NULL;
        }
        if (wcname)
        {
            FREE(wcname);
            wcname = NULL;
        }
        if (wcext)
        {
            FREE(wcext);
            wcext = NULL;
        }
    }
    return wcfilename;
}

/*--------------------------------------------------------------------------*/
static int returnMoveFileResultOnStack(int ierr, char *fname)
{
    double dError = 0.;
    wchar_t **sciError = NULL;
    int m_out = 1, n_out = 1;

    sciError = (wchar_t **) MALLOC(sizeof(wchar_t *) * 1);
    if (sciError == NULL)
    {
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

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

        sciError[0] = (wchar_t *) MALLOC(sizeof(wchar_t) * ((int)wcslen(buffer) + 1));
        if (sciError[0] == NULL)
        {
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }

        wcscpy(sciError[0], buffer);
    }
    else
    {
        dError = 1.;
        sciError[0] = (wchar_t *) MALLOC(sizeof(wchar_t) * 1);
        if (sciError[0] == NULL)
        {
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }
        wcscpy(sciError[0], L"");
    }
#else
    if (ierr)
    {
        // for compatibilty with copyfile, we return 0 (error)
        //dError = (double) ierr;
        dError = (double)0.;

        sciError[0] = to_wide_string(strerror(errno));
        if (sciError[0] == NULL)
        {
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }
    }
    else
    {
        dError = 1.;
        sciError[0] = (wchar_t *) MALLOC(sizeof(wchar_t) * 1);
        if (sciError[0] == NULL)
        {
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            freeArrayOfWideString(sciError, 1);
            return 0;
        }
        wcscpy(sciError[0], L"");
    }
#endif

    createMatrixOfDouble(pvApiCtx, Rhs + 1, m_out, n_out, &dError);
    LhsVar(1) = Rhs + 1;

    if (Lhs == 2)
    {
        createMatrixOfWideString(pvApiCtx, Rhs + 2, m_out, n_out, sciError);
        LhsVar(2) = Rhs + 2;
    }

    freeArrayOfWideString(sciError, 1);

    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
