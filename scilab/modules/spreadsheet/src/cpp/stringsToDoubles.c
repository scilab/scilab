/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2011 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 * This code is also published under the GPL v3 license.
 *
 */
#include "stringsToDoubles.h"
#include "MALLOC.h"
// =============================================================================
double *stringsToDoubles(const char **pSTRs, int nbElements,
                         BOOL bConvertByNAN, stringToDoubleError *ierr)
{
    double *dValues = NULL;

    *ierr = STRINGTODOUBLE_ERROR;
    if (nbElements <= 0) return NULL;

    if (pSTRs == NULL)
    {
        *ierr = STRINGTODOUBLE_MEMORY_ALLOCATION;
    }
    else
    {
        dValues = (double*)MALLOC(sizeof(double) * nbElements);
        if (dValues)
        {
            int i = 0;
            for (i = 0; i < nbElements; i++)
            {
                dValues[i] = stringToDouble(pSTRs[i], bConvertByNAN, ierr);
                if (*ierr != STRINGTODOUBLE_NO_ERROR)
                {
                    FREE(dValues);
                    return (dValues = NULL);
                }
            }
        }
        else
        {
            *ierr = STRINGTODOUBLE_MEMORY_ALLOCATION;
        }
    }
    return dValues;
}
// =============================================================================
