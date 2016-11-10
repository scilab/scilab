/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2007 - INRIA
* Copyright (C) 2009 - DIGITEO - Allan CORNET
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
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#else
#include <Windows.h>
#include <stdio.h>
#endif
#include "charEncoding.h"
#include "sci_malloc.h"
#include "createdirectory.h"
#include "isdir.h"
#include "splitpath.h"
#include "scicurdir.h"
#include "os_string.h"
#ifndef _MSC_VER
#define MAX_PATH PATH_MAX
#endif
/*--------------------------------------------------------------------------*/
#define DIRMODE 0777
/*--------------------------------------------------------------------------*/
BOOL createdirectory(const char *path)
{
    BOOL bOK = FALSE;
    if (path)
    {
        if (!isdir(path))
        {
#ifndef _MCS_VER
            if (mkdir(path, DIRMODE) == 0)
            {
                bOK = TRUE;
            }
#else
            char path_out[MAX_PATH];
            char drv[MAX_PATH];
            char dir[MAX_PATH];
            splitpath(path, TRUE, drv, dir, NULL, NULL);

            os_strcpy(path_out, drv);
            strcat(path_out, dir);

            //if there is no path_out, get current dir as reference.
            if (strlen(path_out) == 0)
            {
                int err = 0;
                char* cur = NULL;
                if (strlen(path)  == 0)
                {
                    return FALSE;
                }

                cur = scigetcwd(&err);
                os_strcpy(path_out, cur);
                FREE(cur);
            }

            if (CreateDirectoryExA(path_out, path, NULL))
            {
                bOK = TRUE;
            }
#endif // _MCS_VER

        }
    }
    return bOK;
}
/*--------------------------------------------------------------------------*/
