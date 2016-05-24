/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Allan CORNET
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
#include <unistd.h>
#else
#include <windows.h>
#endif
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "createtempfilename.h"
#include "sci_tmpdir.h"
#include "splitpath.h"
#include "machine.h"
#include "PATH_MAX.h"
#include "getshortpathname.h"
#include "FileExist.h"
#include "sci_malloc.h"
#include "os_string.h"
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
        tempfilename = os_strdup(TempFileName);
    }
    FREE(TmpDir);
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

        FREE(wcTmpDir);
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
#endif
    return wcReturnedTempFilename;
}
/*--------------------------------------------------------------------------*/
