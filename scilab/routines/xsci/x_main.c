/*---------------------------------------------------------- 
 * Real main function for Scilab on X11 platform 
 *----------------------------------------------------------*/
/* main.c */

#include <pwd.h>
#include <ctype.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "x_ptyxP.h"
#include "x_data.h"
#include "x_error.h"
#include "x_menu.h"
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xaw3d/Form.h>
#include <X11/Xos.h>
#include <X11/cursorfont.h>
#include <X11/Xaw3d/SimpleMenu.h>

#include "../version.h"
#include "../machine.h"
#include "../graphics/Math.h"

#include "../graphics/DestroyObjects.h"

extern void  controlC_handler(int n);

#include "All-extern-x.h"
#include "All-extern.h"

#ifdef USE_TERMIOS
#include <termios.h>
/* this hacked termios support only works on SYSV */
#define USE_SYSV_TERMIO
#define termio termios
#undef TCGETA
#define TCGETA TCGETS
#undef TCSETA
#define TCSETA TCSETS
#else /* USE_TERMIOS */
#ifdef SYSV
#include <sys/termio.h>
#endif /* SYSV */
#endif /* USE_TERMIOS else */

#ifndef SYSV				/* BSD systems */
/* #include <sgtty.h> */
#include <sys/resource.h>
#endif	/* !SYSV */

#include <stdio.h>
#include <errno.h>

#include <sys/param.h>	/* for NOFILE */

#ifdef  PUCC_PTYD
#include <local/openpty.h>
int	Ptyfd;
#endif /* PUCC_PTYD */

#ifndef X_NOT_POSIX
#include <unistd.h>
#else
extern long lseek();
#ifdef USG
extern unsigned sleep();
#else
extern void sleep();
#endif
#endif

#ifndef X_NOT_STDC_ENV
#include <stdlib.h>
#else
extern char *malloc();
extern char *calloc();
extern char *realloc();
extern char *getenv();
extern void exit();
#endif

char *ProgramName;

#ifdef WITH_TK
/*  extern void initTCLTK(void);
  extern void flushTKEvents(void); */
#endif


extern void sci_clear_and_exit (int);
extern int C2F(nofpex) (void);
extern int C2F(scigetarg) (int *,char *,long int l);
extern int C2F(sciiargc) (void);
extern void C2F(settmpdir) (void);
extern char *get_sci_data_strings (int n);
extern void C2F(tmpdirc)();
extern char *strindex ();
extern void do_hangup();
extern void do_kill();
extern void sci_usr1_signal(int n) ;

static void Syntax  __PARAMS((char *badOption));  
static void strip_blank  __PARAMS((char *source));  
static void Syntax  (char *badOption);  
static char ** create_argv(int *argc);
static void strip_blank(char *source);

extern sciPointObj *pfiguremdl; /* F.Leray 18.11.04 : used to be destroyed with sciquit */
extern sciPointObj *paxesmdl;   /* F.Leray 18.11.04 : used to be destroyed with sciquit */

/*---------------------------------------------------------- 
 * mainsci.f directly call this function 
 * thus this is the real main for scilab 
 * Copyright Inria/Enpc 
 *----------------------------------------------------------*/

#define MIN_STACKSIZE 180000

static int  no_startup_flag=0;
static int  memory = MIN_STACKSIZE;
static int  no_window = 0;
static char * initial_script = NULL;
static int  initial_script_type = 0; /* 0 means filename 1 means code */
extern void settexmacs();

int  sci_show_banner=1;

void C2F(realmain)()
{
  int ierr, argc,i;
  static int ini=-1;
  char startup[256];
  char **argv, *display = NULL;

  /* floating point exceptions */
  C2F(nofpex)(); 
  /* create argv */
  if (( argv = create_argv(&argc))== NULL) 
    exit(1);
  ProgramName = argv[0];
  /* scanning options */
  for ( i=0 ; i < argc ; i++) 
    {
      if ( strcmp(argv[i],"-nw") == 0) 
	{ no_window = 1; } 
      if ( strcmp(argv[i],"-nwni") == 0) 
	{ no_window = 1; } 
      else if ( strcmp(argv[i],"-display") == 0) 
	{ 
	  char dpy[128];
	  sprintf(dpy,"DISPLAY=%s",display);
	  putenv(dpy);
	} 
      else if ( strcmp(argv[i],"-nb") == 0) { sci_show_banner = 0; }
      else if ( strcmp(argv[i],"-ns") == 0)  { no_startup_flag = 1;}
      else if ( strcmp(argv[i],"-mem") == 0) { memory = Max(atoi(argv[++i]),MIN_STACKSIZE );} 
      else if ( strcmp(argv[i],"-f") == 0) { initial_script = argv[++i];} 
      else if ( strcmp(argv[i],"-e") == 0) 
	{
	  initial_script = argv[++i];
	  initial_script_type = 1;
	} 
      else if ( strcmp(argv[i],"--texmacs") == 0)  { no_window = 1;settexmacs();}

    }
  /* create temp directory */
  C2F(settmpdir)();
  /* signals */
  signal(SIGINT,sci_clear_and_exit);
  signal(SIGBUS,sci_clear_and_exit);
  signal(SIGSEGV,sci_clear_and_exit);
  signal(SIGQUIT,sci_clear_and_exit);
  signal(SIGHUP,sci_clear_and_exit);
  signal(SIGUSR1,sci_usr1_signal);
  /*  prepare startup script  */

  if ( no_startup_flag == 0) 
    {
      /* execute a startup */
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
    
 #ifdef WITH_TK
   /*initTCLTK();
   flushTKEvents();*/
 #endif
  

  if ( no_window == 0 ) 
    {
      /* we are in window mode */
      SetXsciOn();
    }

  if ( no_window == 0 ) 
    {
      /* enters window mode first then 
       * call inisci and scirun 
       */
      main_sci(argc,argv,startup,strlen(startup),memory);
    }
  else 
    {
      /* initialize scilab interp  */
      C2F(inisci)(&ini, &memory, &ierr);
      if (ierr > 0) sci_exit(1) ;
      /* execute the initial script and enter scilab */ 
      C2F(scirun)(startup,strlen(startup));
    }
  /* cleaning */
  C2F(sciquit)();
  return ;
}


Boolean   sunFunctionKeys = False;

static struct _resource {
    char *xterm_name;
    char *icon_geometry;
    char *title;
    char *icon_name;
    char *term_name;
    char *tty_modes;
    Boolean noWindow;
    Boolean noStartup;
    Boolean useInsertMode;
    String visualType;
} resource;

/* used by VT (charproc.c) */

#define offset(field)	XtOffsetOf(struct _resource, field)

XtResource application_resources[] = {
    {"noWindow", "NoWindow", XtRBoolean, sizeof(Boolean),
	offset(noWindow), XtRString, "false"},
    {"noStartup", "NoStartup",XtRBoolean, sizeof(Boolean),
	offset(noStartup), XtRString, "false"},
    {"termName", "TermName", XtRString, sizeof(char *),
	offset(term_name), XtRString, (caddr_t) NULL},
    {"name", "Name", XtRString, sizeof(char *),
	offset(xterm_name), XtRString, "xterm"},
    {"iconGeometry", "IconGeometry", XtRString, sizeof(char *),
	offset(icon_geometry), XtRString, (caddr_t) NULL},
    {XtNtitle, XtCTitle, XtRString, sizeof(char *),
	offset(title), XtRString, VERSION},
    {XtNiconName, XtCIconName, XtRString, sizeof(char *),
	offset(icon_name), XtRString, VERSION},
    {"ttyModes", "TtyModes", XtRString, sizeof(char *),
	offset(tty_modes), XtRString, (caddr_t) NULL},
    {"useInsertMode", "UseInsertMode", XtRBoolean, sizeof (Boolean),
        offset(useInsertMode), XtRString, "false"},
    {"visualType", "VisualType", XtRString, sizeof(String),
	offset(visualType), XtRImmediate, (XtPointer)"" }
};

struct _color {
  char *c1;
}; /**  colors; **/

XtResource app_colors[] = {
  {"xscilab.color*Command.background", "Xscilab*Command.Background", 
     XtRString, sizeof(String),
     XtOffsetOf(struct _color, c1), NULL,NULL}
};

static char *fallback_resources[] = {
#include "Xscilab.ad.h"
    NULL
};

/* Command line options table.  Only resources are entered here...there is a
   pass over the remaining options after XrmParseCommand is let loose. */

static XrmOptionDescRec optionDescList[] = {
{"-geometry",	"*vtsci.geometry",XrmoptionSepArg,	(caddr_t) NULL},
{"-tm",		"*ttyModes",  XrmoptionSepArg,	(caddr_t) NULL},
{"-tn",		"*termName",  XrmoptionSepArg,	(caddr_t) NULL},
{"-nw",		"*noWindow",  XrmoptionNoArg,	(caddr_t) "on"},
{"-nwni",		"*noWindow",  XrmoptionNoArg,	(caddr_t) "on"},
{"-ns",		"*noStartup", XrmoptionNoArg,	(caddr_t) "on"},
};

static struct _options {
  char *opt;
  char *desc;
} options[] = {
  { "-help",                 "print out this message" },
  { "-ns",                   "no startup mode " },
  { "-nw",                   "no window mode " },
  { "-nwni",                 "no window mode also" },
  { "-display displayname",  "X server to contact" },
  { "-name string",          "client instance, icon, and title strings" },
  { "-xrm resourcestring",   "additional resource specifications" },
  { "-tm string",            "terminal mode keywords and characters" },
  { NULL, NULL }
};

extern WidgetClass xtermWidgetClass;

Arg ourTopLevelShellArgs[] = {
  { XtNallowShellResize, (XtArgVal) TRUE },	
  { XtNinput, (XtArgVal) TRUE },
};
int number_ourTopLevelShellArgs = 2;
	
XtAppContext app_con;
Widget toplevel = (Widget) NULL;
Widget realToplevel = (Widget) NULL;

/*
 * DeleteWindow(): Action proc to implement ICCCM delete_window.
 */

void
DeleteWindow(w, event, params, num_params)
    Widget w;
    XEvent *event;
    String *params;
    Cardinal *num_params;
{
 sci_clear_and_exit(0); 
}


void
KeyboardMapping(w, event, params, num_params)
    Widget w;
    XEvent *event;
    String *params;
    Cardinal *num_params;
{
    switch (event->type) {
       case MappingNotify:
	  XRefreshKeyboardMapping(&event->xmapping);
	  break;
    }
}

/** extern void  SGDeleteWindow(); **/

static XtActionsRec actionProcs[] = {
  {"DeleteWindow", DeleteWindow},
  {"SGDeleteWindow", SGDeleteWindow},
  {"KeyboardMapping", KeyboardMapping}
};


/* used by scilab to know the dpy and toplevel */

int Xscilab(dpy,topwid)
     Display **dpy;
     Widget *topwid;
{
  *topwid=toplevel;
  if ( toplevel != (Widget) NULL) *dpy=XtDisplay(toplevel);
  return(1);
}

Atom wm_delete_window;

/*
 * initColors: To allow resources to be specified separately for color
 *	and mono displays, we add a dummy Form widget below realToplevel
 *	in appropriate circumstances.
 */

#define ADDTOPLEVEL(NAME) \
	toplevel_w = XtCreateManagedWidget(NAME,formWidgetClass, \
					 realToplevel_w,(ArgList)0 ,(Cardinal)0);

static int screencolor = 1 ; /* default screen color status */

/* return the current screencolor */

void getcolordef(screenc)
     integer *screenc;
{
  *screenc= screencolor;
}

void setcolordef(screenc)
     int screenc;
{
  screencolor = screenc;
}


Widget initColors(realToplevel_w)
     Widget realToplevel_w;
{
  int i;
  Widget toplevel_w ;
  Screen *Xscreen;
  Visual *visual;
  /* The default is no extra widget (ie. mono) */
  toplevel_w = realToplevel_w;
  Xscreen = XtScreen(realToplevel_w);
  C2F(xscion)(&i);/* is it a scilab -nw */
  if ( i != 0 ) 
    {
      /* If we are using a scilab -nw 
       *  resource.visualType == NULL
       *
       * See if the user has specified a type */
      if (strcmp(resource.visualType,"mono") == 0) {
	/* Ton van Overbeek patch 22/08/00 */
	return toplevel_w;
      } else if (strcmp(resource.visualType,"color") == 0) {
	ADDTOPLEVEL("color");
	/* Ton van Overbeek patch 22/08/00 */
	return toplevel_w;
      } else if (strcmp(resource.visualType,"gray") == 0) {
	ADDTOPLEVEL("gray");
	/* Ton van Overbeek patch 22/08/00 */
	return toplevel_w;
      }
    }
  /* Otherwise we try to figure it out */
  if ((visual=XDefaultVisualOfScreen(Xscreen)) == NULL) {
    fprintf(stderr,"Scilab: can't get info about visual!\n");
    return toplevel_w;
  }
  if (visual->map_entries > 2) {
    switch (visual->class) {
    case StaticColor:
    case PseudoColor:
    case TrueColor:
    case DirectColor:
      ADDTOPLEVEL("color");
      screencolor = 1;
      break;
    case StaticGray:
    case GrayScale:
      ADDTOPLEVEL("gray");
      screencolor = 0;
      break;
    default:
      screencolor = 0;
      toplevel_w = realToplevel_w;
    }
  } else {
      screencolor = 0;
      toplevel_w = realToplevel_w;
  }
  /* Ton van Overbeek patch 22/08/00 */
  return toplevel_w;
}

void main_sci (int argc,char ** argv, char *startup, int lstartup,int memory)
{
  XtermWidget CreateSubWindows();
  register TScreen *screen;
  register int  pty;
  int Xsocket;
  /* Init the Toolkit. */
  realToplevel = toplevel = 
    XtAppInitialize(&app_con, "Xscilab", 
		    optionDescList, XtNumber(optionDescList), 
		    &argc, argv, fallback_resources, 
		    (ArgList) 0,(Cardinal) 0);				     
  XtGetApplicationResources(toplevel, (XtPointer) &resource,
			    application_resources,
			    XtNumber(application_resources), 
			    (ArgList) 0,(Cardinal) 0);
  /** 
  XtGetApplicationResources(toplevel, (XtPointer) &colors, 
			    app_colors,
			    XtNumber(app_colors), 
			    (ArgList) 0,(Cardinal) 0);
  printf("Color: %s\n",colors.c1);
  **/
  XtAppAddActions(app_con, actionProcs, XtNumber(actionProcs));
  toplevel =   initColors(realToplevel);
   /* SetXsciOn();  */ /*Already done in realmain */
  xterm_name = resource.xterm_name;
  if (strcmp(xterm_name, "-") == 0) xterm_name = "xterm";
  XtSetValues (toplevel, ourTopLevelShellArgs,
	       (Cardinal)  number_ourTopLevelShellArgs);
  /* Parse the rest of the command line 
   * the arguments parsed by XtAppInitialize must be the last ones 
   * and here we can check that arguments belong to options 
   * this is to be updated 
   */
  /* 
  for (argc--, argv++ ; argc > 0 ; argc--, argv++) 
    {
      if(**argv != '-') Syntax (*argv);
      switch(argv[0][1]) {
      case 'h':
	Help ();      break;
	/ * NOTREACHED * /
      default:
	Syntax (*argv);
      }
    }
  */
  XawSimpleMenuAddGlobalActions (app_con);
  XtRegisterGrabAction (HandlePopupMenu, True,
			(ButtonPressMask|ButtonReleaseMask),
			GrabModeAsync, GrabModeAsync);
  term = CreateSubWindows(toplevel);
  screen = &term->screen;
  if (screen->savelines < 0) screen->savelines = 0;
  term->flags = 0;
  if (!screen->jumpscroll) {
    term->flags |= SMOOTHSCROLL;
    update_jumpscroll();
  }
  if (term->misc.reverseWrap) {
    term->flags |= REVERSEWRAP;
    update_reversewrap();
  }
  if (term->misc.autoWrap) {
    term->flags |= WRAPAROUND;
    update_autowrap();
  }
  if (term->misc.re_verse) {
    term->flags |= REVERSE_VIDEO;
    update_reversevideo();
  }
  
  term->initflags = term->flags;
  
  if (term->misc.appcursorDefault) {
    term->keyboard.flags |= CURSOR_APL;
    update_appcursor();
  }
  
  if (term->misc.appkeypadDefault) {
    term->keyboard.flags |= KYPD_APL;
    update_appkeypad();
  }
  
  /* open a terminal for client */
  screen->arrow = make_colored_cursor (XC_left_ptr, 
				       screen->mousecolor,
				       screen->mousecolorback);
  /* avoid double MapWindow requests */
  XtSetMappedWhenManaged( XtParent(term), False );
  wm_delete_window = XInternAtom(XtDisplay(realToplevel),"WM_DELETE_WINDOW",False);
  VTInit1(realToplevel);
  Xsocket = ConnectionNumber(screen->display);
  pty = screen->respond;
  pty_mask = 1 << pty;
  X_mask = 1 << Xsocket;
  Select_mask = pty_mask | X_mask;
  max_plus1 = (pty < Xsocket) ? (1 + Xsocket) : (1 + pty); 
  XSetErrorHandler((XErrorHandler)xerror);
  XSetIOErrorHandler((XIOErrorHandler)xioerror);
  /* initialize xterm and run scilab  */
  VTRun(startup,lstartup,memory);

}



#ifdef sun 
#ifndef SYSV
#include <sys/ieeefp.h>
clear_ieee_warnings()
{
  char *mode, **out, *in;
  ieee_flags("clearall","exeption","all", &out);
}
#endif 
#endif 

/* following include needed for solaris */
#if defined(solaris) && defined(__STDC__) 
#include <stropts.h>
#include <poll.h>
#endif

int GetBytesAvailable (fd)
    int fd;
{
#ifdef FIONREAD
    static long arg;
    ioctl (fd, FIONREAD, (char *) &arg);
    return (int) arg;
#else
    struct pollfd pollfds[1];
    pollfds[0].fd = fd;
    pollfds[0].events = POLLIN;
    return poll (pollfds, 1, 0);
#endif
}

/* utility */

#define BSIZE 128 

static char ** create_argv(int *argc)
{
  int i;
  char **argv;
  *argc = C2F(sciiargc)() + 1;
  if ( ( argv = malloc((*argc)*sizeof(char *))) == NULL) return NULL;
  for ( i=0 ; i < *argc ; i++) 
    {
      char buf[BSIZE];
      C2F(scigetarg)(&i,buf,BSIZE);
      buf[BSIZE-1]='\0';
      strip_blank(buf);
      argv[i] = malloc((strlen(buf)+1)*sizeof(char));
      if ( argv[i] == NULL) return NULL;
      strcpy(argv[i],buf);
      /* fprintf(stderr,"arg[%d] %s\n",i,argv[i]);*/
    }
  return argv;
}

/* utility */

static void strip_blank(char *source)
{
  char *p;
  p = source;
  /* look for end of string */
  while(*p != '\0') p++;
  while(p != source) {
    p--;
    if(*p != ' ') break;
    *p = '\0';
  }
}

/*-------------------------------------------------------
 * Exit function called by some 
 * X11 functions 
 * call sciquit which call clear_exit
 *-------------------------------------------------------*/

int C2F(sciquit)(void)            /* used at Fortran level */
{
  int status = 0;
  /* fprintf(stderr,"I Quit Scilab through sciquit\n"); */

 /* F.Leray : graphic objects models (for axe and figure) can now be deleted */
  DestroyAllGraphicsSons(pfiguremdl); paxesmdl = (sciPointObj *) NULL;
  DestroyFigure(pfiguremdl); pfiguremdl = (sciPointObj *) NULL;
  
  if ( no_startup_flag == 0) 
    {
      char *quit_script =  get_sci_data_strings(5);
      C2F(scirun)(quit_script,strlen(quit_script));
    }
  sci_exit(status) ;
  return 0;
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
  /** clean ieee **/
#ifdef sun 
#ifndef SYSV
#include <sys/ieeefp.h>
  {
    char *mode, **out, *in;
    ieee_flags("clearall","exeption","all", &out);
  }
#endif 
#endif 
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
  /* fprintf(stderr," usr1 signal "); */
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

/*-------------------------------------------------------
 * Utility function to try to hide system differences from
 * everybody who used to call killpg() 
 *-------------------------------------------------------*/

int kill_process_group(pid, sig)
    int pid;
    int sig;
{
    return kill (-pid, sig);
}

/*-------------------------------------------------------
 * Syntax 
 *-------------------------------------------------------*/

static void Syntax (badOption)
    char *badOption;
{
  struct _options *opt;
  int col;

  fprintf (stderr, "%s:  bad command line option \"%s\"\r\n\n",
	   ProgramName, badOption);

  fprintf (stderr, "usage:  %s", ProgramName);
  col = 8 + strlen(ProgramName);
  for (opt = options; opt->opt; opt++) {
    int len = 3 + strlen(opt->opt);	 /* space [ string ] */
    if (col + len > 79) {
      fprintf (stderr, "\r\n   ");  /* 3 spaces */
      col = 3;
    }
    fprintf (stderr, " [%s]", opt->opt);
    col += len;
  }

  fprintf (stderr, "\r\n\nType %s -help for a full description.\r\n\n",
	   ProgramName);
  exit (1);
}

/* V.C 04/2004 */
/* Function used to know if we are in window mode (returned value is 1) */
/* or in console mode (returned value is 0) */
int IsConsoleMode(void)
{
  return no_window;
}
