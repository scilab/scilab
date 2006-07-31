/* Copyright (C) 1998 Chancelier Jean-Philippe */
/*
 * jpc_Xloop.c : 
 * (1997) : Jean-Philippe Chancelier 
 * 
 */
/*-----------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#ifndef STRICT
#define STRICT
#endif
#include "wresource.h"
#include "wcommon.h"
#include "../machine.h"

#include "Messages.h"
#include "Warnings.h"
#include "Errors.h"


#ifdef WITH_TK
extern void flushTKEvents ();
int TclEventsLoop(void);
static int BasicScilab = 0;
#endif

extern int GetWITH_GUI(void);

/*-----------------------------------------------------------------------------------*/
/** do I want a scilab or an xscilab (here it means Windows ) */

int INXscilab = 0; /**  XXXX just to use zzledt1 **/

/*-----------------------------------------------------------------------------------*/
void SetXsciOn ()
{
switch_rlgets (1);
INXscilab = 1;
}
/*-----------------------------------------------------------------------------------*/
int C2F (xscion) (int *i)
{
  *i = INXscilab;
  return (0);
}
/*-----------------------------------------------------------------------------------*/
/* used to know if we must check events 
 * inside the scilab interpreter (parse/*)
 */

int C2F(checkevts)(int *i)
{
  /*  *i= INXscilab; */
  #if WITH_TK
  *i= Max(INXscilab,1);
  #else
  *i= INXscilab;
  #endif
  return(0);
}
/*-----------------------------------------------------------------------------------*/
/*************************************************
 * Dealing with Events on the queue when computing in Scilab 
 * we also try to detect when CTRLC was  activated 
 *************************************************/
extern TW textwin;

void TextMessage1 (int ctrlflag)
{
  MSG msg;
#ifdef WITH_TK
  flushTKEvents ();
#endif
  while (PeekMessage (&msg, 0, 0, 0, PM_NOREMOVE))
    {
#ifdef WITH_TK
      /* check for a tcl/tk event */
     if ( TclEventsLoop() ) continue ;
#endif
      PeekMessage (&msg, 0, 0, 0, PM_REMOVE);
      TranslateMessage (&msg);
      DispatchMessage (&msg);
    }
  if (ctrlflag == 1) 
    {
      CtrlCHit (&textwin);
    }
}
/*-----------------------------------------------------------------------------------*/
int C2F (sxevents) ()
{
  if ( GetWITH_GUI() )
  {
#ifdef WITH_TK
	  if (INXscilab == 1 || BasicScilab == 0 )
#else
	  if (INXscilab == 1 )
#endif
	  {
		  TextMessage1 (1);
	  }
  }
  return (0);
}
/*-----------------------------------------------------------------------------------*/
/**********************************************************************
 * For Fortran call 
 **********************************************************************/

static void strip_blank (source)
     char *source;
{
  char *p;
  p = source;
  /* look for end of string */
  while (*p != '\0')
    p++;
  while (p != source)
    {
      p--;
      if (*p != ' ')
	break;
      *p = '\0';
    }
}

/**********************************************************************/
