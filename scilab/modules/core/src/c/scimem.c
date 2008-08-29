/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "MALLOC.h"
#include "stack-c.h"
#include "scimem.h"
#include "sciprint.h"
#include "localization.h"
#include "BOOL.h"
/*--------------------------------------------------------------------------*/
#ifdef USE_DYNAMIC_STACK
extern integer scimem64(integer *n, integer newsize, BOOL isglobal);
extern void freemem64(BOOL isglobal);
#else
static char *the_p=NULL;
static char *the_ps=NULL;
static char *the_gp=NULL;
static char *the_gps=NULL;
#endif
/*--------------------------------------------------------------------------*/
integer C2F(scimem)(integer *n, integer *ptr)
{
#ifdef USE_DYNAMIC_STACK
  return scimem64(ptr, *n, FALSE);
#else
  register char *p1 = NULL;
  if (*n > 0)
  {
	/* add 1 for alignment problems */
    p1 = (char *) SCISTACKMALLOC(((unsigned long) sizeof(double)) * (*n + 1));
		  
    if (p1 != NULL) 
	{
		the_ps = the_p;
		the_p = p1;
		/* add 1 for alignment problems */
		*ptr = ((int) (the_p - (char *)C2F(stack).Stk))/sizeof(double) + 1;
    }
    else 
    {
		if (the_p == NULL) 
		{
			sciprint(_("No space to allocate Scilab stack.\n"));
			exit(1); 
		}
		*ptr=0;
    }
  }
  return(0);
#endif
}
/*--------------------------------------------------------------------------*/
integer C2F(scigmem)(integer *n, integer *ptr)
{
#ifdef USE_DYNAMIC_STACK
  return scimem64(ptr, *n, TRUE);
#else
  register char *p1=NULL;
  if (*n > 0)
  {
    /* add 1 for alignment problems */
    p1 = (char *) SCISTACKMALLOC((unsigned)sizeof(double) * (*n + 1));
    if (p1 != NULL) 
	{
		the_gps = the_gp;
		the_gp = p1;
		/* add 1 for alignment problems */
		*ptr = ((int) (the_gp - (char *)C2F(stack).Stk))/sizeof(double) + 1;
    }
    else 
    {
		if (the_gp == NULL) 
		{
			sciprint(_("No space to allocate Scilab stack.\n"));
			exit(1); 
		}
		*ptr=0;
    }
  }
  return(0);
#endif
}
/*--------------------------------------------------------------------------*/
void C2F(freegmem)(void)
{
#ifdef USE_DYNAMIC_STACK
  freemem64(TRUE);
#else
  if (the_gps != NULL) SCISTACKFREE(the_gps);
#endif
}
/*--------------------------------------------------------------------------*/
void C2F(freemem)(void)
{
#ifdef USE_DYNAMIC_STACK
  freemem64(FALSE);
#else
  if (the_ps != NULL) SCISTACKFREE(the_ps);
#endif
}
/*--------------------------------------------------------------------------*/
