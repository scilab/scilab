/*-----------------------------------------------------------------------------------*/
/* Copyright INRIA */
/*-----------------------------------------------------------------------------------*/
#include <string.h>

#ifdef HAVE_LIMITS_H
	#include <limits.h>
	#define MAXLONG LONG_MAX
#else 
	#ifdef HAVE_VALUES_H
		#include <values.h>
	#endif /* HAVE_VALUES_H */
#endif /* !HAVE_LIMITS_H */

#include "machine.h"
#include "MALLOC.h"
#include "sciprint.h"
#include "scimem.h"
/*-----------------------------------------------------------------------------------*/
#ifndef MAXLONG
#define MAXLONG LONG_MAX
#endif

#ifndef LONG_MAX 
#define LONG_MAX 2147483647L 
#endif 

#ifndef MAXLONG
#define MAXLONG LONG_MAX
#endif
/*-----------------------------------------------------------------------------------*/
static char *the_p=NULL;
static char *the_ps=NULL;
static char *the_gp=NULL;
static char *the_gps=NULL;
/*-----------------------------------------------------------------------------------*/
static SIZE_T SizeStack=0;
static SIZE_T SizeGStack=0;
/*-----------------------------------------------------------------------------------*/
extern struct 
{
  double stk_1[2];
}
C2F(stack);
/*-----------------------------------------------------------------------------------*/
integer C2F(scimem)(integer *n, integer *ptr)
{
  char *p1 = NULL;
  if (*n > 0)
  {
    /* add 1 for alignment problems */
    double dsize = ((double) sizeof(double)) * (*n + 1);
    unsigned long ulsize = ((unsigned long)sizeof(double)) * (*n + 1);
    if ( dsize != (double) ulsize)
	{
		unsigned long pos = MAXLONG/sizeof(double);  
	    sciprint("stacksize requested size is too big (max < %lu)\r\n",pos);
	}
    else
	{
      p1 = (char *) SCISTACKMALLOC(((unsigned long) sizeof(double)) * (*n + 1));
	  if (p1) SizeStack=((unsigned long) sizeof(double)) * (*n + 1);
	}
	  
    if (p1 != NULL) 
	{
		the_ps = the_p;
		the_p = p1;
		/* add 1 for alignment problems */
		*ptr = ((int) (the_p - (char *)C2F(stack).stk_1))/sizeof(double) + 1;
    }
    else 
    {
		if (the_p == NULL) 
		{
			sciprint("No space to allocate Scilab stack\r\n");
			exit(1); 
		}
		*ptr=0;
    }
  }
  return(0);
}
/*-----------------------------------------------------------------------------------*/
integer C2F(scigmem)(integer *n, integer *ptr)
{
  char *p1;
  if (*n > 0)
  {
    /* add 1 for alignment problems */
    p1 = (char *) SCISTACKMALLOC((unsigned)sizeof(double) * (*n + 1));
    if (p1 != NULL) 
	{
		SizeGStack=((unsigned long) sizeof(double)) * (*n + 1);
		the_gps = the_gp;
		the_gp = p1;
		/* add 1 for alignment problems */
		*ptr = ((int) (the_gp - (char *)C2F(stack).stk_1))/sizeof(double) + 1;
    }
    else 
    {
		if (the_gp == NULL) 
		{
			sciprint("No space to allocate Scilab stack\r\n");
			exit(1); 
		}
		*ptr=0;
    }
  }
  return(0);
}
/*-----------------------------------------------------------------------------------*/
void C2F(freegmem)(void)
{
  if (the_gps != NULL) SCISTACKFREE(the_gps,SizeGStack);
}
/*-----------------------------------------------------------------------------------*/
void C2F(freemem)(void)
{
  if (the_ps != NULL) SCISTACKFREE(the_ps,SizeStack);
}
/*-----------------------------------------------------------------------------------*/
