/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005 - INRIA - Allan CORNET
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
