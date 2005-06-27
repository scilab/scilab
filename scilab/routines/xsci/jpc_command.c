/*****************************************************************************
 *  xscilab - X Window System interface to scilab
 *  
 * 	Copyright 1990  ENPC-Cergrene
 *  Adaptation from  GDB:  Chancelier J.Ph
 *  xscilab Created:   	1992
 *****************************************************************************/
/*****************************************************************************
 *  Based on
 *  xdbx - X Window System interface to the dbx debugger
 *  Author:  	Po Cheung
 *  Created:   	March 10, 1989
 *  xxgdb - X Window System interface to the gdb debugger
 * 	Copyright 1990 Thomson Consumer Electronics, Inc.
 *  Adaptation to GDB:  Pierre Willard
 *  XXGDB Created:   	December, 1990
 *
 *****************************************************************************/

/*  command.c
 *
 *    Create the command window, the command buttons and their callbacks.
 *
 *    CreateCommandPanel() : 	Create a window with command buttons
 *    CreateButtons() :		Create command buttons in panel
 *    CreateFormWithButtons(parent) : Special set of buttons button 
 *    AddButton() :		Add a command button into the command window
 *    Command callbacks for the command buttons:
 */

#include <signal.h>
#include <ctype.h>
#include <sys/wait.h>

#ifdef __STDC__
#include <stdlib.h>
#else
#include <malloc.h>
#endif

#include "jpc_global.h"
#include "../os_specific/Os_specific.h" 
#include "All-extern-x1.h" 
#include "../graphics/Graphics.h" 

#include <X11/cursorfont.h>
#define	 REVERSE	0
#define	 FORWARD	1
#define PI0 (integer *) 0
#define PD0 (double *) 0

extern void SendSGDeleteMessage __PARAMS((int win_num));
extern void do_kill();
extern int get_is_reading();
extern char GetDriver();
#ifdef BSD
static char	savedCommand[LINESIZ] = ""; 
#endif

static void DoIt  __PARAMS((Widget w, XtPointer command, XtPointer ));  
static void Do_Kill  __PARAMS((Widget w, XtPointer command, XtPointer ));  
static void Do_Stop  __PARAMS((Widget w, XtPointer command, XtPointer ));  
static void info_handler  __PARAMS((Widget w, caddr_t , XEvent *));  
static void AddInfoHandler  __PARAMS((Widget , char *));  
static void CreateButtons  __PARAMS((Widget parent));  
static void Countp  __PARAMS((Widget , XtPointer , XtPointer ));  
static void Countm  __PARAMS((Widget , XtPointer , XtPointer ));  
static void SendCountSet  __PARAMS((Widget , XtPointer , XtPointer ));  
static void SendCountRaise  __PARAMS((Widget , XtPointer , XtPointer ));  
static void SendCountDelete  __PARAMS((Widget , XtPointer , XtPointer ));  
static void CreateFormWithButtons  __PARAMS((Widget parent));  
static Widget AddButton __PARAMS((Widget, char *, void (*function)( ), XtPointer ));  
static Widget AddInMenu __PARAMS((Widget, char *, void (*function) ( ), XtPointer ));  

char *Startup= (char *) 0, *Demos= (char *) 0;
Boolean		PopupMode = False;
Widget commandWindow;


/*  Execute the command specifed in client_data  */

static void DoIt (w, command, call_data)
    Widget w;
    XtPointer command;
    XtPointer call_data;
{
    /* run, cont, next, step, where, up, down, status */
    write_scilab(command);
}




static void Do_Kill (w, command, call_data)
    Widget w;
    XtPointer command;
    XtPointer call_data;
{
 sci_clear_and_exit(1);
}

static void Do_Stop (w, command, call_data)
    Widget w;
    XtPointer command;
    XtPointer call_data;
{
  int j = SIGINT;
  C2F(sigbas)(&j);
  if (get_is_reading()) write_scilab("\n");
}


/* for all file operations */
void FileG1(w, closure, call_data)
    Widget w;
    XtPointer closure;
    caddr_t call_data;
{
   popup_file_panel( (Widget)closure,"File Menu");
 }

static Widget AddButton(parent, name, function, client_data)
     Widget parent;
     char *name;
     void (*function) ();
     XtPointer client_data;		/* callback registered data */
{
  Widget 	lbut;
  Arg 	args[MAXARGS];
  Cardinal 	n=0;
  lbut = XtCreateManagedWidget(name, commandWidgetClass, parent, args, n);
  XtAddCallback(lbut, XtNcallback, function, client_data);
  return (lbut);
}


static Widget AddInMenu(parent, name, function, client_data)
     Widget parent;
     char *name;
     void (*function) ();
     XtPointer client_data;		/* callback registered data */
{
  Widget 	lbut;
  Arg 	args[MAXARGS];
  Cardinal 	iargs=0;
  lbut = XtCreateManagedWidget(name,smeBSBObjectClass,parent, args, iargs);
  XtAddCallback(lbut, XtNcallback, function, client_data);
  return (lbut);
}

/**************************************************************************
 * help message associated to buttons
 **************************************************************************/

static void
info_handler(w, client_data, event)
     Widget	w;				/* unused */
     caddr_t	client_data;
     XEvent	*event;
{
  if      (event->type == LeaveNotify ) DefaultMessageWindow();
  else if (event->type == EnterNotify) UpdateMessageWindow("%s", (char *) client_data);
} 

static void
AddInfoHandler(widget, message)
Widget widget;
char *message;
{
  XtAddEventHandler(widget,
		    (EventMask) (EnterWindowMask|LeaveWindowMask),
		    False,
		    (XtEventHandler)info_handler,
		    (caddr_t) message);
}


static Widget menubut0= 0;

/* on en a besoin ds getfile.c */

void getMenuBut0(w) 
     Widget *w;
{
  *w = menubut0;
}

static void CreateButtons (parent)
     Widget parent;
{
  Widget menu0,menuentry;
  Widget menu1,menubut1;
  Cardinal iargs=0;
  Arg args[1];
  integer iopt,nc;

  iargs=0;
  XtSetArg(args[iargs], XtNmenuName, "MenuS"); iargs++;
  menubut0= XtCreateManagedWidget("ButMenuS",menuButtonWidgetClass,parent,args,iargs);
  AddInfoHandler(menubut0,"File Operations/Quit/Kill"); 
  menu0 = XtCreatePopupShell("MenuS", simpleMenuWidgetClass,menubut0,(Arg *) 0,(Cardinal)0);
  menuentry = AddInMenu (menu0, "File", FileG1, (XtPointer) menubut0 );
  /* AddInfoHandler(menuentry,"Getf, Exec, Load and Save operations"); */
  menuentry = AddInMenu(menu0, "Kill", Do_Kill, " ");
  /* AddInfoHandler(menuentry,"Kill Scilab"); */
  menuentry = AddInMenu(menu0, "Quit", DoIt, "quit\n");
  /* AddInfoHandler(menuentry,"Quit Scilab"); */


  iargs=0;
  XtSetArg(args[iargs], XtNmenuName, "MenuC"); iargs++;
  menubut1= XtCreateManagedWidget("ButMenuC",menuButtonWidgetClass,parent,args,iargs);
  AddInfoHandler(menubut1,"Abort/Stop/Resume");
  menu1 = XtCreatePopupShell("MenuC", simpleMenuWidgetClass,menubut1,(Arg *) 0,(Cardinal)0);
  menuentry = AddInMenu(menu1, "Resume", DoIt, "resume\n");
  /* AddInfoHandler(menuentry,"Continue Scilab execution after pause or stop"); */
  menuentry = AddInMenu(menu1, "Abort", DoIt, "abort\n");
  /* AddInfoHandler(menuentry,"Abort Scilab execution after pause or stop"); */

  iopt=2;C2F(infficl)(&iopt,&nc);
  Startup=(char *) malloc( (38+nc+2)*(sizeof(char)));
  strcpy(Startup,"abort;\nsavehistory();\nresethistory();\n");
  C2F(inffic)(&iopt, &(Startup[38]),&nc);strcat(Startup,"\n");
  /*menuentry = AddInMenu(menu1, "Restart", DoIt,(XtPointer)Startup); */
  /*  AddInfoHandler(menuentry,"Clear everything"); */
  menuentry = AddInMenu(menu1, "Stop", Do_Stop, " ");
  /* AddInfoHandler(menuentry,"Stop execution"); */

  iopt=3;C2F(infficl)(&iopt,&nc);
  Demos=(char *) malloc( (nc+2)*(sizeof(char)));
  C2F(inffic)(&iopt,Demos,&nc);strcat(Demos,"\n");
  menuentry = AddButton (parent, "Demos", DoIt, Demos);
  AddInfoHandler(menuentry,"Exec demos");

  CreateFormWithButtons(parent);
}


/*	Function Name: Count
 *	Description: This callback routin will increment that counter
 *                   and display the number as the label of the widget passed 
 *                   in the closure.
 *	Arguments: widget - *** UNUSED ***
 *                 closure - a pointer to the label widge to display the 
 *                           string in.
 *                 callData - *** UNUSED ***
 *	Returns: none
 */



static integer lab_count = 0;
static char gwin_name[100];
static Widget GWinButMenu = (Widget) 0;

void MenuFixCurrentWin(ivalue)
int ivalue;
{
  static int firstentry=0;
  int  i;
  if ( firstentry == 0) 
    {
      strcpy(gwin_name, "Graphic Window 0");
      firstentry++;
    }
  if ( lab_count != ivalue )
    {
      C2F(xscion)(&i);
      if (i==1)
	{
	  Arg arg[1];
	  lab_count = ivalue;
	  sprintf( gwin_name, "Graphic Window %d", (int) ivalue );
	  XtSetArg( arg[0], XtNlabel, gwin_name );
	  XtSetValues(GWinButMenu, arg, ONE );
	}
    }
}



static void 
Countp(widget, closure, callData)
Widget widget;
XtPointer closure, callData;
{
   Arg arg[1];
   sprintf( gwin_name, "Graphic Window %d ", (int) ++lab_count );
   XtSetArg( arg[0], XtNlabel, gwin_name );
   XtSetValues( (Widget)closure, arg, ONE );
}

static void 
Countm(widget, closure, callData)
Widget widget;
XtPointer closure, callData;
{
   Arg arg[1];
   lab_count = (lab_count == 0) ? 0 : lab_count-1;
   sprintf( gwin_name, "Graphic Window %d ", (int) lab_count);
   XtSetArg( arg[0], XtNlabel, gwin_name );
   XtSetValues( (Widget)closure, arg, ONE );
}


static void 
SendCountSet(widget, closure, callData)
     Widget widget;
     XtPointer closure, callData;
{
  scig_sel(lab_count);
}


static void 
SendCountRaise(widget, closure, callData)
     Widget widget;
     XtPointer closure, callData;
{
  scig_raise(lab_count);
}

static void 
SendCountDelete(widget, closure, callData)
     Widget widget;
     XtPointer closure, callData;
{
  /* send a ClientMessage to say that we have 
   * deleted a graphic window ( used in xclick_any) */
  /* SendSGDeleteMessage(lab_count); */
  /** fprintf(stderr,"Destruction de la fenetre %d\n",lab_count); **/
  /* DeleteSGWin(lab_count); */
  C2F(deletewin)(&lab_count);
}

/*
 *
 * buttons with increment or decrement a value stored in a label
 * Creates a Form widget for the Graphic Window Management 
 */


static void
CreateFormWithButtons(parent)
Widget parent;
{
  Widget menu,menuentry1,menuentry2,menuentry3,buttonm,buttonp;
  Cardinal iargs=0;
  Arg args[2];
  iargs = 0;
  XtSetArg(args[iargs], XtNmenuName, "MenuGW"); iargs++;
  XtSetArg(args[iargs], XtNlabel,"Graphic Window  0    "); iargs++;
  GWinButMenu= XtCreateManagedWidget("ButMenuGW",menuButtonWidgetClass,parent,args,iargs);
  iargs=0;
  menu = XtCreatePopupShell("MenuGW", simpleMenuWidgetClass,GWinButMenu,(Arg *) 0,(Cardinal)0);
  menuentry1 = XtCreateManagedWidget("Setwin",smeBSBObjectClass,menu, args, iargs);
  menuentry2 = XtCreateManagedWidget("Raisewin", smeBSBObjectClass, menu, args, iargs);
  menuentry3 = XtCreateManagedWidget("Deletewin", smeBSBObjectClass,menu, args, iargs);

  XtAddCallback( menuentry1, XtNcallback, SendCountSet, (XtPointer) GWinButMenu );
  XtAddCallback( menuentry2, XtNcallback, SendCountRaise, (XtPointer) GWinButMenu );
  XtAddCallback( menuentry3, XtNcallback, SendCountDelete, (XtPointer) GWinButMenu );

  buttonp = XtCreateManagedWidget("Plus", smeBSBObjectClass,menu,args,iargs);
  XtAddCallback( buttonp, XtNcallback, Countp, (XtPointer) GWinButMenu );
  buttonm = XtCreateManagedWidget("Minus", smeBSBObjectClass,menu,args,iargs);
  XtAddCallback( buttonm, XtNcallback, Countm, (XtPointer) GWinButMenu );

  AddInfoHandler(GWinButMenu,"Graphic window raise/create/delete");
  /*
  AddInfoHandler(buttonp,"Increase the number of the window for graphic menu");
  AddInfoHandler(buttonm,"Decrease the number of the window for graphic menu");
  */
}



/**************************************************************************
 * The command panel
 **************************************************************************/

void CreateCommandPanel(parent)
Widget parent;
{
/* supression ici 
  Widget commandWindow;*/
  Cardinal n=0;
  commandWindow = XtCreateManagedWidget("commandWindow", formWidgetClass, 
					parent,(Arg *) NULL,(Cardinal) n);
  CreateButtons(commandWindow);
}






