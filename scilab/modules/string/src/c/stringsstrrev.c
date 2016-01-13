/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) DIGITEO - 2009 - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*----------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "stringsstrrev.h"
#include "freeArrayOfString.h"
#include "sci_malloc.h"
#include "os_string.h"
#include "charEncoding.h"
/*----------------------------------------------------------------------------*/
char** strings_strrev(char **Input_strings, int Dim_Input_strings)
{
    char** Output_strings = NULL;
    if (Input_strings)
    {
        Output_strings = (char**)MALLOC(sizeof(char*) * Dim_Input_strings);
        if (Output_strings)
        {
            int i = 0;
            for (i = 0; i < Dim_Input_strings; i++)
            {
                wchar_t* w = to_wide_string(Input_strings[i]);
                wchar_t* o = scistrrev(w);
                if (o == NULL)
                {
                    freeArrayOfString(Output_strings, i);
                    return NULL;
                }

                Output_strings[i] = wide_string_to_UTF8(o);
                FREE(o);
                FREE(w);
            }
        }
    }
    return Output_strings;
}
/*----------------------------------------------------------------------------*/
wchar_t* scistrrev(wchar_t* str)
{
    wchar_t *revstr = NULL;
    if (str)
    {
#ifdef _MSC_VER
        revstr = _wcsrev(os_wcsdup(str));
#else
        int iLen = 0;
        int j = 0;

        iLen = (int)wcslen(str);
        revstr = (wchar_t*)MALLOC((iLen + 1) * sizeof(wchar_t));
        /* copy character by character to reverse string */
        for (j = 0 ; j < iLen ; j++)
        {
            revstr[iLen - j - 1] = str[j];
        }
        revstr[iLen] = '\0';
#endif
    }
    return revstr;
}
/*----------------------------------------------------------------------------*/
