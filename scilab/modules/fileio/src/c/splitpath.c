/*--------------------------------------------------------------------------*/
/*
* ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2009 - Allan CORNET
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
#include <string.h>
#include "splitpath.h"
#include "charEncoding.h"
#include "PATH_MAX.h"
#include "sci_malloc.h"
#include "expandPathVariable.h"
/*--------------------------------------------------------------------------*/
void splitpathW(const wchar_t* path, BOOL bExpand, wchar_t* drv, wchar_t* dir, wchar_t* name, wchar_t* ext)
{
    wchar_t* duplicate_path = NULL;
    wchar_t* begin_duplicate_path = NULL;
    const wchar_t* lastslash = NULL;
    const wchar_t* lastdot = NULL;
    const wchar_t* begin = NULL;

    if (drv)
    {
        wcscpy(drv, L"");
    }
    if (dir)
    {
        wcscpy(dir, L"");
    }
    if (name)
    {
        wcscpy(name, L"");
    }
    if (ext)
    {
        wcscpy(ext, L"");
    }

    /* pathconvert hardcoded */
    if (path)
    {
        int i = 0;

        if (bExpand)
        {
            duplicate_path = expandPathVariableW((wchar_t*)path);
        }
        else
        {
            duplicate_path = (wchar_t*)MALLOC(sizeof(wchar_t) * ((int)wcslen(path) + 1));
            if (duplicate_path)
            {
                wcscpy(duplicate_path, path);
            }
        }

        if (duplicate_path == NULL)
        {
            return;
        }

        begin_duplicate_path = duplicate_path;

        for (i = 0; i < (int)wcslen(duplicate_path); i++)
        {
#ifdef _MSC_VER
            if (duplicate_path[i] == L'/')
            {
                duplicate_path[i] = L'\\';
            }
#else
            if (duplicate_path[i] == L'\\')
            {
                duplicate_path[i] = L'/';
            }
#endif
        }
    }
    else
    {
        return;
    }

    if (duplicate_path)
    {
        if (wcslen(duplicate_path) > 2)
        {
            if (duplicate_path[0] && duplicate_path[1] == L':' && ( (duplicate_path[2] == L'\\') || (duplicate_path[2] == L'/') ) )
            {
                if (drv)
                {
                    wcsncpy(drv, duplicate_path, 2);
                    drv[2] = L'\0';
                    duplicate_path = duplicate_path + 2;
                }
            }
        }
    }


    /* find the last slash in the path */
#ifdef _MSC_VER
    lastslash = wcsrchr(duplicate_path, L'\\');
#else
    lastslash = wcsrchr(duplicate_path, L'/');
#endif

    /* fill in directory */
    if (dir)
    {
        if (lastslash == 0)
        {
            wcscpy(dir, L"");
        }
        else
        {
            int len = (int)wcslen(duplicate_path) - (int)wcslen(lastslash) + 1;
            wcsncpy(dir, duplicate_path, len);
            dir[len] = L'\0';
        }
    }

    /* Get the last dot in the filename */
    begin = (lastslash != 0) ? lastslash + 1 : duplicate_path;
    lastdot = wcsrchr(begin, L'.');

    /* fill filename and extension */
    if (lastdot == 0)
    {
        if (name)
        {
            wcscpy(name, begin);
        }
    }
    else
    {
        if (name)
        {
            if (lastdot)
            {
                int len = (int)wcslen(begin) - (int)wcslen(lastdot);
                wcsncpy(name, begin, len);
                name[len] = L'\0';
            }
        }
        if (ext)
        {
            wcscpy(ext, lastdot);
        }
    }

    if (begin_duplicate_path)
    {
        FREE(begin_duplicate_path);
        begin_duplicate_path = NULL;
    }

    /* swap name & extension if no name */
    if (name && name[0] == 0 && ext && wcslen(ext) > 0)
    {
        wcscpy(name, ext);
        wcscpy(ext, L"");
    }
}
/*--------------------------------------------------------------------------*/
void splitpath(const char* path, BOOL bExpand, char* drv, char* dir, char* name, char* ext)
{
    wchar_t *wcpath = NULL;
    wchar_t *wcdrv = NULL;
    wchar_t *wcdir = NULL;
    wchar_t *wcname = NULL;
    wchar_t *wcext = NULL;

    char *buffer = NULL;

    if (drv == NULL || dir == NULL || name == NULL || ext == NULL)
    {
        return;
    }

    wcpath = to_wide_string((char*)path);
    wcdrv = (wchar_t*)MALLOC(sizeof(wchar_t) * (PATH_MAX + 1));
    wcdir = (wchar_t*)MALLOC(sizeof(wchar_t) * (PATH_MAX + 1));
    wcname = (wchar_t*)MALLOC(sizeof(wchar_t) * (PATH_MAX + 1));
    wcext = (wchar_t*)MALLOC(sizeof(wchar_t) * (PATH_MAX + 1));

    strcpy(drv, "");
    strcpy(dir, "");
    strcpy(name, "");
    strcpy(ext, "");

    splitpathW(wcpath, bExpand, wcdrv, wcdir, wcname, wcext);

    FREE(wcpath);

    buffer = wide_string_to_UTF8(wcdrv);
    if (buffer)
    {
        strcpy(drv, buffer);
        FREE(buffer);
    }
    FREE(wcdrv);

    buffer = wide_string_to_UTF8(wcdir);
    if (buffer)
    {
        strcpy(dir, buffer);
        FREE(buffer);
    }
    FREE(wcdir);

    buffer = wide_string_to_UTF8(wcname);
    if (buffer)
    {
        strcpy(name, buffer);
        FREE(buffer);
    }
    FREE(wcname);

    buffer = wide_string_to_UTF8(wcext);
    if (buffer)
    {
        strcpy(ext, buffer);
        FREE(buffer);
    }
    FREE(wcext);
}
/*--------------------------------------------------------------------------*/
