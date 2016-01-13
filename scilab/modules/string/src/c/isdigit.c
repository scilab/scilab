/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
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
#include <ctype.h>
#include "isletter.h"
#include "sci_malloc.h"
#include "isdigit.h"
#include "charEncoding.h"

/*--------------------------------------------------------------------------*/
BOOL *IsDigit(const char* input_string, int *returnedSizeArray)
{
    BOOL *returnedValues = NULL;
    *returnedSizeArray = 0;

    if (input_string)
    {
        int i = 0;
        wchar_t* in = to_wide_string(input_string);
        int length_input_string = (int)wcslen(in);

        *returnedSizeArray = length_input_string;

        if (length_input_string > 0)
        {
            returnedValues = (BOOL*)MALLOC(sizeof(BOOL) * length_input_string);
            if (returnedValues)
            {
                for (i = 0; i < length_input_string; i++)
                {
                    if (iswdigit(in[i]))
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

        FREE(in);
    }
    return returnedValues;
}
/*--------------------------------------------------------------------------*/
