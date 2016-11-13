/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Allan CORNET
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
#ifndef _MSC_VER
#include <unistd.h>
#else
#include <windows.h>
#endif
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "createtempfilename.h"
#include "sci_tmpdir.h"
#include "splitpath.h"
#include "machine.h"
#include "PATH_MAX.h"
#include "getshortpathname.h"
#include "FileExist.h"
#include "sci_malloc.h"
#include "os_string.h"
/*--------------------------------------------------------------------------*/
char *createtempfilename(const char *prefix, BOOL bShortFormat)
{
    char* returnedTempFilename = NULL;

#ifdef _MSC_VER
    char *tmpDir = getTMPDIR();
    if (tmpDir)
    {
        unsigned int uRetVal = 0;
        char tempFileName[MAX_PATH];
        uRetVal = GetTempFileNameA(tmpDir, prefix, 0, tempFileName);
        if (uRetVal != 0)
        {
            size_t len = strlen(tempFileName) + 1;
            char* shortTempFilename = (char*)MALLOC(len * sizeof(char));
            if (shortTempFilename)
            {
                if (bShortFormat)
                {
                    GetShortPathNameA(tempFileName, shortTempFilename, (DWORD)len);
                }

                returnedTempFilename = shortTempFilename;
            }
        }

        FREE(tmpDir);
    }
    return returnedTempFilename;
#else
    return createtempfilename(prefix, bShortFormat);
#endif
}
/*--------------------------------------------------------------------------*/
