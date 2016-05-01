/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * ...
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
            wchar_t *wcFilename = to_wide_string(filename);
            if (wcFilename)
            {
                extension = wide_string_to_UTF8(PathFindExtensionW(wcFilename));
                FREE(wcFilename);
                wcFilename = NULL;
            }
        }
#endif
    }
    return extension;
}
/*--------------------------------------------------------------------------*/
