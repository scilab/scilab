/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2005 - INRIA - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifdef _MSC_VER
#include <windows.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "setenvc.h"
#include "../../../core/src/c/dynamic_tclsci.h"
#include "MALLOC.h" /* MALLOC */
#include "charEncoding.h"
#include "os_swprintf.h"
/*--------------------------------------------------------------------------*/
BOOL setenvc(char *stringIn, char *valueIn)
{
#ifdef _MSC_VER
    wchar_t* wstringIn = to_wide_string(stringIn);
    wchar_t* wvalueIn = to_wide_string(valueIn);
    BOOL ret = 0;

    ret = setenvcW(wstringIn, wvalueIn);
    FREE(wstringIn);
    FREE(wvalueIn);
    return ret;
#else
    /* linux and Mac OS X */
    /* setenv() function is strongly preferred to putenv() */
    /* http://developer.apple.com/documentation/Darwin/Reference/ManPages/man3/setenv.3.html */

#ifndef _MAX_ENV
#define _MAX_ENV 32767
#endif

    int len_env = (int)(strlen(stringIn) + strlen(valueIn) + 1);
    if (len_env < _MAX_ENV)
    {
        if ( setenv(stringIn, valueIn, 1) )
        {
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }
    else
    {
        return FALSE;
    }
#endif
}
/*--------------------------------------------------------------------------*/
BOOL setenvcW(wchar_t *wstringIn, wchar_t *wvalueIn)
{
    BOOL ret = TRUE;
    int len_env = 0;
#ifdef _MSC_VER
    return SetEnvironmentVariableW(wstringIn, wvalueIn);
#else
    char * stringIn = wide_string_to_UTF8(wstringIn);
    char * valueIn = wide_string_to_UTF8(wvalueIn);
    ret = setenvc(stringIn, valueIn);
    FREE(stringIn);
    FREE(valueIn);

    return ret;
#endif
}
/*--------------------------------------------------------------------------*/
