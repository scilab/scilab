/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA -
 * Copyright (C) DIGITEO - 2012 - Allan CORNET
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
#ifdef _MSC_VER
#include <sys/timeb.h>
#else
#include <sys/time.h>
#endif
#include <time.h>
#include <stdio.h>
#include "getdate.h"
#include "sci_malloc.h"
#include "transposeMatrix.h"
/*--------------------------------------------------------------------------*/
#define YEAR_INDEX 0
#define MONTH_INDEX 1
#define WEEK_NUMBER_INDEX 2
#define DAY_OF_YEAR_INDEX 3
#define WEEKDAY_INDEX 4
#define DAY_OF_MONTH_INDEX 5
#define HOUR_OF_DAY_INDEX 6
#define MINUTE_INDEX 7
#define SECOND_INDEX 8
#define MILLISECOND_INDEX 9
/*--------------------------------------------------------------------------*/
#define ISO_WEEK_START_WDAY 1 /* Monday */
#define ISO_WEEK1_WDAY 4 /* Thursday */
#define YDAY_MINIMUM (-366)
#define TM_YEAR_BASE 1900
#ifndef __isleap
/* Nonzero if YEAR is a leap year (every 4 years,
except every 100th isn't, and every 400th is).  */
# define __isleap(year) \
    ((year) % 4 == 0 && ((year) % 100 != 0 || (year) % 400 == 0))
#endif
/*--------------------------------------------------------------------------*/
static int week_number (struct tm *tp);
static int week_days (int yday, int wday);
/*--------------------------------------------------------------------------*/
double *getCurrentDateAsDoubleVector(int *iErr)
{
    double *dVector = (double*) MALLOC(sizeof(double) * NB_ELEMNT_ARRAY_GETDATE);
    *iErr = 1;
    if (dVector)
    {
        struct tm *nowstruct = NULL;
        double milliseconds = 0.;
#ifdef _MSC_VER
        /* manages date  up through 23:59:59, December 31, 3000 */
        /* previous version was limited to  19:14:07 January 18, 2038, UTC. */
        __time64_t long_time;
        struct __timeb64 tstruct;
        _ftime64(&tstruct);
        long_time = tstruct.time;
        nowstruct = _localtime64(&long_time);
#else
        struct timeval timebuffer;
        gettimeofday(&timebuffer, NULL);
        nowstruct = localtime(&timebuffer);
#endif

#ifdef _MSC_VER
        milliseconds = (double)(tstruct.millitm);
#else
        milliseconds = (double)(timebuffer.tv_usec / 1000);  /* micro to ms */
#endif
        if (milliseconds < 0)
        {
            milliseconds = 0.;
        }

        if (nowstruct)
        {
            dVector[YEAR_INDEX] =           (double)(1900 + nowstruct->tm_year);
            dVector[MONTH_INDEX] =          (double)(1    + nowstruct->tm_mon);
            dVector[WEEK_NUMBER_INDEX] =    (double)(week_number(nowstruct));
            dVector[DAY_OF_YEAR_INDEX] =    (double)(1    + nowstruct->tm_yday);
            dVector[WEEKDAY_INDEX] =        (double)(1    + nowstruct->tm_wday);
            dVector[DAY_OF_MONTH_INDEX] =   (double)(nowstruct->tm_mday);
            dVector[HOUR_OF_DAY_INDEX] =    (double)(nowstruct->tm_hour);
            dVector[MINUTE_INDEX] =         (double)(nowstruct->tm_min);
            dVector[SECOND_INDEX] =         (double)(nowstruct->tm_sec);
            dVector[MILLISECOND_INDEX] =    (double)milliseconds;
            *iErr = 0;
        }
    }
    return dVector;
}
/*--------------------------------------------------------------------------*/
double getCurrentDateAsUnixTimeConvention(void)
{
    double dValue = 0.;
#ifdef _MSC_VER
    /* manages date  up through 23:59:59, December 31, 3000 */
    /* previous version was limited to  19:14:07 January 18, 2038, UTC. */
    struct __timeb64 tstruct;
    _ftime64(&tstruct);
    dValue = (double)tstruct.time;
#else
    time_t t;
    time(&t);
    dValue = (double)t;
#endif

    if (dValue < 0.)
    {
        dValue = 0.;
    }
    return dValue;
}
/*--------------------------------------------------------------------------*/
double * getConvertedDateAsDoubleVector(double dDate, int *iErr)
{
    double *dVector = (double*) MALLOC(sizeof(double) * NB_ELEMNT_ARRAY_GETDATE);
    *iErr = 1;
    if (dVector)
    {
        struct tm *tstruct = NULL;
        double milliseconds = 0.;
#ifdef _MSC_VER
        __time64_t instantT = (__time64_t)dDate;
        tstruct = _localtime64(&instantT);
#else
        time_t instantT = (time_t)dDate;
        tstruct = localtime(&instantT);
#endif
        milliseconds = dDate - (double)instantT;
        if (milliseconds > 0)
        {
            if (milliseconds > 999)
            {
                milliseconds = (double)999;
            }
        }
        else
        {
            milliseconds = 0.;
        }

        if (tstruct)
        {
            dVector[YEAR_INDEX] =           (double)(1900 + tstruct->tm_year);
            dVector[MONTH_INDEX] =          (double)(1    + tstruct->tm_mon);
            dVector[WEEK_NUMBER_INDEX] =    (double)(week_number(tstruct));
            dVector[DAY_OF_YEAR_INDEX] =    (double)(1    + tstruct->tm_yday);
            dVector[WEEKDAY_INDEX] =        (double)(1    + tstruct->tm_wday);
            dVector[DAY_OF_MONTH_INDEX] =   (double)(tstruct->tm_mday);
            dVector[HOUR_OF_DAY_INDEX] =    (double)(tstruct->tm_hour);
            dVector[MINUTE_INDEX] =         (double)(tstruct->tm_min);
            dVector[SECOND_INDEX] =         (double)(tstruct->tm_sec);
            dVector[MILLISECOND_INDEX] =    (double)milliseconds;
            *iErr = 0;
        }
        else
        {
            *iErr = 2;
        }
    }
    return dVector;
}
/*--------------------------------------------------------------------------*/
double *getConvertedDateAsMatrixOfDouble(double *dDates, int nbElements, int *iErr)
{
    double *dResults = NULL;
    int lenResults = nbElements * NB_ELEMNT_ARRAY_GETDATE;
    *iErr = 1;

    dResults = (double*)MALLOC(sizeof(double) * lenResults);
    if (dResults)
    {
        int k = 0;
        int j = 0;

        int m = 0;
        int n = 0;

        double *dResTmp = NULL;

        for (k = 0; k < lenResults; k++)
        {
            dResults[k] = 0;
        }

        *iErr = 0;
        for (k = 0; k < nbElements; k++)
        {
            int iErr2 = 0;
            double *dConverted = getConvertedDateAsDoubleVector(dDates[k], &iErr2);
            if (iErr2 == 0)
            {
                int n = 0;
                for (n = 0; n < NB_ELEMNT_ARRAY_GETDATE; n++)
                {
                    dResults[j] = dConverted[n];
                    j++;
                }
                FREE(dConverted);
                dConverted = NULL;
            }
            else
            {
                FREE(dResults);
                FREE(dConverted);
                dResults = NULL;
                *iErr = iErr2;
                return NULL;
            }
        }

        m = nbElements;
        n = NB_ELEMNT_ARRAY_GETDATE;
        dResTmp = transposeMatrixDouble(n, m, dResults);
        if (dResTmp)
        {
            FREE(dResults);
            dResults = dResTmp;
            *iErr = 0;
        }
        else
        {
            *iErr = 1;
        }

    }
    return dResults;
}
/*--------------------------------------------------------------------------*/
/* week_days computes
*  The number of days from the first day of the first ISO week of this
*  year to the year day YDAY with week day WDAY.  ISO weeks start on
*  Monday; the first ISO week has the year's first Thursday.  YDAY may
*  be as small as YDAY_MINIMUM.  */
/*--------------------------------------------------------------------------*/
static int week_days (int yday, int wday)
{
    /* Add enough to the first operand of % to make it nonnegative.  */
    int big_enough_multiple_of_7 = (-YDAY_MINIMUM / 7 + 2) * 7;
    return (yday - (yday - wday + ISO_WEEK1_WDAY + big_enough_multiple_of_7) % 7 + ISO_WEEK1_WDAY - ISO_WEEK_START_WDAY);
}
/*--------------------------------------------------------------------------*/
/* week_number computes
*      the ISO 8601  week  number  as  a  decimal  number
*      [01,53].  In the ISO 8601 week-based system, weeks
*      begin on a Monday and week 1 of the  year  is  the
*      week  that includes both January 4th and the first
*      Thursday of the year.   If  the  first  Monday  of
*      January  is  the  2nd,  3rd, or 4th, the preceding
*      days are part of the last week  of  the  preceding
*      year. */
static int week_number(struct tm *tp)
{
    int year = tp->tm_year + TM_YEAR_BASE;
    int days = week_days (tp->tm_yday, tp->tm_wday);

    if (days < 0)
    {
        /* This ISO week belongs to the previous year.  */
        year--;
        days = week_days (tp->tm_yday + (365 + __isleap(year)), tp->tm_wday);
    }
    else
    {
        int d = week_days (tp->tm_yday - (365 + __isleap(year)), tp->tm_wday);
        if (0 <= d)
        {
            /* This ISO week belongs to the next year.  */
            year++;
            days = d;
        }
    }
    return ( (int)days / 7 + 1);
}
/*--------------------------------------------------------------------------*/

