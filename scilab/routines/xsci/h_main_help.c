/* Copyright (C) 1998 Chancelier Jean-Philippe */

/*
 * xarchie : An X browser interface to Archie
 *
 * George Ferguson, ferguson@cs.rochester.edu, 2 Nov 1991.
 * Version 2.0: 23 Apr 1993.
 */

#include <stdio.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>	
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Paned.h>
#include <X11/Xaw/Viewport.h>
#include <X11/Xaw/AsciiText.h>
#include <X11/Xaw/MenuButton.h>
#include <X11/Xaw/SimpleMenu.h>
#include <X11/Xaw/Scrollbar.h>
#include <X11/Xaw/Cardinals.h>	
#include <X11/cursorfont.h>
#include <X11/Xaw/List.h>

#ifdef __STDC__
extern void exit(int status);
extern void abort(void);
#else
extern void exit();
extern void abort();
#endif


#include "h_display.h"

#include "../sun/h_help.h"

/*	-	-	-	-	-	-	-	-	*/
/*
 * Functions defined in this file:
 */
/* main() */
void bye();
void setBusyStatus(),setIconStatus();

static void initGraphics(),initColors(),initWidgets();
static void initErrorHandlers();

static void setWindowBusyStatus();
static void iconifyEventHandler();

static void initActions();
static void quitAction();

/*
 * Global graphics data
 */
Display *display;
Screen *screen;
Window root;
Atom wm_delete_window,WM_PROTOCOLS,WM_STATE;

/*
 * Local graphics data
 */
static Cursor busyCursor;

/*
 * Global widget data
 */
static XtAppContext appContext;
Widget toplevel,realToplevel;


/*
 * Other global data
 */
char *program;


/*
 * Non-widget resources obtained from resource manager
 */

typedef struct _AppResources {
    int		offset;
    XFontStruct	*xarchieFont;
    XFontStruct	*xarchieBoldFont;
    Boolean	autoScroll;
    Boolean	pasteBuffer;
    String	visualType;
    Pixmap	defaultIcon;
    Pixmap	busyIcon;
} AppResources;

/*
 * The application resources
 */
static 
AppResources appResources;

static XtResource resources[] = {
    { "xarchieFont", "Font", XtRFontStruct, sizeof(XFontStruct*),
      XtOffset(AppResources *,xarchieFont), XtRString, XtDefaultFont },
    { "xarchieBoldFont", "Font", XtRFontStruct, sizeof(XFontStruct*),
      XtOffset(AppResources *,xarchieBoldFont), XtRString, XtDefaultFont },
    { "autoScroll", "AutoScroll", XtRBoolean, sizeof(Boolean),
      XtOffset(AppResources *,autoScroll), XtRImmediate, (XtPointer)True },
    { "pasteBuffer", "PasteBuffer", XtRBoolean, sizeof(Boolean),
      XtOffset(AppResources *,pasteBuffer), XtRImmediate, (XtPointer)True },
    { "visualType", "VisualType", XtRString, sizeof(String),
      XtOffset(AppResources *,visualType), XtRImmediate, (XtPointer)"" },
    { "defaultIcon", "Icon", XtRBitmap, sizeof(Pixmap),
      XtOffset(AppResources *,defaultIcon), XtRImmediate, None },
    { "busyIcon", "Icon", XtRBitmap, sizeof(Pixmap),
      XtOffset(AppResources *,busyIcon), XtRImmediate, None },
};


/*ARGS UNUSED*/
static void quitAction(w,event,params,num_params)
     Widget w; 
     XEvent *event; 
     String *params; 
     Cardinal *num_params;
{
    bye(0);
}

/*
 * Data defined here:
 */
static XtActionsRec actionTable[] = {
    { "quit",			quitAction },
};

static void
initActions()
{
    XtAppAddActions(appContext,actionTable,XtNumber(actionTable));
    initHelpActions(appContext);
}


main(argc,argv)
int argc;
char **argv;
{
    program = argv[0];
    /* Init display and application context, other globals, parse args */
    initGraphics(&argc,argv);
    /* Init other non-widget stuff */
    initErrorHandlers();
    initColors();
    initActions();
    /* Then the main panel widgets */
    popupHelpPanel();
    /* Now we realize so the toplevel window is available */
    /* XtRealizeWidget(realToplevel); */
    /* Other initializations that need to go now */
    /* initHelpPanel(); */
    /* XtMapWidget(realToplevel); */
    /* Do it */
    XtAppMainLoop(appContext);
    /*NOTREACHED*/
}

void
bye(ret)
int ret;
{
    XtDestroyApplicationContext(appContext);
    exit(ret);
}

/*	-	-	-	-	-	-	-	-	*/
/* Initialization routines */

static String fallbackResources[] = {
#include "Xscilab.ad.h"
    NULL
};

static void
initGraphics(argcp,argv)
int *argcp;
char **argv;
{
    /* Some systems want (Cardinal*), others want (int*) for argcp. */
    realToplevel = toplevel =
	XtAppInitialize(&appContext,"Xscilab",NULL,ZERO,
			(Cardinal *)argcp,argv,fallbackResources,
			(ArgList) 0,(Cardinal) 0);
    /*
     * Set graphics globals
     */
    display = XtDisplay(realToplevel);
    screen = XtScreen(realToplevel);
    root = RootWindowOfScreen(screen);
    busyCursor = XCreateFontCursor(display,XC_watch);
    WM_PROTOCOLS = XInternAtom(display,"WM_PROTOCOLS",False);
    wm_delete_window = XInternAtom(display,"WM_DELETE_WINDOW",False);
    WM_STATE = XInternAtom(display,"WM_STATE",False);
    /*
     * Get non-widget resources
     */
    XtGetApplicationResources(realToplevel,(XtPointer)&appResources,
			      resources,XtNumber(resources),
			      (ArgList) 0,(Cardinal) 0);
    /*
     * Catch iconify/deiconify so we can do children also
     */
    XtAddEventHandler(realToplevel,PropertyChangeMask,False,
		      iconifyEventHandler,NULL);

}

/*
 * initColors: To allow resources to be specfied separately for color
 *	and mono displays, we add a dummy Form widget below realToplevel
 *	in appropriate circumstances.
 *      This means that:
 *	  - realToplevel gets realized
 *	  - toplevel gets used as parent of children
 */

#define ADDTOPLEVEL(NAME) \
	toplevel = XtCreateManagedWidget(NAME,formWidgetClass, \
					 realToplevel,NULL,0);
static void
initColors()
{
    Visual *visual;
    /* The default is no extra widget (ie. mono) */
    toplevel = realToplevel;
    /* See if the user specified a type */
    if (strcmp(appResources.visualType,"mono") == 0) {
	return;
    } else if (strcmp(appResources.visualType,"color") == 0) {
	ADDTOPLEVEL("color");
	return;
    } else if (strcmp(appResources.visualType,"gray") == 0) {
	ADDTOPLEVEL("gray");
	return;
    }
    /* Otherwise we try to figure it out */
    if ((visual=XDefaultVisualOfScreen(screen)) == NULL) {
	fprintf(stderr,"%s: can't get info about visual!\n",program);
	return;
    }
    if (visual->map_entries > 2) {
	switch (visual->class) {
	  case StaticColor:
	  case PseudoColor:
	  case TrueColor:
	  case DirectColor:
	    ADDTOPLEVEL("color");
	    break;
	  case StaticGray:
	  case GrayScale:
	    ADDTOPLEVEL("gray");
	    break;
	  default:
	    toplevel = realToplevel;
	}
    } else {
	toplevel = realToplevel;
    }
}


static XtCallbackProc
PopupHelpPanel (w, number, client_data)
     Widget w;
     XtPointer number;
     XtPointer client_data;
{
  popupHelpPanel();
}


/*	-	-	-	-	-	-	-	-	*/
/* The following functions attempt to provide information in the event of
 * a crash. If you have trouble compiling them because of UNIX-isms in
 * the signal handlers, then add -DDONT_CATCH_ERRORS to the definition
 * of DEFINES in the Imakefile and re-make (or just #define it here).
 */
#ifndef DONT_CATCH_ERRORS
#include <signal.h>
static void crashHandler();
#endif /* DONT_CATCH_ERRORS */

static void
initErrorHandlers()
{
#ifndef DONT_CATCH_ERRORS
    signal(SIGBUS,crashHandler);
    signal(SIGSEGV,crashHandler);
#endif /* DONT_CATCH_ERRORS */
}

#ifndef DONT_CATCH_ERRORS
static void
crashHandler(sig)
int sig;
{
    char *s;

    switch (sig) {
	case SIGBUS: s = "SIGBUS"; break;
	case SIGSEGV: s = "SIGSEGV"; break;
	default: s = "UNKNOWN";
    }
    fprintf(stderr,"%s: caught a %s signal!\n",program,s);
    fprintf(stderr,"If you want to report an error, please indicate your hardware type,\n");
    fprintf(stderr,"operating system, compiler, and your version of X and include a\n");
    fprintf(stderr,"backtrace. Thanks.\n");
    abort();
}

#endif /* DONT_CATCH_ERRORS */


/*	-	-	-	-	-	-	-	-	*/
/* Misc. routines */
		
/*	-	-	-	-	-	-	-	-	*/

/*ARGSUSED*/
static void
iconifyEventHandler(w,client_data,event,cont)
Widget w;
XtPointer client_data;
XEvent *event;
Boolean *cont;
{
    int state;

    if (event->xproperty.state == PropertyNewValue &&
	event->xproperty.atom == WM_STATE) {
	Atom actual_type;
	int actual_format;
	unsigned long nitems = 0, bytes_after = 0;
	unsigned char* data = NULL;

	if (XGetWindowProperty(display,XtWindow(realToplevel),
			       WM_STATE,0,2,False,AnyPropertyType,
			       &actual_type,&actual_format,
			       &nitems,&bytes_after,&data) == Success) {
	    state = *(int *)data;
	    setHelpShellState(state);
	    XFree((char *)data);
	}
    }
}

void
setIconStatus(state)
Boolean state;
{
    Pixmap pixmap;
    char *name;
    Arg args[2];

    if (state == True) {
	pixmap = appResources.busyIcon;
	name = "xarchie:busy";
    } else {
	pixmap = appResources.defaultIcon;
	name = "xarchie";
    }
    XtSetArg(args[0],XtNiconName,name);
    XtSetArg(args[1],XtNiconPixmap,pixmap);
    XtSetValues(toplevel,args,2);
}


Scistring(str)
     char *str;
{
  fprintf(stderr,"%s",str);
};
