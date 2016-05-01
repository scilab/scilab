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
#include <shlwapi.h>
#include <stdio.h>
#include "FindFileAssociation.h"
#include "sci_malloc.h"
#include "os_string.h"
#include "PATH_MAX.h"
#include "charEncoding.h"
/*--------------------------------------------------------------------------*/
#pragma comment(lib,"shlwapi.lib") /* AssocQueryString */
/*--------------------------------------------------------------------------*/
wchar_t* FindFileAssociation (const wchar_t* ptrFindStr, const wchar_t* Extra)
{
    if (ptrFindStr)
    {
        wchar_t szDefault[PATH_MAX + FILENAME_MAX];
        DWORD ccDefault = PATH_MAX + FILENAME_MAX;

        if (ptrFindStr)
        {
            HRESULT rc = AssocQueryStringW(0, ASSOCSTR_EXECUTABLE, ptrFindStr, Extra, szDefault, &ccDefault);
            if (ccDefault)
            {
                if (rc == S_OK)
                {
                    return os_wcsdup(szDefault);
                }
            }
        }
    }
    return NULL;
}
/*--------------------------------------------------------------------------*/

