/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA
* Copyright (C) 2008 - INRIA - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <stdlib.h>
#include <string.h> /* strlen */
#ifdef _MSC_VER
#include <Windows.h> /* GetEnvironmentVariable */
#include "strdup_windows.h"
#endif
#include "MALLOC.h"
#include "getenvc.h"
#include "localization.h"
#include "sciprint.h"
#include "PATH_MAX.h"
#include "FileExist.h"
#include "charEncoding.h"

/*--------------------------------------------------------------------------*/
#ifndef _MSC_VER
static void searchenv_others(const char *filename, const char *varname,
                             char *pathname);
#endif
/*--------------------------------------------------------------------------*/
void C2F(getenvc)(int *ierr, char *var, char *buf, int *buflen, int *iflag)
{
#ifdef _MSC_VER
    wchar_t* wbuf = NULL;
    wchar_t *wvar = to_wide_string(var);
    char* temp = NULL;
    DWORD iLen = GetEnvironmentVariableW(wvar, NULL, 0);

    *ierr = 0;

    if (iLen == 0)
    {
        if (*iflag == 1)
        {
            sciprint(_("Undefined environment variable %s.\n"), var);
        }

        *ierr = 1;
        return;
    }
    else
    {
        wbuf = (wchar_t*)MALLOC(sizeof(wchar_t) * iLen);
        if (GetEnvironmentVariableW(wvar, wbuf, iLen) == 0)
        {
            if (*iflag == 1)
            {
                sciprint(_("Undefined environment variable %s.\n"), var);
            }

            *ierr = 1;
            return;
        }
    }

    temp = wide_string_to_UTF8(wbuf);
    *buflen = (int)strlen(temp);
    if (buf)
    {
        strcpy(buf, temp);
    }

    FREE(temp);
#else
    char *locale = NULL;
    locale = getenv(var);
    if ( locale == NULL )
    {
        if ( *iflag == 1 )
        {
            sciprint(_("Undefined environment variable %s.\n"), var);
        }
        *ierr = 1;
    }
    else
    {
        // updating the size of char array "buf"
        *buflen = (int)strlen(locale);
        if (buf)
        {
            // to avoid buffer overflow, we check the size of the source buffer
            // and the size of the destination buffer
            if ((int) strlen(locale) <= *buflen)
            {
                // "locale" can be copied entirely to "buf"
                strcpy(buf, locale);
                *ierr = 0;
            }
            else
            {
                *ierr = 2;
            }
        }
    }
#endif
}
/*--------------------------------------------------------------------------*/
#ifndef _MSC_VER
static void searchenv_others(const char *filename,
                             const char *varname,
                             char *pathname)
{
    char *cp = NULL;

    *pathname = '\0';

    if ( filename[0] == DIR_SEPARATOR[0])
    {
        strcpy(pathname, filename);
        return;
    }

    cp = getenv(varname);
    if (cp == NULL)
    {
        /* environment Variable not defined. */
        return;
    }

    while (*cp)
    {
        char *concat = NULL;
        *pathname = '\0';
        concat = pathname;
        /* skip PATH_SEPARATOR[0] and empty entries */
        while ( (*cp) && (*cp == PATH_SEPARATOR[0]) )
        {
            cp++;
        }

        /* copy path */
        while ( (*cp) && (*cp != PATH_SEPARATOR[0]) )
        {
            *concat = *cp;
            cp++;
            concat++;
        }

        if ( concat == pathname )
        {
            /* filename not found */
            *pathname = '\0';
            return;
        }

        if ( *(concat - 1) != DIR_SEPARATOR[0] )
        {
            /* add directory separator */
            *concat = DIR_SEPARATOR[0];
            concat++;
        }

        /* concatate path & filename */
        strcpy(concat, filename);

        /* file exists ? */
        if (FileExist(pathname))
        {
            // file found
            return;
        }
    }

    /* file not found */
    *pathname = '\0';
}
#endif
/*--------------------------------------------------------------------------*/
char *searchEnv(const char *name, const char *env_var)
{
    char *buffer = NULL;
    char fullpath[PATH_MAX];

    strcpy(fullpath, "");

#if _MSC_VER
    {
        wchar_t *wname			= NULL;
        wchar_t *wenv_var		= NULL;
        wchar_t wfullpath[PATH_MAX];

        wname			= to_wide_string((char*)name);
        wenv_var	= to_wide_string((char*)env_var);

        wcscpy(wfullpath, L"");

        _wsearchenv(wname, wenv_var, wfullpath);

        if (wcslen(wfullpath) > 0)
        {
            buffer = wide_string_to_UTF8(wfullpath);
        }

        FREE(wname);
        FREE(wenv_var);
    }
#else
    searchenv_others(name, env_var, fullpath);
    if (strlen(fullpath) > 0)
    {
        buffer = strdup(fullpath);
    }
#endif
    return buffer;
}
/*--------------------------------------------------------------------------*/

