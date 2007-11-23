/*--------------------------------------------------------------------------*/
/* Copyright INRIA/ENPC */
/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include <time.h>
#ifdef _MSC_VER
	#include <windows.h>
#else
	#include <sys/time.h>
#endif
#include "machine.h"
#include "timerEvents.h"
/*--------------------------------------------------------------------------*/
#define DT_TIMER 10000
/*--------------------------------------------------------------------------*/
#ifndef _MSC_VER
	#define X_GETTIMEOFDAY(t) gettimeofday(t, (struct timezone*)0)
#endif
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
static ULARGE_INTEGER ctime_old;
static BOOL Initialize_ctime_old=TRUE;
int scilab_timer_check(void)
{
	int rep=0;
	ULARGE_INTEGER lctime;
	FILETIME ftFileTime;

	if (Initialize_ctime_old)
	{
		ctime_old.LowPart = 0;
		ctime_old.HighPart = 0;
		Initialize_ctime_old=FALSE;
	}

	GetSystemTimeAsFileTime(&ftFileTime); /* Resolution 100 nsec */
	lctime.LowPart = ftFileTime.dwLowDateTime; 
	lctime.HighPart = ftFileTime.dwHighDateTime;

	rep = ( (lctime.QuadPart  - ctime_old.QuadPart)  > DT_TIMER ) ? 1 : 0 ;
	if (rep) ctime_old=lctime;
	return rep;
}
#else 
int scilab_timer_check(void)
{
  int rep;
  static struct timeval ctime_old;
  struct timeval lctime;
  X_GETTIMEOFDAY(&lctime);
  rep = (lctime.tv_sec > ctime_old.tv_sec) ? 1  : ( lctime.tv_usec - ctime_old.tv_usec > DT_TIMER ) ? 1 : 0 ;
  if (rep) ctime_old=lctime;
  return rep;
}
#endif /* _MSC_VER */
/*--------------------------------------------------------------------------*/
