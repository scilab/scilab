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
#ifndef _MSC_VER
#ifndef stricmp
#define stricmp strcasecmp
#endif
#else
#define stricmp _stricmp
#endif
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

static wchar_t* replace_D_By_EW(const wchar_t* _pst)
{
    //find and replace d and D by E for compatibility with strtod Linux/Mac
    wchar_t* pstReturn = os_wcsdup(_pst);
    wchar_t* pstFind = pstReturn;
    do
    {
        pstFind = wcschr(pstFind, L'D');
        if (pstFind)
        {
            pstFind[0] = L'E';
        }
    }
    while (pstFind);

    pstFind = pstReturn;
    do
    {
        pstFind = wcschr(pstFind, L'd');
        if (pstFind)
        {
            pstFind[0] = L'e';
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
        if ((stricmp(pSTR, NanString) == 0) || (stricmp(pSTR, NegNanString) == 0) ||
                (stricmp(pSTR, PosNanString) == 0) || (stricmp(pSTR, ScilabPosNanString) == 0) ||
                (stricmp(pSTR, ScilabNanString) == 0) || (stricmp(pSTR, ScilabNegNanString) == 0))
        {
            dValue = returnNAN();
        }
        else if ((stricmp(pSTR, InfString) == 0) || (stricmp(pSTR, PosInfString) == 0) ||
                 (stricmp(pSTR, ScilabInfString) == 0) || (stricmp(pSTR, ScilabPosInfString) == 0))
        {
            dValue = returnINF(TRUE);
        }
        else if ((stricmp(pSTR, NegInfString) == 0) || (stricmp(pSTR, ScilabNegInfString) == 0))
        {
            dValue = returnINF(FALSE);
        }
        else if ((stricmp(pSTR, ScilabPiString) == 0) || (stricmp(pSTR, ScilabPosPiString) == 0))
        {
            dValue = M_PI;
        }
        else if (stricmp(pSTR, ScilabNegPiString) == 0)
        {
            dValue = -M_PI;
        }
        else if ((stricmp(pSTR, ScilabEpsString) == 0) || (stricmp(pSTR, ScilabPosEpsString) == 0))
        {
            dValue = EPSILON;
        }
        else if (stricmp(pSTR, ScilabNegEpsString) == 0)
        {
            dValue = -EPSILON;
        }
        else if ((stricmp(pSTR, ScilabEString) == 0) || (stricmp(pSTR, ScilabPosEString) == 0))
        {
            dValue = exp(1);
        }
        else if (stricmp(pSTR, ScilabNegEString) == 0)
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
double stringToDoubleW(const wchar_t *pSTR, BOOL bConvertByNAN, stringToDoubleError *ierr)
{
    double dValue = 0.0;
    *ierr = STRINGTODOUBLE_ERROR;
    if (pSTR)
    {
        if ((wcsicmp(pSTR, NanStringW) == 0) || (wcsicmp(pSTR, NegNanStringW) == 0) ||
                (wcsicmp(pSTR, PosNanStringW) == 0) || (wcsicmp(pSTR, ScilabPosNanStringW) == 0) ||
                (wcsicmp(pSTR, ScilabNanStringW) == 0) || (wcsicmp(pSTR, ScilabNegNanStringW) == 0))
        {
            dValue = returnNAN();
        }
        else if ((wcsicmp(pSTR, InfStringW) == 0) || (wcsicmp(pSTR, PosInfStringW) == 0) ||
                 (wcsicmp(pSTR, ScilabInfStringW) == 0) || (wcsicmp(pSTR, ScilabPosInfStringW) == 0))
        {
            dValue = returnINF(TRUE);
        }
        else if ((wcsicmp(pSTR, NegInfStringW) == 0) || (wcsicmp(pSTR, ScilabNegInfStringW) == 0))
        {
            dValue = returnINF(FALSE);
        }
        else if ((wcsicmp(pSTR, ScilabPiStringW) == 0) || (wcsicmp(pSTR, ScilabPosPiStringW) == 0))
        {
            dValue = M_PI;
        }
        else if (wcsicmp(pSTR, ScilabNegPiStringW) == 0)
        {
            dValue = -M_PI;
        }
        else if ((wcsicmp(pSTR, ScilabPosEStringW) == 0) || (wcsicmp(pSTR, ScilabEStringW) == 0))
        {
            dValue = exp(1);
        }
        else if (wcsicmp(pSTR, ScilabNegEStringW) == 0)
        {
            dValue = -exp(1);
        }
        else if ((wcsicmp(pSTR, ScilabEpsStringW) == 0) || (wcsicmp(pSTR, ScilabPosEpsStringW) == 0))
        {
            dValue = EPSILON;
        }
        else if (wcsicmp(pSTR, ScilabNegEpsStringW) == 0)
        {
            dValue = -EPSILON;
        }
        else
        {
            wchar_t* pstReplaced = replace_D_By_EW(pSTR);
            wchar_t *pEnd = NULL;
            double v = wcstod(pstReplaced, &pEnd);
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
                if (wcscmp(pEnd, L"") == 0)
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
