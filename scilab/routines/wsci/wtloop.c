
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
 *        
 */

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

#ifndef STRICT
#define STRICT
#endif
/*#include <windows.h>
#include "wgnuplib.h"*/
#include "wresource.h"
#include "wcommon.h"
#include "../sun/Sun.h"
#include "../stack-c.h" 

extern char input_line[];

jmp_buf env;

extern int C2F (stimer) (void);
extern void C2F (settmpdir) (void);
extern void C2F (tmpdirc) (void);
extern void C2F (getenvc) (int *ierr, char *var, char *buf, int *buflen, int *iflag);
extern char *get_sci_data_strings (int n);
static void interrupt_setup ();
static void realmain (int nos,char *initial_script,int pathtype,
		      int lpath,int memory);
static int sci_exit(int n) ;
extern int C2F(sciquit)(void );
extern int C2F(scirun)(char * startup, int lstartup);
extern int C2F(inisci)(int *,int *,int *);

#ifdef TEST
static int command ();
double C2F (dsort) ();
static void testloop (void);
#endif

/***********************************************
 * SIGINT is not used up to now 
 * CtrC are detected in readline or in jpc_Xloop 
 * TextMessage1  while we are in a scilab window 
 * the function SignalCtrC set a flag 
 * for scilab 
 ***********************************************/

void SignalCtrC (void)
{
#ifdef TEST
  sciprint ("signal CtrC detected\r\n");
#else
  int j = 2;
/**  SIGINT; **/
  C2F (sigbas) (&j);
#endif
}

void inter (int an_int)
{
  (void) signal (SIGINT, inter);
  (void) signal (SIGFPE, SIG_DFL);	/* turn off FPE trapping */
  (void) fflush (stdout);
  sciprint ("\n");
  longjmp (env, TRUE);		/* return to prompt  */
}

/* Set up to catch interrupts */

static void 
interrupt_setup (void)
{
  (void) signal (SIGINT, inter);
}

void sci_windows_main (int nowin, int *nos, char *path,int pathtype,
		  int *lpath,int memory)
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
#ifdef TEST
  testloop ();
#else
  realmain (*nos, path,pathtype,*lpath,memory);
#endif
}

static int  no_startup_flag=0;

static void realmain(int nos,char *initial_script,int initial_script_type,
		     int lpath,int memory)
{
  int ierr;
  static int ini=-1;
  char startup[256];
  /* create temp directory */
  C2F(settmpdir)();
  /* signals */
  /* 
  signal(SIGINT,sci_clear_and_exit);
  signal(SIGBUS,sci_clear_and_exit);
  signal(SIGSEGV,sci_clear_and_exit);
  signal(SIGQUIT,sci_clear_and_exit);
  signal(SIGHUP,sci_clear_and_exit);
  */
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

void sci_clear_and_exit(int n) /* used with handlers */ 
{
  /* fprintf(stderr,"I Quit Scilab through sci_clear_and_exit\n"); */
  C2F(sciquit)();
}

int sci_exit(int n) 
{
  /* fprintf(stderr,"I Quit Scilab through sci_exit\n");*/
  /** clean tmpfiles **/
  C2F(tmpdirc)();
  /* really exit */
  exit(n);
  return(0);
}

/*-------------------------------------------------------
 * usr1 signal : used to transmit a Control C to 
 * scilab 
 *-------------------------------------------------------*/

void sci_usr1_signal(int n) 
{
  controlC_handler(n);
}

/*-------------------------------------------------------
 * Ctrl-Z : stops the current computation 
 *          or the current interface call 
 *-------------------------------------------------------*/

void  sci_sig_tstp(int n)
{
  Scierror(999,"SIGSTP: aborting current computation\r\n");
}



#ifdef TEST

int 
main ()
{
  return (MAIN__ ());
}

/**********************************************
 * a loop function for testing a mini scilab 
 **********************************************/

void 
testloop (void)
{
  C2F (settmpdir) ();
  while (!com_line ())
    {
/** test with a timer of activated menus **/
      static int otimer = 0;
      int ntimer;
      ntimer = C2F (stimer) ();
      if (otimer != ntimer)
	{
	  ntimer = otimer;
	  if (C2F (ismenu) () == 1)
	    {
	      char buf[256];
	      int lb, nentry;
	      C2F (getmen) (buf, &lb, &nentry);
	      sciprint ("menu==> [%s,%d,%d]\r\n", buf, lb, nentry);
	    }

	}
    }
/** done in atexit C2F(tmpdirc)(); */
}


/**********************************************
 * com_line and command are only used when testing
 * with a mini Scilab 
 **********************************************/

#define is_system(c) ((c) == '!')

int 
com_line ()
{
  if (read_line ("SciLoop>",0))
    return (1);
  else if (is_system (input_line[0]))
    {
      do_system ();
      sciprint ("! done \n");
      return (0);
    }
  else if (command (input_line) == 1)
    return (1);
  else
    return (0);
}

#define Equals(str1,str2) ( strcmp(str1,str2)==0 )

static int 
command (char *com)
{
  if (com[0] == '\0')
    {
      return (0);
    }
  else if (Equals (com, "pipe"))
    {
      int i;
      i = InitSh ();
      if (i != 0)
	for (i = 0; i < 10; i++)
	  {
	    SendToSh ("pwd");
	    ReadAnswer ();
	  }
    }
  else if (Equals (com, "env"))
    {
      char buf[256];
      int iflag = 0, ierr = 0, bufl = 256;
      C2F (getenvc) (&ierr, "TMPDIR", buf, &bufl, &iflag);
      if (ierr == 0)
	sciprint ("tmpdir %s\r\n", buf);
      else
	sciprint ("tmpdir not found\r\n");
      bufl = 256;
      C2F (getenvc) (&ierr, "SCI", buf, &bufl, &iflag);
      if (ierr == 0)
	sciprint ("sci %s\r\n", buf);
      else
	sciprint ("sci not found\r\n");
      bufl = 256;
      C2F (getenvc) (&ierr, "HOME", buf, &bufl, &iflag);
      if (ierr == 0)
	sciprint ("home %s\r\n", buf);
      else
	sciprint ("home not found\r\n");
      bufl = 256;
      C2F (getenvc) (&ierr, "PWD", buf, &bufl, &iflag);
      if (ierr == 0)
	sciprint ("pwd %s\r\n", buf);
      else
	sciprint ("pwd not found\r\n");
      bufl = 256;
      C2F (getenvc) (&ierr, "PVM_ROOT", buf, &bufl, &iflag);
      if (ierr == 0)
	sciprint ("PVM_ROOT %s\r\n", buf);
      else
	sciprint ("PVM_ROOT not found\r\n");
      bufl = 256;
      C2F (getenvc) (&ierr, "PVM_TMP", buf, &bufl, &iflag);
      if (ierr == 0)
	sciprint ("PVM_TMP %s\r\n", buf);
      else
	sciprint ("PVM_TMP not found\r\n");
    }
  else if (Equals (com, "stimer"))
    {
      sciprint ("stimer %d\r\n", (int) C2F (stimer) ());
    }
  else if (Equals (com, "timer"))
    {
      double s;
      C2F (timer) (&s);
      sciprint ("time %f\n", s);
    }
  else if (Equals (com, "loop"))
    {
      while (1)
	{
	  C2F (sxevents) ();
	};
    }
  /*else if (strncmp (com, "help", 4) == 0)
    {
      sciprint ("help [%s]\r\n", com + 5);
      Sci_Help (com + 5);
    }
  else if (strncmp (com, "apropos", 7) == 0)
    {
      sciprint ("apropos [%s]\r\n", com + 8);
      Sci_Apropos (com + 8);
      }*/
  else if (strncmp (com, "sci", 3) == 0)
    {
      sciprint ("test %s\r\n", com + 4);
      SciMenuSelect (com + 4);
    }
  else if (Equals (com, "wdim"))
    {
      int x = 200, y = 100, v3, v4;
      C2F (setwindowdim) (&x, &y, &v3, &v4);
    }
  else if (Equals (com, "xdel"))
    {
      int x = 0;
      C2F (deletewin) (&x);
    }
  else if (Equals (com, "delmen"))
    {
      int i = -1;
      C2F (delbtn) (&i, "resume");
      i = 0;
      C2F (delbtn) (&i, "Poo1");
      C2F (delbtn) (&i, "Poo");
      C2F (delbtn) (&i, "File");
    }
  else if (Equals (com, "addmen"))
    {
      int win_num = -1, ne = 0, typ = 0, ierr = 0;
      char **menu_entries = (char **) 0;
      static char *pszTitle[] =
      {
	"first  entry",
	"second  entry",
	"third  entry",
	"fourth entry",
	NULL
      };
      AddMenu (&win_num, "Poo", menu_entries, &ne, &typ, "Poo_func", &ierr);
      ne = 4;
      AddMenu (&win_num, "Poo1", pszTitle, &ne, &typ, "Poo_func1", &ierr);
      win_num = 0;
      ne = 0;
      AddMenu (&win_num, "Poo", menu_entries, &ne, &typ, "Poo_func", &ierr);
      ne = 4;
      AddMenu (&win_num, "Poo1", pszTitle, &ne, &typ, "Poo_func1", &ierr);
      win_num = -1;
      C2F (chmenu) (&win_num, "resume", "new_resume");
    }
  else if (Equals (com, "mouse"))
    {
      test_mouse (0);
    }
  else if (Equals (com, "mouse1"))
    {
      test_mouse (1);
    }
  else if (Equals (com, "choose"))
    {
      int rep;
      rep = TestChoose ();
      sciprint ("Rep :%d\n", rep);
    }

  else if (Equals (com, "xgetf"))
    {
      int rep;
      rep = TestGetFile ();
      sciprint ("Rep :%d\n", rep);
    }

  else if (Equals (com, "mdial"))
    {
      int rep;
      rep = TestmDialogWindow ();
      sciprint ("Rep :%d\n", rep);
    }

  else if (Equals (com, "mch"))
    {
      int rep;
      rep = TestChoice ();
      sciprint ("Rep :%d\n", rep);
    }

  else if (Equals (com, "dialog"))
    {
      TestDialog ();
    }
  else if (Equals (com, "message1"))
    {
      TestMessage (1);
    }
  else if (Equals (com, "message2"))
    {
      TestMessage (2);
    }
  else if (Equals (com, "zoom"))
    {
      scig_2dzoom (0);
    }
  else if (Equals (com, "screendump"))
    {
      /** 
      GraphPrint(&graphwin);
      **/
    }
  else if (Equals (com, "pause"))
    {
      int stime = 0;
      char buf[MAX_LINE_LEN + 1];
      stime = (int) -5;		/* real(const_express(&a)); */
      if (stime < 0)
	{
	  strcpy (buf, " Pause test  ");
	  if (!Pause (buf))
	    longjmp (env, TRUE);	/* bail out to command line */
	}
      if (stime > 0)
	{
/** sleep((unsigned int) stime); **/
	}
    }
  else if (Equals (com, "shell"))
    {
      do_shell ();
    }
  else if (Equals (com, "exit") || Equals (com, "quit"))
    {
      return (1);
    }
  else if (Equals (com, "deltemp"))
    {
      SciRemoveDirectory ("C:/temp/foo");
    }
  else
    {
      if (Equals (com, "help") || Equals (com, "?"))
	sciprint ("test commands:\n");
      else
	sciprint ("[%s] is not a valid command, use :\n", com);
      sciprint ("possible test \n");
      sciprint ("sci(for graphics),mouse,mouse1,choose,xgetf,mdial,mch,menuhelp,dialog\n");
      sciprint ("message1,message2,zoom,pause,screendump,shell,exit,addmen\n");
      sciprint ("delmen,wdim(sci first),time,timer,stimer,env,! xxx,!\n");
      sciprint ("help xxx,apropos xxx,deltemp\n");
    }
  return (0);
}

/**********************************
 * just for test 
 **********************************/

int C2F (sciwin) ()
{
  return (0);
};

double C2F (dsort) ()
{
  return (0.0);
};

static char *dataStrings[] =
{
  "$MANCHAPTERS",
  "exec('SCI/scilab.star',-1);",
  "exec('SCI/demos/alldems.dem');",
  "home/scilab.hist",
  "home/scilab.save",
  "exec('SCI/scilab.quit',-1);quit;"
};

void
  C2F (inffic) (iopt, name, nc)
     integer *iopt, *nc;
     char *name;
{
  *nc = strlen (dataStrings[*iopt - 1]);
  strcpy (name, dataStrings[*iopt - 1]);
}

void C2F (setfbutn) (char *buf, int *rep)
{
  sciprint ("setfbutn : [%s]\r\n", buf);
  *rep = 1;
}

void C2F (fbutn) (char *buf, int *win, int *ent)
{
  sciprint ("fbutn win=%d entry=%d\r\n", *win, *ent);
}

/*************************************
* Scilab Graphic tests 
**************************************/

typedef struct
{
  char *name;
  void (*fonc) ();
}
TestOpTab;

extern TestOpTab GraphicTestTab[];

void 
SciMenuSelect (char *Name)
{
  static firstentry = 0;
  integer i;
  if (firstentry == 0)
    {
      integer verbose = 0, ww, narg;
      C2F (SetDriver) ("Rec", PI0, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0);
      C2F (dr) ("xget", "window", &verbose, &ww, &narg, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 0L, 0L);
      C2F (dr) ("xset", "window", &ww, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 0L, 0L);
      firstentry++;
    }
  else if (strcmp (Name, "testXor") != 0)
    {
      integer num = 0;
      C2F (dr) ("xset", "window", &num, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 0L, 0L);
      C2F (dr) ("xclear", "v", PI0, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 0L, 0L);
      C2F (dr) ("xstart", "v", &num, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 0L, 0L);
    }
  i = 0;
  while (GraphicTestTab[i].name != (char *) NULL)
    {
      if (strcmp (Name, GraphicTestTab[i].name) == 0)
	{
	  (*(GraphicTestTab[i].fonc)) ();
	  return;
	}
      i++;
    }
  sciprint ("%s is an invalid test. Valid tests are\r\n", Name);
  i = 0;
  while (GraphicTestTab[i].name != (char *) NULL)
    {
      sciprint ("sci %s\r\n", GraphicTestTab[i].name);
      i++;
    }
}

void 
test_sci ()
{
  SciMenuSelect ("test2D3");
}

void 
test_mouse (int iflag1)
{
  char str[128];
  integer ibutton;
  double x0, yy0;
  int iflag = 0, istr = 1;
  /* C2F(dr1)("xclickany","one",&ibutton,&iwin,&iflag,PI0,PI0,PI0,&x0,&yy0,PD0,PD0,0L,0L); */

  C2F (dr1) ("xclick", str, &ibutton, &iflag, &istr, PI0, PI0, PI0, &x0, &yy0, PD0, PD0, 0L, 0L);
  sciprint ("Mouse :but=%d x=%f,y=%f\r\n", ibutton, x0, yy0);
}

#endif
