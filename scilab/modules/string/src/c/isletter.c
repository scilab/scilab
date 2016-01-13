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
#include <wchar.h>
#include <wctype.h> /* iswalpha */
#include "isletter.h"
#include "sci_malloc.h"
#include "charEncoding.h"

/*--------------------------------------------------------------------------*/
BOOL *isletter(char *input_string, int *sizeArray)
{
    BOOL *returnedValues = NULL;
    if (input_string)
    {
        int i = 0;
        wchar_t* in = to_wide_string(input_string);
        int len = (int)wcslen(in);

        *sizeArray = len;

        if (len > 0)
        {
            returnedValues = (BOOL*)MALLOC(sizeof(BOOL) * len);
            if (returnedValues)
            {
                for (i = 0; i < len; i++)
                {
                    if (iswalpha(in[i]))
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
