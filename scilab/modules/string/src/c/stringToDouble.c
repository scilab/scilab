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
double stringToDouble(const char *pSTR, BOOL bConvertByNAN, stringToDoubleError * ierr)
{
    double dValue = 0.0;

    *ierr = STRINGTODOUBLE_ERROR;
    if (pSTR)
    {
        if ((stricmp(pSTR, NanString) == 0) || (stricmp(pSTR, NegNanString) == 0) || (stricmp(pSTR, PosNanString) == 0))
        {
            dValue = returnNAN();
        }
        else if ((stricmp(pSTR, InfString) == 0) || (stricmp(pSTR, PosInfString) == 0))
        {
            dValue = returnINF(TRUE);
        }
        else if (stricmp(pSTR, NegInfString) == 0)
        {
            dValue = returnINF(FALSE);
        }
        else
        {
            double v = 0.;

            int err = sscanf(pSTR, DEFAULT_DOUBLE_MAX_DIGIT_FORMAT, &v);

            if (err == 1)
            {
                double v2 = 0.;
                char *pEnd = NULL;

                v2 = strtod(pSTR, &pEnd);
                if (strcmp(pEnd, "") == 0)
                {
                    dValue = v2;
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
                        return (dValue = 0.0);
                    }
                }
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
                    return (dValue = 0.0);
                }
            }
        }
        *ierr = STRINGTODOUBLE_NO_ERROR;
    }
    else
    {
        *ierr = STRINGTODOUBLE_MEMORY_ALLOCATION;
    }
    return dValue;
}

/* ========================================================================== */
static double returnNAN(void)
{
    static int first = 1;
    static double nan = 1.0;

    if (first)
    {
        nan = (nan - (double)first) / (nan - (double)first);
        first = 0;
    }
    return (nan);
}

/* ========================================================================== */
static double returnINF(BOOL bPositive)
{
    double v = 0 - 0;
    double p = 10;

    if (!bPositive)
        p = -10;
    return (double)p / (double)v;
}

/* ========================================================================== */
