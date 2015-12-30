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
                Output_strings[i] = scistrrev(Input_strings[i]);
                if (Output_strings[i] == NULL)
                {
                    freeArrayOfString(Output_strings, i);
                    return NULL;
                }
            }
        }
    }
    return Output_strings;
}
/*----------------------------------------------------------------------------*/
char* scistrrev(char* str)
{
    char *revstr = NULL;
    if (str)
    {
#ifdef _MSC_VER
        revstr = _strrev(str);
#else
        int iLen = 0;
        int j = 0;

        iLen = (int)strlen(str);
        revstr = (char*)MALLOC((iLen + 1) * sizeof(char));
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
