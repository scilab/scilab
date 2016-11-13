/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
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
