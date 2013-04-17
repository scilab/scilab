/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) DIGITEO - Allan CORNET - 2009
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include <string.h>
#include "isascii.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
BOOL* isasciiStringW(wchar_t* input_string, int* returnedSize)
{
    BOOL *returnedValues = NULL;
    *returnedSize = 0;

    if (input_string)
    {
        int i = 0;
        int length_input_string = (int)wcslen(input_string);
        *returnedSize = length_input_string;

        if (length_input_string > 0)
        {
            returnedValues = (BOOL*)MALLOC(sizeof(BOOL) * length_input_string);
            if (returnedValues)
            {
                for (i = 0; i < length_input_string; i++)
                {
                    if ( iswascii(input_string[i]) )
                    {
                        returnedValues[i] = TRUE;
                    }
                    else
                    {
                        returnedValues[i] = FALSE;
                    }
                }
            }
        }
    }
    return returnedValues;
}

BOOL* isasciiMatrix(double* inputValues, int inputSize)
{
    BOOL *returnedValues = NULL;
    if (inputSize && inputValues != NULL)
    {
        int i = 0;
        returnedValues = (BOOL*)MALLOC(sizeof(BOOL) * inputSize);
        if (returnedValues)
        {
            for (i = 0; i < inputSize; i++)
            {
                if ( iswascii(inputValues[i]) )
                {
                    returnedValues[i] = TRUE;
                }
                else
                {
                    returnedValues[i] = FALSE;
                }
            }
        }
    }
    return returnedValues;
}
/*--------------------------------------------------------------------------*/
