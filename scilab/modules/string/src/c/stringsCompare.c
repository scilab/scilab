
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
#include "stringsCompare.h"
#include "sci_malloc.h"
#include "os_string.h"
/*--------------------------------------------------------------------------*/
static int sign(int x)
{
    if (x < 0)
    {
        return -1;
    }
    if (x > 0)
    {
        return 1;
    }
    return 0;
}

int * stringsCompare(wchar_t **Input_String_One, int dim_One, wchar_t **Input_String_Two, int dim_Two, BOOL dostricmp)
{
    int *returnedValues = NULL;
    int j = 0;

    returnedValues = (int*)MALLOC(sizeof(int) * dim_One);
    if (returnedValues)
    {
        int i = 0;
        for (i = 0; i < dim_One; i++)
        {
            if (dim_Two == 1)
            {
                j = 0;
            }
            else
            {
                j = i;
            }

            if (dostricmp)
            {
                returnedValues[i] = sign(wcsicmp(Input_String_One[i], Input_String_Two[j]));
            }
            else
            {
                returnedValues[i] = sign(wcscmp(Input_String_One[i], Input_String_Two[j]));
            }
        }
    }
    return returnedValues;
}
/*--------------------------------------------------------------------------*/
