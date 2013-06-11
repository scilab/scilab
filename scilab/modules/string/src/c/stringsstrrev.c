
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) DIGITEO - 2009 - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*----------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "stringsstrrev.h"
#include "freeArrayOfString.h"
#include "MALLOC.h"
#include "os_wcsdup.h"
/*----------------------------------------------------------------------------*/
wchar_t **strings_strrev(wchar_t **Input_strings, int Dim_Input_strings)
{
    wchar_t **Output_strings = NULL;
    if (Input_strings)
    {
        Output_strings = (wchar_t **)MALLOC(sizeof(wchar_t*) * Dim_Input_strings);
        if (Output_strings)
        {
            int i = 0;
            for (i = 0; i < Dim_Input_strings; i++)
            {
                Output_strings[i] = scistrrev(Input_strings[i]);
                if (!Output_strings[i])
                {
                    freeArrayOfWideString(Output_strings, i);
                    return Output_strings;
                }
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
