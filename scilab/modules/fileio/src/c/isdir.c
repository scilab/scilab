/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
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
#ifdef _MSC_VER
#include <Windows.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#endif
#include <string.h>
#include "isDrive.h"
#include "isdir.h"
#include "sci_malloc.h"
#include "charEncoding.h"
#include "os_string.h"
/*--------------------------------------------------------------------------*/
BOOL isdir(const char * path)
{
    BOOL bOK = FALSE;
#ifndef _MSC_VER
    struct stat buf;
    if (path == NULL)
    {
        return FALSE;
    }
    if (stat(path, &buf) == 0 && S_ISDIR(buf.st_mode))
    {
        bOK = TRUE;
    }
#else
    wchar_t *wcpath = to_wide_string((char*)path);
    if (wcpath == NULL)
    {
        return FALSE;
    }
    bOK = isdirW(wcpath);
    FREE(wcpath);
#endif
    return bOK;
}
/*--------------------------------------------------------------------------*/
BOOL isdirW(const wchar_t * wcpath)
{
    BOOL bOK = FALSE;
#ifndef _MSC_VER
    struct stat buf;
    char *path = wide_string_to_UTF8(wcpath);
    if (path == NULL)
    {
        return FALSE;
    }
    bOK = isdir(path);
    FREE(path);
#else
    if (isDriveW(wcpath))
    {
        return TRUE;
    }
    else
    {
        DWORD attr = 0;
        wchar_t *tmpPath = os_wcsdup(wcpath);

        if ( (tmpPath[wcslen(tmpPath) - 1] == L'\\') || (tmpPath[wcslen(tmpPath) - 1] == L'/') )
        {
            tmpPath[wcslen(tmpPath) - 1] = L'\0';
        }
        attr = GetFileAttributesW(tmpPath);
        FREE(tmpPath);
        if (attr == INVALID_FILE_ATTRIBUTES)
        {
            return FALSE;
        }
        return ((attr & FILE_ATTRIBUTE_DIRECTORY) != 0) ? TRUE : FALSE;
    }
#endif
    return bOK;
}
/*--------------------------------------------------------------------------*/
