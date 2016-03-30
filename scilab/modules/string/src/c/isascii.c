/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) DIGITEO - Allan CORNET - 2009
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
#include "isascii.h"
#include "sci_malloc.h"
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
