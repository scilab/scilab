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
#include "tcl.h"
extern void initTCLTK ();
extern void flushTKEvents ();
extern int  tcl_check_one_event(void);
static int BasicScilab = 1;
#endif

extern int GetWITH_GUI(void);

/*-----------------------------------------------------------------------------------*/
/** do I want a scilab or an xscilab (here it means Windows ) */

int INXscilab = 0; /**  XXXX just to use zzledt1 **/

/*-----------------------------------------------------------------------------------*/
void SetNotBasic()
{
	#ifdef WITH_TK
  BasicScilab=0;
  #endif
}
/*-----------------------------------------------------------------------------------*/
int GetBasic() 
{
#ifdef WITH_TK
	 return  BasicScilab;
#else
	 return  0;
#endif
} 
/*-----------------------------------------------------------------------------------*/

void start_sci_gtk() 
{
#ifdef WITH_TK
  initTCLTK();
  BasicScilab = 0;
  flushTKEvents();
#endif
}

/*-----------------------------------------------------------------------------------*/
void SetXsciOn ()
{
switch_rlgets (1);
INXscilab = 1;
#ifdef WITH_TK
  initTCLTK ();
#endif
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
 * A faire sous windows 
 */

int C2F(checkevts)(int *i)
{
  /*  *i= INXscilab; */
  #if WITH_TK
  *i= Max(INXscilab,1-BasicScilab);
  #else
  *i= INXscilab;
  #endif
  return(0);
}
/*-----------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
 * used to start tk at run time when scilab 
 * was started with scilab -nw 
 * open a display with DisplayInit and initialize 
 * Tk. If TK_Started is set to one then the initialization 
 * was correct.
 *---------------------------------------------------------------------------*/

void sci_tk_activate(void)
{
	#ifdef WITH_TK
  start_sci_gtk();
  initTCLTK();
  BasicScilab = 0;
  #endif
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
      if (Tcl_DoOneEvent (TCL_ALL_EVENTS | TCL_DONT_WAIT) == 1) continue ;
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
