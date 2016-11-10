/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * ...
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#if _MSC_VER
#include <Windows.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#else
#include <string.h>
#include <stdio.h>
#endif
#include "os_string.h"
#include "FindFileExtension.h"
#include "sci_malloc.h"
#include "charEncoding.h"
/*--------------------------------------------------------------------------*/
char *FindFileExtension(char *filename)
{
    char *extension = NULL;
    if (filename)
    {
#ifndef _MSC_VER
        int lengthfilename = (int) strlen(filename);
        int  i = lengthfilename;

        while (filename[i] != '.' && i > 0)
        {
            i--;
        }

        if (i > 0)
        {
            int lengthextension = lengthfilename - i;
            extension = (char*)MALLOC(sizeof(char) * (lengthextension + 1));
            if (extension)
            {
                sprintf(extension, "%s", &filename[i]);
            }
        }
#else
        {
            extension = PathFindExtensionA(filename);
        }
#endif
    }
    return extension;
}
/*--------------------------------------------------------------------------*/
