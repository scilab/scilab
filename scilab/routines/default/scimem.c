/* Copyright INRIA */

#include <string.h>

#ifdef __STDC__
#include <stdlib.h>
#else
#include <malloc.h>
#endif

#include "../machine.h"

#ifdef WIN32 
#include <windows.h>
#endif

#ifdef HAVE_LIMITS_H
#include <limits.h>
#define MAXLONG LONG_MAX
#else 
#ifdef HAVE_VALUES_H
#include <values.h>
#endif /* HAVE_VALUES_H */
#endif /* !HAVE_LIMITS_H */

#ifndef MAXLONG
#define MAXLONG LONG_MAX
#endif

#if defined(netbsd)
#include <ieeefp.h>
#endif

#if defined(freebsd)
#include <floatingpoint.h>
#endif

extern void sciprint __PARAMS((char *fmt,...));

#if defined(netbsd) || defined(freebsd)
void C2F(nofpex)()
{
  fpsetmask(0);   /* Don't dump core on FPE return Inf or NaN */
}
#else
void C2F(nofpex)(void)
{
  return;
}
#endif

char *the_p=NULL;
char *the_ps=NULL;
char *the_gp=NULL;
char *the_gps=NULL;

/* static  char *the_p,*the_ps;*/

extern struct {
  double stk_1[2];
} C2F(stack);

integer C2F(scimem)(integer *n, integer *ptr)
{
  char *p1 = NULL;
  if (*n > 0){
    /* add 1 for alignment problems */
    double dsize = ((double) sizeof(double)) * (*n + 1);
    unsigned long ulsize = ((unsigned long)sizeof(double)) * (*n + 1);
    if ( dsize != (double) ulsize)
	  {
	    unsigned long pos = MAXLONG/sizeof(double);  

	    sciprint("stacksize requested size is too big (max < %lu)\r\n",pos);
	  }
    else 
      p1 = (char *) malloc(((unsigned long) sizeof(double)) * (*n + 1));
    if (p1 != NULL) {
      the_ps = the_p;
      the_p = p1;
      /* add 1 for alignment problems */
      *ptr = ((int) (the_p - (char *)C2F(stack).stk_1))/sizeof(double) + 1;
    }
    else 
      {
	if (the_p == NULL) {
	  sciprint("No space to allocate Scilab stack\r\n");
	  exit(1); 
	}
	*ptr=0;
      }
  }
  return(0);
}
integer C2F(scigmem)(integer *n, integer *ptr)
{
  char *p1;
  if (*n > 0){
    /* add 1 for alignment problems */
    p1 = (char *) malloc((unsigned)sizeof(double) * (*n + 1));
    if (p1 != NULL) {
      the_gps = the_gp;
      the_gp = p1;
      /* add 1 for alignment problems */
      *ptr = ((int) (the_gp - (char *)C2F(stack).stk_1))/sizeof(double) + 1;
    }
    else 
      {
	if (the_gp == NULL) {
	  sciprint("No space to allocate Scilab stack\r\n");
	  exit(1); 
	}
	*ptr=0;
      }
  }
  return(0);
}
void C2F(freegmem)(void)
{
  if (the_gps != NULL) free(the_gps);
}

void C2F(freemem)(void)
{
  if (the_ps != NULL) free(the_ps);
}

