
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Allan CORNET
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
#include "MALLOC.h"
#include "partfunction.h"
#include "freeArrayOfString.h"
#include "charEncoding.h"

/*--------------------------------------------------------------------------*/
#define BLANK_CHAR ' '
/*--------------------------------------------------------------------------*/
char **partfunction(char** stringInput, int m, int n, int *vectInput, int row)
{
    char **parts = NULL;
    int mn = m * n;

    int i = 0;

    parts = (char**)MALLOC(sizeof(char*) * (mn));

    for (i = 0; i < mn; i++)
    {
        int j = 0;
        int lengthstringInput = 0;
        wchar_t *wcInput = to_wide_string(stringInput[i]);
        wchar_t *wcOutput = NULL;

        wcOutput = (wchar_t*)MALLOC(sizeof(wchar_t) * ((row) + 1));

        if (wcInput)
        {
            lengthstringInput = (int)wcslen(wcInput);
            for (j = 0; j < row; j++)
            {
                if ( vectInput[j] > lengthstringInput )
                {
                    wcOutput[j] = L' ';
                }
                else
                {
                    wcOutput[j] = wcInput[vectInput[j] - 1];
                }
            }
            FREE(wcInput);
        }
        else // fill output with blank
        {
            for (j = 0; j < row; j++)
            {
                wcOutput[j] = L' ';
            }
        }

        wcOutput[j] = '\0';
        parts[i] = wide_string_to_UTF8(wcOutput);
        FREE(wcOutput);
    }
    return parts;
}
/*--------------------------------------------------------------------------*/
