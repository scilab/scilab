/* Copyright INRIA/ENPC */

#include <stdio.h>

#if ~defined(THINK_C) && ~defined(__MWERKS__)
#if !(defined __MSC__) && !(defined __ABSC__) && !(defined __MINGW32__) 
#include <sys/time.h>
#else 
#include <windows.h>
static int stimerwin(void);
#endif 
#endif 

#include <time.h>
#include "../machine.h"

#if defined(THINK_C) || defined(__MWERKS__)
#include <Threads.h> 
#endif

#ifndef CLOCKS_PER_SEC
#if defined(sun)
#define CLOCKS_PER_SEC 1000000
#endif
#endif

static clock_t t1;
static int init_clock = 1;

int C2F(timer)(double *etime)
{
  clock_t t2;
  t2 = clock();
  if (init_clock == 1) {init_clock = 0; t1 = t2;}
  *etime=(double)((double)(t2 - t1)/(double)CLOCKS_PER_SEC);
  t1 = t2;
  return(0);
}

/* define X_GETTIMEOFDAY macro, a portable gettimeofday() */
#if  defined(VMS)
#define X_GETTIMEOFDAY(t) gettimeofday(t)
#else
#if defined(THINK_C) || defined(__MWERKS__)
#define X_GETTIMEOFDAY(t) 0 
#else
#if defined(WIN32)
#if !(defined __MSC__) && !(defined __ABSC__)
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

/***********************************************************
 * stimer is used while runing the interpreter (run.f) 
 * to fix a timer for checking X11 or windows events 
 ***********************************************************/
    

int C2F(stimer)(void)
{
#if defined(THINK_C)||defined(__MWERKS__) 
        YieldToAnyThread();
        return(0);
#else 
#if !(defined __MSC__) && !(defined __ABSC__)&& !(defined __MINGW32__)
  struct timeval ctime;
  X_GETTIMEOFDAY(&ctime);
  return(ctime.tv_usec);
#else 
  return(stimerwin());
#endif /* !(defined __MSC__) && !(defined __ABSC__)&& !(defined __MINGW32__) */ 
#endif /* defined(THINK_C)||defined(__MWERKS__) */
}

/****************************
 * stimer for non cygwin win32 compilers 
 ****************************/

#if (defined __MSC__) || (defined __ABSC__) || (defined __MINGW32__)
static int stimerwin(void)
{
  int i;
  union {FILETIME ftFileTime;
    __int64  ftInt64;
  } ftRealTime; 
  SYSTEMTIME st;
  GetSystemTime(&st);
  SystemTimeToFileTime(&st,&ftRealTime.ftFileTime);
  /* Filetimes are in 100NS units */
  i= (int) (ftRealTime.ftInt64  & ((LONGLONG) 0x0ffffffff));
  return( i/10); /** convert to microseconds **/
}
#endif

int C2F(fclock)(void)
{
  return (int)clock();
}
