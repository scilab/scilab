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
#include <string.h>
#include "dynlib_scilab_windows.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
/* strdup is deprecated on windows*/
/* required to fix warnings about strdup */
/*--------------------------------------------------------------------------*/
SCILAB_WINDOWS_IMPEXP char *strdup_windows(const char *strSource)
{
    char *retStr = NULL;
    if (strSource)
    {
        retStr = (char *)MALLOC(sizeof(char) * ((int)strlen(strSource) + 1));
        if (retStr)
        {
            strcpy(retStr, strSource);
        }
    }
    return retStr;
}
/*--------------------------------------------------------------------------*/
SCILAB_WINDOWS_IMPEXP wchar_t *wstrdup_windows(const wchar_t *strSource)
{
    wchar_t *retStr = NULL;
    if (strSource)
    {
        retStr = (wchar_t *)MALLOC(sizeof(wchar_t) * ((int)wcslen(strSource) + 1));
        if (retStr)
        {
            wcscpy(retStr, strSource);
        }
    }
    return retStr;
}
/*--------------------------------------------------------------------------*/