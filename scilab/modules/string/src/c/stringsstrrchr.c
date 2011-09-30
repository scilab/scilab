
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) DIGITEO - 2011 - Allan CORNET
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
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "stringsstrrchr.h"
#include "MALLOC.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
#define EMPTY_WCSTR L""
/*----------------------------------------------------------------------------*/
wchar_t ** strings_wcsrchr(const wchar_t **InputStrings, int Dim_InputStrings,
                           const wchar_t** InputChar, int Dim_InputChar, BOOL do_strchr)
{
    wchar_t **wcOutput_Strings = NULL;

    if ( (InputStrings) && (InputChar) )
    {
        wcOutput_Strings = (wchar_t**)MALLOC(sizeof(wchar_t*)*Dim_InputStrings);
        if (wcOutput_Strings)
        {
            int i = 0;
            for(i = 0;i < Dim_InputStrings; i++)
            {
                int c = 0;
                wchar_t *ptrStr = NULL;

                if (Dim_InputChar == 1) c = InputChar[0][0];
                else  c = InputChar[i][0];

                if (do_strchr) ptrStr = wcschr(InputStrings[i], (wchar_t)c);
                else ptrStr = wcsrchr(InputStrings[i], (wchar_t)c);

                if (ptrStr)
                {
                    wcOutput_Strings[i] = (wchar_t*)MALLOC(sizeof(wchar_t) * (wcslen(ptrStr) + 1));
                    wcscpy(wcOutput_Strings[i], ptrStr);
                }
                else
                {
                    wcOutput_Strings[i] = (wchar_t*)MALLOC(sizeof(wchar_t) * (wcslen(EMPTY_WCSTR) + 1));
                    wcscpy(wcOutput_Strings[i], EMPTY_WCSTR);
                }
            }
        }
    }
    return wcOutput_Strings;
}
/*--------------------------------------------------------------------------*/
