/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
