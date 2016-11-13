/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005 - INRIA - Allan CORNET
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
#ifdef _MSC_VER
#include <Windows.h>
#else
#include <stdio.h>
#endif
#include "FileExist.h"
#include "charEncoding.h"
#include "sci_malloc.h"
/*--------------------------------------------------------------------------*/
BOOL FileExist(const char *filename)
{
#ifdef _MSC_VER
    if (filename)
    {
        WIN32_FIND_DATAA FindFileData;
        HANDLE handle = FindFirstFileA(filename, &FindFileData);
        if (handle != INVALID_HANDLE_VALUE)
        {
            FindClose (handle);
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    return FALSE;
#else
    FILE* tmpFile = fopen(filename, "r");
    if (tmpFile)
    {
        fclose(tmpFile);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
#endif
}
/*--------------------------------------------------------------------------*/
