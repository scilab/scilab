
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
	#ifndef _WIN64
		#define _USE_32BIT_TIME_T 1
	#endif
#endif
/*--------------------------------------------------------------------------*/
#include <time.h>
#include <locale.h>
#include <stdio.h>
#include <errno.h>
#include "getdate.h"
#include "machine.h"
#include "sciprint.h"
#include "Scierror.h"
#include "localization.h"
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
#ifdef _MSC_VER
	#ifndef _MAX__TIME64_T
		#define _MAX__TIME64_T     0x100000000000i64    /* number of seconds from 00:00:00, 01/01/1970 UTC to 23:59:59. 12/31/2999 UTC */
	#endif
#endif
/*--------------------------------------------------------------------------*/
static int week_number (struct tm *tp);
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
    static struct __timeb64 timebufferW;
#else
  static struct timeval timebufferU;
#endif
static int week_days (int yday, int wday);
/*--------------------------------------------------------------------------*/
static int ChronoFlag=0;
/*--------------------------------------------------------------------------*/
void  C2F(scigetdate)(time_t *dt,int *ierr)
{
  *ierr=0;
  if (time(dt) == (time_t) - 1)
  {
    *ierr=errno;
  }
  ChronoFlag=1;

  #ifdef _MSC_VER
    _ftime64( &timebufferW );
  #else
	gettimeofday(&timebufferU,NULL);
  #endif
}
/*--------------------------------------------------------------------------*/
void C2F(convertdate)(time_t *dt,int w[10])
{
	// check that dt > 0 (and dt < _MAX__TIME64_T if _MSC_VER is defined)
	#ifdef _MSC_VER
	if ( (*dt<0) || (*dt> _MAX__TIME64_T) )
	#else
	if (*dt<0)
	#endif
	{
		w[0] = 0;
		w[1] = 0;
		w[2] = 0;
		w[3] = 0;
		w[4] = 0;
		w[5] = 0;
		w[6] = 0;
		w[7] = 0;
		w[8] = 0;
		w[9] = 0;
		if (*dt<0)	Scierror(999,_("%s: Wrong value for input argument #%d: Must be > %d.\n"),"getdate",1,0);
		#ifdef _MSC_VER
		else Scierror(999,_("%s: Wrong value for input argument #%d: Must be < %d.\n"),"getdate",1,_MAX__TIME64_T);
		#endif
	}
	else
	{
		struct tm *nowstruct=NULL;
		nowstruct = localtime(dt);
		if (nowstruct)
		{
			/** @TODO replace [int] by defines ... */
			w[0] = 1900 + nowstruct->tm_year;
			w[1] = 1    + nowstruct->tm_mon;
			w[2] = week_number(nowstruct);
			w[3] = 1    + nowstruct->tm_yday;
			w[4] = 1    + nowstruct->tm_wday;
			w[5] =        nowstruct->tm_mday;
			w[6] =        nowstruct->tm_hour;
			w[7] =        nowstruct->tm_min;
			w[8] =        nowstruct->tm_sec;
            if (ChronoFlag)
			{
       #ifdef _MSC_VER
				w[9] = timebufferW.millitm;
			 #else
				w[9] = timebufferU.tv_usec / 1000;  /* micro to ms */
			 #endif
			 ChronoFlag=0;
			}
			else
			{
				w[9] = 0;
			}
		}
	}
}
/*--------------------------------------------------------------------------*/
/* week_days computes
 *  The number of days from the first day of the first ISO week of this
 *  year to the year day YDAY with week day WDAY.  ISO weeks start on
 *  Monday; the first ISO week has the year's first Thursday.  YDAY may
 *  be as small as YDAY_MINIMUM.  */
/*--------------------------------------------------------------------------*/
static int week_days (int yday,int wday)
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
      days = week_days (tp->tm_yday + (365 + __isleap
					   (year)),
			    tp->tm_wday);
    }
  else
    {
      int d = week_days (tp->tm_yday - (365 + __isleap
					    (year)),
			     tp->tm_wday);
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
