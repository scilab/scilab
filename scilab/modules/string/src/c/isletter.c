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

/*--------------------------------------------------------------------------*/
BOOL *isletter(char *input_string, int *sizeArray)
{
    BOOL *returnedValues = NULL;
    if (input_string)
    {
        int i = 0;
        int length_input_string = (int)strlen(input_string);
        *sizeArray = length_input_string;

        if (length_input_string > 0)
        {
            returnedValues = (BOOL*)MALLOC(sizeof(BOOL) * length_input_string);
            if (returnedValues)
            {
                for (i = 0; i < length_input_string; i++)
                {
                    if ( isalpha(input_string[i]) )
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
/*--------------------------------------------------------------------------*/
BOOL *isletterW(wchar_t *wcInput_string, int *sizeArray)
{
    BOOL *returnedValues = NULL;
    if (wcInput_string)
    {
        int i = 0;
        int length_input_string = (int)wcslen(wcInput_string);

        *sizeArray = length_input_string;

        if (length_input_string > 0)
        {
            returnedValues = (BOOL*)MALLOC(sizeof(BOOL) * length_input_string);
            if (returnedValues)
            {
                for (i = 0; i < length_input_string; i++)
                {
                    if ( iswalpha(wcInput_string[i]) )
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
/*--------------------------------------------------------------------------*/
