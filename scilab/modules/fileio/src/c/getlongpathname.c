/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
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
#include <windows.h>
#endif
#include <string.h>
#include "getlongpathname.h"
#include "sci_malloc.h"
#include "charEncoding.h"
#include "os_string.h"
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
#ifndef MAX_PATH_LONG
#define MAX_PATH_LONG 32767
#endif
#endif
/*--------------------------------------------------------------------------*/
char *getlongpathname(char *shortpathname, BOOL *convertok)
{
    char *LongName = NULL;
    wchar_t *wcshortpathname = to_wide_string(shortpathname);
    if (wcshortpathname)
    {
        wchar_t *wcLongName = getlongpathnameW(wcshortpathname, convertok);
        FREE(wcshortpathname);
        if (wcLongName)
        {
            LongName = wide_string_to_UTF8(wcLongName);
            FREE(wcLongName);
            wcLongName = NULL;
        }
        else
        {
            LongName = os_strdup(shortpathname);
            *convertok = FALSE;
        }
    }
    else
    {
        LongName = os_strdup(shortpathname);
        *convertok = FALSE;
    }
    return LongName;
}
/*--------------------------------------------------------------------------*/
wchar_t *getlongpathnameW(wchar_t *wcshortpathname, BOOL *convertok)
{
    wchar_t *wcLongName = NULL;

#ifdef _MSC_VER
    /* first we try to call to know path length */
    int length = GetLongPathNameW(wcshortpathname, NULL, 0);
    if (length <= 0 )
    {
        length = MAX_PATH_LONG;
    }

    wcLongName = (wchar_t*)MALLOC((length + 1) * sizeof(wchar_t));

    if (wcLongName)
    {
        /* second converts path */
        if (GetLongPathNameW(wcshortpathname, wcLongName, length))
        {
            *convertok = TRUE;
        }
        else
        {
            /* FAILED */
            if (wcLongName)
            {
                wcscpy(wcLongName, wcshortpathname);
            }
            *convertok = FALSE;
        }
    }
    else
    {
        /* FAILED */
        *convertok = FALSE;
    }
#else
    /* Linux */
    int len = (int)wcslen(wcshortpathname) + 1;
    wcLongName = (wchar_t*)MALLOC(len * sizeof(wchar_t));
    if (wcLongName)
    {
        wcscpy(wcLongName, wcshortpathname);
    }
    *convertok = FALSE;
#endif
    return wcLongName;
}
/*--------------------------------------------------------------------------*/
