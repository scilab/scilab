/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
#include <string.h>
#include "basename.h"
#include "sci_malloc.h"
#include "expandPathVariable.h"
#include "splitpath.h"
#include "charEncoding.h"
/*--------------------------------------------------------------------------*/
char *basename(const char* fullfilename, BOOL bExpand)
{
    char *basename_str = NULL;
    if (fullfilename)
    {
        char *expandedPath = expandPathVariable(fullfilename);
        if (expandedPath)
        {
            char* drv = (char*)MALLOC(sizeof(char) * ((int)strlen(expandedPath) + 1));
            char* dir = (char*)MALLOC(sizeof(char) * ((int)strlen(expandedPath) + 1));
            char* name = (char*) MALLOC(sizeof(char) * ((int)strlen(expandedPath) + 1));
            char* ext = (char*)MALLOC(sizeof(char) * ((int)strlen(expandedPath) + 1));

            splitpath(expandedPath, bExpand, drv, dir, name, ext);

            if (name)
            {
                basename_str = name;
            }

            if (drv)
            {
                FREE(drv);
                drv = NULL;
            }

            if (dir)
            {
                FREE(dir);
                dir = NULL;
            }
            if (ext)
            {
                FREE(ext);
                ext = NULL;
            }

            FREE(expandedPath);
            expandedPath = NULL;
        }
    }
    return basename_str;
}
