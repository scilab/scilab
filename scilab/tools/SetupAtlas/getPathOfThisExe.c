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
