/*-----------------------------------------------------------------------------------*/
/* Real main function for Scilab on X11 platform */
 /*-----------------------------------------------------------------------------------*/
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

#include "machine.h"
#include "x_main.h"
#include "math.h"
#include "version.h"
#include "realmain.h" /* realmain */
#include "scimem.h" /* nofpex */
#include "scilabmode.h"

#include "DestroyObjects.h"
#include "periScreen.h"

extern void  controlC_handler(int n);

#include "All-extern.h"
#include "All-extern-x.h"

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
/*-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------*/
extern void sci_clear_and_exit (int);
extern char *strindex (register char *s1, register char *s2);
extern void do_hangup(void);
extern void do_kill(Widget gw, caddr_t closure, caddr_t data);
extern void sci_usr1_signal(int n) ;
extern char ** create_argv(int *argc);
/*-----------------------------------------------------------------------------------*/
/*static void Syntax __PARAMS((char *badOption));  */
/*static void Syntax (char *badOption);  */
/*-----------------------------------------------------------------------------------*/
/**
 * mainsci.f directly call this function 
 * thus this is the real main for scilab 
 * Copyright Inria/Enpc 
 */
/*-----------------------------------------------------------------------------------*/
extern int sci_show_banner;
/*----------------------------------------------------------------------------------*/
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
	offset(title), XtRString, SCI_VERSION_STRING},
    {XtNiconName, XtCIconName, XtRString, sizeof(char *),
	offset(icon_name), XtRString, SCI_VERSION_STRING},
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
/*----------------------------------------------------------------------------------*/
/**
 * DeleteWindow(): Action proc to implement ICCCM delete_window.
 */
 /*----------------------------------------------------------------------------------*/
void DeleteWindow(Widget w, XEvent *event, String *params, Cardinal *num_params)
{
 sci_clear_and_exit(0); 
}
/*----------------------------------------------------------------------------------*/
void KeyboardMapping(Widget w, XEvent *event, String *params, Cardinal *num_params)
{
    switch (event->type) {
       case MappingNotify:
	  XRefreshKeyboardMapping(&event->xmapping);
	  break;
    }
}
/*----------------------------------------------------------------------------------*/
static XtActionsRec actionProcs[] = {
  {"DeleteWindow", DeleteWindow},
  {"SGDeleteWindow", SGDeleteWindow},
  {"KeyboardMapping", KeyboardMapping}
};
/*----------------------------------------------------------------------------------*/
/**
 * used by scilab to know the dpy and toplevel 
 */
/*----------------------------------------------------------------------------------*/
int Xscilab(Display **dpy,Widget *topwid)
{
  *topwid=toplevel;
  if ( toplevel != (Widget) NULL) *dpy=XtDisplay(toplevel);
  return(1);
}
/*----------------------------------------------------------------------------------*/
Atom wm_delete_window;
/*----------------------------------------------------------------------------------*/
/**
 * initColors: To allow resources to be specified separately for color
 *	and mono displays, we add a dummy Form widget below realToplevel
 *	in appropriate circumstances.
 */
/*----------------------------------------------------------------------------------*/ 
#define ADDTOPLEVEL(NAME) \
	toplevel_w = XtCreateManagedWidget(NAME,formWidgetClass, \
					 realToplevel_w,(ArgList)0 ,(Cardinal)0);


/*----------------------------------------------------------------------------------*/
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
      setcolordef(1) ;
      break;
    case StaticGray:
    case GrayScale:
      ADDTOPLEVEL("gray");
      setcolordef(0) ;
      break;
    default:
      setcolordef(0) ;
      toplevel_w = realToplevel_w;
    }
  } else {
      setcolordef(0) ;
      toplevel_w = realToplevel_w;
  }
  /* Ton van Overbeek patch 22/08/00 */
  return toplevel_w;
}
/*----------------------------------------------------------------------------------*/
void main_sci (char *startup, int lstartup,int memory)
{
  XtermWidget CreateSubWindows();
  register TScreen *screen;
  register int  pty;
  int Xsocket;
  int argc =0;
  char ** argv = NULL;
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

  xterm_name = resource.xterm_name;
  if (strcmp(xterm_name, "-") == 0) xterm_name = "xterm";
  XtSetValues (toplevel, ourTopLevelShellArgs,
	       (Cardinal)  number_ourTopLevelShellArgs);
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
/*----------------------------------------------------------------------------------*/
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
#ifdef solaris
#include <stropts.h>
#include <poll.h>
#endif
/*----------------------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------------------*/
void sci_clear_and_exit(int n) /* used with handlers */ 
{
   C2F(sciquit)();
}
/*----------------------------------------------------------------------------------*/
/**
 * usr1 signal : used to transmit a Control C to 
 * scilab 
 */
/*----------------------------------------------------------------------------------*/
void sci_usr1_signal(int n) 
{
   controlC_handler(n);
}
/*----------------------------------------------------------------------------------*/
/**
 * Ctrl-Z : stops the current computation 
 *          or the current interface call 
 */
/*----------------------------------------------------------------------------------*/
void  sci_sig_tstp(int n)
{
  Scierror(999,"SIGSTP: aborting current computation\r\n");
}
/*----------------------------------------------------------------------------------*/
/**
 * Utility function to try to hide system differences from
 * everybody who used to call killpg() 
 */
/*----------------------------------------------------------------------------------*/
int kill_process_group(pid, sig)
    int pid;
    int sig;
{
    return kill (-pid, sig);
}
/*----------------------------------------------------------------------------------*/
/**
 * Syntax 
 */
/*----------------------------------------------------------------------------------*/
/*
is not used at the moment (easy to say that because of the static)
static void Syntax(char *badOption)
{
  struct _options *opt;
  int col;

  fprintf (stderr, "%s:  bad command line option \"%s\"\r\n\n",
	   ProgramName, badOption);

  fprintf (stderr, "usage:  %s", ProgramName);
  col = 8 + strlen(ProgramName);
  for (opt = options; opt->opt; opt++) {
  int len = 3 + strlen(opt->opt);	 // space [ string ] 
    if (col + len > 79) {
	fprintf (stderr, "\r\n   ");  // 3 spaces 
      col = 3;
    }
    fprintf (stderr, " [%s]", opt->opt);
    col += len;
  }

  fprintf (stderr, "\r\n\nType %s -help for a full description.\r\n\n",
	   ProgramName);
  exit (1);
}*/
/*----------------------------------------------------------------------------------*/
/** V.C 04/2004 
 * Function used to know if we are in window mode (returned value is 1)
 * or in console mode (returned value is 0) 
 */
/*----------------------------------------------------------------------------------*/ 
int IsNoInteractiveWindow(void)
{
	return (getScilabMode() == SCILAB_NWNI);
}
/*----------------------------------------------------------------------------------*/
void InitXsession(void)
{ 
	static Display *dpy = (Display *) NULL;
	static  Widget toplevel1;
	int i=0;
	
	DisplayInit("",&dpy,&toplevel1);
	for (i=0;i<1000;i++) app_flush(); /* force to empty X event loop */
	XSync(dpy,0);
}
/*----------------------------------------------------------------------------------*/
