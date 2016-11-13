/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
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
