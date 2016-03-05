
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Allan CORNET
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
#include <string.h>
#include "sci_malloc.h"
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
wchar_t **partfunctionW(wchar_t** _pwstStringInput, int _iRows, int _iCols, int *_piVectInput, int _iVectSize)
{
    int i, j;
    wchar_t **pwstParts = NULL;
    int iSize = _iRows * _iCols;

    pwstParts = (wchar_t**)MALLOC(sizeof(wchar_t*) * (iSize));

    for (i = 0 ; i < iSize ; i++)
    {
        pwstParts[i] = (wchar_t*)MALLOC(sizeof(wchar_t) * (_iVectSize + 1));

        for (j = 0 ; j < _iVectSize ; j++)
        {
            if (_piVectInput[j] > wcslen(_pwstStringInput[i]))
            {
                pwstParts[i][j] = L' ';
            }
            else
            {
                pwstParts[i][j] = _pwstStringInput[i][_piVectInput[j] - 1];
            }
        }
        pwstParts[i][j] = '\0';
    }
    return pwstParts;
}
/*--------------------------------------------------------------------------*/
