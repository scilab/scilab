#include "../machine.h"
#include <time.h>
#include <locale.h>
#include <stdio.h>

#if WIN32
	#include <sys/timeb.h>
#else
	#include <sys/time.h> 
#endif

#if WIN32
extern void sciprint __PARAMS((char *fmt,...));
#endif

static int week_number __PARAMS ((struct tm *tp));
void C2F(scigetdate) __PARAMS ((time_t *dt, int *ierr));
void C2F(convertdate) __PARAMS ((time_t *dt, int w[]));



#if WIN32
static struct __timeb64 timebufferW;
#else
static struct timeval timebufferU;
#endif

static int ChronoFlag=0;

void  C2F(scigetdate)(dt,ierr)
     time_t *dt;
     int *ierr;
{
  *ierr=0;
  if (time(dt) == (time_t) - 1) 
  {
    *ierr=1;
  }
  ChronoFlag=1;
  #if WIN32
	_ftime64( &timebufferW );
  #else
	gettimeofday(&timebufferU,NULL);
  #endif
}

void C2F(convertdate)(dt,w)
     int w[];
     time_t *dt;
{
	struct tm *nowstruct=NULL;

  /* Is setlocale useful?
     (void) setlocale(LC_ALL, ""); */

	nowstruct = localtime(dt);
	if (nowstruct)
	{
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
                        #if WIN32
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
	else
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
		sciprint("dt=getdate(x) x must be >0.\n");
	}
}

/* following code issued from glibc-2.1.2/time/strftime.c, 
   Tanks  to Ton van Overbeek
*/

/* week_days computes
 *  The number of days from the first day of the first ISO week of this
 *  year to the year day YDAY with week day WDAY.  ISO weeks start on
 *  Monday; the first ISO week has the year's first Thursday.  YDAY may
 *  be as small as YDAY_MINIMUM.  */

#define ISO_WEEK_START_WDAY 1 /* Monday */
#define ISO_WEEK1_WDAY 4 /* Thursday */
#define YDAY_MINIMUM (-366)

static int week_days __PARAMS ((int yday, int wday));
static int
week_days (yday, wday)
     int yday;
     int wday;
{
  /* Add enough to the first operand of % to make it nonnegative.  */
  int big_enough_multiple_of_7 = (-YDAY_MINIMUM / 7 + 2) * 7;
  return (yday
          - (yday - wday + ISO_WEEK1_WDAY + big_enough_multiple_of_7) %
7
          + ISO_WEEK1_WDAY - ISO_WEEK_START_WDAY);
}



/* week_number computes 
 *      the ISO 8601  week  number  as  a  decimal  number
 *      [01,53].  In the ISO 8601 week-based system, weeks
 *      begin on a Monday and week 1 of the  year  is  the
 *      week  that includes both January 4th and the first
 *      Thursday of the year.   If  the  first  Monday  of
 *      January  is  the  2nd,  3rd, or 4th, the preceding
 *      days are part of the last week  of  the  preceding
 *      year. */

#define TM_YEAR_BASE 1900
#ifndef __isleap
/* Nonzero if YEAR is a leap year (every 4 years,
   except every 100th isn't, and every 400th is).  */
# define __isleap(year) \
  ((year) % 4 == 0 && ((year) % 100 != 0 || (year) % 400 == 0))
#endif
static int week_number(tp)
     struct tm *tp;
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
