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

#include "plot.h"
#include "wresource.h"
#include "wcommon.h"
#include "../sun/Sun.h"
#include "../stack-c.h" 
/*-----------------------------------------------------------------------------------*/
extern int C2F (stimer) (void);
extern void C2F (settmpdir) (void);
extern void C2F (tmpdirc) (void);
extern void C2F (getenvc) (int *ierr, char *var, char *buf, int *buflen, int *iflag);
extern char *get_sci_data_strings (int n);
extern int C2F(sciquit)(void );
extern int C2F(scirun)(char * startup, int lstartup);
extern int C2F(inisci)(int *,int *,int *);
static void interrupt_setup ();
static void realmain(int nos,char *initial_script,int initial_script_type,int lpath,int memory);
static int sci_exit(int n) ;
/*-----------------------------------------------------------------------------------*/
static int  no_startup_flag=0;
jmp_buf env;
extern char input_line[];
/*-----------------------------------------------------------------------------------*/
/***********************************************
 * SIGINT is not used up to now 
 * CtrC are detected in readline or in jpc_Xloop 
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
void sci_windows_main (int nowin, int *nos, char *path,int pathtype,int *lpath,int memory)
{
#ifdef XPG3_LOCALE
  (void) setlocale (LC_CTYPE, "");
#endif
  setbuf (stderr, (char *) NULL);
  if (!setjmp (env))
    {
      /* first time */
      interrupt_setup ();
    }
  else
    {
      /* come back here from int_error() */
      if (nowin != 1)
	SetCursor (LoadCursor ((HINSTANCE) NULL, IDC_ARROW));
    }
  /* take commands from stdin */
	realmain (*nos, path,pathtype,*lpath,memory);
}
/*-----------------------------------------------------------------------------------*/
void sci_clear_and_exit(int n) /* used with handlers */ 
{
#ifdef _DEBUG
  char Message[256];
  switch (n)
  {
	  case SIGINT:
		  wsprintf(Message,"SIGINT Signal detected");
		  break;
	  case SIGILL:
		  wsprintf(Message,"SIGILL Signal detected");
		  break;
	  case SIGFPE:
		  wsprintf(Message,"SIGFPE Signal detected");
		  break;
	  case SIGSEGV:
		  wsprintf(Message,"SIGSEGV Signal detected");
		  break;
	  case SIGTERM:
		  wsprintf(Message,"SIGTERM Signal detected");
		  break;
	  case SIGBREAK:
		  wsprintf(Message,"SIGBREAK Signal detected");
		  break;
	  case SIGABRT:
		  wsprintf(Message,"SIGABRT Signal detected");
		  break;
	  default:
		  wsprintf(Message,"Unknow Signal detected");
		  break;
  }
  MessageBox(NULL,Message,"Error",MB_ICONWARNING);
#else
  MessageBox(NULL,"Scilab has performed a illegal operation\nand will be shutdown.\n Please save your work ...","Error",MB_ICONWARNING);
#endif
  WinExit();
  C2F(sciquit)();
}
/*-----------------------------------------------------------------------------------*/
static void realmain(int nos,char *initial_script,int initial_script_type,int lpath,int memory)
{
  int ierr;
  static int ini=-1;
  char startup[256];
  /* create temp directory */
  C2F(settmpdir)();

  /* signals */
  signal(SIGINT,sci_clear_and_exit);
  signal(SIGILL,sci_clear_and_exit);
  signal(SIGFPE,sci_clear_and_exit);
  signal(SIGSEGV,sci_clear_and_exit);
  signal(SIGTERM,sci_clear_and_exit);
  signal(SIGBREAK,sci_clear_and_exit);
  signal(SIGABRT,sci_clear_and_exit);

  /*  prepare startup script  */
  if ( nos != 1 ) 
    {
      /* execute a startup */
      no_startup_flag = 0;
      if ( initial_script != NULL ) 
	{
	  switch ( initial_script_type ) 
	    {
	    case 0 : 
	      sprintf(startup,"%s;exec('%s',-1)",get_sci_data_strings(1),
		      initial_script);
	      break;
	    case 1 : 
	      sprintf(startup,"%s;%s;",get_sci_data_strings(1),
		      initial_script);
	      break;
	    }
	}
      else 
	sprintf(startup,"%s;",get_sci_data_strings(1));
    }
  else 
    {
      /* No startup but maybe an initial script  */
      no_startup_flag = 1;
      if ( initial_script != NULL ) 
	  switch ( initial_script_type ) 
	    {
	    case 0 : 
	      sprintf(startup,"exec('%s',-1)",initial_script); break;
	    case 1 : 
	      sprintf(startup,"%s;",initial_script);   break;
	    }
      else 
	sprintf(startup," ");
    }

  /* initialize scilab interp  */
  
  C2F(inisci)(&ini, &memory, &ierr);
  if (ierr > 0) sci_exit(1) ;
  /* execute the initial script and enter scilab */ 
  
  C2F(scirun)(startup,strlen(startup));
  
  /* cleaning */ /* Allan CORNET 18/01/2004 */
  C2F(sciquit)(); 
  return ;
}
/*-----------------------------------------------------------------------------------*/
/*-------------------------------------------------------
 * Exit function called by some 
 * X11 functions 
 * call sciquit which call clear_exit
 *-------------------------------------------------------*/
int C2F(sciquit)()            /* used at Fortran level */
{
  int status = 0;
  /* fprintf(stderr,"I Quit Scilab through sciquit\n"); */
  if ( no_startup_flag == 0) 
    {
      char *quit_script =  get_sci_data_strings(5);
      C2F(scirun)(quit_script,strlen(quit_script));
    }
  return sci_exit(status) ;
} 
/*-----------------------------------------------------------------------------------*/
int sci_exit(int n) 
{
  /* fprintf(stderr,"I Quit Scilab through sci_exit\n");*/
  /** clean tmpfiles **/
  C2F(tmpdirc)();
  /* really exit */
  exit(n);
  return(0);
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
  Scierror(999,"SIGSTP: aborting current computation\r\n");
}
/*-----------------------------------------------------------------------------------*/
