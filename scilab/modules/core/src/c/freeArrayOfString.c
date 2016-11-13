/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
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
#include <stdlib.h>
#include "freeArrayOfString.h"
#include "sci_malloc.h"
/*---------------------------------------------------------------------------*/
BOOL freeArrayOfString(char **Str, int dim)
{
    return freeArray((void**)Str, dim);
}
/*---------------------------------------------------------------------------*/
BOOL freeArrayOfWideString(wchar_t **wcStr, int dim)
{
    return freeArray((void**)wcStr, dim);
}
/*---------------------------------------------------------------------------*/
BOOL freeArray(void **pArray, int dim)
{
    BOOL bRet = TRUE;

    if (pArray)
    {
        int i = 0;
        for (i = 0; i < dim; i++)
        {
            if (pArray[i])
            {
                FREE(pArray[i]);
                pArray[i] = NULL;
            }
            else
            {
                bRet = FALSE;
            }
        }
        FREE(pArray);
        pArray = NULL;
        return bRet;
    }
    else
    {
        return FALSE;
    }
}
/*---------------------------------------------------------------------------*/