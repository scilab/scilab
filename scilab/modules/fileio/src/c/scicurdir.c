/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * Copyright (C) ENPC
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
