/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
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
#include <stdlib.h>
#include "getScilabDirectory.h"
#include "MALLOC.h"
#include "setgetSCIpath.h"
#include "charEncoding.h"
/*--------------------------------------------------------------------------*/
char *getScilabDirectory(BOOL UnixStyle)
{
    char *SciPathName = NULL;
    wchar_t* wcSciPathName = NULL;
    wchar_t ScilabModuleName[MAX_PATH + 1];
    wchar_t drive[_MAX_DRIVE];
    wchar_t dir[_MAX_DIR];
    wchar_t fname[_MAX_FNAME];
    wchar_t ext[_MAX_EXT];
    wchar_t *DirTmp = NULL;


    if (!GetModuleFileNameW ((HINSTANCE)GetModuleHandleW(L"core"), (wchar_t*) ScilabModuleName, MAX_PATH))
    {
        return NULL;
    }

    _wsplitpath(ScilabModuleName, drive, dir, fname, ext);

    if (dir[wcslen(dir) - 1] == L'\\')
    {
        dir[wcslen(dir) - 1] = L'\0';
    }

    DirTmp = wcsrchr (dir, L'\\');

    if (wcslen(dir) - wcslen(DirTmp) > 0)
    {
        dir[wcslen(dir) - wcslen(DirTmp)] = L'\0';
    }
    else
    {
        return NULL;
    }

    wcSciPathName = (wchar_t*)MALLOC((int)( wcslen(drive) + wcslen(dir) + 5) * sizeof(wchar_t));
    if (wcSciPathName)
    {
        _wmakepath(wcSciPathName, drive, dir, NULL, NULL);
        if ( UnixStyle )
        {
            int i = 0;
            for (i = 0; i < (int)wcslen(wcSciPathName); i++)
            {
                if (wcSciPathName[i] == L'\\')
                {
                    wcSciPathName[i] = L'/';
                }
            }
        }
        wcSciPathName[wcslen(wcSciPathName) - 1] = '\0';

        SciPathName = wide_string_to_UTF8(wcSciPathName);
        FREE(wcSciPathName);
        wcSciPathName = NULL;
    }

    if (SciPathName)
    {
        setSCIpath(SciPathName);
    }

    return SciPathName;
}
/*--------------------------------------------------------------------------*/
