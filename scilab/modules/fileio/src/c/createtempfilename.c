/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef _MSC_VER
#include <unistd.h>
#else
#include <windows.h>
#endif
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "createtempfilename.h"
#include "tmpdir.h"
#include "splitpath.h"
#include "machine.h"
#include "PATH_MAX.h"
#include "getshortpathname.h"
#include "FileExist.h"
/*--------------------------------------------------------------------------*/
char *createtempfilename(const char *prefix, BOOL bShortFormat)
{
    char *tempfilename = NULL;
#ifdef _MSC_VER
    wchar_t *wcprefix = to_wide_string(prefix);
    wchar_t *wcresult = createtempfilenameW(wcprefix, bShortFormat);

    tempfilename = wide_string_to_UTF8(wcresult);

    if (wcresult)
    {
        FREE(wcresult);
        wcresult = NULL;
    }
    if (wcresult)
    {
        FREE(wcresult);
        wcresult = NULL;
    }
#else
    char *TmpDir = getTMPDIR();
    if (TmpDir)
    {
        char TempFileName[PATH_MAX];
        int fd = 0;
        sprintf(TempFileName, "%s/%sXXXXXX", TmpDir, prefix);
        fd = mkstemp(TempFileName);
        if (fd != -1)
        {
            close(fd);
        }
        tempfilename = strdup(TempFileName);
    }
#endif
    return tempfilename;
}
/*--------------------------------------------------------------------------*/
wchar_t *createtempfilenameW(const wchar_t *wcprefix, BOOL bShortFormat)
{
    wchar_t *wcReturnedTempFilename = NULL;

#ifdef _MSC_VER
    wchar_t *wcTmpDir = getTMPDIRW();
    if (wcTmpDir)
    {
        unsigned int uRetVal = 0;
        wchar_t wcTempFileName[MAX_PATH];
        uRetVal = GetTempFileNameW(wcTmpDir, wcprefix, 0, wcTempFileName);
        if (uRetVal != 0)
        {
            size_t len = wcslen(wcTempFileName) + 1;
            wchar_t* shortTempFilename = (wchar_t *)MALLOC(len * sizeof(wchar_t));
            if (shortTempFilename)
            {
                if (bShortFormat)
                {
                    GetShortPathNameW(wcTempFileName, shortTempFilename, (DWORD)len);
                }
                wcReturnedTempFilename = shortTempFilename;
            }
        }
    }
#else
    char *prefix = wide_string_to_UTF8(wcprefix);
    char *result = createtempfilename(prefix, bShortFormat);

    wcReturnedTempFilename = to_wide_string(result);

    if (result)
    {
        FREE(result);
        result = NULL;
    }
    if (prefix)
    {
        FREE(prefix);
        prefix = NULL;
    }
    if (result)
    {
        FREE(result);
        result = NULL;
    }
#endif
    return wcReturnedTempFilename;
}
/*--------------------------------------------------------------------------*/
