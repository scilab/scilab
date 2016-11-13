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
void splitpath(const char* path, BOOL bExpand, char* drv, char* dir, char* name, char* ext)
{
    char* duplicate_path = NULL;
    char* begin_duplicate_path = NULL;
    const char* lastslash = NULL;
    const char* lastdot = NULL;
    const char* begin = NULL;

    if (drv)
    {
        strcpy(drv, "");
    }
    if (dir)
    {
        strcpy(dir, "");
    }
    if (name)
    {
        strcpy(name, "");
    }
    if (ext)
    {
        strcpy(ext, "");
    }

    /* pathconvert hardcoded */
    if (path)
    {
        int i = 0;

        if (bExpand)
        {
            duplicate_path = expandPathVariable(path);
        }
        else
        {
            duplicate_path = (char*)MALLOC(sizeof(char) * ((int)strlen(path) + 1));
            if (duplicate_path)
            {
                strcpy(duplicate_path, path);
            }
        }

        if (duplicate_path == NULL)
        {
            return;
        }

        begin_duplicate_path = duplicate_path;

        for (i = 0; i < (int)strlen(duplicate_path); i++)
        {
#ifdef _MSC_VER
            if (duplicate_path[i] == '/')
            {
                duplicate_path[i] = '\\';
            }
#else
            if (duplicate_path[i] == '\\')
            {
                duplicate_path[i] = '/';
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
        if (strlen(duplicate_path) > 2)
        {
            if (duplicate_path[0] && duplicate_path[1] == ':' && ((duplicate_path[2] == '\\') || (duplicate_path[2] == '/')))
            {
                if (drv)
                {
                    strncpy(drv, duplicate_path, 2);
                    drv[2] = '\0';
                    duplicate_path = duplicate_path + 2;
                }
            }
        }
    }


    /* find the last slash in the path */
#ifdef _MSC_VER
    lastslash = strrchr(duplicate_path, '\\');
#else
    lastslash = strrchr(duplicate_path, '/');
#endif

    /* fill in directory */
    if (dir)
    {
        if (lastslash == 0)
        {
            strcpy(dir, "");
        }
        else
        {
            int len = (int)strlen(duplicate_path) - (int)strlen(lastslash) + 1;
            strncpy(dir, duplicate_path, len);
            dir[len] = '\0';
        }
    }

    /* Get the last dot in the filename */
    begin = (lastslash != 0) ? lastslash + 1 : duplicate_path;
    lastdot = strrchr(begin, '.');

    /* fill filename and extension */
    if (lastdot == 0)
    {
        if (name)
        {
            strcpy(name, begin);
        }
    }
    else
    {
        if (name)
        {
            if (lastdot)
            {
                int len = (int)strlen(begin) - (int)strlen(lastdot);
                strncpy(name, begin, len);
                name[len] = '\0';
            }
        }
        if (ext)
        {
            strcpy(ext, lastdot);
        }
    }

    if (begin_duplicate_path)
    {
        FREE(begin_duplicate_path);
        begin_duplicate_path = NULL;
    }

    /* swap name & extension if no name */
    if (name && name[0] == 0 && ext && strlen(ext) > 0)
    {
        strcpy(name, ext);
        strcpy(ext, "");
    }
}
/*--------------------------------------------------------------------------*/
