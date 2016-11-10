/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * Copyright (C) ENPC
 * Copyright (C) DIGITEO - 2009 - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#include <string.h>
#ifdef _MSC_VER
#include <windows.h>
#include <direct.h>
#include <errno.h>
#else
#include <errno.h>
#include <unistd.h>
#define GETCWD(x,y) getcwd(x,y)
#endif
#include "os_string.h"
#include "sciprint.h"
#include "scicurdir.h"
#include "sci_malloc.h"
#include "localization.h"
#include "charEncoding.h"
#include "configvariable_interface.h"
#include "PATH_MAX.h"
/*--------------------------------------------------------------------------*/
int scichdir(const char *path)
{
    if (path == NULL)
    {
        return 1;
    }

#ifndef _MSC_VER

    if (chdir(path) == -1)
    {
        if ( getWarningMode() )
        {
            sciprint(_("Can't go to directory %s: %s\n"), path, strerror(errno));
        }
        return 1;
    }

#else
    if (_chdir(path) )
    {
        switch (errno)
        {
            case ENOENT:
            {
                if ( getWarningMode() )
                {
                    sciprint(_("Can't go to directory %s.\n"), path);
                }
            }
            break;
            case EINVAL:
            {
                if ( getWarningMode() )
                {
                    sciprint(_("Invalid buffer.\n"));
                }
            }
            break;
            default:
            {
                if ( getWarningMode() )
                {
                    sciprint(_("Unknown error.\n"));
                }
            }
        }
        return 1;
    }
#endif
    return 0;
}
/*--------------------------------------------------------------------------*/
char * scigetcwd(int *err)
{
    char currentDir[PATH_MAX + 1];
#ifndef _MSC_VER
    if (GETCWD(currentDir, PATH_MAX) == NULL)
#else
    if (_getcwd(currentDir, PATH_MAX) == NULL )
#endif
    {
        if (getWarningMode())
        {
            sciprint(_("Can't get current directory.\n"));
        }
        *err = 1;
    }

    *err = 0;

    return os_strdup(currentDir);
}
/*--------------------------------------------------------------------------*/
