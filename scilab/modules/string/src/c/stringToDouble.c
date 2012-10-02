/*
*  Copyright (C) 2010-2011 - DIGITEO - Allan CORNET
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
#include "MALLOC.h"
#ifdef  _MSC_VER
#include "strdup_windows.h"
#endif
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
    char* pstReturn = strdup(_pst);
    char* pstFind = pstReturn;
    do
    {
        pstFind = strchr(pstFind, 'D');
        if(pstFind)
        {
            pstFind[0] = 'E';
        }
    }while(pstFind);

    pstFind = pstReturn;
    do
    {
        pstFind = strchr(pstFind, 'd');
        if(pstFind)
        {
            pstFind[0] = 'e';
        }
    }while(pstFind);

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
