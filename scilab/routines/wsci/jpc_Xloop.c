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
/*#include <windows.h>
  #include "wgnuplib.h"*/
#include "wresource.h"
#include "wcommon.h"
#include "../machine.h"
#ifdef WITH_TK
#include "tcl.h"
extern void inittk ();
extern void flushTKEvents ();
extern int  tcl_check_one_event(void);
static int BasicScilab = 1;
#endif
/*-----------------------------------------------------------------------------------*/
/** do I want a scilab or an xscilab (here it means Windows ) */

int INXscilab = 0; /**  XXXX just to use zzledt1 **/

/*-----------------------------------------------------------------------------------*/
void SetNotBasic()
{
  BasicScilab=0;
}
/*-----------------------------------------------------------------------------------*/
int GetBasic() { return  BasicScilab;} 
/*-----------------------------------------------------------------------------------*/

void start_sci_gtk() 
{
#ifdef WITH_TK
  inittk();
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
  inittk ();
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
  *i= Max(INXscilab,1-BasicScilab);
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
  start_sci_gtk();
  inittk();
  BasicScilab = 0;

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
/** function used in wtext.c in function TextGetCh  must wait for an event **/
/*-----------------------------------------------------------------------------------*/
void TextMessage2()
{
#ifdef WITH_TK
  flushTKEvents ();
#else 
  MSG msg;
#endif
#ifdef WITH_TK
  if (  tcl_check_one_event() == 1) 
    {
      /*sciprint("tcl event %l\r\n",msg.hwnd);*/
    }
#else 
  GetMessage(&msg, 0, 0, 0);
  TranslateMessage(&msg);
  DispatchMessage(&msg);
  while (  PeekMessage(&msg, 0, 0, 0,PM_REMOVE) ) 
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
#endif
}
/*-----------------------------------------------------------------------------------*/
int C2F (sxevents) ()
{
  if (INXscilab == 1 || BasicScilab == 0 )
    {
      TextMessage1 (1);
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

void C2F (winsci) (char *pname, int *nos, int *idisp, char *display,
		   long int dummy1, long int dummy2)
{
  char *argv[10];
  int argc = 1;
  strip_blank (pname);
  argv[0] = pname;
  if (*idisp == 1)
    {
      argv[argc++] = "-display";
      strip_blank (display);
      argv[argc++] = display;
    }
  if (*nos == 1)
    argv[argc++] = "-ns";
  argv[argc++] = "-name";
  argv[argc++] = "Scilab";
  /* XXXXXX main_sci(argc,argv); */
}

void
sigblock ()
{
}

