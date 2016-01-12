/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
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
#include <windows.h>
#endif
#include <string.h>
#include "getlongpathname.h"
#include "sci_malloc.h"
#include "charEncoding.h"
#include "os_string.h"
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
#ifndef MAX_PATH_LONG
#define MAX_PATH_LONG 32767
#endif
#endif
/*--------------------------------------------------------------------------*/
char *getlongpathname(char *shortpathname, BOOL *convertok)
{
    char *cLongName = NULL;

#ifdef _MSC_VER
    /* first we try to call to know path length */
    int length = GetLongPathNameA(shortpathname, NULL, 0);
    if (length <= 0 )
    {
        length = MAX_PATH_LONG;
    }

    cLongName = (char*)MALLOC((length + 1) * sizeof(char));
    if (cLongName)
    {
        /* second converts path */
        if (GetLongPathNameA(shortpathname, cLongName, length))
        {
            *convertok = TRUE;
        }
        else
        {
            /* FAILED */
            strcpy(cLongName, shortpathname);
            *convertok = FALSE;
        }
    }
    else
    {
        /* FAILED */
        *convertok = FALSE;
    }
#else
    /* Linux */
    cLongName = os_strdup(shortpathname);
    *convertok = FALSE;
#endif
    return cLongName;
}
/*--------------------------------------------------------------------------*/
