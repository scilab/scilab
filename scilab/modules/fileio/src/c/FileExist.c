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
    wchar_t *wcFilename = to_wide_string(filename);
    if (wcFilename)
    {
        BOOL bOK = FileExistW(wcFilename);
        FREE(wcFilename);
        return bOK;
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
BOOL FileExistW(const wchar_t *wcfilename)
{
#ifdef _MSC_VER
    if (wcfilename)
    {
        WIN32_FIND_DATAW FindFileData;
        HANDLE handle = FindFirstFileW (wcfilename, &FindFileData);
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
    else
    {
        return FALSE;
    }

#else
    char *filename = wide_string_to_UTF8(wcfilename);
    if (filename)
    {
        BOOL bOK = FileExist(filename);
        FREE(filename);
        return bOK;
    }
    return FALSE;
#endif
}
/*--------------------------------------------------------------------------*/
