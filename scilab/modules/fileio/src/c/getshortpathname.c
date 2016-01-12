/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
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
#include <string.h>
#include "getshortpathname.h"
#include "sci_malloc.h"
#include "charEncoding.h"
#include "os_string.h"
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
#include <Windows.h> /* GetShortPathNameA */
#ifndef MAX_PATH_SHORT
#define MAX_PATH_SHORT 260
#endif
#endif
/*--------------------------------------------------------------------------*/
int C2F(getshortpathname)(char *pathname, int *len)
{
    if (pathname)
    {
        BOOL bConvert = FALSE;
        char *result = NULL;
        pathname[*len] = 0;
        result = getshortpathname(pathname, &bConvert);
        if (result)
        {
            strcpy(pathname, result);
            *len = (int)strlen(result);
            FREE(result);
            result = NULL;
            return 1;
        }
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
char *getshortpathname(const char *longpathname, BOOL *convertok)
{
    char *ShortName = NULL;

    if (longpathname)
    {
#ifdef _MSC_VER
        /* first we try to call to know path length */
        char* pstShortName = NULL;
        int length = GetShortPathNameA(longpathname, NULL, 0);

        if (length <= 0 )
        {
            length = MAX_PATH_SHORT;
        }

        pstShortName = (char*)MALLOC((length + 1) * sizeof(char));

        if (pstShortName)
        {
            /* second converts path */
            if (GetShortPathNameA(longpathname, pstShortName, length))
            {
                ShortName = pstShortName;
                *convertok = TRUE;
            }
            else
            {
                /* FAILED */
                ShortName = os_strdup(longpathname);
                *convertok = FALSE;
            }
        }
        else
        {
            /* FAILED */
            ShortName = os_strdup(longpathname);
            *convertok = FALSE;
        }
#else
        /* Linux */
        ShortName = os_strdup(longpathname);
        *convertok = FALSE;
#endif
    }
    else
    {
        /* FAILED */
        *convertok = FALSE;
    }
    return ShortName;
}
/*--------------------------------------------------------------------------*/
