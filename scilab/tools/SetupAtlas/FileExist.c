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
#include <windows.h>
#include "FileExist.h"
/*--------------------------------------------------------------------------*/
BOOL FileExist(wchar_t  *wcfilename)
{
    WIN32_FIND_DATAW FindFileData;
    HANDLE handle = FindFirstFileW (wcfilename, &FindFileData);
    if (handle != INVALID_HANDLE_VALUE)
    {
        FindClose (handle);
        return TRUE;
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/