/*-----------------------------------------------------------------------------------*/
/*
 * Copyright (C) 1986 - 1993   Thomas Williams, Colin Kelley
 *
 * Permission to use, copy, and distribute this software and its
 * documentation for any purpose with or without fee is hereby granted, 
 * provided that the above copyright notice appear in all copies and 
 * that both that copyright notice and this permission notice appear 
 * in supporting documentation.
 *
 * Permission to modify the software is granted, but not the right to
 * distribute the modified code.  Modifications are to be distributed 
 * as patches to released version.
 *  
 * This software is provided "as is" without express or implied warranty.
 *
 * AUTHORS
 *   Original Software:
 *     Thomas Williams,  Colin Kelley.
 *   Gnuplot 2.0 additions:
 *       Russell Lang, Dave Kotz, John Campbell.
 *   Gnuplot 3.0 additions:
 *       Gershon Elber and many others.
 *   
 * Modified for Scilab (1996) : Jean-Philippe Chancelier 
 * (2005) Allan CORNET
 *        
 */
/*-----------------------------------------------------------------------------------*/
#ifndef STRICT
  #define STRICT
#endif

#include <stdio.h>
#include <setjmp.h>
#include <signal.h>
#ifdef XPG3_LOCALE
  #include <locale.h>
#endif
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include "wresource.h"
#include "wcommon.h"
#include "stack-c.h" 
#include "Os_specific.h" /* isanan */ 
#include "Messages.h"
#include "Warnings.h"
#include "Errors.h"
#include "realmain.h"
#include "scilabmode.h"
/*-----------------------------------------------------------------------------------*/
extern int C2F(sciquit)(void );
static void interrupt_setup ();
/*-----------------------------------------------------------------------------------*/
jmp_buf env;
/*-----------------------------------------------------------------------------------*/
/***********************************************
 * SIGINT is not used up to now 
 * CtrC are detected in readline or in ScilabXloop 
 * TextMessage1  while we are in a scilab window 
 * the function SignalCtrC set a flag 
 * for scilab 
 ***********************************************/
void SignalCtrC (void)
{
  int j = 2;
/**  SIGINT; **/
  C2F (sigbas) (&j);

}
/*-----------------------------------------------------------------------------------*/
void inter (int an_int)
{
  (void) signal (SIGINT, inter);
  (void) signal (SIGFPE, SIG_DFL);	/* turn off FPE trapping */
  (void) fflush (stdout);
  sciprint ("\n");
  longjmp (env, TRUE);		/* return to prompt  */
}
/*-----------------------------------------------------------------------------------*/
/* Set up to catch interrupts */
static void interrupt_setup (void)
{
  (void) signal (SIGINT, inter);
}
/*-----------------------------------------------------------------------------------*/
void sci_windows_main ( int *nos, char *path, InitScriptType pathtype, int *lpath, int memory)
{
  setbuf (stderr, (char *) NULL);
  if (!setjmp (env))
    {
      /* first time */
      interrupt_setup ();
    }
  else
    {
      /* come back here from int_error() */
		if (getScilabMode() == SCILAB_STD ) SetCursor (LoadCursor ((HINSTANCE) NULL, IDC_ARROW));
    }
  /* take commands from stdin */
  realmain(*nos,path,pathtype,memory);

}
/*-----------------------------------------------------------------------------------*/
void sci_clear_and_exit(int n) /* used with handlers */ 
{
#ifdef _DEBUG
  char Message[256];
  switch (n)
  {
	  case SIGINT:
		  wsprintf(Message,MSG_ERROR59);
		  break;
	  case SIGILL:
		  wsprintf(Message,MSG_ERROR60);
		  break;
	  case SIGFPE:
		  wsprintf(Message,MSG_ERROR61);
		  break;
	  case SIGSEGV:
		  wsprintf(Message,MSG_ERROR62);
		  break;
	  case SIGTERM:
		  wsprintf(Message,MSG_ERROR63);
		  break;
	  case SIGBREAK:
		  wsprintf(Message,MSG_ERROR64);
		  break;
	  case SIGABRT:
		  wsprintf(Message,MSG_ERROR65);
		  break;
	  default:
		  wsprintf(Message,MSG_ERROR66);
		  break;
  }
  MessageBox(NULL,Message,MSG_ERROR20,MB_ICONWARNING);
#else
  /*MessageBox(NULL,MSG_WARNING30,MSG_WARNING22,MB_ICONWARNING);*/
#endif
  C2F(sciquit)();
}
/*-----------------------------------------------------------------------------------*/
/*-------------------------------------------------------
 * usr1 signal : used to transmit a Control C to 
 * scilab 
 *-------------------------------------------------------*/
void sci_usr1_signal(int n) 
{
  controlC_handler(n);
}
/*-----------------------------------------------------------------------------------*/
/*-------------------------------------------------------
 * Ctrl-Z : stops the current computation 
 *          or the current interface call 
 *-------------------------------------------------------*/
void  sci_sig_tstp(int n)
{
  Scierror(999,MSG_ERROR67);
}
/*-----------------------------------------------------------------------------------*/
