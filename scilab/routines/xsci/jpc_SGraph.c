 /*------------------------------------------------------------------------
 *    X11 interface 
 *    Copyright (C) 1998-2001 Enpc/Jean-Philippe Chancelier
 *    jpc@cermics.enpc.fr 
 --------------------------------------------------------------------------*/

#include <stdio.h>			/* For the Syntax message */
#include <signal.h>

#include <X11/keysym.h>
#include <X11/DECkeysym.h>

#include <X11/Xatom.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xaw3d/Box.h>
#include <X11/Xaw3d/Command.h>
#include <X11/Xaw3d/Form.h>
#include <X11/Xaw3d/Label.h>
#include <X11/Xaw3d/Cardinals.h>
#include <X11/Xaw3d/Paned.h>
#include <X11/Xaw3d/Grip.h>
#include <X11/Xaw3d/MenuButton.h>
#include <X11/Xaw3d/SimpleMenu.h>
#include <X11/Xaw3d/SmeBSB.h>
#include <X11/Xaw3d/SmeLine.h>
#include <X11/Xaw3d/Viewport.h>
#include <X11/Xaw3d/Panner.h>	
#include <X11/IntrinsicP.h>
#include <X11/Xaw3d/ViewportP.h>
#include <string.h>

#include "../sci_mem_alloc.h"
#include "../graphics/GetProperty.h"
#include "../graphics/DestroyObjects.h"

extern int StoreCommand  __PARAMS((char *command));


/** getpid **/
#ifdef __STDC__
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#else 
#include <malloc.h>
extern char * getenv();
extern int getpid();
#endif

#include "../graphics/Math.h"
#include "../os_specific/men_Sutils.h"
#include "../menusX/men_scilab.h"
#include "All-extern.h"
#include "All-extern-x1.h"
#include "../graphics/bcg.h"
#include "../graphics/Events.h"

extern XtAppContext app_con;


extern Widget initColors  __PARAMS((Widget));  
extern Window GetBGWindowNumber __PARAMS((int));
extern void DisplayInit __PARAMS((char *string,Display **dpy,Widget *toplevel));
extern void ChangeBandF __PARAMS((int win_num,Pixel fg, Pixel bg));
extern integer F2C(fbutn) __PARAMS((char *,integer*,integer*));
extern void DeleteObjs __PARAMS((int win_num)); /* NG */
struct BCG *GetWindowXgcNumber __PARAMS((int));
extern int demo_menu_activate; /* add a demo menu in the graphic Window */
extern int GetEventWindow(XEvent* event);
/*---------------------------------------------------------------
  Functions and buttons associated with the Graphic Scilab Window 
  -----------------------------------------------------------------*/
static void ChangeBF1 __PARAMS(( Widget w,     char *str,     Pixel fg,     Pixel bg));
static void SetHints  __PARAMS((Widget topW));  
static int AddNewWin  __PARAMS((int, struct BCG *));
static XtEventHandler EventProc  __PARAMS((Widget widget, XtPointer , XEvent *));  
static XtEventHandler EventProc1  __PARAMS((Widget widget, XtPointer , XEvent *));  
/*static XtEventHandler ResizeWindow  __PARAMS((Widget, XtPointer , XConfigureEvent *));  */

static void Efface  __PARAMS((Widget, XtPointer , XtPointer ));  
static void Select  __PARAMS((Widget, XtPointer , XtPointer ));  
static void Delete  __PARAMS((Widget, XtPointer , XtPointer ));  
static void Print  __PARAMS((Widget, XtPointer , XtPointer ));  
static void SavePs  __PARAMS((Widget, XtPointer , XtPointer ));  
static void Save  __PARAMS((Widget, XtPointer , XtPointer ));  
static void Load  __PARAMS((Widget, XtPointer , XtPointer ));  
static void Zoom  __PARAMS((Widget, XtPointer , XtPointer ));  
static void UnZoom  __PARAMS((Widget, XtPointer , XtPointer ));  
static void Rot3D  __PARAMS((Widget, XtPointer , XtPointer ));  
static void ChangeBF1  __PARAMS((Widget, char *str, Pixel fg, Pixel bg));  
static void KillButton  __PARAMS((Widget, XtPointer , caddr_t ));  
static void SelMenu  __PARAMS((Widget, XtPointer , caddr_t));
static void SetUnsetMenu  __PARAMS((integer *, char *, integer *, int));  
static void PannerCallback __PARAMS((Widget, XtPointer , XtPointer ));
static void ViewportCallback __PARAMS((Widget, XtPointer , XtPointer ));
static void MoveChild  __PARAMS((Widget viewp, Position x,Position  y));
static int GetChilds __PARAMS((int win_num,int *nc,WidgetList *wL,Widget *outer,char *name,
			      int *name_pos) );
static void ignore_events( Widget widget, XEvent *event2,long mask );

static char popupname[sizeof("ScilabGraphic")+32];
static Arg args[10] ;
static int iargs=0;
static int MultiClickTime=200;/* to be changed after 3.1rc1, uncomment  also in  CreatePopupWindow below*/
/*
 * Fixes hints for the graphic windows (min sizes and wm delete window flag)
 */

static  Atom		wmDeleteWindow;
static  Atom  Close_SG_Window_Activated ;

static void SetHints(topW)
     Widget topW;
{
 XSizeHints		size_hints;
 size_hints.width	= 600;
 size_hints.height	= 400;
 size_hints.min_width	= 400;
 size_hints.min_height	= 300;
 size_hints.flags = /** USPosition |**/ USSize | PMinSize;
 XSetNormalHints(XtDisplay(topW),XtWindow(topW), &size_hints);
 /*
  *  Add a protocol flag indicating we wish to handle
  *  WM_DELETE_WINDOW requests (in fact we ignore it)
  */
 wmDeleteWindow = XInternAtom(XtDisplay(topW), "WM_DELETE_WINDOW", False);
 XSetWMProtocols(XtDisplay(topW),XtWindow(topW),&wmDeleteWindow, 1);
 /** to be able to detect close action in xclick_any **/
 Close_SG_Window_Activated = XInternAtom(XtDisplay(topW), 
					 "SCI_DELETE_WINDOW", False);
}

/** to check that an event is of type wmDeleteWindow **/

int IswmDeleteWindow(event) 
     XEvent *event;
{
  return event->type == ClientMessage &&
    event->xclient.data.l[0] == (int)wmDeleteWindow ;
}

/** to check that an event is of type Close_SG_Window_Activated **/
/** if true the window number is returnde **/

int IsCloseSGWindow(event) 
     XEvent *event;
{
  if ( event->type == ClientMessage &&
       ((XClientMessageEvent *)event)->message_type == Close_SG_Window_Activated )
    return event->xclient.data.l[0];
  else
    return -1;
}

/* Following variables are used for click, double-click detection
 * the associated code comes from the wx_canvs.cc code written by Julian Smart 1993
 * http://www.mit.edu/afs/athena/astaff/project/bugsquash/wx/src/x/wx_canvs.cc
 */
static Bool wait_dclick = FALSE;
static Bool dclick = FALSE;
static Bool lose_up = FALSE;
static long timerId;
static void
reset_click_counter (XtPointer ptr)
{
  dclick = FALSE;
  wait_dclick = FALSE;
}


void
btn_pressed(widget, event, params, num_params)
    Widget	widget;
    XEvent*	event;
    String*	params;
    Cardinal*	num_params;
{
  /* The following instruction has been commented out not to lose btn_pressed 
     emited just before an xclick or xgemouse call
    if (!get_wait_click()) return;
  */

  if (wait_dclick) {
   /* If we are waiting for a double-click, we only handle Button events
      in a special fashion.*/
      wait_dclick = FALSE;
      dclick = TRUE;
      lose_up = FALSE;
      XtRemoveTimeOut (timerId);
      return;
 }
  timerId = XtAppAddTimeOut(app_con, MultiClickTime,
			     (XtTimerCallbackProc) reset_click_counter, (XtPointer) NULL);
  wait_dclick = TRUE;
  lose_up = FALSE;
  dclick = FALSE;

  /*Wait until wait_dclick becomes FALSE 
    - either because of two consecutive btn press
    - either because of timeout 
    - either because of a mouse move  */
  do {
    if (XtAppPending (app_con))
      XtAppProcessEvent (app_con, XtIMAll);
  }  while (wait_dclick); 

  /* So here, dclick&lose_up have correct values.*/

  if (dclick ) { /* double click detected [10 11 12]*/
     PushClickQueue(GetEventWindow(event),event->xbutton.x,
		     event->xbutton.y,
		     event->xbutton.button-1+10,0,0);
  }
  else  {
    if  (lose_up) { /* simple click detected [3 4 5]*/
      if (params[0][0]=='0')
	PushClickQueue(GetEventWindow(event),event->xbutton.x,
		       event->xbutton.y,
		       event->xbutton.button-1+3,0,0);
      else if (params[0][0]=='1') /*shift press*/
	PushClickQueue(GetEventWindow(event),event->xbutton.x,
		       event->xbutton.y,
		       event->xbutton.button-1+3+2000,0,0);
      else if (params[0][0]=='2') /*ctrl press*/
	PushClickQueue(GetEventWindow(event),event->xbutton.x,
		       event->xbutton.y,
		       event->xbutton.button-1+3+1000,0,0);

      lose_up = FALSE;
      
    }
    else { /* button pressed detected [0 1 2]*/
      if (params[0][0]=='0')
	PushClickQueue(GetEventWindow(event),event->xbutton.x,
		       event->xbutton.y,
		       event->xbutton.button-1,0,0);
      else if (params[0][0]=='1') /*shift press*/
	PushClickQueue(GetEventWindow(event),event->xbutton.x,
		       event->xbutton.y,
		       event->xbutton.button-1+2000,0,0);
      else if (params[0][0]=='2') /*ctrl press*/
	PushClickQueue(GetEventWindow(event),event->xbutton.x,
		       event->xbutton.y,
		       event->xbutton.button-1+1000,0,0);

    }
   
 }



}

void
btn_released(widget, event, params, num_params)
    Widget	widget;
    XEvent*	event;
    String*	params;
    Cardinal*	num_params;
{
  if (wait_dclick) {
    lose_up = TRUE;
    return;
  }
  if (dclick)  return;
 /* button released detected [-5 -4 -3]*/
  PushClickQueue(GetEventWindow(event),event->xbutton.x,
		 event->xbutton.y,
		 event->xbutton.button-6,0,1);
}

void
ctrl_key(widget, event, params, num_params)
    Widget	widget;
    XEvent*	event;
    String*	params;
    Cardinal*	num_params;
{
  char buf[10];
  int ten=10;
  KeySym keysym;
  XComposeStatus compose_status = {NULL, 0};
  XLookupString((XKeyEvent *)&(event->xkey), buf, 
		ten, &keysym,&compose_status);
  /* <Ctrl> key  detected return  )keysym+1000 */
  PushClickQueue(GetEventWindow(event),event->xkey.x,
		 event->xkey.y,(int)keysym+1000,0,0);
}

void
key_pressed(widget, event, params, num_params)
    Widget	widget;
    XEvent*	event;
    String*	params;
    Cardinal*	num_params;
{
  char buf[10];
  int ten=10;
  KeySym keysym;
  XComposeStatus compose_status = {NULL, 0};
  XLookupString((XKeyEvent *)&(event->xkey), buf, 
		ten, &keysym,&compose_status);
  if ((int)keysym < XK_Shift_L ||(int)keysym ==XK_Delete ){
    /* regular key pressed detected. return  keysym */
    PushClickQueue(GetEventWindow(event),event->xkey.x,
		   event->xkey.y,(int)keysym,0,0);
  }
}
void
key_released(widget, event, params, num_params)
    Widget	widget;
    XEvent*	event;
    String*	params;
    Cardinal*	num_params;
{
  char buf[10];
  int ten=10;
  KeySym keysym;
  XComposeStatus compose_status = {NULL, 0};
  XLookupString((XKeyEvent *)&(event->xkey), buf, 
		ten, &keysym,&compose_status);
  if ((int)keysym <65000) {
    /* regular key released detected. return  -keysym */
    PushClickQueue(GetEventWindow(event),event->xkey.x,
		   event->xkey.y,-(int)keysym,0,1);
  }
}
void
mouse_moved(widget, event, params, num_params)
    Widget	widget;
    XEvent*	event;
    String*	params;
    Cardinal*	num_params;
{
  /*ignore_events(widget,&event2, ExposureMask) ;*/
  if (wait_dclick) { /*move during a double click detection*/
    wait_dclick=FALSE;
    XtRemoveTimeOut (timerId);
    return;
  }
  PushClickQueue(GetEventWindow(event),event->xbutton.x,
		 event->xbutton.y,
		 -1,1,0);
}

/*
<Btn1Motion>:MouseMoved(1)\n\

 */

static XtActionsRec event_actions[] =
{
    {"BtnPressed", (XtActionProc) btn_pressed},
    {"BtnReleased", (XtActionProc) btn_released},
    {"CtrlKey", (XtActionProc) ctrl_key},
    {"KeyPressed", (XtActionProc) key_pressed},
    {"KeyReleased", (XtActionProc) key_released},
    {"MouseMoved", (XtActionProc) mouse_moved},
};

static String	event_translations =
"\
<MouseMoved>:MouseMoved()\n\
Ctrl<Key>:CtrlKey()\n\
!Shift<KeyDown>:KeyPressed(1)\n\
!Lock<KeyDown>:KeyPressed(1)\n\
!Lock Shift <KeyDown>:KeyPressed(0)\n\
None<KeyDown>:KeyPressed(0)\n\
!Shift<KeyUp>:KeyReleased(1)\n\
!Lock<KeyUp>:KeyReleased(1)\n\
!Lock Shift<KeyUp>:KeyReleased(1)\n\
None<KeyUp>:KeyReleased(0)\n\
<BtnUp>:BtnReleased()\n\
!Shift<BtnDown>:BtnPressed(1)\n\
!Ctrl<BtnDown>:BtnPressed(2)\n\
<BtnDown>:BtnPressed(0)\n\
";
/*
 * Creates a new graphic window 
 */
#define DEFAULT_MES "graphic window"

static int AddNewWin( int popupc,   struct BCG *ScilabXgc)
{
  Widget bbox,color;
  Widget outer,zoom,rot3d,unzoom,sel,menuform;
  Widget filebutton,filemenu,clear,prnt,save,load,delete,saveps;

  iargs=0;
  /* eventually add a box to check for visual type */
  color = initColors(ScilabXgc->popup); 
  outer = XtCreateManagedWidget( "scigForm", panedWidgetClass,
				 color,
				 args, iargs);

  /** a form for menus and buttons **/
  iargs = 0;
  menuform = XtCreateManagedWidget( "scigmForm", formWidgetClass,outer,
				  args, iargs);

  ScilabXgc->Panner = XtCreateManagedWidget("Panner", pannerWidgetClass, 
				 menuform , NULL, ZERO);

  iargs=0;
  XtSetArg(args[iargs], XtNlabel," ");iargs++;
  ScilabXgc->CinfoW = XtCreateManagedWidget("scigraphicinfo",labelWidgetClass, 
				     menuform, args, iargs);

  /** menus and buttons **/
  /* Menu File */
  iargs=0;
  filebutton = XtCreateManagedWidget("File",
				  menuButtonWidgetClass,menuform,args, iargs);
  iargs=0;
  filemenu = XtCreatePopupShell("menu", simpleMenuWidgetClass, 
			    filebutton,args, iargs);
  iargs=0;
  clear = XtCreateManagedWidget("Clear", smeBSBObjectClass,filemenu,args, iargs);
  XtAddCallback(clear, XtNcallback,(XtCallbackProc) Efface,(XtPointer) popupc); 
  sel = XtCreateManagedWidget("Select", smeBSBObjectClass,filemenu,args, iargs);
  XtAddCallback(sel, XtNcallback,(XtCallbackProc) Select,(XtPointer) popupc); 
  prnt = XtCreateManagedWidget("Print", smeBSBObjectClass,filemenu,args, iargs);
  XtAddCallback(prnt, XtNcallback,(XtCallbackProc)  Print,(XtPointer) popupc);
  saveps = XtCreateManagedWidget("Export", smeBSBObjectClass,filemenu,args, iargs);
  XtAddCallback(saveps, XtNcallback,(XtCallbackProc)  SavePs,(XtPointer)popupc);
  save = XtCreateManagedWidget("Save", smeBSBObjectClass,filemenu,args, iargs);
  XtAddCallback(save, XtNcallback,(XtCallbackProc)  Save,(XtPointer)popupc);
  load = XtCreateManagedWidget("Load", smeBSBObjectClass,filemenu,args, iargs);
  XtAddCallback(load, XtNcallback,(XtCallbackProc)  Load,(XtPointer)popupc);
  delete = XtCreateManagedWidget("Close", smeBSBObjectClass,filemenu,args, iargs);
  XtAddCallback(delete, XtNcallback,(XtCallbackProc)  Delete,(XtPointer)popupc);
  
  /* Other buttons */
  zoom = XtCreateManagedWidget("Zoom", commandWidgetClass,menuform,args, iargs);
  XtAddCallback(zoom, XtNcallback,(XtCallbackProc)  Zoom,
		(XtPointer) popupc);
  unzoom = XtCreateManagedWidget("UnZoom", commandWidgetClass,menuform,args, iargs);
  XtAddCallback(unzoom, XtNcallback,(XtCallbackProc)  UnZoom,
		(XtPointer) popupc);
  rot3d = XtCreateManagedWidget("Rot3D", commandWidgetClass,menuform,args, iargs);
  XtAddCallback(rot3d, XtNcallback,(XtCallbackProc) Rot3D,
		(XtPointer) popupc);

  /** Graphic window viewport **/

  iargs =0;

  ScilabXgc->Viewport = XtCreateManagedWidget("graphicviewport",viewportWidgetClass,outer,args,iargs);

  bbox= XtCreateManagedWidget("bbox",boxWidgetClass,ScilabXgc->Viewport, args, iargs);

  /* I use a label in order to have foreground and background */
  iargs=0;
  XtSetArg(args[iargs], XtNlabel," ");iargs++;
  ScilabXgc->drawbox= XtCreateManagedWidget("scigraphic",labelWidgetClass,bbox, args, iargs);

  /* EventProc Must select the client Message Events */

  XtAddEventHandler(ScilabXgc->drawbox, ExposureMask| StructureNotifyMask,
		    False,
		    (XtEventHandler) EventProc1,
		    (XtPointer) popupc);  

  /* even if EventProc is unused by xclick or xgetmouse : the next line 
     set the proper mask for the drawbox window and this is necessary for 
     xclick and xgetmouse to work. Moreover this EventHandler keeps the 
     queue of unused events ok 
  */

  /* XtAddEventHandler(ScilabXgc->drawbox, 
		    ButtonPressMask|PointerMotionMask|ButtonReleaseMask|
		    KeyPressMask|KeyReleaseMask,False,
		    (XtEventHandler) EventProc, (XtPointer) popupc); */

  XtOverrideTranslations(ScilabXgc->drawbox,
		 XtParseTranslationTable(event_translations));
  XtAppAddActions(app_con, event_actions, XtNumber(event_actions));

  /** Communication between panner and viewport **/
  
  XtAddCallback( ScilabXgc->Viewport,
		XtNreportCallback, ViewportCallback, (XtPointer) ScilabXgc);
  XtAddCallback(ScilabXgc->Panner, 
		XtNreportCallback, PannerCallback, (XtPointer) ScilabXgc);

  /* For Graphic demo */
  if ( demo_menu_activate == 1 )  AddNewMenu(outer,ScilabXgc->drawbox);
  return(0);
}


/*	Function Name: PannerCallback
 *	Description: called when the slider is moved inside the panner 
 *	Arguments: w - the panner widget.
 *                 scigc_ptr -  scilabgc associated to win .
 *                 report_ptr - the panner record.
 *	Returns: none.
 */


/* ARGSUSED */
void PannerCallback(w, scigc_ptr , report_ptr)
     Widget w;
     XtPointer scigc_ptr, report_ptr;
{
  struct BCG *ScilabXgc = ((struct BCG *) scigc_ptr);
  Widget viewport = ScilabXgc->Viewport;
  XawPannerReport *report = (XawPannerReport *) report_ptr;
  MoveChild(viewport, report->slider_x ,report->slider_y);
}

/*	Function Name: SciViewportMove
 *	Description: used to move the panner and the viewport interactively 
 *                   through scilab command.
 *	Arguments: ScilabXgc : structure associated to a Scilab Graphic window
 *                 x,y : the x,y point of the graphic window to be moved at 
 *                 the up-left position of the viewport
 *	Returns: none.
 */

void SciViewportMove(ScilabXgc,x,y) 
     struct BCG *ScilabXgc;
     int x,y;
     
{
  Widget viewport,panner;
  Cardinal n=0;
  if ( ScilabXgc != NULL) 
    {
      viewport = ScilabXgc->Viewport;
      panner = ScilabXgc->Panner;
      MoveChild(viewport,x,y);
      XtSetArg (args[n], XtNsliderX, x); n++;  
      XtSetArg (args[n], XtNsliderY, y); n++;
      XtSetValues (panner, args, n);
    }
}


/*	Function Name: SciViewportGet
 *	Description: used to get panner position through scilab command.
 *	Arguments: ScilabXgc : structure associated to a Scilab Graphic window
 *                 x,y : the returned position 
 */

void SciViewportGet(ScilabXgc,x,y) 
     struct BCG *ScilabXgc;
     int *x,*y;
     
{
  Widget panner;
  Cardinal n=0;
  if ( ScilabXgc != NULL) 
    {
      Dimension dx,dy;
      panner = ScilabXgc->Panner;
      XtSetArg (args[n], XtNsliderX, &dx); n++;  
      XtSetArg (args[n], XtNsliderY, &dy); n++;
      XtGetValues (panner, args, n);
      *x = dx; *y=dy;
    }
  else
    {
      *x=0;*y=0;
    }
}

/*	Function Name: SciViewportClipGetSize
 *	Description: used to get clip size 
 *	Arguments: ScilabXgc : structure associated to a Scilab Graphic window
 *                 w,h : returned size ;
 */

void SciViewportClipGetSize(ScilabXgc,w,h) 
     struct BCG *ScilabXgc;
     int *w,*h;
{
  if ( ScilabXgc != NULL) 
    {
      ViewportWidget widg = (  ViewportWidget ) ScilabXgc->Viewport;
      register Widget clip = widg->viewport.clip;
      *w = clip->core.width;
      *h = clip->core.height;
    }
}

/*	Function Name: ViewportCallback
 *	Description: called when the viewport or its child has
 *                   changed 
 *	Arguments: viewport - the viewport widget.
 *                 scigc_ptr -  scilabgc associated to win .
 *                 report_ptr - the viewport record.
 *	Returns: none.
 */

extern void sci_pixmap_resize(struct BCG *Xgc,int x,int y);

/* ARGSUSED */


void ViewportCallback(Widget w,XtPointer scigc_ptr, XtPointer report_ptr)
{
  Dimension cwidth,cheight,clwidth,clheight,gwidth,gheight;
  ViewportWidget viewpw = (  ViewportWidget ) w;
  register Widget clip = viewpw->viewport.clip;
  register Widget child = viewpw->viewport.child;

  Cardinal n = 0;
  XawPannerReport *report = (XawPannerReport *) report_ptr;
  struct BCG *ScilabXgc = ((struct BCG *) scigc_ptr);
  Widget panner = ScilabXgc->Panner;
  

  if ( ScilabXgc->CurResizeStatus  == 0) 
    {

      XtSetArg (args[n], XtNsliderX, report->slider_x); n++;
      XtSetArg (args[n], XtNsliderY, report->slider_y); n++;
      if (report->changed != (XawPRSliderX | XawPRSliderY)) {
	XtSetArg (args[n], XtNsliderWidth, report->slider_width); n++;
	XtSetArg (args[n], XtNsliderHeight, report->slider_height); n++;
	XtSetArg (args[n], XtNcanvasWidth, report->canvas_width); n++;
	XtSetArg (args[n], XtNcanvasHeight, report->canvas_height); n++;
	/*
	  fprintf(stderr,"new reported %d %d %d %d \n",report->slider_width, report->slider_height,
	  report->canvas_width, report->canvas_height);
	*/
      }
      XtSetValues (panner, args, n);

      /** try to resize the child region if necessary **/
      /** Hope this won't create a loop in ViewportCallback **/

      iargs = 0 ;
      XtSetArg (args[iargs], XtNwidth, &cwidth ); iargs++;
      XtSetArg (args[iargs], XtNheight, &cheight); iargs++;
      XtGetValues (child, args, iargs);
      iargs = 0;
      XtSetArg (args[iargs], XtNwidth, &clwidth ); iargs++;
      XtSetArg (args[iargs], XtNheight, &clheight); iargs++;
      XtGetValues (clip, args, iargs);

      if (  ScilabXgc->drawbox != NULL) 
	{
	  int nw,nh;
	  iargs = 0;
	  XtSetArg (args[iargs], XtNwidth, &gwidth ); iargs++;
	  XtSetArg (args[iargs], XtNheight, &gheight); iargs++;
	  XtGetValues (ScilabXgc->drawbox, args, iargs);
	  nw= Max(gwidth,clwidth);
	  nh= Max(gheight,clheight);
	  if ( nw < cwidth || nh < cheight )
	    {
	      XtResizeWidget(child,nw,nh,0);
	      n=0;
	      XtSetArg (args[n], XtNcanvasWidth, nw); n++;
	      XtSetArg (args[n], XtNcanvasHeight,nh); n++;
	      XtSetValues (panner, args, n);
	    }
	}
    }
  else
    {
      /** here the graphic window must follow the window size **/
      /** try to resize the child region if necessary **/
      /** Hope this won't create a loop in ViewportCallback **/

      iargs = 0 ;
      XtSetArg (args[iargs], XtNwidth, &cwidth ); iargs++;
      XtSetArg (args[iargs], XtNheight, &cheight); iargs++;
      XtGetValues (child, args, iargs);
      iargs = 0;
      XtSetArg (args[iargs], XtNwidth, &clwidth ); iargs++;
      XtSetArg (args[iargs], XtNheight, &clheight); iargs++;
      XtGetValues (clip, args, iargs);
      if (   ScilabXgc->drawbox != NULL) 
	{
	  /* We resize Scilab drawbox and update window dimensions */
	  XtResizeWidget(ScilabXgc->drawbox,clwidth,clheight,0);
	  ScilabXgc->CWindowWidth  = clwidth;
	  ScilabXgc->CWindowHeight = clheight;
	  XtResizeWidget(child,clwidth,clheight,0);
	  /* if necessary we must resize the pixmap */
	  if (ScilabXgc->Cdrawable != (Drawable) NULL) /* ScilabXgc->CWindow ) */
	    {
	      /* here the best stuff to do is a scig_resize 
	       * scig_resize(ScilabXgc->CurWindow); 
	       * unfortunately this callback is called too 
	       * frequently when resizing and scig_resize can be slow 
	       * thus we just resize the window pixmap 
	       * and set a flag.
	       * Thus the last expose will have to properly redraw the window
	       */
	      /* will be done by expose 
		 sci_pixmap_resize(ScilabXgc,ScilabXgc->CWindowWidth, 
		 ScilabXgc->CWindowHeight);
	      */
	      ScilabXgc->Cdrawable_flag=1;
	    }
	  n=0;
	  XtSetArg (args[n], XtNsliderWidth, clwidth); n++;
	  XtSetArg (args[n], XtNsliderHeight,clheight); n++;
	  XtSetArg (args[n], XtNcanvasWidth, clwidth); n++;
	  XtSetArg (args[n], XtNcanvasHeight,clheight); n++;
	  XtSetValues (panner, args, n);
	}
    }
}

void SetBar(w, top, length, total)
     Widget w;
     Position top;
     Dimension length, total;
{
  XawScrollbarSetThumb(w, (float) top / (float) total, 
		       (float) length / (float) total );
}

/* reposition viewport bars */

void RedrawThumbs(w)
     ViewportWidget w;
{
  register Widget child = w->viewport.child;
  register Widget clip = w->viewport.clip;

  if (w->viewport.horiz_bar)
    SetBar( w->viewport.horiz_bar, -(child->core.x),
	   clip->core.width, child->core.width );

  if (w->viewport.vert_bar)
    SetBar( w->viewport.vert_bar, -(child->core.y),
	   clip->core.height, child->core.height );
}

/* move the child of the viewport */

static void MoveChild(viewp, x, y)
     Widget viewp;
     Position x, y;
{
  ViewportWidget w =   (ViewportWidget) viewp;
  register Widget child = w->viewport.child;
  register Widget clip = w->viewport.clip;

  /* make sure we never move past right/bottom borders */
  if ((int) (x + clip->core.width) > (int) child->core.width)
    x = child->core.width - clip->core.width;

  if ((int) (y + clip->core.height) > (int ) child->core.height)
    y = child->core.height - clip->core.height;

  /* make sure we never move past left/top borders */
  if (x < 0) x = 0;
  if (y < 0) y = 0;

  XtMoveWidget(child, - x, - y);
  RedrawThumbs(w);
}

/** used when we resize the graphic window (i.e the drawbox) at Scilab level **/

#define WINMINW 100
#define WINMINH 100

void GViewportResize(ScilabXgc,width,height) 
     struct BCG *ScilabXgc;
     int *width,*height;
{
  int Width = Max(*width,  WINMINW);
  int Height= Max(*height, WINMINH);
  Position x,y;
  Dimension cwidth,cheight,clwidth,clheight,wv,hv;
  ViewportWidget w = (  ViewportWidget ) ScilabXgc->Viewport;
  register Widget clip = w->viewport.clip;
  register Widget child = w->viewport.child;

  if ( ScilabXgc->CurResizeStatus == 0) 
    {
      /** Resizing the widget used for graphics **/
      XtResizeWidget(ScilabXgc->drawbox ,Width,Height,0);
      /** and if necessary its associated pixmap **/
      if (ScilabXgc->Cdrawable != (Drawable) 0 ) /* ScilabXgc->CWindow ) */
	{
	  sci_pixmap_resize(ScilabXgc,Width,Height);
	}
      /*
       * Resize the viewport child which contains the graphic widget 
       * we don't want the viewport child to be smaller than the 
       * clip region 
       */
      wv = Max(Width,clip->core.width);
      hv = Max(Height,clip->core.height);
      XtResizeWidget(child,wv,hv,0);
      RedrawThumbs(w);
      
      /** Now change the panner size to fit the new viewport sizes  **/
      /** Get child size **/
      iargs = 0 ;
      XtSetArg (args[iargs], XtNx, &x); iargs++;
      XtSetArg (args[iargs], XtNy, &y); iargs++;
      XtSetArg (args[iargs], XtNwidth, &cwidth ); iargs++;
      XtSetArg (args[iargs], XtNheight, &cheight); iargs++;
      XtGetValues (child, args, iargs);
      /** fprintf(stderr,"sizes child %d %d %d %d\n",x,y,cwidth,cheight); **/
      iargs = 0 ;
      XtSetArg (args[iargs], XtNwidth, &clwidth ); iargs++;
      XtSetArg (args[iargs], XtNheight, &clheight); iargs++;
      XtGetValues (clip, args, iargs);
      /** fprintf(stderr,"sizes clip %d %d %d %d\n",x,y,cwidth,cheight); **/
      /** Change the slider size inside the panner **/
      iargs =0;
      XtSetArg (args[iargs], XtNsliderX, -x); iargs++;
      XtSetArg (args[iargs], XtNsliderY, -y); iargs++;
      XtSetArg (args[iargs], XtNsliderWidth, clwidth ); iargs++;
      XtSetArg (args[iargs], XtNsliderHeight, clheight); iargs++;
      XtSetArg (args[iargs], XtNcanvasWidth,  cwidth); iargs++;
      XtSetArg (args[iargs], XtNcanvasHeight,  cheight); iargs++;
      /** fprintf(stderr,"sizes %d %d %d %d %d %d\n",x,y,cwidth,cheight,
	  clwidth, clheight); **/
      XtSetValues (ScilabXgc->Panner, args, iargs);
    }
  else
    {
      /* 
       * in order to resize the drawbox, we must resize the 
       * popup window.
       */
      iargs =0;
      XtSetArg (args[iargs], XtNwidth, &clwidth ); iargs++;
      XtSetArg (args[iargs], XtNheight, &clheight); iargs++;
      XtGetValues (ScilabXgc->popup, args, iargs);
      iargs =0;
      XtSetArg (args[iargs], XtNwidth, &cwidth ); iargs++;
      XtSetArg (args[iargs], XtNheight, &cheight); iargs++;
      XtGetValues (ScilabXgc->drawbox, args, iargs);
      /* we add to Width and Height the expected offset 
       * the drawbox can be bigger than the popup if 
       * we are comming back from a ScilabXgc->CurResizeStatus==0 state 
       */
      clwidth = Width  + Max(0,(int)clwidth - (int)cwidth);
      clheight= Height + Max(0,(int)clheight - (int)cheight);
      iargs = 0;
      XtSetArg (args[iargs], XtNwidth, clwidth ); iargs++;
      XtSetArg (args[iargs], XtNheight,clheight); iargs++;
      XtSetValues (ScilabXgc->popup, args, iargs);
      /** And now if necessary resize the pixmap associated to the drawbox **/
      if (ScilabXgc->Cdrawable != (Drawable) 0 ) /* ScilabXgc->CWindow ) */
	{
	  /** get new sizes of the drawbox **/
	  iargs =0;
	  XtSetArg (args[iargs], XtNwidth, &cwidth ); iargs++;
	  XtSetArg (args[iargs], XtNheight, &cheight); iargs++;
	  XtGetValues (ScilabXgc->drawbox, args, iargs);
	  sci_pixmap_resize(ScilabXgc,cwidth,cheight); 
	  /* scig_resize(ScilabXgc->CurWindow);  */
	}
    }
}

/** used when we resize the popup at scilab level **/
void GPopupResize(ScilabXgc,width,height) 
     struct BCG *ScilabXgc;
     int *width,*height;
{
  Dimension clwidth = Max(*width,  WINMINW);
  Dimension clheight= Max(*height, WINMINH);
  if ( ScilabXgc->popup!=(Widget)0) { /* check if it is not a tk canvas */
    iargs = 0;
    XtSetArg (args[iargs], XtNwidth, clwidth ); iargs++;
    XtSetArg (args[iargs], XtNheight,clheight); iargs++;
    XtSetValues (ScilabXgc->popup, args, iargs);
  }
}

/* 
 * Checking events in the Graphic Window 
 * in fact just the XtAddEventHandler is used 
 * used in xclick 
 * We keep the last  MaxCB click events on a queue just in case 
 * xclick wants them ( xclick(1)) 
 */


#ifdef  EPDEBUG
#define FDEBUG(x) fprintf x 
#else 
#define FDEBUG(x) 
#endif 


static void ignore_events( Widget widget, XEvent *event2,long mask )
{
  while (XCheckWindowEvent(XtDisplay(widget),XtWindow(widget),mask ,event2)==True)
    { 
      FDEBUG((stderr,"inside ignore_events count = %d\n",((XExposeEvent *) event2)->count));
    }
}



static XtEventHandler
EventProc( Widget widget, XtPointer number, XEvent *event)
{
  /*EventWindow = (int)number;*/
  return(0);
}


/*******************************************************
 * StructureNotifyMask | ExposureMask  Handler for graphic windows
 *******************************************************/

#ifdef  EP1DEBUG
#define FDEBUG_EP1(x) fprintf x 
#else 
#define FDEBUG_EP1(x) 
#endif 


static XtEventHandler 
EventProc1( Widget widget,  XtPointer number,  XEvent *event)
{
  XEvent event2;
  integer win_num= (integer) number;
  XExposeEvent *event1;
#ifdef EP1DEBUG
  static int counter=0;
  counter++;
#endif 
  switch (event->type) 
    {
    case Expose :
      /** Expose events **/
      event1= (XExposeEvent *) event;
      /* 
	 FDEBUG_EP1((stderr,"EventProc1: Expose: count %d x %d y %d width %d height %d\n",
	 event1->count ,event1->x,event1->y,event1->width,event1->height));
      */
      if ( event1->count == 0) 
	{
	  struct BCG *SciGc =  GetWindowXgcNumber(win_num );
	  if ( SciGc->Cdrawable_flag ==  1) 
	    {
	      FDEBUG_EP1((stderr,"EventProc1: Expose: --> scig_resize %d \r\n",counter)); 
	      SciGc->Cdrawable_flag = 0;
	      scig_resize(win_num);
	    }
	  else 
	    {
	      FDEBUG_EP1((stderr,"EventProc1: Expose: --> scig_expose %d \r\n",counter)); 
	      scig_expose(win_num);
	    }
	  ignore_events(widget,&event2, ExposureMask | StructureNotifyMask) ;
      	}
      break;
    case VisibilityNotify :
      /* We should get there only if server does not performs save_under */
      FDEBUG_EP1((stderr,"EventProc1: VisibilityNotify\n"));
      break;
    case ConfigureNotify :
      FDEBUG_EP1((stderr,"EventProc1: ConfigureNotify %d \n",counter));
      scig_resize(win_num);
      /** I ignore the following expose events **/
      ignore_events(widget,&event2, ExposureMask | StructureNotifyMask);
      break; 
    case CreateNotify  :
      FDEBUG_EP1((stderr,"EventProc1: CreateNotify\r\n",counter));
      break ;
    case MapNotify : 
      FDEBUG_EP1((stderr,"EventProc1: MapNotify\r\n",counter)); 
      /** I ignore the Expose after MapNotify **/
      ignore_events(widget,&event2, ExposureMask );
      break;
    case ReparentNotify :
      FDEBUG_EP1((stderr,"EventProc1: ReparentNotify\r\n"));
      break;
    default:
      FDEBUG_EP1((stderr,"EventProc1: default, event=%d window=%d\r\n",event->type,(int) win_num));
      return(0);
    }
  return(0);
}


/**************************************************************
 *
 * To clear the graphic window and clear the recorded graphics 
 * w and client_data are unused 
 * 
 *************************************************************/

static void 
Efface(w, number, client_data)
     Widget w;
     XtPointer number;
     XtPointer client_data;
{
  integer win_num = (integer) number ;
  scig_erase(win_num);
}

/**************************************************************
 *
 * To select the graphic window 
 * 
 *************************************************************/

static void
Select(w, number, client_data)
     Widget w;
     XtPointer number;
     XtPointer client_data;
{
  integer win_num = (integer) number ;
  scig_sel(win_num);
}

/*
 * send a ClientMessage to say that we have 
 * deleted a graphic window ( used in xclick_any)
 */

static void SendSGDeleteMessage(int win_num)
{
  Window Win;
  Widget toplevel;
  Display * dpy;
  XClientMessageEvent ev;
  DisplayInit("",&dpy,&toplevel);
  Win=GetBGWindowNumber(win_num);
  /** sending a message for xclick_any **/
  ev.type = ClientMessage;
  ev.window = Win;
  ev.message_type = Close_SG_Window_Activated;
  ev.format = 32;
  ev.data.l[0] =  win_num;
  ev.data.l[1] =  CurrentTime; 
  XSendEvent (dpy,Win , False, 0L, (XEvent *) &ev);
}  


/*------------------------------------------------------------------
 * Delete Window 
 *------------------------------------------------------------------*/


extern int  get_xclick_client_message_flag(void);
 
static void
Delete(w, number, client_data)
     Widget w;
     XtPointer number;
     XtPointer client_data;
{
  integer win_num = (integer) number ;
  int v_flag = 1;

  if (  get_delete_win_mode() == 1 )
    {
      wininfo("Cannot destroy window while acquiring zoom rectangle ");
      return;
    }
  if ( get_xclick_client_message_flag() == 1 ) 
    {
      /* the client message is only used while waiting for 
       * a mouse click 
       */
      PushClickQueue(win_num,0,0,-100,0,0);
    }
  /** Delete the graphic window **/
  Efface((Widget) 0,(XtPointer) number, (XtPointer) 0);
  if (version_flag() == 0) {DeleteObjs(win_num); v_flag = 0;}
  scig_deletegwin_handler(win_num);
  DeleteSGWin(win_num); /* Here we 1) destroy the ScilabXgc (set to NULL) if it is the last window in the list */
                        /*         2) or reset the ScilabXgc to the next one see DeleteSGWin*/

  /* That's why we can not use version_flag below because this function uses ScilabXgc->graphicsversion 
     that could have been possibly previously deleted !! */

  /* So, we use another flag named v_flag :*/
  delete_sgwin_entities(win_num,v_flag);
}

/* for Fortran call */

int C2F(deletewin)(number) 
     integer *number;
{
  Delete((Widget) 0,(XtPointer) *number,(XtPointer) 0);
  return(0);
}


/*********************************************************
 * Replot in Postscript style and send to printer 
 ********************************************************/

static  char bufname[256];
static  char printer[128];
static  char file[256];

static void
Print(w, number, client_data)
     Widget w;
     XtPointer number;
     XtPointer client_data;
{
  char *p1;
  integer win_num = (integer) number ;
  integer colored,orientation,flag=1,ok;

  prtdlg(&flag,printer,&colored,&orientation,file,&ok);
  if (ok==1) 
    {
      if ( ( p1 = getenv("TMPDIR"))  == (char *) 0 )
	{
	  sciprint("Cannot find environment variable TMPDIR\r\n");
	}

      sprintf(bufname,"%s/scilab-%d",p1,(int)win_num);
      scig_tops(win_num,colored,bufname,"Pos");
      sprintf(bufname,"$SCI/bin/scilab -%s %s/scilab-%d %s",
	      (orientation == 1) ? "print_l" : "print_p",
	      p1,(int)win_num,printer);
      system(bufname);
  }
}

/* for use inside menus */

void scig_print(number) 
     integer number;
{
  Print(NULL,(XtPointer) number,NULL);
}

/*********************************************************
 * Replot in Postscript or Xfig style and save 
 ********************************************************/

static void
SavePs(w, number, client_data)
     Widget w;
     XtPointer number;
     XtPointer client_data;
{
  integer win_num = (integer) number ;
  integer colored,orientation,flag=2,ok;
  prtdlg(&flag,printer,&colored,&orientation,file,&ok);

  if ( colored == 2 ) { colored = 0 ; } /* black and white selected => false */

  if (ok==1) 
    {
      if (strncmp(printer,"Postscript",10)==0 ) 
	{
	  /** Postscript*   **/
	  scig_tops(win_num,colored,file,"Pos");
	}
      else if (strcmp(printer,"Xfig")==0)
	{
	  /** Xfig   **/
	  scig_tops(win_num,colored,file,"Fig");
	}
      else if (strcmp(printer,"Gif")==0)
	{
	  /** Gif file **/
	  scig_tops(win_num,colored,file,"GIF");
	}
      else if (strcmp(printer,"PPM")==0)
	{
	  /** PPM file **/
	  scig_tops(win_num,colored,file,"PPM");
	}
      if ( strcmp(printer,"Postscript No Preamble") != 0)
	{
	  sprintf(bufname,"$SCI/bin/scilab -%s %s %s",
		  ( orientation == 1) ? "save_l" : "save_p",file,printer);
	  system(bufname);
	}
  }
}

/* for use inside menus */

void scig_export(number) 
     integer number;
{
  SavePs(NULL,(XtPointer) number,NULL);
}

/******************************************************
 * Binary File save 
 ******************************************************/

static void
Save(w, number, client_data)
     Widget w;
     XtPointer number;
     XtPointer client_data;
{
  char *filename;
  integer win_num = (integer) number ;
  int ierr=0,rep;
  static char *init ="*.scg";
  rep=GetFileWindow(init,&filename,".",0,&ierr,"Save Graphic File");
  if ( ierr == 0 && rep == TRUE )
    {
      C2F(xsaveplots)(&win_num,filename,0L);
    }
}

/******************************************************
 * Binary File load 
 ******************************************************/

static void
Load(w, number, client_data)
     Widget w;
     XtPointer number;
     XtPointer client_data;
{
  char *filename;
  integer win_num = (integer) number ;
  int ierr=0,rep;
  static char *init ="*.scg";
  rep=GetFileWindow(init,&filename,".",0,&ierr,"Load Graphic File");
  if ( ierr == 0 && rep == TRUE )
    {
      integer verb=0,cur,na;
      C2F(dr)("xget","window",&verb,&cur,&na,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
      C2F(dr)("xset","window",&win_num,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      C2F(xloadplots)(filename,0L);
      C2F(dr)("xset","window",&cur,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    }
}

/*
 * Inhinit zoom,unoom,rot3d 
 */

/*******************************************************
 * 2D Zoom calback 
 ******************************************************/

static void
Zoom(w, number, client_data)
     Widget w;
     XtPointer number;
     XtPointer client_data;
{

  integer win_num = (integer) number,ne=0;
  iargs = 0;
  XtSetArg(args[iargs], XtNsensitive, False); iargs++;
  XtSetValues(w, args, iargs);
  SetUnsetMenu(&win_num,"3D Rot.",&ne,False);
  SetUnsetMenu(&win_num,"UnZoom",&ne,False);
  SetUnsetMenu(&win_num,"File",&ne,False);
  if(scig_2dzoom(win_num) != 1){ /* ==1 <=> zoom failed (the window has been closed while zooming ) */
    iargs = 0;
    XtSetArg(args[iargs], XtNsensitive,True); iargs++;
    XtSetValues(w, args, iargs);
  }
  SetUnsetMenu(&win_num,"3D Rot.",&ne,True);
  SetUnsetMenu(&win_num,"UnZoom",&ne,True);
  SetUnsetMenu(&win_num,"File",&ne,True);
}


/*******************************************************
 * Unzoom Callback 
 ******************************************************/

static void
UnZoom(w, number, client_data)
     Widget w;
     XtPointer number;
     XtPointer client_data;
{
  integer win_num = (integer) number;
  iargs = 0;
  XtSetArg(args[iargs], XtNsensitive, False); iargs++;
  XtSetValues(w, args, iargs);
  scig_unzoom(win_num);
  iargs = 0;
  XtSetArg(args[iargs], XtNsensitive,True); iargs++;
  XtSetValues(w, args, iargs);
}


/*******************************************************
 * 3D Rotation callback 
 ******************************************************/


static void
Rot3D(w, number, client_data)
     Widget w;
     XtPointer number;
     XtPointer client_data;
{
  integer win_num = (integer) number,ne=0;
  iargs=0;
  XtSetArg(args[iargs], XtNsensitive, False); iargs++;
  XtSetValues(w, args, iargs);
  SetUnsetMenu(&win_num,"UnZoom",&ne,False);
  SetUnsetMenu(&win_num,"Zoom",&ne,False);
  SetUnsetMenu(&win_num,"File",&ne,False);
  scig_3drot(win_num);
  iargs = 0;
  XtSetArg(args[iargs], XtNsensitive,True); iargs++;
  XtSetValues(w, args, iargs);
  SetUnsetMenu(&win_num,"UnZoom",&ne,True);
  SetUnsetMenu(&win_num,"Zoom",&ne,True);
  SetUnsetMenu(&win_num,"File",&ne,True);
}


/*	Function Name: CreatePopupWindow
 *	Description: Creates and pops up the New Graphic Window
 *      Returns the graphic window id (CWindow) and the window of the top level widget (SciGWindow)
 *      and the info widget associated with the graphic window 
 */

/*
 * DeleteWindow(): Action proc to implement ICCCM delete_window.
 */


/** returns the number of the deleted window or -1 if no deleted windows **/

void SGDeleteWindow(w, event, params, num__PARAMS)
     Widget w;
     XEvent *event;
     String *params;
     Cardinal *num__PARAMS;
{
  int i;
  Window Win = XtWindow(w), Win1;
  int wincount =  GetWinsMaxId()+1;
  for (i=0 ; i < wincount ; i++) 
    {
      Win1=GetBGWindowNumber(i);
      if (Win1 == Win ) 
	{
	  /** sciprint("Le window manager veut tuer une fenetre graphique %d\n",i); **/
	  Delete((Widget) 0,(XtPointer) i,(XtPointer) 0);
	  return ;
	}
    }
  return ;
}


static String sg_trans =
"<Message>WM_PROTOCOLS: SGDeleteWindow()\n\
     <ClientMessage>WM_PROTOCOLS: SGDeleteWindow()\n";

void CreatePopupWindow(WinNum, button, ScilabXgc, fg, bg)
     integer WinNum;
     Widget button;
     struct BCG *ScilabXgc;
     Pixel *fg;
     Pixel *bg;
{
  Widget toplevel;
  Display *dpy;
  XColor x_fg_color,x_bg_color;
  static XSetWindowAttributes attributes;
  unsigned char winname[6];
  iargs=0;
  XtSetArg(args[iargs], XtNx, 100+20*WinNum % 200);	iargs++;
  XtSetArg(args[iargs], XtNy, 100+20*WinNum % 200);	iargs++;
  sprintf(popupname,"ScilabGraphic%d",(int)WinNum);
  XtSetArg(args[iargs],XtNtitle,popupname);iargs++;
  
  DisplayInit("",&dpy,&toplevel);
  MultiClickTime=XtGetMultiClickTime(dpy);

  /* 
  ScilabXgc->popup = XtCreatePopupShell(popupname,
			     topLevelShellWidgetClass, button, args, iargs); 
			     */
  ScilabXgc->popup = XtAppCreateShell("Xscilab",popupname,topLevelShellWidgetClass,dpy,
				      args,iargs);
  AddNewWin((int) WinNum,ScilabXgc);
  XtPopup(ScilabXgc->popup, XtGrabNone);
  SetHints(ScilabXgc->popup);
  
  sprintf((char *)winname,"BG%d",(int)WinNum);
  XChangeProperty(XtDisplay(ScilabXgc->drawbox),
		  XtWindow(ScilabXgc->drawbox), XA_WM_NAME, XA_STRING, 8, 
		  PropModeReplace, winname, 5);
  attributes.backing_store = Always;
  attributes.bit_gravity = NorthWestGravity;
  XChangeWindowAttributes(XtDisplay((ScilabXgc->drawbox)), 
			  XtWindow((ScilabXgc->drawbox)),
			  CWBackingStore | CWBitGravity,&attributes);
  XSync(XtDisplay(ScilabXgc->popup),0);
  ScilabXgc->CWindow=XtWindow(ScilabXgc->drawbox);
  ScilabXgc->CBGWindow=XtWindow(ScilabXgc->popup);
  XtOverrideTranslations(ScilabXgc->popup, XtParseTranslationTable(sg_trans));
  /*  Getting the values of foreground and background */
  XtSetArg(args[0],XtNforeground, &x_fg_color.pixel);
  XtSetArg(args[1],XtNbackground, &x_bg_color.pixel);
  XtGetValues(ScilabXgc->drawbox,args,2);
  *fg = x_fg_color.pixel;
  *bg = x_bg_color.pixel;
}


/*******************************************************
 * recursively changes the background and foreground 
 * of widgets 
 ******************************************************/

void ChangeBandF(win_num,fg,bg)
     int win_num;
     Pixel fg;
     Pixel bg;
{
  struct BCG *SciGc;
  Widget popup,toplevel;
  static Display *dpy = (Display *) NULL;
  DisplayInit("",&dpy,&toplevel);
  SciGc = GetWindowXgcNumber(win_num);
  if ( SciGc != NULL ) 
    {
      popup = SciGc->popup;
      if (popup == NULL) return;
      ChangeBF1(popup,"*.scigForm.scigmForm.File",fg,bg);
      ChangeBF1(popup,"*.scigForm.scigmForm.File.menu",fg,bg);
      ChangeBF1(popup,"*.scigForm.scigmForm.File.menu.Clear",fg,bg);
      ChangeBF1(popup,"*.scigForm.scigmForm.File.menu.Select",fg,bg);
      ChangeBF1(popup,"*.scigForm.scigmForm.File.menu.Print",fg,bg);
      ChangeBF1(popup,"*.scigForm.scigmForm.File.menu.Export",fg,bg);
      ChangeBF1(popup,"*.scigForm.scigmForm.File.menu.Save",fg,bg);
      ChangeBF1(popup,"*.scigForm.scigmForm.File.menu.Load",fg,bg);
      ChangeBF1(popup,"*.scigForm.scigmForm.File.menu.Close",fg,bg);
      ChangeBF1(popup,"*.scigForm.scigmForm.Zoom",fg,bg);
      ChangeBF1(popup,"*.scigForm.scigmForm.UnZoom",fg,bg);
      ChangeBF1(popup,"*.scigForm.scigmForm.Rot3D",fg,bg);
    }
}

static void ChangeBF1(w,str,fg,bg)
     Widget w;
     char *str;
     Pixel fg;
     Pixel bg;
{
  Widget loc;
  loc = XtNameToWidget(w,str);
  if ( loc != NULL)  
    {
      iargs=0;
      XtSetArg(args[iargs], XtNforeground, fg); iargs++;
      XtSetArg(args[iargs], XtNbackground, bg); iargs++;
      XtSetValues(loc, args, iargs);
    }
  else
    {
      sciprint("widget %s not found\r\n",str);
    }
}

/*--------------------------------------------------------------
 *  Add dynamically buttons and menus in The Scilab Graphic Window
 * or in the Scilab main window 
 ----------------------------------------------------------------*/

typedef struct {
  int    win_num;      /* graphic window number or -1 for main window */
  int    entry;        /* clicked sub_menu number */
  int    type;         /* interpreded action (0), hard coded action(1) */
  char   *fname;       /* name of the action function  */
} MenuData,*MenuDataPtr;

extern Widget commandWindow; /* Scilab main window */

static void
KillButton(w, client_data, call_data)
     Widget w;
     XtPointer client_data;
     caddr_t call_data;
{
  MenuDataPtr datas =(MenuDataPtr)client_data;
  /** Warning fname is shared so we only free fname once **/
  if ( datas->entry == 0)
    FREE(datas->fname);
  FREE(datas);
}
/****************************************************
 * callBack associated to a user defined menu 
 ****************************************************/

static void
SelMenu(w, client_data, call_data)
     Widget w;
     XtPointer client_data;
     caddr_t call_data;
{
  static char buf[256];
  MenuDataPtr datas = (MenuDataPtr) client_data;
  if (datas->type == 0) 
    { 
      /* Interpreted mode : we store the action on a queue */
      if ( datas->win_num < 0 ) 
	{
	  sprintf(buf,"execstr(%s(%d))",datas->fname,datas->entry+1);
	}
      else 
	{
	  sprintf(buf,"execstr(%s_%d(%d))",datas->fname,datas->win_num,datas->entry+1);
	}
      StoreCommand(buf);
    }
  else if (datas->type == 2) 
    { 
      /* Interpreted mode : we store the action on a queue */
      if ( datas->win_num < 0 ) 
	{
	  sprintf(buf,"%s(%d)",datas->fname,datas->entry+1);
	}
      else 
	{
	  sprintf(buf,"%s(%d,%d)",datas->fname,datas->entry+1,datas->win_num);
	}
      StoreCommand(buf);
    }
  else
    { 
      /* hard coded mode */
      int rep ;
      C2F(setfbutn)(datas->fname,&rep);
      if ( rep == 0) 
	F2C(fbutn)((datas->fname),&(datas->win_num),&(datas->entry));
    }
}


/****************************************************
 *Delete the button named button_name in window 
 * number win_num
 ****************************************************/

int C2F(delbtn)(win_num,button_name)
     integer *win_num;
     char *button_name;
{  
  Cardinal nc=0;
  WidgetList childs;
  Widget outer,h,v,w;
  int i,pos=0;
  int name_pos ;

  if (GetChilds(*win_num,&nc,&childs,&outer,button_name,&name_pos)== FALSE)
    return 0;
  if (name_pos  == -1 ) return 0; /* button_name not found */
  /* position in widget list */
  pos = name_pos +1;
  if ( pos==(int)nc ) 
    {
      /* button_name is the  last button : simply destroy it */
      XtDestroyWidget(childs[name_pos]);
      return 0;
    }
  iargs=0;
  XtSetArg(args[iargs], XtNfromHoriz,&h);    iargs++;
  XtSetArg(args[iargs], XtNfromVert,&v);     iargs++;
  XtGetValues(childs[pos-1], args, iargs);
  /* reconstruct next widget layout */
  iargs=0;
  XtSetArg(args[iargs], XtNfromHoriz,h);    iargs++;
  XtSetArg(args[iargs], XtNtop,XawChainTop); iargs++;
  XtSetArg(args[iargs], XtNbottom,XawChainTop); iargs++;
  XtSetValues(childs[pos], args, iargs);

  if (v!=NULL) {
    iargs=0;
    XtSetArg(args[iargs], XtNfromVert,v);    iargs++;
    XtSetArg(args[iargs], XtNtop,XawChainTop); iargs++;
    XtSetArg(args[iargs], XtNbottom,XawChainTop); iargs++;
    XtSetValues(childs[pos], args, iargs);
    for (i=pos+1;i<(int)nc;i++) {
      iargs=0;
      XtSetArg(args[iargs], XtNfromVert,&w);     iargs++;
      XtGetValues(childs[i], args, iargs);
      if (w== childs[pos-1]) {
	iargs=0;
	XtSetArg(args[iargs], XtNfromVert,v);     iargs++;
	XtSetArg(args[iargs], XtNtop,XawChainTop); iargs++;
	XtSetArg(args[iargs], XtNbottom,XawChainTop); iargs++;
	XtSetValues(childs[i], args, iargs);
      }
    }
  }
  /* destroy widget */
  if ( childs[pos-1] != NULL)
    XtDestroyWidget(childs[pos-1]);
  return(0);
}

/****************************************************
 * Add a menu in  window  number wun_num or in Main window
 *  win_num     : graphic window number or -1 for main scilab window
 *  button_name : label of button
 *  entries     : labels of submenus if any
 *  ne          : number of submenus
 *  typ         : Action mode
 *                typ==0 : interpreted (execution of scilab instruction
 *  typ!=0 : hard coded a routine is called
 *  fname;      : name of the action function  
 ****************************************************/

void AddMenu(win_num, button_name, entries, ne, typ, fname, ierr)
     integer *win_num;
     char *button_name;
     char **entries;
     integer *ne;
     integer *typ;
     char *fname;
     integer *ierr;
{  
  int newline,i;
  WidgetList childs;
  Widget outer,command,w,menu,entry;
  Position x;
  Dimension width,height,mainwidth,mainheight,bw=0;
  XFontStruct     *temp_font;
  char *func;
  MenuDataPtr datas;
  int nc, name_pos;
  if (GetChilds(*win_num,&nc,&childs,&outer,NULL,&name_pos)== FALSE) return;
  if ( nc < 2 ) return ; /** childs[0] is the panner and childs[1] is scigraphicinfo **/
  iargs=0;
  XtSetArg(args[iargs], XtNx,&x);             iargs++;
  XtSetArg(args[iargs], XtNwidth,&width);     iargs++;
  XtSetArg(args[iargs], XtNheight,&height);     iargs++;
  XtSetArg(args[iargs], XtNfont, &temp_font);  iargs++;
  XtGetValues(childs[nc-1], args, iargs);
  iargs=0;
  XtSetArg(args[iargs], XtNwidth,&mainwidth);       iargs++;
  XtSetArg(args[iargs], XtNheight,&mainheight);       iargs++;
  XtGetValues(outer, args, iargs);

  bw= ( temp_font != NULL )? strlen(button_name)*((temp_font)->max_bounds.width) : 0;
  if ( (Dimension) (width+bw+x)  < mainwidth)
    {
      /** we can add the new button on the current line **/
      newline=0;
      iargs=0;
      XtSetArg(args[iargs], XtNfromVert,&w);  iargs++;
      XtGetValues(childs[nc-1], args, iargs);
      iargs=0;
      XtSetArg(args[iargs], XtNfromHoriz,childs[nc-1]);  iargs++;
      XtSetArg(args[iargs], XtNfromVert,w);  iargs++;
    }
  else
    {
      /** we must fill a new line **/
      newline=1;
      iargs=0;
      XtSetArg(args[iargs], XtNfromVert,childs[nc-1]);  iargs++;
      XtSetArg(args[iargs], XtNfromHoriz,childs[0]);  iargs++;
    }

  func=(char *) MALLOC( (strlen(fname)+1)*(sizeof(char)));
  if ( func == ( char *) 0 ) 
    {
      *ierr=1 ;return;
    }
  strcpy(func,fname);

  XtSetArg(args[iargs], XtNheight,height);     iargs++;
  if (*ne==0) {
    command = XtCreateManagedWidget(button_name,
				    commandWidgetClass,outer,args, iargs);
    datas= (MenuDataPtr) MALLOC(sizeof(MenuData) );
    if ( datas == (MenuDataPtr) 0) 
      {
	*ierr=1 ;return;
      }
    datas->win_num= *win_num;
    datas->entry=0;
    datas->type=*typ;
    datas->fname=func;
    XtAddCallback(command, XtNcallback,(XtCallbackProc) SelMenu,
		  (XtPointer)datas);
    XtAddCallback(command, XtNdestroyCallback,(XtCallbackProc) KillButton,
		  (XtPointer)datas);
  }
  else {
    command = XtCreateManagedWidget(button_name,
				    menuButtonWidgetClass,outer,args, iargs);
    iargs=0;
    menu = XtCreatePopupShell("menu", simpleMenuWidgetClass, 
			      command,args, iargs);
    for (i=0;i<*ne;i++){
      iargs=0;
      entry= XtCreateManagedWidget(entries[i], smeBSBObjectClass, menu,
				   args, iargs);
      datas= (MenuDataPtr) MALLOC(sizeof(MenuData) );
      if ( datas == (MenuDataPtr) 0) 
	{
	  *ierr=1 ;return;
	}
      datas->win_num = *win_num;
      datas->entry=i;
      datas->type=*typ;
      datas->fname=func;
      XtAddCallback(entry,XtNcallback,(XtCallbackProc)SelMenu, 
		    (XtPointer)datas);
      XtAddCallback(entry, XtNdestroyCallback,(XtCallbackProc) KillButton,
		    (XtPointer)datas);
    }
  }
  if (newline==1) {
    /* here we should increase the height of scigmForm */
  }
}

/****************************************************
 * Scilab interface for the AddMenu function 
 *  Add a menu in  window  number win_num or in Main window
 *
 *  win_num     : graphic window number or -1 for main scilab window
 *  button_name : label of button
 *  entries     : labels of submenus if any (in scilab code)
 *  ptrentries  : table of pointers on each entries
 *  ne          : number of submenus
 *  typ         : Action mode
 *                typ==0 : interpreted (execution of scilab instruction
 *                typ!=0 : hard coded a routine is called
 *  fname;      : name of the action function  
 *******************************************************/

int C2F(addmen)(win_num,button_name,entries,ptrentries,ne,typ,fname,ierr)
     integer *win_num,*entries,*ptrentries,*ne,*ierr,*typ;
     char *button_name,*fname;
{
  char ** menu_entries = NULL ;
  int i ;
  *ierr =0;
  if (*ne!=0) {
    ScilabMStr2CM(entries,ne,ptrentries,&menu_entries,ierr);
    if ( *ierr == 1) return(0);
  }
  AddMenu(win_num,button_name,menu_entries,ne,typ,fname,ierr);
  /* ScilabMStr2CM allocate menu_entries */
  for ( i = 0 ; i  < *ne ; i++ )
  {
    FREE( menu_entries[i] ) ;
  }
  FREE( menu_entries ) ;
  return(0);
}

/***************************************************
 * Activate or deactivate a menu 
 ***************************************************/

static void SetUnsetMenu(win_num, button_name, ne,flag)
     integer *win_num;
     char *button_name;
     integer *ne;
     int flag;
{  
  Cardinal nc=0;
  WidgetList childs;
  Widget outer;
  int nc1,name_pos;
  if (GetChilds(*win_num,&nc1,&childs,&outer,button_name,&name_pos)== FALSE) return;
  if (name_pos  == -1 ) return; /* button_name not found */
  if (*ne==0) 
    {
      /** no submenu specified **/
      iargs=0;
      XtSetArg(args[iargs],XtNsensitive,flag); iargs++;
      XtSetValues(childs[name_pos], args, iargs );
    }
  else 
    {
      Widget w1;
      String mname=NULL;
      /* a submenu is specified by its position 
       * in the menu item list  (*ne) 
       */
      iargs=0;
      XtSetArg(args[iargs], XtNmenuName, &mname);    iargs++;
      XtGetValues(childs[name_pos], args, iargs); 
      w1= (mname == NULL) ? NULL : XtNameToWidget(childs[name_pos],mname);
      if (w1==NULL) {
	iargs=0;
	XtSetArg(args[iargs],XtNsensitive,flag); iargs++;
	XtSetValues(childs[name_pos], args, iargs );
	return;
      }
      iargs=0;
      XtSetArg(args[iargs], XtNnumChildren, &nc);    iargs++;
      XtSetArg(args[iargs], XtNchildren, &childs);   iargs++;
      XtGetValues(w1, args, iargs); 
      if (*ne <= (int)nc) {
	iargs=0;
	XtSetArg(args[iargs],XtNsensitive,flag); iargs++;
	XtSetValues(childs[*ne-1], args, iargs ); 
      }
    }
}


/** activate a menu (scilab interface) **/

int C2F(setmen)(win_num,button_name,entries,ptrentries,ne,ierr)
     integer *win_num,*entries,*ptrentries,*ne,*ierr;
     char *button_name;
{
  SetUnsetMenu(win_num,button_name,ne,True);
  return(0);
}

int C2F(unsmen)(win_num,button_name,entries,ptrentries,ne,ierr)
     integer *win_num,*entries,*ptrentries,*ne,*ierr;
     char *button_name;
{
  SetUnsetMenu(win_num,button_name,ne,False);
  return(0);
}


/************************************
 * Utility function : 
 * find the child list of graphic window win_num 
 * or scilab main window if win_num == -1 
 * then if name is non nul name_pos is set to 
 * the position in wL of the child which has label <<name>>
 * or to  -1 in case of error  
 * outer : the parent of the widgets contained in wL
 ************************************/

static int GetChilds(win_num,nc,wL,outer,name,name_pos) 
  int win_num; /* id of the window */
  int *nc; /* number of children */
  WidgetList *wL;
  Widget *outer;
  char *name; /* name of the menu */
  int * name_pos; /* id of the menu */
{ 
  Cardinal nc1=0;
  Widget popup,toplevel;
  char * label;
  static Display *dpy = (Display *) NULL;
  DisplayInit("",&dpy,&toplevel);
  if ( win_num == -1) 
    {
      *outer = commandWindow;
    }
  else 
    {
      struct BCG *SciGc;
      SciGc = GetWindowXgcNumber(win_num);
      if ( SciGc ==  NULL ) return FALSE;
      if ((popup = SciGc->popup)== NULL) return FALSE;
      *outer=XtNameToWidget(popup,"*.scigForm.scigmForm");
      if (*outer == NULL) return FALSE;
    }
  iargs=0;
  XtSetArg(args[iargs], XtNnumChildren, &nc1);    iargs++;
  XtSetArg(args[iargs], XtNchildren, wL);   iargs++;
  XtGetValues(*outer, args, iargs);
  *nc=nc1;
  *name_pos = -1;
  if ( name != NULL) 
    {
      int i;
      for (i=0; i < (int)nc1;i++) 
	{
	  iargs=0;
	  label=NULL;
	  XtSetArg(args[iargs], XtNlabel,&label);   iargs++;
	  XtGetValues( (*wL)[i], args, iargs);
	  if ( label != NULL && (strcmp(label,name)==0))
	    {
	      *name_pos = i;
	      break;
	    }
	}
    }
  return TRUE;
}

/*------------------------------------------------------------------------------------*/
/* get the number of submenus of a given menu                                         */
/*------------------------------------------------------------------------------------*/
int getNbSubMenus( int winNumber, char * menuName )
{
  int        nbMenus        ;
  WidgetList subMenus       ;
  Widget     menu           ;
  Widget     subMenu        ;
  int        menuId         ;
  int        nbSubMenus = 0 ;
  String     mname      = NULL ;

  if ( GetChilds( winNumber ,
                  &nbMenus   ,
                  &subMenus ,
                  &menu     ,
                  menuName  ,
                  &menuId    )  == FALSE )
  { 
    return 0 ;
  }
  /* a submenu is specified by its position 
   * in the menu item list  (*ne) 
   */
  iargs=0;
  XtSetArg(args[iargs], XtNmenuName, &mname);
  iargs++;
  XtGetValues(subMenus[menuId], args, iargs);
  
  if ( mname == NULL )
  {
    return 0 ;
  }
  
  subMenu = XtNameToWidget(subMenus[menuId],mname) ;
  iargs = 0 ;
  XtSetArg(args[iargs], XtNnumChildren, &nbSubMenus);
  iargs++;
  XtGetValues( subMenu , args, iargs);
  
  return nbSubMenus ;
}

/*------------------------------------------------------------------------------------*/
/* put some menus in gray under old graphic style                                     */
/* or put them back under new graphic style                                           */
void refreshMenus( struct BCG * ScilabGC )
{

  /* put some menu in grey in old graphic style */
  if ( version_flag() == 0 )
  {
    /* new graphic style */
    int        subMenuNumber = 0 ;
    int        nbChildren        ;
    
    /*SetUnsetMenu( &ScilabGC->CurWindow, "Insert", &subMenuNumber, True ) ; */
    
    /* get the number of children of the Edit menu */
    nbChildren = getNbSubMenus( ScilabGC->CurWindow, "Edit" ) ;
    for ( subMenuNumber = 5 ; subMenuNumber <= nbChildren ; subMenuNumber++ )
    {
      SetUnsetMenu( &ScilabGC->CurWindow, "Edit", &subMenuNumber, True ) ;
    }
  }
  else
  {
    /* old graphic style */
    int        subMenuNumber = 0 ;
    int        nbChildren        ;
    
    /*SetUnsetMenu( &ScilabGC->CurWindow, "Insert", &subMenuNumber, False ) ; */

    nbChildren = getNbSubMenus( ScilabGC->CurWindow, "Edit" ) ;
    for ( subMenuNumber = 5 ; subMenuNumber <= nbChildren ; subMenuNumber++ )
    {
      SetUnsetMenu( &ScilabGC->CurWindow, "Edit", &subMenuNumber, False ) ;
    }
  }
    
}
/*------------------------------------------------------------------------------------*/
