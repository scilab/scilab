/*-----------------------------------------------------------------------------------*/
/* Copyright INRIA/ENPC */
/*-----------------------------------------------------------------------------------*/
#include <stdio.h>

#if ~defined(THINK_C) && ~defined(__MWERKS__)
	#if !(defined __MSC__) && !(defined __MINGW32__) 
		#include <sys/time.h>
	#else 
		#include <windows.h>
		#include <winbase.h> /* header du compteur haute résolution */
	#endif 
#endif 

#include <time.h>
#include "../machine.h"

#if defined(THINK_C) || defined(__MWERKS__)
	#include <Threads.h> 
#endif
/*-----------------------------------------------------------------------------------*/
#define DT_TIMER 10000

#ifndef CLOCKS_PER_SEC
	#if defined(sun)
		#define CLOCKS_PER_SEC 1000000
	#endif
#endif
/*-----------------------------------------------------------------------------------*/
#if WIN32
	static __int64 i64UserTick1;
	static LARGE_INTEGER   Tick1;
	typedef enum
  {
    WINNT,	WIN2K_XP, WIN9X, UNKNOWN
  }PLATFORM;
#else
	static clock_t t1;
#endif

static int init_clock = 1;

#if WIN32
	PLATFORM GetPlatform(void);
	static long stimerwin(void);
#endif
/*-----------------------------------------------------------------------------------*/
#if WIN32
PLATFORM GetPlatform()
{
  OSVERSIONINFO osvi;
  osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
  if (!GetVersionEx(&osvi)) return UNKNOWN;
  switch (osvi.dwPlatformId)
  {
    case VER_PLATFORM_WIN32_WINDOWS: return WIN9X;
    case VER_PLATFORM_WIN32_NT: 
			if (osvi.dwMajorVersion == 4)
				return WINNT;
      else
				return WIN2K_XP;
  }
  return UNKNOWN;
}
#endif
/*-----------------------------------------------------------------------------------*/
/* Allan CORNET 2004 */
int C2F(timer)(double *etime)
{
#if WIN32 
  if (GetPlatform() == WIN9X)
  {
    /* Timer high Precision */
    /* It is not easy to have CPU Time on Win9x */
    /* timer don't return CPU Time but time between 2 calls of Timer() 2.7 compatibility*/
    LARGE_INTEGER   Tick2;
    LARGE_INTEGER   freq;
      
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&Tick2);
      
    if (init_clock == 1) {init_clock = 0; Tick1 = Tick2;}
    *etime=(double) ( (double) (Tick2.QuadPart - Tick1.QuadPart) / (double)(freq.QuadPart));
    Tick1 = Tick2;
  }
  else
  if (GetPlatform() == UNKNOWN)
  {
		*etime=(double)-1;
  }
  else
  {
		/* NT 3.5 & > */
		/* Return CPU Time */
		FILETIME  ftCreation, ftExit, ftKernel,  ftUser;
   	__int64 i64UserTick2;
	
		GetProcessTimes(GetCurrentProcess(), &ftCreation, &ftExit, &ftKernel, &ftUser);
		i64UserTick2=*((__int64 *) &ftUser);
		if (init_clock == 1) {init_clock = 0; i64UserTick1 = i64UserTick2;}
		*etime=(double) ((double)(i64UserTick2 - i64UserTick1)/(double)10000000U);
		i64UserTick1 = i64UserTick2;
	}
#else
  clock_t t2;
  t2 = clock();
  if (init_clock == 1) {init_clock = 0; t1 = t2;}
  *etime=(double)((double)(t2 - t1)/(double)CLOCKS_PER_SEC);
  t1 = t2;
#endif
  return(0);
}
/*-----------------------------------------------------------------------------------*/
/* define X_GETTIMEOFDAY macro, a portable gettimeofday() */
#if  defined(VMS)
	#define X_GETTIMEOFDAY(t) gettimeofday(t)
#else
	#if defined(THINK_C) || defined(__MWERKS__)
		#define X_GETTIMEOFDAY(t) 0 
	#else
		#if defined(WIN32)
			#if !(defined __MSC__)
				#ifndef  __MINGW32__
					#define X_GETTIMEOFDAY(t) gettimeofday(t, &tmz )
					static struct timezone tmz;
				#else
					#define X_GETTIMEOFDAY(t) 0
				#endif /** __MINGW32__ **/
			#else
				#define X_GETTIMEOFDAY(t) 0
			#endif /* MSC__ */
		#else
			#define X_GETTIMEOFDAY(t) gettimeofday(t, (struct timezone*)0)
		#endif
	#endif
#endif 
/*-----------------------------------------------------------------------------------*/
/* this function is no more used  */
/*-----------------------------------------------------------------------------------*/
static long int scilab_stimer_deprecated(void)
{
#if defined(THINK_C)||defined(__MWERKS__) 
        YieldToAnyThread();
        return(0);
#else 
#if !(defined __MSC__) && !(defined __MINGW32__)
  struct timeval ctime;
  X_GETTIMEOFDAY(&ctime);
  scilab_timer_check();
  return(ctime.tv_usec);
#else 
  return(stimerwin());
#endif /* !(defined __MSC__) && !(defined __MINGW32__) */ 
#endif /* defined(THINK_C)||defined(__MWERKS__) */
}
/*-----------------------------------------------------------------------------------*/
/* stimer */
/*-----------------------------------------------------------------------------------*/
#if WIN32
static long stimerwin(void)
{
	long int i;
	union {FILETIME ftFileTime;__int64  ftInt64;} ftRealTime; 
	GetSystemTimeAsFileTime(&ftRealTime.ftFileTime); 
	i= (int) (ftRealTime.ftInt64  & ((LONGLONG) 0x0ffffffff));
	return( i/10); /** convert to microseconds **/
}
#endif
/*-----------------------------------------------------------------------------------*/
/* returns 1 if interval from last call is greater than 
 * a time interval of dt microsec (dt=10000)
 */
/*-----------------------------------------------------------------------------------*/
#if WIN32
static long int ctime_old=0;
int scilab_timer_check(void)
{
	int rep;
	long int ctime = stimerwin();
	rep = ( ctime - ctime_old > DT_TIMER ) ? 1 : 0 ;
	ctime_old=ctime;
	return rep;
}
#else 
int scilab_timer_check(void)
{
  int rep;
  static struct timeval ctime_old;
  struct timeval ctime;
  X_GETTIMEOFDAY(&ctime);
  rep = (ctime.tv_sec > ctime_old.tv_sec) ? 1  : ( ctime.tv_usec - ctime_old.tv_usec > DT_TIMER ) ? 1 : 0 ;
  if (rep) ctime_old=ctime;
  return rep;
}
#endif /* WIN32  */
/*-----------------------------------------------------------------------------------*/
