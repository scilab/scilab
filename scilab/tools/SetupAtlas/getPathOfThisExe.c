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
/*--------------------------------------------------------------------------*/
wchar_t * GetPathOfThisExe(void)
{
    wchar_t *pathexe = NULL;
    wchar_t *tail;
    wchar_t wcfullfilename[MAX_PATH * 2];

    GetModuleFileNameW(GetModuleHandleW(NULL), wcfullfilename, MAX_PATH * 2);

    if ((tail = wcsrchr (wcfullfilename, L'\\')) != NULL)
    {
        tail++;
        *tail = L'\0';
        pathexe = _wcsdup(wcfullfilename);
    }

    return (wchar_t *)pathexe;
}
/*--------------------------------------------------------------------------*/