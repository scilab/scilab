/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
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
#include <windows.h>
#include <stdlib.h>

extern "C"
{
#include "getScilabDirectory.h"
#include "sci_malloc.h"
#include "sci_path.h"
#include "charEncoding.h"
#include "os_string.h"
}
/*--------------------------------------------------------------------------*/
char *getScilabDirectory(BOOL UnixStyle)
{
    char *SciPathName = NULL;
    char* cSciPathName = NULL;
    char ScilabModuleName[MAX_PATH + 1];
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
    char fname[_MAX_FNAME];
    char ext[_MAX_EXT];
    char *DirTmp = NULL;


    if (!GetModuleFileNameA((HINSTANCE)GetModuleHandleA("core"), ScilabModuleName, MAX_PATH))
    {
        return NULL;
    }

    os_splitpath(ScilabModuleName, drive, dir, fname, ext);

    if (dir[strlen(dir) - 1] == '\\')
    {
        dir[strlen(dir) - 1] = '\0';
    }

    DirTmp = strrchr (dir, '\\');

    if (strlen(dir) - strlen(DirTmp) > 0)
    {
        dir[strlen(dir) - strlen(DirTmp)] = '\0';
    }
    else
    {
        return NULL;
    }

    cSciPathName = (char*)MALLOC((int)(strlen(drive) + strlen(dir) + 5) * sizeof(char));
    if (cSciPathName)
    {
        _makepath(cSciPathName, drive, dir, NULL, NULL);
        if ( UnixStyle )
        {
            int i = 0;
            for (i = 0; i < (int)strlen(cSciPathName); i++)
            {
                if (cSciPathName[i] == '\\')
                {
                    cSciPathName[i] = '/';
                }
            }
        }
        cSciPathName[strlen(cSciPathName) - 1] = '\0';

        SciPathName = os_strdup(cSciPathName);
        FREE(cSciPathName);
        cSciPathName = NULL;
    }

    if (SciPathName)
    {
        setSCI(SciPathName);
    }

    return SciPathName;
}
/*--------------------------------------------------------------------------*/
