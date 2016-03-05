/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
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
#include <stdlib.h>

extern "C"
{
#include "getScilabDirectory.h"
#include "sci_malloc.h"
#include "sci_path.h"
#include "charEncoding.h"
#include "os_string.h"
}
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

    os_wsplitpath(ScilabModuleName, drive, dir, fname, ext);

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
        setSCI(SciPathName);
    }

    return SciPathName;
}
/*--------------------------------------------------------------------------*/
