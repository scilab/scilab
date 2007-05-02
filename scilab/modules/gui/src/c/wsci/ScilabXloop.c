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
#include "machine.h"

#include "Messages.h"
#include "Warnings.h"
#include "Errors.h"
#include "xscion.h"
#include "scilabmode.h"

extern void flushTKEvents ();
extern int TclEventsLoop(void);
static int BasicScilab = 0;
/*-----------------------------------------------------------------------------------*/
/* used to know if we must check events 
 * inside the scilab interpreter (parse/*)
 */
int C2F(checkevts)(int *i)
{
  *i= Max(getINXscilab(),1);
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

  flushTKEvents ();

  while (PeekMessage (&msg, 0, 0, 0, PM_NOREMOVE))
    {
      /* check for a tcl/tk event */
     if ( TclEventsLoop() ) continue ;

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
  if ( getScilabMode() != SCILAB_NWNI )
  {
	  if (getINXscilab() == 1 || BasicScilab == 0 )
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
