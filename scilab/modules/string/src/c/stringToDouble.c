/*
*  Copyright (C) 2010-2011 - DIGITEO - Allan CORNET
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

/* ========================================================================== */
#if defined(__linux__)
#define _GNU_SOURCE             /* avoid dependency on GLIBC_2.7 */
#endif
/* ========================================================================== */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "stringToDouble.h"
#include "core_math.h"
#include "sci_malloc.h"
#include "os_string.h"
/* ========================================================================== */
#define DEFAULT_DOUBLE_MAX_DIGIT_FORMAT "%lg"
/* ========================================================================== */
static double returnINF(BOOL bPositive);
static double returnNAN(void);
/* ========================================================================== */

static char* replace_D_By_E(const char* _pst)
{
    //find and replace d and D by E for compatibility with strtod Linux/Mac
    char* pstReturn = os_strdup(_pst);
    char* pstFind = pstReturn;
    do
    {
        pstFind = strchr(pstFind, 'D');
        if (pstFind)
        {
            pstFind[0] = 'E';
        }
    }
    while (pstFind);

    pstFind = pstReturn;
    do
    {
        pstFind = strchr(pstFind, 'd');
        if (pstFind)
        {
            pstFind[0] = 'e';
        }
    }
    while (pstFind);

    return pstReturn;
}

double stringToDouble(const char *pSTR, BOOL bConvertByNAN, stringToDoubleError *ierr)
{
    double dValue = 0.0;
    *ierr = STRINGTODOUBLE_ERROR;
    if (pSTR)
    {
        if ((os_stricmp(pSTR, NanString) == 0) || (os_stricmp(pSTR, NegNanString) == 0) ||
                (os_stricmp(pSTR, PosNanString) == 0) || (os_stricmp(pSTR, ScilabPosNanString) == 0) ||
                (os_stricmp(pSTR, ScilabNanString) == 0) || (os_stricmp(pSTR, ScilabNegNanString) == 0))
        {
            dValue = returnNAN();
        }
        else if ((os_stricmp(pSTR, InfString) == 0) || (os_stricmp(pSTR, PosInfString) == 0) ||
                 (os_stricmp(pSTR, ScilabInfString) == 0) || (os_stricmp(pSTR, ScilabPosInfString) == 0))
        {
            dValue = returnINF(TRUE);
        }
        else if ((os_stricmp(pSTR, NegInfString) == 0) || (os_stricmp(pSTR, ScilabNegInfString) == 0))
        {
            dValue = returnINF(FALSE);
        }
        else if ((os_stricmp(pSTR, ScilabPiString) == 0) || (os_stricmp(pSTR, ScilabPosPiString) == 0))
        {
            dValue = M_PI;
        }
        else if (os_stricmp(pSTR, ScilabNegPiString) == 0)
        {
            dValue = -M_PI;
        }
        else if ((os_stricmp(pSTR, ScilabEpsString) == 0) || (os_stricmp(pSTR, ScilabPosEpsString) == 0))
        {
            dValue = EPSILON;
        }
        else if (os_stricmp(pSTR, ScilabNegEpsString) == 0)
        {
            dValue = -EPSILON;
        }
        else if ((os_stricmp(pSTR, ScilabEString) == 0) || (os_stricmp(pSTR, ScilabPosEString) == 0))
        {
            dValue = exp(1);
        }
        else if (os_stricmp(pSTR, ScilabNegEString) == 0)
        {
            dValue = -exp(1);
        }
        else
        {
            char* pstReplaced = replace_D_By_E(pSTR);
            char *pEnd = NULL;
            double v = strtod(pstReplaced, &pEnd);
            if ((v == 0) && (pEnd == pstReplaced))
            {
                if (bConvertByNAN)
                {
                    dValue = returnNAN();
                }
                else
                {
                    *ierr = STRINGTODOUBLE_NOT_A_NUMBER;
                    FREE(pstReplaced);
                    return (dValue = 0.0);
                }
            }
            else
            {
                if (strcmp(pEnd, "") == 0)
                {
                    dValue = v;
                }
                else
                {
                    if (bConvertByNAN)
                    {
                        dValue = returnNAN();
                    }
                    else
                    {
                        *ierr = STRINGTODOUBLE_NOT_A_NUMBER;
                        FREE(pstReplaced);
                        return (dValue = 0.0);
                    }
                }
            }

            FREE(pstReplaced);
        }
        *ierr = STRINGTODOUBLE_NO_ERROR;
    }
    else
    {
        *ierr = STRINGTODOUBLE_MEMORY_ALLOCATION;
    }
    return dValue;
}
// =============================================================================
static double returnINF(BOOL bPositive)
{
    double dbl1 = 1.0;
    double dbl0 = dbl1 - dbl1;
    int iSign = bPositive == 1 ? 1 : -1;

    return iSign * dbl1 / dbl0;
}
// =============================================================================
static double returnNAN(void)
{
    static int first = 1;
    static double nan = 1.0;

    if ( first )
    {
        nan = (nan - (double) first) / (nan - (double) first);
        first = 0;
    }
    return (nan);
}
// =============================================================================
