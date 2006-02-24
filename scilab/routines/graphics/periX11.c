/*------------------------------------------------------------------------
 *    Graphic library
 *    Copyright (C) 1998-2001 Enpc/Jean-Philippe Chancelier
 *    jpc@cermics.enpc.fr 
 --------------------------------------------------------------------------*/
/*------------------------------------------------------------------------
 *    X11 Driver 
 *--------------------------------------------------------------------------*/
#if defined sgi && ! defined __STDC__     
#define __STDC__
#endif

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/time.h> 

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>

#include "../sci_mem_alloc.h"
#include "Math.h"
#include "periX11.h"
#include "../version.h"
#include "color.h"
#include "bcg.h"
#include "Events.h"
#ifdef WITH_TK
#include <tcl.h>
#include <tk.h>

extern Tcl_Interp *TCLinterp;
extern Tk_Window TKmainWindow;
#endif

double t=0.; /*for xclick_any */
extern void AddMenu  __PARAMS((integer *win_num, char *button_name, char **entries, integer *ne, integer *typ, char *fname, integer *ierr));
extern int XRotDrawString();
/** jpc_SGraph.c **/
extern void ChangeBandF __PARAMS((int win_num,Pixel fg, Pixel bg));

void CreatePopupWindow  __PARAMS((integer WinNum,Widget top,struct BCG *, Pixel *fg,Pixel *bg));
extern void GViewportResize __PARAMS((struct BCG *ScilabXgc, int *width,int *height));

/** jpc_Xloop.c **/
extern integer C2F(ismenu) __PARAMS((void));
extern int C2F(getmen) __PARAMS((char *btn_cmd,integer *lb,integer *entry));
extern  integer C2F(sxevents)();
extern void DisplayInit __PARAMS((char *string,Display **dpy,Widget *toplevel));
extern void MenuFixCurrentWin __PARAMS(( int ivalue));

extern void GPopupResize __PARAMS((struct BCG *ScilabXgc,int *,int *));
extern int IswmDeleteWindow __PARAMS((XEvent *));
extern int IsCloseSGWindow __PARAMS((XEvent *));
extern void SciViewportMove __PARAMS((struct BCG *,int,int));
extern void SciViewportGet __PARAMS((struct BCG *,int *,int*));
extern void SciViewportClipGetSize __PARAMS((struct BCG *,int *,int*));
#ifdef WITH_TK
extern void flushTKEvents();
#endif
extern int versionflag;
extern int version_flag() ;
extern void refreshMenus( struct BCG * ScilabGC ) ;


#define MESSAGE4 "Can't allocate point vector"
#define MESSAGE5 "Can't re-allocate point vector" 
#define Char2Int(x)   ( x & 0x000000ff )

void C2F(DispStringAngle)(integer *x0, integer *yy0, char *string, double *angle);
void C2F(MissileGCGetorSet)(char *str, integer flag, integer *verbose, integer *x1, integer *x2, integer *x3, integer *x4, integer *x5, integer *x6, double *dv1);
void C2F(WriteGeneric1X11)();
void C2F(xgetmarkX11)();
void C2F(xsetmarkX11)();
void C2F(xgetfontX11)();
void C2F(xsetfontX11)();
void C2F(setdashstyleX11)();
void C2F(Write2VectX11)();
void C2F(WriteGenericX11)(char *string, integer nobj, integer sizeobj, integer *vx, integer *vy, integer sizev, integer flag, integer *fvect);
void C2F(InitScilabGCX11)(integer *v1, integer *v2, integer *v3, integer *v4);
void C2F(setforegroundX11)(integer *num, integer *v2, integer *v3, integer *v4);
void C2F(ScilabGCGetorSetX11)(char *str, integer flag, integer *verbose, integer *x1, integer *x2, integer *x3, integer *x4, integer *x5, integer *x6, double *dx1);
void C2F(setbackgroundX11)(integer *num, integer *v2, integer *v3, integer *v4);
void C2F(set_cX11)(integer i);
void C2F(idfromnameX11) (char *name1, integer *num);
void C2F(getdashX11)(integer *verbose, integer *value, integer *narg, double *dummy);

BOOL IsTKGraphicalMode(void);

/** Global variables to deal with X11 **/

static GC gc;
static Cursor arrowcursor,normalcursor,crosscursor;
static Window root=(Window) NULL;
static Display *dpy = (Display *) NULL;
static Pixel DefaultBackground, DefaultForeground;
static double *vdouble = 0; /* used when a double argument is needed */
static int depth;
static unsigned long maxcol;
Visual *visual;
static int wpixel, bpixel;

/* These DEFAULTNUMCOLORS colors come from Xfig */

unsigned short default_colors[] = {
  0,   0,   0, /* Black: DEFAULTBLACK */
  0,   0, 255, /* Blue */
  0, 255,   0, /* Green */
  0, 255, 255, /* Cyan */
  255,   0,   0, /* Red */
  255,   0, 255, /* Magenta */
  255,   255,   0, /* Yellow */
  255, 255, 255, /* White: DEFAULTWHITE */
  0,   0, 144, /* Blue4 */
  0,   0, 176, /* Blue3 */
  0,   0, 208, /* Blue2 */
  135, 206, 255, /* LtBlue */
  0, 144,   0, /* Green4 */
  0, 176,   0, /* Green3 */
  0, 208,   0, /* Green2 */
  0, 144, 144, /* Cyan4 */
  0, 176, 176, /* Cyan3 */
  0, 208, 208, /* Cyan2 */
  144,   0,   0, /* Red4 */
  176,   0,   0, /* Red3 */
  208,   0,   0, /* Red2 */
  144,   0, 144, /* Magenta4 */
  176,   0, 176, /* Magenta3 */
  208,   0, 208, /* Magenta2 */
  128,  48,   0, /* Brown4 */
  160,  64,   0, /* Brown3 */
  192,  96,   0, /* Brown2 */
  255, 128, 128, /* Pink4 */
  255, 160, 160, /* Pink3 */
  255, 192, 192, /* Pink2 */
  255, 224, 224, /* Pink */
  255, 215,   0  /* Gold */
};

/*
 * structure for Window List 
 */

typedef  struct  
{
  struct BCG winxgc;
  struct WindowList *next;
} WindowList  ;

static WindowList *The_List  = (WindowList *) NULL;
struct BCG *ScilabXgc = (struct BCG *) 0;

/** functions **/

Window Find_X_Scilab __PARAMS((void));
Window Find_BG_Window __PARAMS((int i));
Window Find_ScilabGraphic_Window __PARAMS((int i));
Window GetWindowNumber  __PARAMS((int));
Window GetBGWindowNumber __PARAMS((int));

struct BCG *GetWindowXgcNumber __PARAMS((int));
struct BCG *AddNewWindowToList __PARAMS((void));

static void ResetScilabXgc __PARAMS((void));
static XPoint *get_xpoints __PARAMS((void));
static int ReallocVector  __PARAMS((int));
static void DrawMark(integer *x, integer *y),LoadFonts(void), LoadSymbFonts(void);
/* static void C2F(analyze_points)(integer n, integer *vx, integer *vy, integer onemore); */
/* static void My2draw(integer j, integer *vx, integer *vy) ; */
/* static void MyDraw(integer iib, integer iif, integer *vx, integer *vy) ; */
/* static void change_points(integer i, integer x, integer y); */
static void C2F(loadfamily_n)(char *name, integer *j);
static void PixmapClear   __PARAMS((struct BCG *Xgc,int x,int y,int w,int h));
static void xset_colormap __PARAMS((integer *v1,integer *v2,integer *v3,integer *v4,integer *v5,integer *v6,double *a));
static void xset_dashstyle __PARAMS((integer *v1,integer *v2,integer *v3));

/* declaration for xset('name',...) functions */
typedef void (xset_f) __PARAMS((integer * v1,integer * v2,integer * v3,integer * v4));
static xset_f xset_windowpos,xset_windowdim,xset_popupdim,xset_viewport,xset_curwin,xset_clip;
static xset_f xset_absourel,xset_alufunction1,xset_thickness,xset_pattern,xset_dash;
static xset_f xset_pixmapOn,xset_wresize,xset_background,xset_foreground,xset_hidden3d; 
static xset_f xset_unclip,xset_font,xset_usecolor,xset_mark,xset_pixmapclear,xset_show,xset_dash_or_color;
static xset_f xset_scilabxgc,xset_scilabVersion;/* NG */
static void xset_scilabFigure(integer *v1,integer *v2,integer *v3,integer *v4,integer *v5,integer *v6,double *figure);
static void xset_alufunction2(struct BCG *Xgc,integer *num, integer *v2, integer *v3, integer *v4);

/* declaration for xget('name',...) functions */
typedef void (xget_f) __PARAMS((integer *verbose, integer *x,integer *narg, double *dummy));
static xget_f xget_windowpos,xget_windowdim,xget_popupdim,xget_viewport,xget_curwin ,xget_clip;
static xget_f xget_absourel,xget_alufunction,xget_thickness,xget_pattern,xget_last,xget_dash;
static xget_f xget_usecolor,xget_pixmapOn,xget_wresize,xget_colormap,xget_background,xget_foreground,xget_hidden3d;
static xget_f xget_font,xget_mark,xget_dash_or_color;
static xget_f xget_scilabxgc,xget_scilabFigure,xget_scilabVersion;/* NG */
static xget_f xget_colormap_size;


#define FONTNUMBER 11 
#define FONTMAXSIZE 6
#define SYMBOLNUMBER 10

/* FontsList : storing font informations
 *             the font i with size fsiz is stored at 
 *             FontsList_[i][fsiz]->fid
 */

static XFontStruct *FontsList_[FONTNUMBER][FONTMAXSIZE];

/* Allocating colors in BCG struct */

int XgcAllocColors(struct BCG *xgc, int m)
{
  int mm;
  /* don't forget black and white */
  mm = m + 2;
  if (!(xgc->Red = (float *) MALLOC(mm*sizeof(float)))) {
    Scistring("XgcAllocColors: unable to alloc\n");
    return 0;
  }
  if (!(xgc->Green = (float *) MALLOC(mm*sizeof(float)))) {
    Scistring("XgcAllocColors: unable to alloc\n");
    FREE(xgc->Red);
    return 0;
  }
  if (!(xgc->Blue = (float *) MALLOC(mm*sizeof(float)))) {
    Scistring("XgcAllocColors: unable to alloc\n");
    FREE(xgc->Red);
    FREE(xgc->Green);
    return 0;
  }
  if (!(xgc->Colors = (Pixel *) MALLOC(mm*sizeof(Pixel)))) {
    Scistring("XgcAllocColors: unable to alloc\n");
    FREE(xgc->Red);
    FREE(xgc->Green);
    FREE(xgc->Blue);
    return 0;
  }
  return 1;
}

void XgcFreeColors(struct BCG *xgc)
{
  FREE(xgc->Red); xgc->Red = (float *) 0;
  FREE(xgc->Green);xgc->Green = (float  *) 0;
  FREE(xgc->Blue); xgc->Blue = (float *) 0;
  FREE(xgc->Colors); xgc->Colors = (Pixel *) 0;
}

/** Pixmap routines **/

static void xset_pixmapclear(integer *v1, integer *v2, integer *v3, integer *v4)
{
  XWindowAttributes war;
  /** Un clip zone (rectangle ) **/
  XSetClipMask(dpy,gc,None);
  XGetWindowAttributes(dpy,ScilabXgc->CWindow,&war); 
  PixmapClear(ScilabXgc,0,0,war.width,war.height);
  /** Restore the  clip zone (rectangle ) **/
  if ( ScilabXgc->ClipRegionSet == 1) 
    {
      XRectangle rects[1];
      rects[0].x = ScilabXgc->CurClipRegion[0];
      rects[0].y = ScilabXgc->CurClipRegion[1];
      rects[0].width = ScilabXgc->CurClipRegion[2];
      rects[0].height = ScilabXgc->CurClipRegion[3];
      XSetClipRectangles(dpy,gc,0,0,rects,1,Unsorted);
    }
}

int WithBackingStore()

{
  return (ScilabXgc->Cdrawable != (Drawable) 0) ;
}

static void xset_show(integer *v1, integer *v2, integer *v3, integer *v4)
{
  /*if (ScilabXgc->CurPixmapStatus == 0) return; */
  /* if CurPixmapStatus ==0 the pixmap is used as a backing store memory*/
  if (ScilabXgc->Cdrawable != (Drawable) 0) {
    XSetWindowBackgroundPixmap(dpy, ScilabXgc->CWindow, (Pixmap) ScilabXgc->Cdrawable);
    XClearWindow(dpy,ScilabXgc->CWindow);
  }
  XFlush(dpy);
}

/* 
 * Resize the pixmap associated to ScilabXgc->CWindow and store it back 
 * in ScilabXgc.
 */


void sci_pixmap_resize(struct BCG *Xgc,int x,int y)
{
  /*if (Xgc->CurPixmapStatus != 0) { use pixmap as backing store if CurPixmapStatus==0 */
  if (Xgc->Cdrawable != (Drawable) 0) 
    XFreePixmap(dpy,(Pixmap)Xgc->Cdrawable);
  Xgc->Cdrawable = (Drawable) XCreatePixmap(dpy, root,Max(x,400),Max(y,300),depth);
  if ( Xgc->Cdrawable == (Drawable) 0) 
    sciprint("Not enough memory  to allocate pixmap\r\n");
  else
    {
      PixmapClear(Xgc,0,0,x,y);
      XSetWindowBackgroundPixmap(dpy, Xgc->CWindow,(Pixmap) Xgc->Cdrawable);
    }
  /*}*/
}  

void CPixmapResize(int x, int y)
{
  sci_pixmap_resize(ScilabXgc,x,y);
}



/*
 * Pixmap clear : 
 */

static void PixmapClear(struct BCG *Xgc,int x, int y, int w, int h)
{
  /* switch to a clear gc */
  int cur_alu = Xgc->CurDrawFunction;
  int clear = GXclear;
  xset_alufunction2(Xgc,&clear,PI0,PI0,PI0);
  if (Xgc->Cdrawable != (Drawable) 0) XFillRectangle(dpy, Xgc->Cdrawable, gc, x,y,w,h);
  /* back to standard value */
  xset_alufunction2(Xgc,&cur_alu,PI0,PI0,PI0);
}

/* 
 * Resize the Pixmap according to window size change 
 * But only if there's a pixmap 
 */

void CPixmapResize1(void)
{
  XWindowAttributes war;
  if (ScilabXgc->Cdrawable != (Drawable) 0 ) 
    {
      XGetWindowAttributes(dpy,ScilabXgc->CWindow,&war); 
      CPixmapResize(war.width,war.height);
    }
}

/*-----------------------------------------------------
 * General routines callable from Scilab 
 -----------------------------------------------------*/

/* 
 * To select (raise on the screen )the current graphic Window
 * If there's no graphic window then select creates one 
 */

void C2F(xselgraphic)(char *v1, integer *v2, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{ 
  /** Test not really usefull: see sciwin in matdes.f **/
  integer ierr;
  if (ScilabXgc == (struct BCG *)0 || ScilabXgc->CBGWindow == (Window ) NULL) 
    C2F(initgraphic)("",PI0,&ierr,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
  XMapWindow(dpy,ScilabXgc->CBGWindow);
  XRaiseWindow(dpy,ScilabXgc->CBGWindow);
  XFlush(dpy);
}

/** End of graphic (do nothing)  **/

void C2F(xendgraphic)(void)
{
} 

void C2F(xend)(char *v1, integer *v2, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{
  /** Must destroy everything  **/
}

/** Clear the current graphic window     **/

void C2F(clearwindow)(char *v1, integer *v2, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{
  if (ScilabXgc->Cdrawable != (Drawable) 0 ) 
    xset_pixmapclear(PI0,PI0,PI0,PI0);
  XClearWindow(dpy, ScilabXgc->CWindow);
  XFlush(dpy);
}

/* generates a pause, in seconds */

#if defined(__STDC__) || defined(_IBMR2)
/** for usleep **/
#include <unistd.h> 
#endif 

void C2F(xpause)(char *str, integer *sec_time, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{ 
  unsigned useconds;
  if (*sec_time>0)
    {
      useconds=*sec_time;
      if (useconds != 0)  
#ifdef HAVE_USLEEP
	{ usleep(useconds); }
#else
#ifdef HAVE_SLEEP
      {  sleep(useconds/1000000); }
#else
      return;
#endif
#endif
    }
  else
    {
      Scierror(999,"xpause: error time must be >0.\r\n");
      return;
    }
}

/*************************************************************
 * Changes the popupname 
 *************************************************************/

void Setpopupname(char *string)
{ 
  int iargs = 0;
  static Arg args[2] ;
  if (ScilabXgc->popup !=  (Widget)0 ){
    XtSetArg(args[iargs],XtNtitle,string);iargs++;
    XtSetArg(args[iargs],XtNiconName,string);iargs++;
    XtSetValues(ScilabXgc->popup,args,iargs);
  }
}

/* appelle ds Xcall.c */

void C2F(setpopupname)(char *x0, integer *v2, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{
  Setpopupname(x0);
}

/*-----------------------------------------------------------------
 * Wait for mouse click in graphic window 
 *   send back mouse location  (x1,y1)  and button number  {0,1,2}
 *   and the window number 
 *-----------------------------------------------------------------*/

/** returns the graphic window number which owns the event **/

/* static int GraphicWinEvent(XEvent *event, int wincount, int flag) */
/* { */
/*   Window CW; */
/*   int i; */
/*   for (i=0;i < wincount;i++)  */
/*     { */
/*       CW = (flag == 0) ? GetWindowNumber(i) : GetBGWindowNumber(i); */
/*       if ( CW != (Window ) NULL  &&  event->xany.window == CW) */
/* 	return i; */
/*     } */
/*   return -1; */
/* } */

static int client_message=0;
static void set_client_message_on() { client_message=1;};
static void set_client_message_off() { client_message=0;};
int get_xclick_client_message_flag () { return client_message;}

extern void set_wait_click(int val); 
extern void set_event_select(int val); 

void C2F(xclick_any)(char *str, integer *ibutton, integer *x1, integer *yy1, integer *iwin, integer *iflag, integer *istr, double *dv1, double *dv2, double *dv3, double *dv4)
{
  Window CW;
  int buttons = 0;
  integer i,win,ok;
  integer wincount;
  integer lstr ;
  int motion,release;
  struct timeval delay; /* usec, to slow down event loop */

  wincount =  GetWinsMaxId()+1;
  if (wincount == 0) 
    {
      *x1=0;
      *yy1=0;
      *iwin=0;
      *ibutton = -100;
      *istr = 0;
      return;
    }
  for (i=0; i < wincount ; i++ ) 
    {
      if (( CW=GetWindowNumber(i)) != (Window ) NULL)
	XDefineCursor(dpy, CW ,crosscursor);
    }
  /** if we already have something on the queue **/
  if ( *iflag ==0 )  ClearClickQueue(-1);

  /* ignore the first event if it is a ClientMessage */ 
  set_client_message_on();
  set_wait_click(1); /*disable event handler if any */
  set_event_select(1+4);/*only record press and release events */
  while (buttons == 0) {
    /** first check if an event has been store in the queue while wait_for_click was 0 **/
    win=-1;
    ok=0;
    while (CheckClickQueue(&win,x1,yy1,ibutton,&motion,&release) == 1) {
      if ((motion==0) && (release==0)) {
	*iwin = win ;
	ok=1;
	break;
      }

      win=-1;
    }
    if(ok) {*istr = 0;break;};
    /* get next event */

    C2F(sxevents)();

   /** Check menu activation **/
    if ( *istr==1 && C2F(ismenu)()==1 ) {
      int entry;
      C2F(getmen)(str,&lstr,&entry);
      *ibutton = -2;
      *istr=lstr;
      *x1=0;
      *yy1=0;
      *iwin=-1;
      break;
    }
    /* to slow down event loop not to use all cpu when nothing happen*/
    delay.tv_sec = 0; delay.tv_usec = 10;
    select(0, 0, 0, 0, &delay);
  }
  set_wait_click(0);
  set_client_message_off();

  /** Cleanup **/
  wincount =  GetWinsMaxId()+1;
  for (i=0;i < wincount;i++) {
    CW=GetWindowNumber(i);
    if (CW!=(Window ) NULL) 
      XDefineCursor(dpy, CW ,arrowcursor);
  }
  XSync (dpy, 0);
}


void C2F(xclick)(char *str, integer *ibutton, integer *x1, integer *yy1, integer *iflag, integer *istr, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{
  integer lstr ;
  SciClick(ibutton,x1, yy1,iflag,0,0,*istr,str,&lstr);
  if ( *istr == 1) 
    {
      if (*ibutton == -2) 
	{
	  /*	  sciprint("Menu activated %s %d",str,lstr);*/
	  *istr = lstr;
	}
      else
	*istr = 0;
    }
}

void C2F(xgetmouse)(char *str, integer *ibutton, integer *x1, integer *yy1, integer *iflag, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{  /* v7 is used to indicate if lhs is 1 or 2. lhs=1=> v7=0, lhs=2=> v7=1;*/
  SciClick(ibutton,x1, yy1,iflag,v6[0],v6[1],(v7==NULL)?0:*v7,(char *) 0,(integer *)0);
}


/*****************************************
 * general function for mouse click or 
 * dynamic menu activation 
 * 
 * if iflag = 0 : clear previous mouse click 
 * if iflag = 1 : doesn't
 * iflag is also used to output window number in case win is required
 * if getmouse = 1 : check also mouse move 
 * if getrelease=1 : check also mouse release 
 * if dyn_men = 1 ; check also dynamic menus, win not required
 *              ( return the button code in str )
 * if dyn_men = 0 ; don't check dynamic menus, win not required
 * if dyn_men = 2 ; don't check dynamic menus and win is required
 * if dyn_men = 3 ; check dynamic menus and win is required
 * return value : 0,1,2 ButtonPressed 
 *                -5,-4,-3: ButtonReleased
 *                -100 : error 
 *                -2   : menu 
 *****************************************/

void SciClick(integer *ibutton, integer *x1, integer *yy1, integer *iflag, int getmouse, int getrelease, int dyn_men, char *str, integer *lstr)
{
  integer buttons = 0;
  integer win,ok,choice,motion,release;
  Window SCWindow;
  struct timeval delay; /* usec, to slow down event loop */

  choice=(dyn_men>1); /* depending on lhs */

  if ( ScilabXgc == (struct BCG *) 0 || ScilabXgc->CWindow == (Window) 0)
    {
      *x1   =  -1;
      *yy1  =  -1;
      *ibutton = -100;     return;
    }
  win = ScilabXgc->CurWindow;
  SCWindow = ScilabXgc->CWindow;

  if ( *iflag ==0 )  ClearClickQueue(ScilabXgc->CurWindow);
  XDefineCursor(dpy, ScilabXgc->CWindow ,crosscursor);

  /*set wait_for_click=1 to diable event handler if any */
  set_wait_click(1);
  set_event_select(1+2*getmouse+4*getrelease);

  while (buttons == 0) 
    {
      /** first check if an event has been store in the queue while wait_for_click was 0 **/
      if (choice) win=-1;
      ok=0;
      if (CheckClickQueue(&win,x1,yy1,ibutton,&motion,&release) == 1)  {
	if ((release&&getrelease) || (motion&&getmouse) || ((motion==0)&&(release==0))){
	  *iflag=win;
	  ok=1;
	  break;
	}
	if (choice) win=-1;
      }
      if(ok) break;
      /* make the X and tk event loop */
      C2F(sxevents)();

      /** maybe someone decided to destroy scilab Graphic window **/
      if ( ScilabXgc == (struct BCG *) 0 || ScilabXgc->CWindow != SCWindow)
	{
	  *x1   =  -1;
	  *yy1  =  -1;
	  *ibutton = -100;
	  set_event_select(1+4); 
	  set_wait_click(0);
	  return;
	}
      /** check for dynamic menu  **/
      if ( (dyn_men == 1||dyn_men==3) &&  C2F(ismenu)()==1 ) {
	int entry;
	C2F(getmen)(str,lstr,&entry);
	*ibutton = -2;
	*x1=0;
	*yy1=0;
	break;
      }

    /* to slow down event loop not to use all cpu when nothing happen*/
      delay.tv_sec = 0; delay.tv_usec = 10;
      select(0, 0, 0, 0, &delay);
    }
  set_event_select(1+4); 
  set_wait_click(0);
  if ( ScilabXgc != (struct BCG *) 0 && ScilabXgc->CWindow != (Window) 0)
    XDefineCursor(dpy, ScilabXgc->CWindow ,arrowcursor);
  XSync (dpy, 0);
}

/*******************************************************
 * clear a rectangle zone 
 *******************************************************/

void C2F(cleararea)(char *str, integer *x, integer *y, integer *w, integer *h, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{
  if (ScilabXgc->Cdrawable != (Drawable) 0 ) 
      PixmapClear(ScilabXgc,*x,*y,*w,*h);
  if (ScilabXgc->CurPixmapStatus != 1) 
    XClearArea(dpy,ScilabXgc->CWindow,(int)*x,(int) *y,(unsigned) *w,
	       (unsigned) *h,False);
  XFlush(dpy);
}

/************************************************************************
 * graphic context modifications 
 ************************************************************************/

/** to get the window upper-left point coordinates on the screen  **/

static void xget_windowpos(integer *verbose, integer *x, integer *narg, double *dummy)
{
  int xx[2];
  XWindowAttributes war;
  Window CHR;
  *narg = 2;
  XGetWindowAttributes(dpy,ScilabXgc->CBGWindow,&war); 
  XTranslateCoordinates(dpy,ScilabXgc->CBGWindow,root,war.x,war.y,&(xx[0]),&(xx[1]),&CHR);
  x[0]=xx[0];x[1]=xx[1];
  if (*verbose == 1) 
    sciprint("\n ScilabXgc->CWindow position :%d,%d\r\n",xx[0],xx[1]);
}


/** to set the window upper-left point position on the screen **/

static void xset_windowpos(integer *x, integer *y, integer *v3, integer *v4)
{
  /** test Normalement inutile XXXX **/
  integer ierr;
  if (ScilabXgc == (struct BCG *)0 || ScilabXgc->CBGWindow == (Window) NULL) 
    C2F(initgraphic)("",PI0,&ierr,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
  XMoveWindow(dpy,ScilabXgc->CBGWindow,(int) *x,(int) *y);
}

/** To get the drawbox  window size : used by periGif **/

void C2F(getwindowdim) (integer *verbose, integer *x, integer *narg, double *dummy)
{     
  xget_windowdim(verbose, x, narg,dummy);
}

/** To get the drawbox  window size **/

static void xget_windowdim(integer *verbose, integer *x, integer *narg, double *dummy)
{     
  *narg = 2;
  x[0]= ScilabXgc->CWindowWidth;
  x[1]= ScilabXgc->CWindowHeight;
  if (*verbose == 1) 
    sciprint("\n ScilabXgc->CWindow dim :%d,%d\r\n",(int) x[0],(int) x[1]);
} 

/** To change the drawbox window size  **/

static void xset_windowdim(integer *x, integer *y, integer *v3, integer *v4)
{
  XWindowAttributes war;
  if (ScilabXgc == (struct BCG *) NULL || ScilabXgc->CBGWindow ==  (Window) NULL) return ;
  GViewportResize(ScilabXgc,x,y) ;
  /* check dimensions : GViewportResize can set slighly different values */
  XGetWindowAttributes(dpy,ScilabXgc->CWindow,&war); 
  ScilabXgc->CWindowWidth  = war.width;
  ScilabXgc->CWindowHeight = war.height;
  XFlush(dpy);
}

/** To get the popup  window size **/

static void xget_popupdim(integer *verbose, integer *x, integer *narg, double *dummy)
{ 
  Dimension clwidth,clheight;
  static Arg args[2] ;
  int iargs = 0;
  XtSetArg (args[iargs], XtNwidth, &clwidth ); iargs++;
  XtSetArg (args[iargs], XtNheight, &clheight); iargs++;
  XtGetValues (ScilabXgc->popup, args, iargs);
  x[0]= clwidth; 
  x[1]= clheight;
  *narg = 2;
  if (*verbose == 1) 
    sciprint("\n ScilabXgc->CWindow dim :%d,%d\r\n",(int) x[0],(int) x[1]);
} 

/** To change the popup window size  **/


static void xset_popupdim(integer *x, integer *y, integer *v3, integer *v4)
{
  GPopupResize(ScilabXgc,x,y);
}

/** To get the viewport Upper/Left point Position **/

static void xget_viewport(integer *verbose, integer *x, integer *narg, double *dummy)
{     
  *narg = 2;
  if ( ScilabXgc->CurResizeStatus != 1) 
    {
      SciViewportGet(ScilabXgc,x,x+1) ;
    }
  else 
    { 
      x[0]=x[1]=0;
    }
  if (*verbose == 1) 
    sciprint("\n Viewport position:%d,%d\r\n",(int) x[0],(int) x[1]);
} 

/** To change the window size  **/

static void xset_viewport(integer *x, integer *y, integer *v3, integer *v4)
{
  if ( ScilabXgc->CurResizeStatus != 1) 
    SciViewportMove(ScilabXgc,*x,*y);
}

/********************************************
 * select window intnum as the current window 
 * window is created if necessary 
 ********************************************/

static void xset_curwin(integer *intnum, integer *v2, integer *v3, integer *v4)
{ 
  XWindowAttributes war;
  struct BCG *bcgk;
  integer ierr;
  bcgk =  ScilabXgc ;
  /** send info to menu **/
  if (v2 != (integer *) 0) MenuFixCurrentWin(*intnum);
  if ( ScilabXgc == (struct BCG *) 0 ) 
    {
      /** First entry or no more graphic window **/
      C2F(initgraphic)("",intnum,&ierr,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
    }
  else 
    {
      if ( ScilabXgc->CurWindow != *intnum )
	{
	  SwitchWindow(intnum);
	}
    }
  if ( ScilabXgc == (struct BCG *) 0 && bcgk != (struct BCG *) 0)
    {
      /** back to previous value **/
      ScilabXgc = bcgk ;
      if (v2 != (integer *) 0) MenuFixCurrentWin(bcgk->CurWindow);
    }
  else 
    {
      /* update the dimensions   */
      XGetWindowAttributes(dpy,ScilabXgc->CWindow,&war); 
      ScilabXgc->CWindowWidth = war.width;
      ScilabXgc->CWindowHeight = war.height;
    }
}
/* NG beg */
static void xset_scilabFigure(integer *v1,integer *v2,integer *v3,integer *v4,integer *v5,integer *v6,double *figure)
{
 ScilabXgc->mafigure=(sciPointObj *)figure;
}

static void xget_scilabFigure(integer *verbose, integer *x,integer *narg, double *figure)
{   
  *narg=1;
  figure=(double *)ScilabXgc->mafigure;
}

static void xset_scilabVersion(integer *vers, integer *v2, integer *v3, integer *v4)
{
  ScilabXgc->graphicsversion=*vers;
}

static void xget_scilabVersion(integer *verbose, integer *vers, integer *narg, double *dummy)
{   
  *narg =1 ;
  *vers = ScilabXgc->graphicsversion;
}

static void xset_scilabxgc(v1, v2, v3, v4)
     integer *v1;
     integer *v2;
     integer *v3;
     integer *v4; 
     
{

}
static void xget_scilabxgc(verbose, x,narg, dummy)
     integer *verbose;
     integer *x;
     integer *narg;
     double *dummy;
{   
  double **XGC;
  
  XGC=(double **)dummy;
  *XGC= (double *)ScilabXgc;
  
}
/* NG end */


/* used in the previous function to set back the graphic scales 
 * when changing form one window to an other 
 * Also used in scig_tops : to force a reset of scilab graphic scales 
 * after a print in Postscript or Xfig 
 */

void SwitchWindow(integer *intnum)
{
  /** trying to get window *intnum **/
  struct BCG *SXgc;
  integer ierr;
  SXgc = GetWindowXgcNumber(*intnum);
  if ( SXgc != (struct BCG *) 0 ) 
    {
      /** Window intnum exists **/
      ScilabXgc = SXgc ;
      ResetScilabXgc ();
      get_window_scale(*intnum,NULL);
    }
  else 
    {
      /** Create window **/
      C2F(initgraphic)("",intnum,&ierr,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
    }
}


/*
 *  Get the id number of the Current Graphic Window 
 * In all the other functions we are sure that ScilabXgc exists 
 * when we call them ( see sciwin in matdes.f ) 
 * exept for this function which is called in sciwin and the previous one 
 */
 
static void xget_curwin(integer *verbose, integer *intnum, integer *narg, double *dummy)
{
  *narg =1 ;
  *intnum = (ScilabXgc != (struct BCG *) 0) ? ScilabXgc->CurWindow : 0;
  if (*verbose == 1) 
    sciprint("\nCurrent Graphic Window :%d\r\n",(int) *intnum);

}

/** Set a clip zone (rectangle ) **/

static void xset_clip(integer *x, integer *y, integer *w, integer *h)
{
  integer verbose=0,wd[2],narg;
  XRectangle rects[1];
  ScilabXgc->ClipRegionSet = 1;
  xget_windowdim(&verbose,wd,&narg,vdouble);
  rects[0].x= *x;
  rects[0].y= *y;
  rects[0].width= *w;
  rects[0].height= *h;
  ScilabXgc->CurClipRegion[0]= rects[0].x;
  ScilabXgc->CurClipRegion[1]= rects[0].y;
  ScilabXgc->CurClipRegion[2]= rects[0].width;
  ScilabXgc->CurClipRegion[3]= rects[0].height;
  XSetClipRectangles(dpy,gc,0,0,rects,1,Unsorted);
}

/** unset clip zone **/

static void xset_unclip(integer *v1, integer *v2, integer *v3, integer *v4)
{
  ScilabXgc->ClipRegionSet = 0;
  XSetClipMask(dpy,gc,None);
}

/** Get the boundaries of the current clip zone **/

static void xget_clip(integer *verbose, integer *x, integer *narg, double *dummy)
{
  x[0] = ScilabXgc->ClipRegionSet;
  if ( x[0] == 1)
    {
      *narg = 5;
      x[1] =ScilabXgc->CurClipRegion[0];
      x[2] =ScilabXgc->CurClipRegion[1];
      x[3] =ScilabXgc->CurClipRegion[2];
      x[4] =ScilabXgc->CurClipRegion[3];
    }
  else *narg = 1;
  if (*verbose == 1)
    {
      if (ScilabXgc->ClipRegionSet == 1)
	sciprint("\nThere's a Clip Region :x:%d,y:%d,w:%d,h:%d\r\n",
		 ScilabXgc->CurClipRegion[0],
		 ScilabXgc->CurClipRegion[1],
		 ScilabXgc->CurClipRegion[2],
		 ScilabXgc->CurClipRegion[3]);
      else 
	Scistring("\nNo Clip Region");
    }
}



/*
 * For the drawing functions dealing with vectors of 
 * points, the following routine is used to select the mode 
 * absolute or relative 
 * Absolute mode if *num==0, relative mode if *num != 0
 */

/** to set absolute or relative mode **/

static void xset_absourel(integer *num, integer *v2, integer *v3, integer *v4)
{
  if (*num == CoordModeOrigin)
    ScilabXgc->CurVectorStyle =  CoordModeOrigin;
  else 
    ScilabXgc->CurVectorStyle =  CoordModePrevious ;
}

/** to get information on absolute or relative mode **/

static void xget_absourel(integer *verbose, integer *num, integer *narg, double *dummy)
{
  *narg = 1;
  *num = ScilabXgc->CurVectorStyle  ;
  if (*verbose == 1) 
    {
      if (ScilabXgc->CurVectorStyle == CoordModeOrigin)
	Scistring("\nTrace Absolu");
      else 
	Scistring("\nTrace Relatif");
    }
}

/* The alu function for drawing : Works only with X11
 * Not in Postscript, Read The X11 manual to get more informations 
 */

static struct alinfo { 
  char *name;
  char id;
  char *info;} AluStruc_[] =
  { 
    {"GXclear" ,GXclear," 0 "},
    {"GXand" ,GXand," src AND dst "},
    {"GXandReverse" ,GXandReverse," src AND NOT dst "},
    {"GXcopy" ,GXcopy," src "},
    {"GXandInverted" ,GXandInverted," NOT src AND dst "},
    {"GXnoop" ,GXnoop," dst "},
    {"GXxor" ,GXxor," src XOR dst "},
    {"GXor" ,GXor," src OR dst "},
    {"GXnor" ,GXnor," NOT src AND NOT dst "},
    {"GXequiv" ,GXequiv," NOT src XOR dst "},
    {"GXinvert" ,GXinvert," NOT dst "},
    {"GXorReverse" ,GXorReverse," src OR NOT dst "},
    {"GXcopyInverted" ,GXcopyInverted," NOT src "},
    {"GXorInverted" ,GXorInverted," NOT src OR dst "},
    {"GXnand" ,GXnand," NOT src OR NOT dst "},
    {"GXset" ,GXset," 1 "}
  };


static void idfromname(char *name1, integer *num)
{int i;
 *num = -1;
 for ( i =0 ; i < 16;i++)
   if (strcmp(AluStruc_[i].name,name1)== 0) 
     *num=AluStruc_[i].id;
 if (*num == -1 ) 
   {
     Scistring("\n Use the following keys (integer in scilab");
     for ( i=0 ; i < 16 ; i++)
       sciprint("\nkey %s   -> %s\r\n",AluStruc_[i].name,
		AluStruc_[i].info);
   }
}

void xset_alufunction(char *string)
{     
  integer value;
  XGCValues gcvalues;
  idfromname(string,&value);
  if ( value != -1)
    {
      ScilabXgc->CurDrawFunction = value;
      gcvalues.function = value;
      XChangeGC(dpy, gc, GCFunction, &gcvalues);
    }
}


static void xset_alufunction1(integer *num, integer *v2, integer *v3, integer *v4)
{
  xset_alufunction2(ScilabXgc,num,v2,v3,v4);
}



static void xset_alufunction2(struct BCG *Xgc,integer *num, integer *v2, integer *v3, integer *v4)
{     
  integer value;
  XGCValues gcvalues;
  static Pixel pxb,pxf;
  pxb = (Xgc->Colors == NULL)? DefaultBackground 
    :  Xgc->Colors[Xgc->NumBackground];
  pxf = (Xgc->Colors == NULL)? DefaultForeground 
    :  Xgc->Colors[Xgc->NumForeground];
  value=AluStruc_[Min(15,Max(0,*num))].id;
  if ( value != -1)
    {
      Xgc->CurDrawFunction = value;
      /* XChangeGC(dpy, gc, GCFunction, &gcvalues); */
      /** Using diff gc **/
      switch (value) 
	{
	case GXclear : 
	  gcvalues.foreground = pxb;
	  gcvalues.background = pxb;
	  gcvalues.function = GXcopy;
	  break;
	case GXxor   : 
	  gcvalues.foreground = pxf ^ pxb ;
	  gcvalues.background = pxb ; 
	  gcvalues.function = GXxor;
	  break;
	default :
	  gcvalues.foreground  = pxf ;
	  gcvalues.background = pxb;
	  gcvalues.function = value;
	  break;
	}
      XChangeGC(dpy,gc,(GCFunction|GCForeground|GCBackground), &gcvalues);
      if ( value == GXxor  && Xgc->CurColorStatus == 1 )
	{
	  /** the way colors are computed changes if we are in Xor mode **/
	  /** so we force here the computation of current color  **/
	  set_c(Xgc->CurColor);
	}
    }
}

static void xget_alufunction(integer *verbose, integer *value, integer *narg, double *dummy)
{ 
  *narg =1 ;
  *value = ScilabXgc->CurDrawFunction ;
  if (*verbose ==1 ) 
    { 
      sciprint("\nThe Alufunction is %s -> <%s>\r\n",
	       AluStruc_[*value].name,
	       AluStruc_[*value].info);
    }
}

/*
 *  to set the thickness of lines : 0 is a possible value 
 *  it gives the thinest line (0 and 1 are the same for X11 but
 * with diferent algorithms 
 * defaut value is 1 
 */

static void xset_thickness(integer *value, integer *v2, integer *v3, integer *v4)
{ 
  XGCValues gcvalues;
  ScilabXgc->CurLineWidth =Max(0, *value);
  gcvalues.line_width = Max(0, *value);
  XChangeGC(dpy, gc, GCLineWidth, &gcvalues); }

/** to get the thickness value **/

static void xget_thickness(integer *verbose, integer *value, integer *narg, double *dummy)
{
  *narg =1 ;
  *value = ScilabXgc->CurLineWidth ;
  if (*verbose ==1 ) 
    sciprint("\nLine Width:%d\r\n", ScilabXgc->CurLineWidth ) ;
}

/** To set grey level for filing areas **/
/** from black (*num =0 ) to white     **/

Pixmap  Tabpix_[GREYNUMBER];

static char grey0[GREYNUMBER][8]={
  {(char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00},
  {(char)0x00, (char)0x00, (char)0x44, (char)0x00, (char)0x00, (char)0x00, (char)0x44, (char)0x00},
  {(char)0x00, (char)0x44, (char)0x00, (char)0x22, (char)0x08, (char)0x40, (char)0x01, (char)0x20},
  {(char)0x00, (char)0x92, (char)0x00, (char)0x25, (char)0x00, (char)0x92, (char)0x00, (char)0xa4},
  {(char)0x55, (char)0x00, (char)0xaa, (char)0x00, (char)0x55, (char)0x00, (char)0xaa, (char)0x00},
  {(char)0xad, (char)0x00, (char)0x5b, (char)0x00, (char)0xda, (char)0x00, (char)0x6d, (char)0x00},
  {(char)0x6d, (char)0x02, (char)0xda, (char)0x08, (char)0x6b, (char)0x10, (char)0xb6, (char)0x20},
  {(char)0x6d, (char)0x22, (char)0xda, (char)0x0c, (char)0x6b, (char)0x18, (char)0xb6, (char)0x24},
  {(char)0x55, (char)0xaa, (char)0x55, (char)0xaa, (char)0x55, (char)0xaa, (char)0x55, (char)0xaa},
  {(char)0x92, (char)0xdd, (char)0x25, (char)0xf3, (char)0x94, (char)0xe7, (char)0x49, (char)0xdb},
  {(char)0x92, (char)0xfd, (char)0x25, (char)0xf7, (char)0x94, (char)0xef, (char)0x49, (char)0xdf},
  {(char)0x52, (char)0xff, (char)0xa4, (char)0xff, (char)0x25, (char)0xff, (char)0x92, (char)0xff},
  {(char)0xaa, (char)0xff, (char)0x55, (char)0xff, (char)0xaa, (char)0xff, (char)0x55, (char)0xff},
  {(char)0xff, (char)0x6d, (char)0xff, (char)0xda, (char)0xff, (char)0x6d, (char)0xff, (char)0x5b},
  {(char)0xff, (char)0xbb, (char)0xff, (char)0xdd, (char)0xf7, (char)0xbf, (char)0xfe, (char)0xdf},
  {(char)0xff, (char)0xff, (char)0xbb, (char)0xff, (char)0xff, (char)0xff, (char)0xbb, (char)0xff},
  {(char)0xff, (char)0xff, (char)0xff, (char)0xff, (char)0xff, (char)0xff, (char)0xff, (char)0xff},
};

void C2F(CreatePatterns)(Pixel whitepixel, Pixel blackpixel)
{ integer i ;
 for ( i=0 ; i < GREYNUMBER ; i++)
   Tabpix_[i] =XCreatePixmapFromBitmapData(dpy, root,grey0[i] ,8,8,whitepixel
					   ,blackpixel,XDefaultDepth (dpy,DefaultScreen(dpy)));
}

static void xset_pattern(integer *num, integer *v2, integer *v3, integer *v4)
{ integer i ; 

 if (ScilabXgc->CurColorStatus == 1) 
   {
     set_c(*num-1);
   }
 else 
   {
     i= Max(0,Min(*num - 1,GREYNUMBER - 1));
     ScilabXgc->CurPattern = i;
     XSetTile (dpy, gc, Tabpix_[i]); 
     if (i ==0)
       XSetFillStyle(dpy,gc,FillSolid);
     else 
       XSetFillStyle(dpy,gc,FillTiled);
   }
}

/** To get the id of the current pattern  **/

static void xget_pattern(integer *verbose, integer *num, integer *narg, double *dummy)
{ 
  *narg=1;
  if ( ScilabXgc->CurColorStatus == 1 ) 
    *num = ScilabXgc->CurColor + 1;
  else 
    *num = ScilabXgc->CurPattern + 1;
  if (*verbose == 1) 
    sciprint("\n Pattern : %d\r\n",ScilabXgc->CurPattern + 1);
}

/** To get the id of the last pattern **/

static void xget_last(integer *verbose, integer *num, integer *narg, double *dummy)
{
  if ( ScilabXgc->CurColorStatus == 1 ) 
    {
      *num = ScilabXgc->IDLastPattern + 1;
      if (*verbose == 1) 
	sciprint("\n Id of Last Color %d\r\n",(int)*num);
    }
      
  else 
    {
      *num = ScilabXgc->IDLastPattern + 1;
      if (*verbose == 1) 
	sciprint("\n Id of Last Pattern %d\r\n",(int)*num);
    }
  *narg=1;
}

/* Line style 
 * use a table of dashes and set default X11-dash style to 
 *  one of the possible value. value points 
 *  to a strictly positive integer 
 *   if *value == 0 -> Solid line  
 * else Dashed Line 
 */

#define MAXDASH 6

static integer DashTab[MAXDASH][4] = {
  {2,5,2,5}, {7,2,7,2},  {2,2,2,2}, {7,2,2,2},
  {11,3,2,3}, {11,3,5,3}};

static void xset_dash(integer *value, integer *v2, integer *v3, integer *v4)
{
  static integer  l2 = 4, l3;

  l3 = Max(0,Min(MAXDASH - 1,*value - 1));
  xset_dashstyle(&l3,DashTab[l3],&l2);
  ScilabXgc->CurDashStyle = l3;
}

/* old version of xset_dash retained for compatibility */

static void xset_dash_or_color(integer *value, integer *v2, integer *v3, integer *v4)
{
  if ( ScilabXgc->CurColorStatus == 1) 
    {
      set_c(*value-1);
    }
  else
    xset_dash(value, v2, v3, v4);
}

static void xset_dash_and_color(integer *value, integer *v2, integer *v3, integer *v4)
{
  xset_dash(value, v2, v3, v4);
  xset_pattern(value+6, v2, v3, v4);
}

static void xset_line_style(integer *value, integer *v2, integer *v3, integer *v4)
{
  integer j;
  if (ScilabXgc->CurColorStatus == 0) 
    xset_dash(value,PI0,PI0,PI0);
  else {
    j= ScilabXgc->CurDashStyle + 1;
    xset_dash(&j,PI0,PI0,PI0);
    xset_pattern(value,PI0,PI0,PI0);
  }
}


/*
 *  To change The X11-default dash style
 * if *value == 0, use a solid line, if *value != 0 
 * the dash style is specified by the xx vector of n values 
 * xx[3]={5,3,7} and *n == 3 means :  5white 3 void 7 white \ldots 
 */


static void xset_dashstyle(integer *value, integer *xx, integer *n)
{
  int dashok= LineOnOffDash ;
  if ( *value == 0) dashok = LineSolid;
  else 
    {
      integer i; char buffdash[18];
      for ( i =0 ; i < *n ; i++) buffdash[i]=xx[i];
      XSetDashes(dpy,gc,0,buffdash,(int)*n);
    }
  XSetLineAttributes(dpy,gc,(unsigned) ScilabXgc->CurLineWidth,dashok,
		     CapButt,JoinMiter);
}


/** to get the current dash-style **/
/* old version of xget_dash retained for compatibility */

static void xget_dash_or_color(integer *verbose, integer *value, integer *narg, double *dummy)
{
  if ( ScilabXgc->CurColorStatus ==1) 
    {
      *narg =1 ;
      *value = ScilabXgc->CurColor + 1;
      if (*verbose == 1) sciprint("Color %d",(int)*value);
      return;
    }
  xget_dash(verbose, value, narg,dummy);
}

static void xget_dash(integer *verbose, integer *value, integer *narg, double *dummy)
{
  int i ;
  *narg =1 ;
  *value = ScilabXgc->CurDashStyle + 1;
  if (*value == 1) 
    { if (*verbose == 1) Scistring("\nLine style = Line Solid");}
  else 
    {
      value[1]=4;
      *narg = value[1]+2;
      for (i = 0 ; i < value[1]; i++) value[i+2]=DashTab[*value-2][i];
      if (*verbose ==1) 
	{
	  sciprint("\nDash Style %d:<",(int)*value - 1);
	  for (i = 0 ; i < value[1]; i++)
	    sciprint("%d ",(int)value[i+2]);
	  Scistring(">\n");
	}
    }
}

static void xget_dash_and_color(integer *verbose, integer *value, integer *narg, double *dummy)
{
/*may be improved replacing 6 by narg */
  xget_dash(verbose, value, narg,dummy);
  xget_pattern(verbose, value+6, narg,dummy);
  *narg = 6;
}


/* used to switch from color to b&w and reverse */

static void xset_usecolor(integer *num, integer *v1, integer *v2, integer *v3)
{
  integer i;
  i =  Min(Max(*num,0),1);
  if ( ScilabXgc->CurColorStatus != (int) i) 
    {
      if (ScilabXgc->CurColorStatus == 1) 
	{
	  /* from color to b&w */
	  ScilabXgc->CurColorStatus = 1;
	  xset_pattern((i=1,&i),PI0,PI0,PI0);
	  /* go to b&w */
	  ScilabXgc->CurColorStatus = 0;
	  i= ScilabXgc->CurPattern + 1;
	  xset_pattern(&i,PI0,PI0,PI0);
	  i= ScilabXgc->CurDashStyle + 1;
	  xset_dash(&i,PI0,PI0,PI0);


	  ScilabXgc->IDLastPattern = GREYNUMBER - 1;
	}
      else 
	{
	  /* switching to color mode */
	  /* patterns and dashes reinitialization */
	  /* colors too */
	  ScilabXgc->CurColorStatus = 0;
	  xset_pattern((i=1,&i),PI0,PI0,PI0);
	  xset_dash((i=1,&i),PI0,PI0,PI0);
	  /* switching to color mode */
	  ScilabXgc->CurColorStatus = 1;
	  i= ScilabXgc->CurColor + 1;
	  xset_pattern(&i,PI0,PI0,PI0);
	  xset_pattern(&i,PI0,PI0,PI0);
	  ScilabXgc->IDLastPattern = ScilabXgc->Numcolors - 1;

	}
    }
}

static void xget_usecolor(integer *verbose, integer *num, integer *narg, double *dummy)
{
  *num = ScilabXgc->CurColorStatus;
  if (*verbose == 1) 
    sciprint("\n Use color %d\r\n",(int)*num);
  *narg=1;
}


/* 
   Change the pixmap status of a Graphic Window. 

   CurPixmapStatus==0 uses a pixmap as backing store memory if it is possible to allocate it
   if allocation fails no backing store will be used (graphics will be redrawn on expose).
   In backing store mode the graphics are drawn on the display AND into the pixmap. The pixmap
   is used when expose events are received

   CurPixmapStatus==1 uses a pixmap as double buffer for animation if it is possible to 
   allocate it. if allocation fails CurPixmapStatus is set to 0
   In this mode the graphics are only drawn into the pixmap. The pixmap is send to the display
   when user explicitly requires it.
*/

static void xset_pixmapOn(integer *num, integer *v2, integer *v3, integer *v4)
{
  integer num1= Min(Max(*num,0),1);
  Pixel px;


  /* create the pixmap if it does not exist */
  if (ScilabXgc->Cdrawable == (Drawable) 0) {
    XWindowAttributes war;
    XGetWindowAttributes(dpy,ScilabXgc->CWindow,&war); 
    ScilabXgc->Cdrawable = (Drawable) XCreatePixmap(dpy,root,war.width,war.height,depth);
    if (ScilabXgc->Cdrawable != (Drawable) 0) {
      PixmapClear(ScilabXgc,0,0,war.width,war.height);
      XSetWindowBackgroundPixmap(dpy, ScilabXgc->CWindow, (Pixmap) ScilabXgc->Cdrawable);
    }
    else {
      px = (ScilabXgc->Colors == NULL) ? DefaultBackground 
	:  ScilabXgc->Colors[ScilabXgc->NumBackground];
      XSetWindowBackground(dpy, ScilabXgc->CWindow,px);
    }
  }

  if ( num1 == 0 ) {
    C2F(xinfo)(" ",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
    ScilabXgc->CurPixmapStatus = 0;
  }
  else if ( num1 == 1 ) {
      if (ScilabXgc->Cdrawable != (Drawable) 0) {
	XWindowAttributes war;
	C2F(xinfo)("Animation mode is on,( xset('pixmap',0) to leave)",
		   PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
	XGetWindowAttributes(dpy,ScilabXgc->CWindow,&war); 	

	ScilabXgc->CurPixmapStatus = 1;
	PixmapClear(ScilabXgc,0,0,war.width,war.height);
      }
      else /*no pixmap */
	ScilabXgc->CurPixmapStatus = 0;

    }

}

static void xget_pixmapOn(integer *verbose, integer *value, integer *narg, double *dummy)
{
  *value=ScilabXgc->CurPixmapStatus;
  *narg =1 ;
  if (*verbose == 1) sciprint("Pixmap status %d",(int)*value);
}

/* Change the status of a Graphic Window
 * follow or dont follow the viewport resize
 */

static void xset_wresize(integer *num, integer *v2, integer *v3, integer *v4)
{
  integer num1= Min(Max(*num,0),1);
  if ( num1 != ScilabXgc->CurResizeStatus && num1 == 1) 
    {
      /* we are switching back to wresize 1 we must adapt the drawbox size */
      /* by asking the popup to change its size to the same ones !! **/
      integer clwidth,clheight;
      /* first move the viewport to the upper left position */
      SciViewportMove(ScilabXgc,0,0);
      /* get the clip box size */
      SciViewportClipGetSize (ScilabXgc, &clwidth, &clheight);
      /* change the resize status */
      ScilabXgc->CurResizeStatus = num1 ;
      /* force a redim for the drawbox to fit the popup */
      xset_windowdim(&clwidth,&clheight,NULL,NULL);
      return ; 
    }
  ScilabXgc->CurResizeStatus = num1 ;
}

static void xget_wresize(integer *verbose, integer *value, integer *narg, double *dummy)
{
  *value=ScilabXgc->CurResizeStatus;
  *narg =1 ;
  if (*verbose == 1) sciprint("Resize status %d",(int)*value);
}

/* setting the default colormap with colors defined in color.h */

static int set_default_colormap_flag = 1;

int C2F(sedeco)(int *flag)
{
  set_default_colormap_flag = *flag;
  return(0);
}

/* 
 * Our function to translate from RGB to pixels for True Color screens:
 * very tricky... 
 */




Pixel RGB2pixO(unsigned int r, unsigned int g, unsigned int b, long unsigned int r_mask, long unsigned int g_mask, long unsigned int b_mask)
{
  static int r_bits,g_bits,b_bits,first_entry=0;
  unsigned long p2;
  int i;
  Pixel pix;

  if (first_entry == 0) {
    p2 = 1;
    for (i = 1; i <= depth; i++) {
      if (p2 << i == b_mask + 1) {
	b_bits = i;
	break;
      }
    }
    g_mask = g_mask >> b_bits; 
    p2 = 1;
    for (i = 1; i <= depth - b_bits; i++) {
      if (p2 << i == g_mask + 1) {
	g_bits = i;
	break;
      }
    }

    r_bits = depth - g_bits - b_bits;
    first_entry = 1;
  }
  
  pix = ((r >> (8 - r_bits)) << (g_bits + b_bits))
    + ((g >> (8 - g_bits)) << b_bits)
    + (b >> (8 - b_bits));
  return pix;
}


static char  *hex_string(int r, int g, int b);

/*
 * Get the hex string corresponding to a r,g,b spec
 */

static char hex_str[10];

static char *hex_string(int r, int g, int b)
{
  char rlabel[10], glabel[10], blabel[10];
 
  /* Copy the rgb to char strings */
  (void) sprintf(rlabel,"00%x",r);
  (void) sprintf(glabel,"00%x",g);
  (void) sprintf(blabel,"00%x",b);

  /* The hex label is composed of last 2 chars from r,g,b */
  (void) sprintf(hex_str,"#000000");
  hex_str[1] = rlabel[strlen(rlabel)-2];
  hex_str[2] = rlabel[strlen(rlabel)-1];
  hex_str[3] = glabel[strlen(glabel)-2];
  hex_str[4] = glabel[strlen(glabel)-1];
  hex_str[5] = blabel[strlen(blabel)-2];
  hex_str[6] = blabel[strlen(blabel)-1];

  return(hex_str);
}

Pixel RGB2pix(unsigned int r, unsigned int g, unsigned int b, long unsigned int r_mask, long unsigned int g_mask, long unsigned int b_mask)
{
  Colormap cmap;
  XColor      cdef;
  cdef.red   = r *256 ;
  cdef.green = g *256 ;
  cdef.blue  = b *256 ;
  cdef.flags = DoRed | DoGreen | DoBlue;
  cmap = XDefaultColormap(dpy,XDefaultScreen(dpy));
  if (XAllocColor(dpy,cmap,&cdef))
    return( cdef.pixel);
  else if (XParseColor(dpy,cmap,hex_string(r,g,b),&cdef) &&
	   XAllocColor(dpy,cmap,&cdef)) {
    return( cdef.pixel);
  }
  return (Pixel) 0;
}

/* set_default_colormap is called when raising a window for the first 
 *  time by xset('window',...) or by getting back to default by 
 *  xset('default',...) 
 */

void set_default_colormap1(int m);
void set_default_colormap2(int m);
void set_default_colormap3(int m);

void set_default_colormap(void)
{
  int m;

  /* we don't want to set the default colormap at window creation 
     if the scilab command was xset("colormap"); */

  if (set_default_colormap_flag == 0) return;

  if (DEFAULTNUMCOLORS > maxcol) {
    sciprint("Not enough colors for default colormap. Maximum is %d\r\n",
	     maxcol);
    return;
  }
  m = DEFAULTNUMCOLORS;

  switch (visual->class) {
  case TrueColor:
    set_default_colormap1(m);
    break;
  case DirectColor:
    set_default_colormap2(m);
    break;
  default:
    set_default_colormap2(m);
    break;
  }
}

/* True Color visuals */

void set_default_colormap1(int m)
{
  int i;
  Colormap cmap;
  Pixel pix;
  unsigned int red, green, blue;
  Pixel *c = ScilabXgc->Colors;  /* Save old color vectors compônents */
  float *r = ScilabXgc->Red;
  float *g = ScilabXgc->Green;
  float *b = ScilabXgc->Blue;

  if (!XgcAllocColors(ScilabXgc,m)) {
    ScilabXgc->Colors = c;
    ScilabXgc->Red = r;
    ScilabXgc->Green = g;
    ScilabXgc->Blue = b;
    return;
  }

  /* Getting RGB values */
  for (i = 0; i < m; i++) {
    ScilabXgc->Red[i] = (float)default_colors[3*i]/255.0;
    ScilabXgc->Green[i] = (float)default_colors[3*i+1]/255.0;
    ScilabXgc->Blue[i] = (float)default_colors[3*i+2]/255.0;  
  }

  /* Black */
  ScilabXgc->Red[m] = 0;
  ScilabXgc->Green[m] = 0;
  ScilabXgc->Blue[m] = 0;

  /* White */
  ScilabXgc->Red[m+1] = 1;
  ScilabXgc->Green[m+1] = 1;
  ScilabXgc->Blue[m+1] = 1;

  cmap = XDefaultColormap(dpy,XDefaultScreen(dpy));

  for (i = 0; i < m; i++) {
    red = (unsigned int)default_colors[3*i];
    green = (unsigned int)default_colors[3*i+1];
    blue = (unsigned int)default_colors[3*i+2];
    pix = RGB2pix(red,green,blue,visual->red_mask,visual->green_mask,
		  visual->blue_mask);
    ScilabXgc->Colors[i] = pix;
  }

  XSetWindowColormap(dpy,ScilabXgc->CBGWindow,cmap);
  ScilabXgc->Cmap = cmap;
  ScilabXgc->Numcolors = m;
  ScilabXgc->IDLastPattern = m - 1;
  ScilabXgc->CmapFlag = 1;
  /* Black and white pixels */
  ScilabXgc->Colors[m] = ScilabXgc->Colors[DEFAULTBLACK];
  ScilabXgc->Colors[m+1] = ScilabXgc->Colors[DEFAULTWHITE];
  ScilabXgc->NumForeground = m;
  ScilabXgc->NumBackground = m + 1;
  XFlush(dpy);
  FREE(c); FREE(r); FREE(g); FREE(b);
}

/* Direct Color visuals */
void set_default_colormap2(int m)
{
  /* Use same code as Pseudo Color for the moment */
  set_default_colormap3(m);
}

/* Other visuals, mainly Pseudo Color */
void set_default_colormap3(int m)
{
  int i,j;
  int missing_col_mess=-1;
  Colormap cmap,dcmap,ocmap;
  XColor color;
  Pixel *pixels = (Pixel *) NULL;
  Pixel *c = ScilabXgc->Colors;   /* Save old colors components */
  float *r = ScilabXgc->Red;
  float *g = ScilabXgc->Green;
  float *b = ScilabXgc->Blue;
  
  int bp1,wp1;
  
  if (!XgcAllocColors(ScilabXgc,m)) {
    ScilabXgc->Colors = c;
    ScilabXgc->Red = r;
    ScilabXgc->Green = g;
    ScilabXgc->Blue = b;
    return;
  }

  if (!(pixels = (Pixel *) MALLOC(m*sizeof(Pixel)))) {
    Scistring("set_default_colormap: unable to alloc\n");
    XgcFreeColors(ScilabXgc);
    ScilabXgc->Colors = c;
    ScilabXgc->Red = r;
    ScilabXgc->Green = g;
    ScilabXgc->Blue = b;    
    return;
  }

  /* Getting RGB values */
  for (i = 0; i < m; i++) {
    ScilabXgc->Red[i] = (float)default_colors[3*i]/255.0;
    ScilabXgc->Green[i] = (float)default_colors[3*i+1]/255.0;
    ScilabXgc->Blue[i] = (float)default_colors[3*i+2]/255.0;  
  }
  /* Black */
  ScilabXgc->Red[m] = 0;
  ScilabXgc->Green[m] = 0;
  ScilabXgc->Blue[m] = 0;

  /* White */
  ScilabXgc->Red[m+1] = 1;
  ScilabXgc->Green[m+1] = 1;
  ScilabXgc->Blue[m+1] = 1;

  dcmap = XDefaultColormap(dpy,XDefaultScreen(dpy));
  ocmap = ScilabXgc->Cmap;

  /* If old colormap is the default colormap, free already
     allocated colors */
  if (ScilabXgc->Numcolors != 0 && ocmap == dcmap) {
    XFreeColors(dpy,dcmap,c,ScilabXgc->Numcolors,0);
  }

  /* First try to alloc readonly colors from the default colormap */
  for (i = 0; i < m; i++) {
    color.red = default_colors[3*i]<<8;
    color.green = default_colors[3*i+1]<<8;
    color.blue = default_colors[3*i+2]<<8;
    color.flags = DoRed|DoGreen|DoBlue;
    if (!XAllocColor(dpy,dcmap,&color))  {
      /* No enough room in default colormap, free allocated pixels */
      if (i != 0) XFreeColors(dpy,dcmap,pixels,i,0);
      break;
    }
    pixels[i] = color.pixel;
    ScilabXgc->Colors[i] = color.pixel;
  }
  
  cmap = dcmap;

  if (i < m - 1) 
    {
      /* Can't allocate all colors in default colormap; if old colormap was 
	 the default colormap, create a new one, otherwise use old one */
      if (ocmap == 0 || ocmap == dcmap) 
	{
	  /* Create a new private colormap */
	  missing_col_mess=i;
	  if ((cmap=XCreateColormap(dpy,ScilabXgc->CBGWindow,visual,AllocNone)) == 0) 
	    {
	      XgcFreeColors(ScilabXgc);
	      FREE(pixels);
	      ScilabXgc->Colors = c;
	      ScilabXgc->Red = r;
	      ScilabXgc->Green = g;
	      ScilabXgc->Blue = b;
	      sciprint("%d colors missing, switch to private colormap\r\n",m - i);
	      sciprint("Cannot allocate new colormap\r\n");
	      return;
	    }
	}
      else 
	{
	  /* Use old private colormap */
	  cmap = ocmap;
	  /* Free already allocated colors */
	  if (ScilabXgc->CmapFlag) 
	    XFreeColors(dpy,cmap,c,ScilabXgc->Numcolors,0);
	  else XFreeColors(dpy,cmap,c,ScilabXgc->Numcolors+2,0);
	}
      /* Try to alloc readwrite colors from the private colormap */
      for (i = 0; i < m; i++) 
	{
	  if (!XAllocColorCells(dpy,cmap,False,NULL,0,&pixels[i],1)) 
	    {
	      XgcFreeColors(ScilabXgc);
	      FREE(pixels);
	      ScilabXgc->Colors = c;
	      ScilabXgc->Red = r;
	      ScilabXgc->Green = g;
	      ScilabXgc->Blue = b;
	      sciprint("%d colors missing, unable to allocate colormap\r\n",m - i);
	      return;
	    }
	}
    
      color.flags = DoRed|DoGreen|DoBlue;

      /* First store white(wpixel) and black(bpixel) 
	 wpixel and bpixel are usually 0 and 1 or 1 and 0 */
      color.red = default_colors[3*DEFAULTWHITE]<<8;
      color.green = default_colors[3*DEFAULTWHITE+1]<<8;
      color.blue = default_colors[3*DEFAULTWHITE+2]<<8;
      if (wpixel == 1) wp1 = wpixel;
      else if (wpixel == 0) wp1 = wpixel;
      else if (bpixel == 0) wp1 = 1;
      else if (bpixel == 1) wp1 = 0;
      else wp1 =0;
      color.pixel = ScilabXgc->Colors[DEFAULTWHITE] = pixels[wp1];
      XStoreColor(dpy,cmap,&color);
      color.red = default_colors[3*DEFAULTBLACK]<<8;
      color.green = default_colors[3*DEFAULTBLACK+1]<<8;
      color.blue = default_colors[3*DEFAULTBLACK+2]<<8;   
      if (bpixel == 1) bp1 = bpixel;
      else if (bpixel == 0) bp1 = bpixel;
      else if (wpixel == 0) bp1 = 1;
      else if (wpixel == 1) bp1 = 0;
      else bp1 =1;
      color.pixel = ScilabXgc->Colors[DEFAULTBLACK] = pixels[bp1];
      XStoreColor(dpy,cmap,&color);
      j = 2;
      for (i = 0; i < m; i++) {
	if (i == DEFAULTBLACK || i == DEFAULTWHITE) continue;
	color.red = default_colors[3*i]<<8;
	color.green = default_colors[3*i+1]<<8;
	color.blue = default_colors[3*i+2]<<8;
	color.pixel = ScilabXgc->Colors[i] = pixels[j++];
	XStoreColor(dpy,cmap,&color);
      }
      /* Change decoration of graphics windows */
      ChangeBandF(ScilabXgc->CurWindow,pixels[bp1],pixels[wp1]);
    }

  if (ocmap != (Colormap)0 && ocmap != cmap && ocmap != dcmap) 
    XFreeColormap(dpy,ocmap);
  XSetWindowColormap(dpy,ScilabXgc->CBGWindow,cmap);
  ScilabXgc->Cmap = cmap;
  ScilabXgc->Numcolors = m;
  ScilabXgc->IDLastPattern = m - 1;
  ScilabXgc->CmapFlag = 1;
  /* Black and white pixels */
  ScilabXgc->Colors[m] = ScilabXgc->Colors[DEFAULTBLACK];
  ScilabXgc->Colors[m+1] = ScilabXgc->Colors[DEFAULTWHITE];
  ScilabXgc->NumForeground = m;
  ScilabXgc->NumBackground = m + 1;
  XFlush(dpy);
  FREE(c); FREE(r); FREE(g); FREE(b);
  FREE(pixels);
  /** Warning : sciprint induces Xevents evaluation and this can 
      lead to the executaion of a redraw graphic event so we must 
      be sure that ScilabXgc is in a correct state before caling 
      sciprint **/
  if (missing_col_mess != -1 ) 
    sciprint("%d colors missing, switch to private colormap\r\n",m - 
	     missing_col_mess);
}
void setcolormapg(struct  BCG *XGC,integer *v1,integer *v2, double *a, integer *v3);/* NG */
void setcolormap1(struct  BCG *XGC,integer m, double *a, integer *v3);
void setcolormap2(struct  BCG *XGC,integer m, double *a, integer *v3);
void setcolormap3(struct  BCG *XGC,integer m, double *a, integer *v3);

/* Setting the colormap 
   a must be a m x 3 double RGB matrix: 
     a[i] = RED
     a[i+m] = GREEN
     a[i+2*m] = BLUE
     *v2 gives the value of m and *v3 must be equal to 3 */
/* add *v3 (OUT) to know if colormap allocation has succeeded: */
/* 0: succeed */
/* 1: failed */
/* NG beg*/
static void xset_colormap(v1,v2,v3,v4,v5,v6,a)
     integer *v1,*v2;
     integer *v3;
     integer *v4,*v5,*v6;
     double *a;
{
  *v3 = 0;
  
  setcolormapg(ScilabXgc,v1,v2,a,v3);
}
static void xset_gccolormap(v1,v2,a,XGC,v3)
     integer *v1,*v2;
     double *a;
     struct BCG *XGC;
     integer *v3;
{

  setcolormapg(XGC,v1,v2,a,v3);
}
/* NG end*/

extern void setcolormapg(struct BCG *XGC,integer *v1, integer *v2, double *a, integer *v3)
{
  int m;
  /* 2 colors reserved for black and white */
  if (*v2 != 3 || *v1 < 0 || *v1 >(int) maxcol - 2) {
    sciprint("Colormap must be a m x 3 array with m <= %ld, previous one kept\r\n",maxcol-2);
    *v3 =1;
    return;
  }
  m = *v1;
  switch (visual->class) {
  case TrueColor:
    setcolormap1(XGC,m,a,v3);
    break;
  case DirectColor:
    setcolormap2(XGC,m,a,v3);
    break;
  default:
    setcolormap3(XGC,m,a,v3);
    break;
  }
}

/* True Color visuals */
void setcolormap1(struct BCG *Xgc,integer m, double *a, integer *v3) /*NG*/
{
  int i;
  Colormap cmap;
  unsigned int red, green, blue;
  Pixel pix;

  Pixel *c = Xgc->Colors;  /* Save old color vectors components */
  float *r = Xgc->Red;
  float *g = Xgc->Green;
  float *b = Xgc->Blue;
 
  if (!XgcAllocColors(ScilabXgc,m)) { /* Why ScilabXgc and not Xgc ?? F.Leray 26.08.04 */
    Xgc->Colors = c;
    Xgc->Red = r;
    Xgc->Green = g;
    Xgc->Blue = b;
    *v3 = 1;
    return;
  }

  /* Checking RGB values */
  for (i = 0; i < m; i++) {
    if (a[i] < 0 || a[i] > 1 || a[i+m] < 0 || a[i+m] > 1 ||
	a[i+2*m] < 0 || a[i+2*m]> 1) {
      Scistring("RGB values must be between 0 and 1\n");
      Xgc->Colors = c;
      Xgc->Red = r;
      Xgc->Green = g;
      Xgc->Blue = b;
      *v3 = 1;
      return;
    }
    Xgc->Red[i] = (float)a[i];
    Xgc->Green[i] = (float)a[i+m];
    Xgc->Blue[i] = (float)a[i+2*m];  
  }
  /* Black */
  Xgc->Red[m] = 0;
  Xgc->Green[m] = 0;
  Xgc->Blue[m] = 0;
  Xgc->Colors[m] =  RGB2pix(0,0,0,visual->red_mask,
				  visual->green_mask,visual->blue_mask);

  /* White */
  Xgc->Red[m+1] = 1;
  Xgc->Green[m+1] = 1;
  Xgc->Blue[m+1] = 1;
  Xgc->Colors[m+1] = RGB2pix(255,255,255,visual->red_mask,
				   visual->green_mask,visual->blue_mask);

  for (i = 0; i < m; i++) {
    red = (unsigned short)(a[i]*255.0);
    green = (unsigned short)(a[i+m]*255.0);
    blue = (unsigned short)(a[i+2*m]*255.0);
    pix = RGB2pix(red,green,blue,visual->red_mask,visual->green_mask,
		  visual->blue_mask);
    Xgc->Colors[i] = pix;
  }

  cmap = XDefaultColormap(dpy,XDefaultScreen(dpy));

  XSetWindowColormap(dpy,Xgc->CBGWindow,cmap);
  Xgc->Cmap = cmap;
  Xgc->Numcolors = m;
  Xgc->IDLastPattern = m - 1;
  Xgc->CmapFlag = 0;
  Xgc->NumForeground = m;
  Xgc->NumBackground = m + 1;
  xset_usecolor((i=1,&i) ,PI0,PI0,PI0);
  xset_alufunction1(&Xgc->CurDrawFunction,PI0,PI0,PI0);
  xset_pattern((i=Xgc->NumForeground+1,&i),PI0,PI0,PI0);  
  xset_foreground((i=Xgc->NumForeground+1,&i),PI0,PI0,PI0);
  xset_background((i=Xgc->NumForeground+2,&i),PI0,PI0,PI0);
  XFlush(dpy);
  FREE(c); FREE(r); FREE(g); FREE(b);
}

/* Direct Color visuals */
void setcolormap2(struct BCG *Xgc,integer m, double *a, integer *v3)
{
  /* Use same code as Pseudo Color for the moment */
  setcolormap3(Xgc,m,a,v3);
}

/* Other visuals, mainly Pseudo Color */
void setcolormap3(struct BCG *Xgc,integer m, double *a, integer *v3)
{
  int missing_col_mess=-1;
  int i;
  Colormap cmap,dcmap,ocmap;
  XColor color;
  int bp1,wp1;
  Pixel *pixels = (Pixel *) NULL;

  Pixel *c = Xgc->Colors;  /* Save old color vectors compônents */
  float *r = Xgc->Red;
  float *g = Xgc->Green;
  float *b = Xgc->Blue;

  if (!XgcAllocColors(Xgc,m)) {
    Xgc->Colors = c;
    Xgc->Red = r;
    Xgc->Green = g;
    Xgc->Blue = b;
    *v3 = 1;
    return;
  }

  if (!(pixels = (Pixel *) MALLOC((m+2)*sizeof(Pixel)))) {
    Scistring("setcolormap: unable to alloc\n");
    XgcFreeColors(Xgc);
    Xgc->Colors = c;
    Xgc->Red = r;
    Xgc->Green = g;
    Xgc->Blue = b;
    *v3 = 1;
    return;
  }

  /* Checking RGB values */
  for (i = 0; i < m; i++) {
    if (a[i] < 0 || a[i] > 1 || a[i+m] < 0 || a[i+m] > 1 ||
	a[i+2*m] < 0 || a[i+2*m]> 1) {
      Scistring("RGB values must be between 0 and 1\n");
      Xgc->Colors = c;
      Xgc->Red = r;
      Xgc->Green = g;
      Xgc->Blue = b;
      *v3 = 1;
      return;
    }
    Xgc->Red[i] = (float)a[i];
    Xgc->Green[i] = (float)a[i+m];
    Xgc->Blue[i] = (float)a[i+2*m];  
  }
  /* Black */
  Xgc->Red[m] = 0;
  Xgc->Green[m] = 0;
  Xgc->Blue[m] = 0;

  /* White */
  Xgc->Red[m+1] = 1;
  Xgc->Green[m+1] = 1;
  Xgc->Blue[m+1] = 1;

  dcmap = XDefaultColormap(dpy,XDefaultScreen(dpy));
  ocmap = Xgc->Cmap;

  /* If old colormap is the default colormap, free already
     allocated colors */
  if (Xgc->Numcolors!= 0 && ocmap == dcmap) {
    XFreeColors(dpy,dcmap,c,Xgc->Numcolors,0);
  }

  color.flags = DoRed|DoGreen|DoBlue;

  /* First try to alloc readonly colors from the default colormap 
     We begin with white(wpixel) and black(bpixel) 
     wpixel and bpixel are usually 0 and 1 or 1 and 0 */
  if (wpixel == 1) wp1 = wpixel;
  else if (wpixel == 0) wp1 = wpixel;
  else if (bpixel == 0) wp1 = 1;
  else if (bpixel == 1) wp1 = 0;
  else wp1 =0;
  color.red = default_colors[3*DEFAULTWHITE]<<8;
  color.green = default_colors[3*DEFAULTWHITE+1]<<8;
  color.blue = default_colors[3*DEFAULTWHITE+2]<<8;   
  XAllocColor(dpy,dcmap,&color);
  Xgc->Colors[m+1] = pixels[wp1] = color.pixel;

  if (bpixel == 1) bp1 = bpixel;
  else if (bpixel == 0) bp1 = bpixel;
  else if (wpixel == 0) bp1 = 1;
  else if (wpixel == 1) bp1 = 0;
  else bp1 =1;
  color.red = default_colors[3*DEFAULTBLACK]<<8;
  color.green = default_colors[3*DEFAULTBLACK+1]<<8;
  color.blue = default_colors[3*DEFAULTBLACK+2]<<8;   
  XAllocColor(dpy,dcmap,&color);
  Xgc->Colors[m] = pixels[bp1] = color.pixel;

  for (i = 2; i < m+2; i++) {
    color.red = (unsigned short)(a[i-2]*65535.0);
    color.green = (unsigned short)(a[i-2+m]*65535.0);
    color.blue = (unsigned short)(a[i-2+2*m]*65535.0);
    if (!XAllocColor(dpy,dcmap,&color))  {
      /* No enough room in default colormap, free allocated pixels */
      if (i != 0) XFreeColors(dpy,dcmap,pixels,i,0);
      break;
    }
    Xgc->Colors[i-2] = pixels[i] = color.pixel;
  }
  
  cmap = dcmap;

  if (i < (m+2) - 1) {
    /* Can't allocate all colors in default colormap; if old colormap was 
       the default colormap, create a new one, otherwise use old one */
    if (ocmap == 0 || ocmap == dcmap) {
      /* Create a new private colormap */
      missing_col_mess=i;
      if ((cmap = XCreateColormap(dpy,Xgc->CBGWindow,visual,AllocNone)) == 0) {
	XgcFreeColors(Xgc);
	FREE(pixels);
	Xgc->Colors = c;
	Xgc->Red = r;
	Xgc->Green = g;
	Xgc->Blue = b;
	sciprint("%d colors missing, switch to private colormap\r\n",m+2 - i);
	sciprint("Cannot allocate new colormap\n");
	return;
      }
    } else {
      /* Use old private colormap */
      cmap = ocmap;
      /* Free already allocated colors */
      if (Xgc->CmapFlag)
	XFreeColors(dpy,cmap,c,Xgc->Numcolors,0);
      else XFreeColors(dpy,cmap,c,Xgc->Numcolors+2,0);
    }
    /* Try to alloc readwrite colors from the private colormap */
    for (i = 0; i < m+2; i++) {
      if (!XAllocColorCells(dpy,cmap,False,NULL,0,&pixels[i],1)) {
	/* sciprint is dangerous here we use wininfo : see the Warning bellow*/
	XgcFreeColors(Xgc);
	FREE(pixels);
	Xgc->Colors = c;
	Xgc->Red = r;
	Xgc->Green = g;
	Xgc->Blue = b;
	sciprint("%d colors missing, unable to allocate colormap\r\n",m+2 - i);
	return;
      }
    }
      
    color.flags = DoRed|DoGreen|DoBlue;
    /* First store white(wpixel) and black(bpixel) 
       wpixel and bpixel are usually 0 and 1 or 1 and 0 */
    if (wpixel == 1) wp1 = wpixel;
    else if (wpixel == 0) wp1 = wpixel;
    else if (bpixel == 0) wp1 = 1;
    else if (bpixel == 1) wp1 = 0;
    else wp1 =0;
    color.red = 65535; color.green = 65535; color.blue = 65535;
    color.pixel = Xgc->Colors[m+1] = pixels[wp1];
    XStoreColor(dpy,cmap,&color);
    if (bpixel == 1) bp1 = bpixel;
    else if (bpixel == 0) bp1 = bpixel;
    else if (wpixel == 0) bp1 = 1;
    else if (wpixel == 1) bp1 = 0;
    else bp1 =1;
    color.red = 0; color.green = 0; color.blue = 0;
    color.pixel = Xgc->Colors[m] = pixels[bp1];    
    XStoreColor(dpy,cmap,&color);    
    for (i = 0; i < m; i++) {
      color.red = (unsigned short)(a[i]*65535.0);
      color.green = (unsigned short)(a[i+m]*65535.0);
      color.blue = (unsigned short)(a[i+2*m]*65535.0);      
      color.pixel = Xgc->Colors[i] = pixels[i+2];
      XStoreColor(dpy,cmap,&color);     
    }
    /* Change decoration of graphics windows */
    ChangeBandF(Xgc->CurWindow,pixels[bp1],pixels[wp1]);
  }

  if (ocmap != (Colormap)0 && ocmap != cmap && ocmap != dcmap) 
    XFreeColormap(dpy,ocmap);
  XSetWindowColormap(dpy,Xgc->CBGWindow,cmap);
  Xgc->Cmap = cmap;
  Xgc->Numcolors = m;
  Xgc->IDLastPattern = m - 1;
  Xgc->CmapFlag = 0;
  Xgc->NumForeground = m;
  Xgc->NumBackground = m + 1;
  xset_usecolor((i=1,&i) ,PI0,PI0,PI0);
  xset_alufunction1(&Xgc->CurDrawFunction,PI0,PI0,PI0);
  xset_pattern((i=Xgc->NumForeground+1,&i),PI0,PI0,PI0);  
  xset_foreground((i=Xgc->NumForeground+1,&i),PI0,PI0,PI0);
  xset_background((i=Xgc->NumForeground+2,&i),PI0,PI0,PI0);
  XFlush(dpy);
  FREE(c); FREE(r); FREE(g); FREE(b);
  FREE(pixels);
  if (missing_col_mess != -1) 
    sciprint("%d colors missing, switch to private colormap\r\n",m+2 - missing_col_mess);
}

/* getting the colormap size */

static void xget_colormap_size(integer *verbose, integer *num, integer *narg, double *val)
{
  if(ScilabXgc != NULL)
    *num =  ScilabXgc->Numcolors;
  else
    {
      sciprint("No ScilabXgc allocated\n");
      *num = 0;
      return;
    }
}

/* getting the colormap */

static void xget_colormap(integer *verbose, integer *num, integer *narg, double *val)
{
  int m = ScilabXgc->Numcolors;
  int i;
  *narg = 1;
  *num = m;
  for (i = 0; i < m; i++) {
    val[i] = (double)ScilabXgc->Red[i];
    val[i+m] = (double)ScilabXgc->Green[i];
    val[i+2*m] = (double)ScilabXgc->Blue[i];
  }
  if (*verbose == 1) {
    sciprint("Size of colormap: %d colors\r\n",m);
  }
}

/** checks if the current colormap is a private one **/

int IsPrivateCmap(void)
{
  int screen;
  if (dpy == (Display *) NULL) return(0);
  screen = XDefaultScreen(dpy);
  if (ScilabXgc == (struct BCG *)0 ) return(0);
  if ( ScilabXgc->Cmap ==  XDefaultColormap(dpy,screen)) return(0);
  else return(1);
}


/** set and get the number of the background or foreground */

static void xset_background(integer *num, integer *v2, integer *v3, integer *v4)
{ 
  if (ScilabXgc->CurColorStatus == 1) 
    {
      /* 
       * if we change the background of the window we must change 
       * the gc ( with alufunction ) and the window background 
       */
      Pixel px;
      ScilabXgc->NumBackground = Max(0,Min(*num - 1,ScilabXgc->Numcolors + 1));
      xset_alufunction1(&ScilabXgc->CurDrawFunction,PI0,PI0,PI0);
      px = (ScilabXgc->Colors == NULL) ? DefaultBackground 
	:  ScilabXgc->Colors[ScilabXgc->NumBackground];
      
      XSetWindowBackground(dpy, ScilabXgc->CWindow,px);
      /*if(ScilabXgc->Cdrawable != (Drawable) 0 ) 
	XSetWindowBackground(dpy, ScilabXgc->Cdrawable,px);*/
    }
}
 
static void xget_background(integer *verbose, integer *num, integer *narg, double *dummy)
{ 
  *narg=1;
  if ( ScilabXgc->CurColorStatus == 1 ) 
    {
      *num = ScilabXgc->NumBackground + 1;
    }
  else 
    {
      *num = 1;
    }
  if (*verbose == 1) 
    sciprint("\n Background : %d\r\n",*num);
}

/* set and get the number of the background or foreground */

static void xset_foreground(integer *num, integer *v2, integer *v3, integer *v4)
{ 
  if (ScilabXgc->CurColorStatus == 1) 
    {
      Pixel px;
      ScilabXgc->NumForeground = Max(0,Min(*num - 1,ScilabXgc->Numcolors + 1));
      xset_alufunction1(&ScilabXgc->CurDrawFunction,PI0,PI0,PI0);
      px = (ScilabXgc->Colors == NULL) ? DefaultForeground 
	:  ScilabXgc->Colors[ScilabXgc->NumForeground];
      /*** XXXXXXX attention regarder le mode pixmap ****/
      /** n''existe pas XSetWindowForeground(dpy, ScilabXgc->CWindow,px); **/
    }
}

static void xget_foreground(integer *verbose, integer *num, integer *narg, double *dummy)
{ 
  *narg=1;
  if ( ScilabXgc->CurColorStatus == 1 ) 
    {
      *num = ScilabXgc->NumForeground + 1;
    }
  else 
    {
      *num = 1; /** the foreground is a solid line style in b&w */
    }
  if (*verbose == 1) 
    sciprint("\n Foreground : %d\r\n",*num);
}

/** set and get the number of the hidden3d color */

static void xset_hidden3d(integer *num, integer *v2, integer *v3, integer *v4)
{ 
  if (ScilabXgc->CurColorStatus == 1) 
    {
      /* e Segre: Max(0,... -> Max(-1,... */
      /* S Mottelet: Max(-1,... -> Max(-2,...  to take into account the value -1 */
      ScilabXgc->NumHidden3d = Max(-2,Min(*num - 1,ScilabXgc->Numcolors + 1));
    }
}

static void xget_hidden3d(integer *verbose, integer *num, integer *narg, double *dummy)
{ 
  *narg=1;
  if ( ScilabXgc->CurColorStatus == 1 ) 
    {
      *num = ScilabXgc->NumHidden3d + 1;
    }
  else 
    {
      *num = 1; /** the hidden3d is a solid line style in b&w */
    }
  if (*verbose == 1) 
    sciprint("\n Hidden3d : %d\r\n",*num);
}

/**********************************************************
 * Used in xsetm()
 *    to see the colormap of current graphic window
 ******************************************************/

void set_cmap(Window w)
{
  if ( ScilabXgc != (struct BCG *) 0 && ScilabXgc->Cmap != (Colormap)0)
    XSetWindowColormap(dpy,w,ScilabXgc->Cmap);
}

int get_pixel(int i)
{
  if ( ScilabXgc != (struct BCG *) 0 && ScilabXgc->Cmap != (Colormap)0)
    return(ScilabXgc->Colors[Max(Min(i,ScilabXgc->Numcolors + 1),0)]);
  else 
    return(0);
}

Pixmap get_pixmap(int i)
{
  return(Tabpix_[ Max(0,Min(i - 1,GREYNUMBER - 1))]);
}

/*****************************************************
 * return 1 : if the current window exists 
 *            and its colormap is not the default 
 *            colormap (the number of colors is returned in m
 * else return 0 
 *****************************************************/

int CheckColormap(int *m)
{
  if (  ScilabXgc != (struct BCG *) 0 ) 
    {
      *m =  ScilabXgc->Numcolors;
      if ( ScilabXgc->CmapFlag  != 1) 
	return 1;
      else 
	return 0;
    }
  else 
    { 
      *m=0;
      return(0);
    }
}

void get_r(int i, float *r)
{
  *r = ScilabXgc->Red[i];
}
void  get_g(int i, float *g)
{
  *g = ScilabXgc->Green[i];
}
void get_b(int i, float *b)
{
  *b = ScilabXgc->Blue[i];
}

/*-----------------------------------------------------------
 * general routines accessing the previous  set<> or get<> 
 * 
 *-----------------------------------------------------------*/

static void InitMissileXgc(integer *v1, integer *v2, integer *v3, integer *v4);
static void SetGraphicsVersion();


static void xset_empty(integer *verbose, integer *v2, integer *v3, integer *v4)
{
  if ( *verbose ==1 ) Scistring("\n No operation ");
}

static void xget_empty(integer *verbose, integer *v2, integer *v3, double *dummy)
{
  if ( *verbose ==1 ) Scistring("\n No operation ");
}

#define NUMSETFONC 33

/** Table in lexicographic order **/

static struct bgc { char *name ;
  void  (*setfonc )() ;
  void  (*getfonc )() ;}

MissileGCTab_[] = {
  {"alufunction",xset_alufunction1,xget_alufunction},
  {"background",xset_background,xget_background},
  {"clipoff",xset_unclip,xget_clip},
  {"clipping",xset_clip,xget_clip},
  {"cmap_size",xset_empty,xget_colormap_size},
  {"color",xset_pattern,xget_pattern},
  {"colormap",xset_colormap,xget_colormap},
  {"dashes",xset_dash_or_color,xget_dash_or_color}, /* obsolet */ 
  {"default",InitMissileXgc, xget_empty},
  {"figure",xset_scilabFigure,xget_scilabFigure},/* NG */
  {"font",xset_font,xget_font},
  {"foreground",xset_foreground,xget_foreground},
  {"gc",xset_scilabxgc,xget_scilabxgc},/* NG */
  {"gccolormap",xset_gccolormap,xget_colormap}, /* NG */
  {"hidden3d",xset_hidden3d,xget_hidden3d},
  {"lastpattern",xset_empty,xget_last},
  {"line mode",xset_absourel,xget_absourel},
  {"line style",xset_dash,xget_dash},
  {"mark",xset_mark,xget_mark},
  {"pattern",xset_pattern,xget_pattern},
  {"pixmap",xset_pixmapOn,xget_pixmapOn},
  {"thickness",xset_thickness,xget_thickness},
  {"use color",xset_usecolor,xget_usecolor},
  {"version",xset_scilabVersion,xget_scilabVersion},/* NG */
  {"viewport",xset_viewport,xget_viewport},
  {"wdim",xset_windowdim,xget_windowdim},
  {"white",xset_empty,xget_last},
  {"window",xset_curwin,xget_curwin},
  {"wpdim",xset_popupdim,xget_popupdim},
  {"wpos",xset_windowpos,xget_windowpos},
  {"wresize",xset_wresize,xget_wresize},
  {"wshow",xset_show,xget_empty},
  {"wwpc",xset_pixmapclear,xget_empty}
};

void C2F(MissileGCget)(char *str, integer *verbose, integer *x1, integer *x2, integer *x3, integer *x4, integer *x5, double *dv1, double *dv2, double *dv3, double *dv4)
{ 
  int x6=0;
  C2F(MissileGCGetorSet)(str,(integer)1L,verbose,x1,x2,x3,x4,x5,&x6,dv1);
}

void C2F(MissileGCset)(char *str, integer *x1, integer *x2, integer *x3, integer *x4, integer *x5, integer *x6, double *dv1, double *dv2, double *dv3, double *dv4)
{
  integer verbose=0 ;
  C2F(MissileGCGetorSet)(str,(integer)0L,&verbose,x1,x2,x3,x4,x5,x6,dv1);
}

void C2F(MissileGCGetorSet)(char *str, integer flag, integer *verbose, integer *x1, integer *x2, integer *x3, integer *x4, integer *x5, integer *x6, double *dv1)
{ 
  integer i ;
  for (i=0; i < NUMSETFONC ; i++)
    {
      integer j;
      j = strcmp(str,MissileGCTab_[i].name);
      if ( j == 0 ) 
	{ if (*verbose == 1)
	  sciprint("\nGetting Info on %s\r\n",str);
	if (flag == 1)
	  (MissileGCTab_[i].getfonc)(verbose,x1,x2,dv1);
	else 
	  (MissileGCTab_[i].setfonc)(x1,x2,x3,x4,x5,x6,dv1);
	return;}
      else 
	{ if ( j <= 0)
	  {
	    sciprint("\nUnknow X operator <%s>\r\n",str);
	    if ( flag == 1) 
	      {
		/** set x1 and x2 they are used in scixget 
		    to size the return variable  **/
		*x1=1;*x2=0;
	      }
	    return;
	  }
	}
    }
  sciprint("\n Unknow X operator <%s>\r\n",str);
  *x1=1;*x2=0;
}

/* Functions for drawing */

/**************************************************
 *  display of a string
 *  at (x,y) position whith slope angle alpha in degree . 
 * Angle are given clockwise. 
 * If *flag ==1 and angle is z\'ero a framed box is added 
 * around the string}.
 * 
 * (x,y) defines the lower left point of the bounding box 
 * of the string ( we do not separate asc and desc 
 **************************************************/

void C2F(displaystring)(char *string, integer *x, integer *y, integer *v1, integer *flag, integer *v6, integer *v7, double *angle, double *dv2, double *dv3, double *dv4)
{ 
  if ( Abs(*angle) <= 0.1) 
    {
      int dir,asc,dsc,xpos;
      XCharStruct charret;
      if(strlen(string) !=0) {
	XQueryTextExtents(dpy,ScilabXgc->FontXID,
			  string,strlen(string),&dir,&asc,&dsc,&charret);
	xpos= *x+ (charret.width)/(2.0*strlen(string));
	if(ScilabXgc->Cdrawable != (Drawable) 0 ) 
	  XDrawString(dpy, ScilabXgc->Cdrawable,gc,(int) *x,(int) *y-charret.descent,
		      string,strlen(string));
	if(ScilabXgc->CurPixmapStatus !=1)
	  XDrawString(dpy, (Drawable) ScilabXgc->CWindow,gc,(int) *x,(int) *y-charret.descent,
		      string,strlen(string));

	if ( *flag == 1) 
	  {
	    integer rect[4];
	    rect[0]= *x ;
	    rect[1]= *y-charret.ascent-charret.descent;
	    rect[2]= charret.width;
	    rect[3]= charret.ascent+charret.descent;
	    C2F(drawrectangle)(string,rect,rect+1,rect+2,rect+3,
			       PI0,PI0,PD0,PD0,PD0,PD0);
	  }
      }
    }
  else 
    C2F(DispStringAngle)(x,y,string,angle);
  XFlush(dpy);
}

void C2F(DispStringAngle)(integer *x, integer *y, char *string, double *angle)
{
  float ang;
  XFontStruct *font;

  font=FontsList_[ScilabXgc->FontId][ScilabXgc->FontSize];
  ang= - *angle * M_PI/180.0;

  if (ScilabXgc->Cdrawable != (Drawable) 0)	
    XRotDrawString(dpy, font, ang, ScilabXgc->Cdrawable, gc, *x, *y, string);
  if (ScilabXgc->CurPixmapStatus != 1) 
    XRotDrawString(dpy, font, ang, (Drawable) ScilabXgc->CWindow, gc, *x, *y, string);
}

void C2F(DispStringAngleold)(integer *x0, integer *yy0, char *string, double *angle)
{
  int i;
  integer x,y, rect[4];
  double sina ,cosa,l;
  char str1[2];
  str1[1]='\0';
  x= *x0;
  y= *yy0;
  sina= sin(*angle * M_PI/180.0);
  cosa= cos(*angle * M_PI/180.0);
  for ( i = 0 ; i < (int)strlen(string); i++)
    { 
      str1[0]=string[i];
      if (ScilabXgc->Cdrawable != (Drawable) 0) 
	XDrawString(dpy,ScilabXgc->Cdrawable,gc,(int) x,(int) y ,str1,1);
      if (ScilabXgc->CurPixmapStatus != 1) 
	XDrawString(dpy,(Drawable) ScilabXgc->CWindow,gc,(int) x,(int) y ,str1,1);

      C2F(boundingbox)(str1,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
      /** C2F(drawrectangle)(string,rect,rect+1,rect+2,rect+3); **/
      if ( cosa <= 0.0 && i < (int)strlen(string)-1)
	{ char str2[2];
	/** si le cosinus est negatif le deplacement est a calculer **/
	/** sur la boite du caractere suivant **/
	str2[1]='\0';str2[0]=string[i+1];
	C2F(boundingbox)(str2,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
	}
      if ( Abs(cosa) >= 1.e-8 )
	{
	  if ( Abs(sina/cosa) <= Abs(((double)rect[3])/((double)rect[2])))
	    l = Abs(rect[2]/cosa);
	  else 
	    l = Abs(rect[3]/sina);
	}
      else 
	l = Abs(rect[3]/sina);
      x +=  cosa*l*1.1;
      y +=  sina*l*1.1;
    }
}

/** To get the bounding rectangle of a string **/

void C2F(boundingbox)(char *string, integer *x, integer *y, integer *rect, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{ 
  int dir,asc,dsc;
  XCharStruct charret;
  if (strlen(string) !=0) {
  XQueryTextExtents(dpy,ScilabXgc->FontXID,
		    string,strlen(string),&dir,&asc,&dsc,&charret);
  rect[0]= *x ;
  rect[1]= *y-charret.ascent-charret.descent;
  rect[2]= charret.width;
  rect[3]= charret.ascent+charret.descent;
  }
  else {
    rect[0]= *x ;
    rect[1]= *y;
    rect[2]= 0;
    rect[3]= 0;
  }
}

/*------------------------------------------------
subsection{ Segments and Arrows }
-------------------------------------------------*/

void C2F(drawline)(integer *x1, integer *yy1, integer *x2, integer *y2)
{
  if (ScilabXgc->Cdrawable != (Drawable) 0) 
    XDrawLine(dpy, ScilabXgc->Cdrawable, gc,(int) *x1,(int) *yy1,(int) *x2,(int) *y2); 
  if (ScilabXgc->CurPixmapStatus != 1) 
    XDrawLine(dpy,(Drawable) ScilabXgc->CWindow, gc,(int) *x1,(int) *yy1,(int) *x2,(int) *y2); 
  XFlush(dpy);
}

/** Draw a set of segments **/
/** segments are defined by (vx[i],vy[i])->(vx[i+1],vy[i+1]) **/
/** for i=0 step 2 **/
/** n is the size of vx and vy **/

void C2F(drawsegments)(char *str, integer *vx, integer *vy, integer *n, integer *style, integer *iflag, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{
  integer verbose=0,Dnarg,Dvalue[10],NDvalue;
  integer i;
  xget_dash_and_color(&verbose,Dvalue,&Dnarg,vdouble);

  if ( (int) *iflag == 1) { /* one style per segment */
    for (i=0 ; i < *n/2 ; i++) {
      NDvalue = style[i];
      xset_line_style(&NDvalue,PI0,PI0,PI0);
      if (ScilabXgc->Cdrawable != (Drawable) 0) 
	XDrawLine(dpy,ScilabXgc->Cdrawable,gc, (int) vx[2*i],
		  (int) vy[2*i],(int) vx[2*i+1],(int) vy[2*i+1]) ;
      if (ScilabXgc->CurPixmapStatus != 1) 
	XDrawLine(dpy,(Drawable) ScilabXgc->CWindow,gc, (int) vx[2*i],
		  (int) vy[2*i],(int) vx[2*i+1],(int) vy[2*i+1]) ;
      XFlush(dpy);
    }
  }
  else {
    if (*style >= 1) /* set color */
      xset_line_style(style,PI0,PI0,PI0);

    for (i=0 ; i < *n/2 ; i++) {
      if (ScilabXgc->Cdrawable != (Drawable) 0) 
	XDrawLine(dpy,ScilabXgc->Cdrawable,gc, (int) vx[2*i],
		  (int) vy[2*i],(int) vx[2*i+1],(int) vy[2*i+1]) ;
      if (ScilabXgc->CurPixmapStatus != 1) 
	XDrawLine(dpy,(Drawable) ScilabXgc->CWindow,gc, (int) vx[2*i],
		  (int) vy[2*i],(int) vx[2*i+1],(int) vy[2*i+1]) ;
      XFlush(dpy);
    }
  }

  xset_dash_and_color( Dvalue,PI0,PI0,PI0);
}

/* Draw a set of arrows 
 * arrows are defined by (vx[i],vy[i])->(vx[i+1],vy[i+1]) 
 * for i=0 step 2 
 * n is the size of vx and vy 
 * as is 10*arsize (arsize) the size of the arrow head in pixels 
 */

void C2F(drawarrows)(char *str, integer *vx, integer *vy, integer *n, integer *as, integer *style, integer *iflag, double *dv1, double *dv2, double *dv3, double *dv4)
{ 
  integer verbose=0,Dnarg,Dvalue[10],NDvalue,i;
  double cos20=cos(20.0*M_PI/180.0);
  double sin20=sin(20.0*M_PI/180.0);
  integer polyx[4],polyy[4];
  xget_dash_and_color(&verbose,Dvalue,&Dnarg,vdouble);
  for (i=0 ; i < *n/2 ; i++)
    { 
      double dx,dy,norm;
      NDvalue = ( (int) *iflag == 1) ? style[i] : ((*style >= 1) ?  *style : Dvalue[0]);
      xset_line_style(&NDvalue,PI0,PI0,PI0);
      dx=( vx[2*i+1]-vx[2*i]);
      dy=( vy[2*i+1]-vy[2*i]);
      norm = sqrt(dx*dx+dy*dy);
      if ( Abs(norm) >  SMDOUBLE ) 
	{ integer nn=1,p=3;
	dx=(*as/10.0)*dx/norm;dy=(*as/10.0)*dy/norm;
	polyx[0]= polyx[3]=vx[2*i+1];
	polyx[1]= inint(polyx[0]  - cos20*dx -sin20*dy );
	polyx[2]= inint(polyx[0]  - cos20*dx + sin20*dy);
	polyy[0]= polyy[3]=vy[2*i+1];
	polyy[1]= inint(polyy[0] + sin20*dx -cos20*dy) ;
	polyy[2]= inint(polyy[0] - sin20*dx - cos20*dy) ;
	C2F(fillpolylines)("v",polyx,polyy,&NDvalue,&nn,&p,PI0,PD0,PD0,PD0,PD0);
	}
      if (ScilabXgc->Cdrawable != (Drawable) 0)
	XDrawLine(dpy,ScilabXgc->Cdrawable,gc,(int) vx[2*i],(int)vy[2*i],
		  (int)(vx[2*i+1]-dx*cos20),(int)(vy[2*i+1]-dy*cos20));
      if (ScilabXgc->CurPixmapStatus != 1) 
	XDrawLine(dpy,(Drawable) ScilabXgc->CWindow,gc,(int) vx[2*i],(int)vy[2*i],
		   (int)(vx[2*i+1]-dx*cos20),(int)(vy[2*i+1]-dy*cos20));
      
    }
  xset_dash_and_color( Dvalue,PI0,PI0,PI0);
  XFlush(dpy);
}

/*
 * Rectangles
 * Draw or fill a set of rectangle 
 * rectangle i is specified by (vect[i],vect[i+1],vect[i+2],vect[i+3]) 
 * for x,y,width,height 
 * for i=0 step 4 
 * (*n) : number of rectangles 
 * fillvect[*n] : specify the action  
 * if fillvect[i] is > 0 then fill the rectangle i 
 * if fillvect[i] is == 0  then only draw the rectangle i 
 *                         with the current drawing style 
 * if fillvect[i] is < 0 then draw the  rectangle with -fillvect[i] 
 */

void C2F(drawrectangles)(char *str, integer *vects, integer *fillvect, integer *n, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{
  integer i,cpat,verbose=0,num,cd[10];
  xget_pattern(&verbose,&cpat,&num,vdouble);
  xget_dash_and_color(&verbose,cd,&num,vdouble);
  for (i = 0 ; i < *n ; i++)
    {
      if ( fillvect[i] < 0 )
	{
	  int dash = - fillvect[i];
	  xset_line_style(&dash,PI0,PI0,PI0);
	  C2F(drawrectangle)(str,vects+4*i,vects+4*i+1,vects+4*i+2,vects+4*i+3
			     ,PI0,PI0,PD0,PD0,PD0,PD0);
	}
      else if ( fillvect[i] == 0 ) 
	{
	  /* xset_line_style(cd,PI0,PI0,PI0);*/
	  C2F(drawrectangle)(str,vects+4*i,vects+4*i+1,vects+4*i+2,vects+4*i+3
			     ,PI0,PI0,PD0,PD0,PD0,PD0);
	}
      else
	{
	  xset_pattern(&(fillvect[i]),PI0,PI0,PI0);
	  C2F(fillrectangle)(str,vects+4*i,vects+4*i+1,vects+4*i+2,vects+4*i+3,PI0,PI0,PD0,PD0,PD0,PD0);
	}
    }
  xset_dash_and_color(cd,PI0,PI0,PI0);
}

/** Draw one rectangle with current line style **/

void C2F(drawrectangle)(char *str, integer *x, integer *y, integer *width, integer *height, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{ 
  if (ScilabXgc->Cdrawable != (Drawable) 0)
    XDrawRectangle(dpy, ScilabXgc->Cdrawable, gc, *x, *y, (unsigned)*width,(unsigned)*height);
  if (ScilabXgc->CurPixmapStatus != 1) 
    XDrawRectangle(dpy, (Drawable) ScilabXgc->CWindow, gc, *x, *y, (unsigned)*width,(unsigned)*height);
  XFlush(dpy); }

/** fill one rectangle, with current pattern **/

void C2F(fillrectangle)(char *str, integer *x, integer *y, integer *width, integer *height, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{ 
  if (ScilabXgc->Cdrawable != (Drawable) 0)
    XFillRectangle(dpy, ScilabXgc->Cdrawable, gc,(int) *x,(int) *y,(unsigned) *width,(unsigned) *height); 
  if (ScilabXgc->CurPixmapStatus != 1) 
    XFillRectangle(dpy, (Drawable) ScilabXgc->CWindow, gc,(int) *x,(int) *y,(unsigned) *width,(unsigned) *height); 
  XFlush(dpy);
}

/*----------------------------------------------------------------------------------
 * draw a set of rectangles, provided here to accelerate GraySquare for X11 device 
 *  x : of size n1 gives the x-values of the grid 
 *  y : of size n2 gives the y-values of the grid 
 *  z : is the value of a function on the grid defined by x,y 
 *  on each rectangle the average value of z is computed 
 *----------------------------------------------------------------------------------*/

void fill_grid_rectangles(integer *x, integer *y, double *z, integer n1, integer n2)
{
  double zmoy,zmax,zmin,zmaxmin;
  integer i,j,verbose=0,whiteid,narg,fill[1],cpat,xz[2];
  zmin=Mini(z,(n1)*(n2));
  zmax=Maxi(z,(n1)*(n2));
  zmaxmin=zmax-zmin;
  if (zmaxmin <= SMDOUBLE) zmaxmin=SMDOUBLE;
  
  xget_last(&verbose,&whiteid,&narg,vdouble);
  xget_pattern(&verbose,&cpat,&narg,vdouble);
  xget_windowdim(&verbose,xz,&narg,vdouble);

  for (i = 0 ; i < (n1)-1 ; i++)
    for (j = 0 ; j < (n2)-1 ; j++)
      {
	integer w,h;
	zmoy=1/4.0*(z[i+n1*j]+z[i+n1*(j+1)]+z[i+1+n1*j]+z[i+1+n1*(j+1)]);
	fill[0]=1 + inint((whiteid-1)*(zmoy-zmin)/(zmaxmin));
	/* if (x[n1] == 1)  fill[0]= inint(z[j+ (i*n2)]);*/ /* NG */
	xset_pattern(fill,PI0,PI0,PI0);
        w=Abs(x[i+1]-x[i]);h=Abs(y[j+1]-y[j]);
	/* We don't trace rectangle which are totally out **/
	if ( w != 0 && h != 0 && x[i] < xz[0] && y[j+1] < xz[1] && x[i]+w > 0 && y[j+1]+h > 0 )
	  if ( Abs(x[i]) < int16max && Abs(y[j+1]) < int16max && w < uns16max && h < uns16max) {
	    if (ScilabXgc->Cdrawable != (Drawable) 0) 
	      XFillRectangle(dpy,ScilabXgc->Cdrawable,gc,x[i],y[j+1],w,h); 
	    if (ScilabXgc->CurPixmapStatus != 1) 
	      XFillRectangle(dpy,(Drawable) ScilabXgc->CWindow,gc,x[i],y[j+1],w,h); 
	  }
      }
  xset_pattern(&cpat,PI0,PI0,PI0);
}

/*----------------------------------------------------------------------------------
 * draw a set of rectangles, provided here to accelerate GraySquare1 for X11 device 
 *  x : of size n1 gives the x-values of the grid 
 *  y : of size n2 gives the y-values of the grid 
 *  z : of size (n1-1)*(n2-1)  gives the f-values on the middle 
 *  of each rectangle. 
 *  z[i,j] is the value on the middle of rectangle 
 *        P1= x[i],y[j] x[i+1],y[j+1]
 *----------------------------------------------------------------------------------*/

void fill_grid_rectangles1(integer *x, integer *y, double *z, integer n1, integer n2)
{
  integer i,j,verbose=0,narg,fill[1],cpat,xz[2];
  xget_pattern(&verbose,&cpat,&narg,vdouble);
  xget_windowdim(&verbose,xz,&narg,vdouble);
  for (i = 0 ; i < (n1)-1 ; i++)
    for (j = 0 ; j < (n2)-1 ; j++)
      {
	integer w,h;
	fill[0]= z[i+(n1-1)*j];
	xset_pattern(fill,PI0,PI0,PI0);
	w=Abs(x[j+1]-x[j]);
	h=Abs(y[i+1]-y[i]);
	/* We don't trace rectangle which are totally out **/
	if ( w != 0 && h != 0 && x[j] < xz[0] && y[i] < xz[1] && x[j]+w > 0 && y[i]+h > 0 )
	  if ( Abs(x[j]) < int16max && Abs(y[i+1]) < int16max && w < uns16max && h < uns16max) {
	    if (ScilabXgc->Cdrawable != (Drawable) 0) 
	      XFillRectangle(dpy,ScilabXgc->Cdrawable,gc,x[j],y[i],w,h); 
	    if (ScilabXgc->CurPixmapStatus != 1) 
	      XFillRectangle(dpy,(Drawable) ScilabXgc->CWindow,gc,x[j],y[i],w,h); 
	  }
      }
  xset_pattern(&cpat,PI0,PI0,PI0);
}





/*----------------------------------------------------------------------------------
 * Circles and Ellipsis 
 * Draw or fill a set of ellipsis or part of ellipsis 
 * Each is defined by 6-parameters, 
 * ellipsis i is specified by $vect[6*i+k]_{k=0,5}= x,y,width,height,angle1,angle2$ 
 * <x,y,width,height> is the bounding box 
 * angle1,angle2 specifies the portion of the ellipsis 
 * caution : angle=degreangle*64          
 * if fillvect[i] is in [1,lastpattern] then  fill the ellipsis i 
 * with pattern fillvect[i] 
 * if fillvect[i] is > lastpattern  then only draw the ellipsis i 
 * The drawing style is the current drawing 
 *----------------------------------------------------------------------------------*/


void C2F(fillarcs)(char *str, integer *vects, integer *fillvect, integer *n, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{
  integer i,cpat,verb,num;
  verb=0;
  xget_pattern(&verb,&cpat,&num,vdouble);
  for (i=0 ; i< *n ; i++)
    {
      if (fillvect[i] > ScilabXgc->IDLastPattern + 1)
	{
	  xset_pattern(&(cpat),PI0,PI0,PI0);
	  C2F(drawarc)(str,vects+6*i,vects+6*i+1,
		       vects+6*i+2,vects+6*i+3,
		       vects+6*i+4,vects+6*i+5,PD0,PD0,PD0,PD0);
	}
      else
	{
	  xset_pattern(&(fillvect[i]),PI0,PI0,PI0);
	  C2F(fillarc)(str,vects+6*i,vects+6*i+1,
		       vects+6*i+2,vects+6*i+3,
		       vects+6*i+4,vects+6*i+5,PD0,PD0,PD0,PD0);
	}
    }
  xset_pattern(&(cpat),PI0,PI0,PI0);
}

/*
 * Draw a set of ellipsis or part of ellipsis 
 * Each is defined by 6-parameters, 
 * ellipsis i is specified by $vect[6*i+k]_{k=0,5}= x,y,width,height,angle1,angle2$ 
 * <x,y,width,height> is the bounding box 
 * angle1,angle2 specifies the portion of the ellipsis 
 * caution : angle=degreangle*64          
 */


void C2F(drawarcs)(char *str, integer *vects, integer *style, integer *n, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{
  integer verbose=0,Dnarg,Dvalue[10],NDvalue,i;
  /* store the current values */
  xget_dash_and_color(&verbose,Dvalue,&Dnarg,vdouble);
  for (i=0 ; i< *n ; i++)
    {
      NDvalue = style[i];
      xset_line_style(&NDvalue,PI0,PI0,PI0);
      C2F(drawarc)(str,vects+6*i,vects+6*i+1,
		   vects+6*i+2,vects+6*i+3,
		   vects+6*i+4,vects+6*i+5,PD0,PD0,PD0,PD0);
    }
  xset_dash_and_color( Dvalue,PI0,PI0,PI0);
}

/** Draw a single ellipsis or part of it **/

void C2F(drawarc)(char *str, integer *x, integer *y, integer *width, integer *height, integer *angle1, integer *angle2, double *dv1, double *dv2, double *dv3, double *dv4)
{ 
  if (ScilabXgc->Cdrawable != (Drawable) 0)
    XDrawArc(dpy, ScilabXgc->Cdrawable, gc, *x, *y,(unsigned)*width,
	     (unsigned)*height,*angle1, *angle2);
  if (ScilabXgc->CurPixmapStatus != 1) 
    XDrawArc(dpy, (Drawable) ScilabXgc->CWindow, gc, *x, *y,(unsigned)*width,
	     (unsigned)*height,*angle1, *angle2);
  XFlush(dpy); }

/** Fill a single elipsis or part of it with current pattern **/

void C2F(fillarc)(char *str, integer *x, integer *y, integer *width, integer *height, integer *angle1, integer *angle2, double *dv1, double *dv2, double *dv3, double *dv4)
{ 
  if (ScilabXgc->Cdrawable != (Drawable) 0)
    XFillArc(dpy, ScilabXgc->Cdrawable, gc, *x, *y, *width, *height, *angle1, *angle2);    
  if (ScilabXgc->CurPixmapStatus != 1) 
    XFillArc(dpy, (Drawable) ScilabXgc->CWindow, gc, *x, *y, *width, *height, *angle1, *angle2);    
  XFlush(dpy);}

/*
 * Filling or Drawing Polylines and Polygons
 */

/* 
 * Draw a set of (*n) polylines (each of which have (*p) points) 
 * with lines or marks 
 * drawvect[i] <= 0 use a mark for polyline i
 * drawvect[i] >  0 use a line style for polyline i 
 */

void C2F(drawpolylines)(char *str, integer *vectsx, integer *vectsy, integer *drawvect, integer *n, integer *p, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{ integer verbose=0 ,symb[2],Mnarg,Dnarg,Dvalue[10],NDvalue,i,close;
/* store the current values */
 xget_mark(&verbose,symb,&Mnarg,vdouble);
 xget_dash_and_color(&verbose,Dvalue,&Dnarg,vdouble);
 for (i=0 ; i< *n ; i++)
   {
     if (drawvect[i] <= 0)
       { /** we use the markid : drawvect[i] : with current dash **/
	 NDvalue = - drawvect[i];
	 xset_mark(&NDvalue,symb+1,PI0,PI0);
	 xset_pattern(Dvalue+6,PI0,PI0,PI0);
	
	 C2F(drawpolymark)(str,p,vectsx+(*p)*i,vectsy+(*p)*i,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
       }
     else
       {/** we use the line-style number abs(drawvect[i])  **/
	 xset_line_style(drawvect+i,PI0,PI0,PI0);
	 close = 0;
	 C2F(drawpolyline)(str,p,vectsx+(*p)*i,vectsy+(*p)*i,&close,
			   PI0,PI0,PD0,PD0,PD0,PD0);
       }
   }
 /** back to default values **/
 xset_dash_and_color( Dvalue,PI0,PI0,PI0);
 xset_mark(symb,symb+1,PI0,PI0);
}

/***********************************************************
 *  fill a set of polygons each of which is defined by 
 * (*p) points (*n) is the number of polygons 
 * the polygon is closed by the routine 
 * fillvect[*n] :         
 * if fillvect[i] == 0 draw the boundaries with current color 
 * if fillvect[i] > 0  draw the boundaries with current color 
 *               then fill with pattern fillvect[i]
 * if fillvect[i] < 0  fill with pattern - fillvect[i]
 **************************************************************/

void C2F(fillpolylines)(char *str, integer *vectsx, integer *vectsy, integer *fillvect, integer *n, integer *p, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{
  integer Dnarg,Dvalue[10];
  integer i,cpat,verbose=0,num,close=1,pattern;
  xget_pattern(&verbose,&cpat,&num,vdouble);
  xget_dash_and_color(&verbose,Dvalue,&Dnarg,vdouble);
  for (i = 0 ; i< *n ; i++)
    {
      if (fillvect[i] > 0 )
	{ 
	  /** fill + boundaries **/
	  xset_pattern(&(fillvect[i]),PI0,PI0,PI0);
	  C2F(fillpolyline)(str,p,vectsx+(*p)*i,vectsy+(*p)*i,(close=1,&close),
			    PI0,PI0,PD0,PD0,PD0,PD0);
	  xset_line_style(Dvalue,PI0,PI0,PI0);
	  xset_pattern(&cpat,PI0,PI0,PI0);
	  C2F(drawpolyline)(str,p,vectsx+(*p)*i,vectsy+(*p)*i,(close=1,&close)
			    ,PI0,PI0,PD0,PD0,PD0,PD0);
	}
      else  if (fillvect[i] == 0 )
	{
	  /** boundaries ONLY **/
	  xset_line_style(Dvalue,PI0,PI0,PI0);
	  xset_pattern(&cpat,PI0,PI0,PI0);
	  C2F(drawpolyline)(str,p,vectsx+(*p)*i,vectsy+(*p)*i,(close=1,&close)
			    ,PI0,PI0,PD0,PD0,PD0,PD0);
	}
      else 
	{
	   /** fill ONLY **/
	  pattern = -fillvect[i] ;
	  xset_pattern(&pattern,PI0,PI0,PI0);
	  C2F(fillpolyline)(str,p,vectsx+(*p)*i,vectsy+(*p)*i,(close=1,&close)
			    ,PI0,PI0,PD0,PD0,PD0,PD0);
	  xset_pattern(&(cpat),PI0,PI0,PI0); 
	}
    }
  xset_dash_and_color(Dvalue,PI0,PI0,PI0);  /* ajoute ss le 13/09/00 */
}

/* 
 * Only draw one polygon  with current line style 
 * according to *closeflag : it's a polyline or a polygon
 * n is the number of points of the polyline 
 */

void C2F(drawpolyline)(char *str, integer *n, integer *vx, integer *vy, integer *closeflag, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{ 
  integer n1;
  if ( *closeflag )
  {
    n1 = *n+1 ;
  }
  else
  {
    n1= *n ;
  }
  if (n1 >= 2) 
  {
    /*C2F(analyze_points)(*n, vx, vy,*closeflag);*/
    /*Old code replaced by a routine with clipping */
    if (C2F(store_points)(*n, vx, vy,*closeflag))
    {
      /* draw the points */
      XDroutine( n1 ) ;
      /*XDrawLines (dpy, ScilabXgc->Cdrawable, gc, get_xpoints(), (int) n1,
        ScilabXgc->CurVectorStyle);*/
      XFlush(dpy);
    }
    /*XFlush(dpy);*/
  }
}

/* 
 * Fill the polygon or polyline 
 * according to *closeflag : the given vector is a polyline or a polygon 
 */

void C2F(fillpolyline)(char *str, integer *n, integer *vx, integer *vy, integer *closeflag, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{
  integer n1;
  if (*closeflag == 1) n1 = *n+1;else n1= *n;
  if (C2F(store_points)(*n, vx, vy,*closeflag)){
    if (ScilabXgc->Cdrawable != (Drawable) 0) 
      XFillPolygon (dpy, ScilabXgc->Cdrawable, gc, get_xpoints(), n1,
		  Complex, ScilabXgc->CurVectorStyle);
    if (ScilabXgc->CurPixmapStatus != 1) 
      XFillPolygon (dpy, (Drawable) ScilabXgc->CWindow, gc, get_xpoints(), n1,
		  Complex, ScilabXgc->CurVectorStyle);
  }
  XFlush(dpy);
}

/* 
 * Draw the current mark centred at points defined
 * by vx and vy (vx[i],vy[i]) 
 */

void C2F(drawpolymark)(char *str, integer *n, integer *vx, integer *vy, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{
  if ( ScilabXgc->CurHardSymb == 0 )
    {if (C2F(store_points)(*n, vx, vy,(integer)0L)){		
      if (ScilabXgc->Cdrawable != (Drawable) 0)
	XDrawPoints (dpy, ScilabXgc->Cdrawable, gc, get_xpoints(), *n,CoordModeOrigin);
      if (ScilabXgc->CurPixmapStatus != 1) 
	XDrawPoints (dpy, (Drawable) ScilabXgc->CWindow, gc, get_xpoints(), *n,CoordModeOrigin);
    }
    XFlush(dpy);
    }
  else 
    { 
      integer i,keepid,keepsize,hds;
      i=1;
      keepid =  ScilabXgc->FontId;
      keepsize= ScilabXgc->FontSize;
      hds= ScilabXgc->CurHardSymbSize;
      xset_font(&i,&hds,PI0,PI0);
      for ( i=0; i< *n ;i++) DrawMark(vx+i,vy+i);
      xset_font(&keepid,&keepsize,PI0,PI0);
    }
}

/**********************************************
 * A List of Windows with their Graphic contexts 
 **********************************************/

/* 
 * Adds a new entry at the end of the Window List 
 * and returns a pointer to that entry 
 */

static struct BCG *AddNewWindow __PARAMS((WindowList **));

struct BCG *AddNewWindowToList(void)
{
  return( AddNewWindow(&The_List));
}

static struct BCG *AddNewWindow(WindowList **listptr)
{ 
  if ( *listptr == (WindowList *) NULL)
    {
      *listptr = (WindowList *) MALLOC (sizeof(WindowList));
      if ( listptr == 0) 
	{
	  Scistring("AddNewWindow No More Place ");
	  return((struct BCG *) 0);
	}
      else 
	{ 
	  (*listptr)->winxgc.CWindow = (Window) NULL;
	  (*listptr)->winxgc.CBGWindow = (Window) NULL;
	  (*listptr)->winxgc.Cdrawable =  (Drawable) NULL;
	  (*listptr)->winxgc.CinfoW =  (Widget) NULL ;
	  (*listptr)->winxgc.Viewport =  (Widget) NULL ;
	  (*listptr)->winxgc.CurWindow = 0;
	  (*listptr)->winxgc.Red = (float *) 0;
	  (*listptr)->winxgc.Green = (float *) 0;
	  (*listptr)->winxgc.Blue = (float *) 0;
	  (*listptr)->winxgc.Colors = (Pixel *) 0;
	  (*listptr)->winxgc.Cmap = (Colormap) 0 ;
	  (*listptr)->winxgc.CmapFlag  = 1;
	  (*listptr)->winxgc.Cdrawable_flag = 0; 
	  (*listptr)->winxgc.EventHandler[0] = '\0';
	  (*listptr)->next = (struct WindowList *) NULL ;
	  return(&((*listptr)->winxgc));
	}
    }
  else
    {
      return( AddNewWindow((WindowList **) &((*listptr)->next)));
    }
}

/* delete a graphic window  **/

void DeleteSGWin(integer intnum)
{ 
  int curwin;
  if ( ScilabXgc == (struct BCG *) 0) return;
  curwin = ScilabXgc->CurWindow ;
  DeleteWindowToList(intnum);
  /*XXXX: jpc 2000: I also delete the scale list associated to that window */
  del_window_scale(intnum);
  if ( curwin  == intnum )
    {
      if ( The_List == (WindowList *) NULL)
	{
	  /** No more graphic window ; **/
	  ScilabXgc = (struct BCG *) 0;
	}
      else 
	{
	  /** fix the new current graphic window **/
	  ScilabXgc = &(The_List->winxgc);
	  ResetScilabXgc ();
	  get_window_scale(ScilabXgc->CurWindow,NULL);
	}
    }
}

/***********************************************
 * Free the entry in window list for window number num 
 * The X Objects are also freed 
 *     WARNING : A Finir  
 *   [1] Detruire physiquement la fenetre 
 *      C'est fait dans la fonction suiante 
 *	reste le Pixmap a detruire si besoin 
 *	ainsi que le colormap ? 
 *   [2] del_window_scale(intnum); 
 ************************************************/

void DeleteWindowToList(integer num)
{
  WindowList *L1,*L2;
  L1 = The_List;
  L2 = The_List;
  while ( L1 != (WindowList *) NULL)
    {
      if ( L1->winxgc.CurWindow == num )
	{
	  /** must free the pixmap if there's one XXXXXX */
	  Widget popup =  XtWindowToWidget(dpy,L1->winxgc.CBGWindow);
	  XtDestroyWidget(popup);
	  XgcFreeColors(&(L1->winxgc));
	  /** fenetre a detruire trouvee **/
	  if ( L1 != L2 )
	    {
	      /** Ce n'est pas la premiere fenetre de la liste **/
	      L2->next= L1->next ;
	      FREE(L1);
	      return ;
	    }
	  else 
	    {
	      /** C'est la premiere fenetre de la liste **/
	      The_List = (WindowList *) L1->next ;
	      FREE(L1);
	      return;
	    }
	}
      else 
	{
	  L2 = L1;
	  L1 = (WindowList *) L1->next;
	}
    }
}

/********************************************
 * Get Window number wincount ( or 0 ) 
 ********************************************/

Window GetWindowNumber(int wincount)
{
  struct BCG *bcg;
  bcg = GetWindowXgcNumber(wincount);
  if ( bcg != (struct BCG *) 0) 
    return( bcg->CWindow);
  else 
    return( (Window) 0);
}

/********************************************
 * returns the graphic context of window i 
 * or 0 if this window does not exists
 ********************************************/

static struct BCG *GetWinXgc __PARAMS((WindowList *,integer));

struct BCG *GetWindowXgcNumber(integer i)
{ 
  return( GetWinXgc(The_List,Max(0,i)));
}

static struct BCG *GetWinXgc(WindowList *listptr, integer i)
{
  if (listptr == (WindowList  *) NULL)
    {
      return((struct BCG *) 0);
    }
  else 
    { 
      if ((listptr->winxgc.CurWindow) == i)
	{
	  return( &(listptr->winxgc));
	}
      else 
	{
	  return(GetWinXgc((WindowList *) listptr->next,i));
	}
    }
}

/********************************************
 * Get BGWindow number wincount ( or 0 ) 
 ********************************************/

Window GetBGWindowNumber(int wincount)
{
  struct BCG *bcg;
  bcg = GetWindowXgcNumber(wincount);
  if ( bcg != (struct BCG *) 0) 
    return( bcg->CBGWindow);
  else 
    return( (Window) 0);
}

/***************************
 * get ids of scilab windows
 * in array Ids,
 * Num gives the number of windows
 * flag == 1 ==> get the Ids 
 * flag == 0 ==> just get the Number Num 
 ***************************/

void C2F(getwins)(integer *Num, integer *Ids, integer *flag)
{
  WindowList *listptr = The_List;
  *Num = 0;
  if ( *flag == 0 )
    {
      while ( listptr != (WindowList  *) 0 ) 
	{
	  (*Num)++;
	  listptr = (WindowList *) listptr->next;
	}
    }
  else 
    {
      while ( listptr != (WindowList  *) 0 ) 
	{
	  Ids[*Num] = listptr->winxgc.CurWindow;
	  listptr =  (WindowList *)listptr->next;
	  (*Num)++;
	}
    }
}

/***************************
 * get the highest id of scilab windows
 * or -1 if no windows 
 ***************************/

int GetWinsMaxId(void)
{
  WindowList *listptr = The_List;
  int Num = -1;
  while ( listptr != (WindowList  *) 0 ) 
    {
      Num = Max(listptr->winxgc.CurWindow,Num);
      listptr =  (WindowList *)listptr->next;
    }
  return(Num);
}

int GetEventWindow(XEvent* event)
{
  WindowList *listptr = The_List;

  while ( listptr != (WindowList  *) 0 ) {
    if (event->xbutton.window == listptr->winxgc.CWindow) 
      return(listptr->winxgc.CurWindow);
    listptr =  (WindowList *)listptr->next;
  }
  return(-1);

}

/********************************************
 * Routines for initialization : string is a display name 
 ********************************************/

#define MAXERRMSGLEN 512

static int X_error_handler(Display *d, XErrorEvent *err_ev)
{
  char            err_msg[MAXERRMSGLEN];

  XGetErrorText(dpy, (int) (err_ev->error_code), err_msg, MAXERRMSGLEN - 1);
  #ifndef NDEBUG
  printf("Scilab : X error trapped - error message follows:\r\n%s\r\n", err_msg);
  #endif
  return(0);
}

void set_c(integer col)
{
  int i,bk;
  /* colors from 1 to ScilabXgc->Numcolors */
  i= Max(0,Min(col,ScilabXgc->Numcolors + 1));     
  ScilabXgc->CurColor = i;
  bk= Max(0,Min(ScilabXgc->NumBackground,ScilabXgc->Numcolors + 1));
  if (ScilabXgc->Colors == NULL) return;
  switch ( ScilabXgc->CurDrawFunction  ) 
    {
    case GXclear :
      break ;
    case GXxor   : 
      XSetForeground(dpy, gc,(unsigned long) 
		     ScilabXgc->Colors[i] ^ ScilabXgc->Colors[bk]);break;
    default :  
      XSetForeground(dpy, gc,(unsigned long) ScilabXgc->Colors[i] );break;
    }
}

/*
 * initgraphic : initialize graphic window
 * If v2 is not a nul pointer *v2 is the window number to create 
 * EntryCounter is used to check for first Entry + to know the next 
 * available window number 
 */

void C2F(initgraphic)(char *string, integer *v2, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{ 
  XWindowAttributes war;
  struct BCG *NewXgc ;
  static integer EntryCounter = 0;
  integer WinNum;
  
#ifdef WITH_TK
  integer ne=7, menutyp=2, ierr;
  char *EditMenusE[]={"Select figure as current","Redraw figure","Erase figure",/*"Copy object","Paste object","Move object","Delete object",*/"Figure properties","Current axes properties","Start entity picker","Stop entity picker"};
  

  /*integer ni=6;
  char *InsertMenusE[]={"Line","Polyline","Arrow","Text","Rectangle","Circle"};
  */

#else
  integer ne=3, menutyp=2, ierr;
  char *EditMenusE[]={"Select as current","Redraw figure","Erase figure"};
#endif
  
  GC XCreateGC(Display *, Drawable, long unsigned int, XGCValues *);
  static int screen;
  static XGCValues gcvalues;
  static Widget toplevel = (Widget) NULL;
  *v3=0;
  if ( v2 != (integer *) NULL && *v2 != -1 )
    WinNum= *v2;
  else
    WinNum= EntryCounter;
  if (EntryCounter == 0)
    {
      /** This is done only at the first entry */
      DisplayInit(string,&dpy,&toplevel);
      screen =DefaultScreen(dpy);
      root = XRootWindow (dpy,screen); 
      depth = XDefaultDepth (dpy,screen);
      maxcol = 1 << depth;
      visual = XDefaultVisual(dpy,screen);
      wpixel = WhitePixelOfScreen(DefaultScreenOfDisplay(dpy));
      bpixel = BlackPixelOfScreen(DefaultScreenOfDisplay(dpy));
      LoadFonts();
      crosscursor = XCreateFontCursor(dpy, XC_crosshair);
      arrowcursor  = XCreateFontCursor (dpy, (char)0x2e);
      normalcursor = XCreateFontCursor (dpy, XC_X_cursor);
    }

  NewXgc = AddNewWindowToList();
  if ( NewXgc == (struct BCG *) 0) 
    {
      Scistring("initgraphics: unable to alloc\n");
      return;
    }
  else 
    {
      ScilabXgc= NewXgc;
    }
#ifdef WITH_TK
  if (IsTKGraphicalMode()) {
    Tk_Window  win;

    /* TKmainWindow est initialise dans tclsci.c  Tk_CreateMainWindow */
    if (TKmainWindow != (Tk_Window)0) {
      win = Tk_NameToWindow(TCLinterp, string, (Tk_Window) TKmainWindow);
      if (win != (Tk_Window)0) {
	ScilabXgc->CWindow=Tk_WindowId(win);
	ScilabXgc->CBGWindow=(Window)0;
	ScilabXgc->popup=(Widget)0;

	/*	Tk_CreateEventHandler(win, ExposureMask, (Tk_EventProc *) redrawProc,NULL);*/

      }
    }
  }
  else
#endif
    CreatePopupWindow(WinNum,toplevel,ScilabXgc,&DefaultForeground,&DefaultBackground) ;


 /*** XXXX ScilabXgc->CWindow = Find_TK_Window(WinNum); **/
  if (EntryCounter == 0)
    {
      /** Initialize default Scilab Xgc **/
      C2F(CreatePatterns)(DefaultBackground,DefaultForeground);
    }
  XGetWindowAttributes(dpy,ScilabXgc->CWindow,&war); 
  ScilabXgc->CWindowWidth =  war.width;
  ScilabXgc->CWindowHeight =  war.height;



  /** Default value is without Pixmap **/
  ScilabXgc->CurPixmapStatus = 0; 
  ScilabXgc->CurResizeStatus = 1; 
  ScilabXgc->CurWindow = WinNum;
  if (EntryCounter == 0)
    {
      /* GC Set: for drawing */
      gcvalues.foreground = DefaultForeground;
      gcvalues.background = DefaultBackground;
      gcvalues.function   =  GXcopy ;
      gcvalues.line_width = 1;
      gc = XCreateGC(dpy, ScilabXgc->CWindow, GCFunction | GCForeground 
		     | GCBackground | GCLineWidth, &gcvalues);
      XSetWindowColormap(dpy,ScilabXgc->CBGWindow,
			 XDefaultColormap(dpy,XDefaultScreen(dpy)));
      XSetErrorHandler(X_error_handler);
      XSetIOErrorHandler((XIOErrorHandler) X_error_handler);
    }
  InitMissileXgc(PI0,PI0,PI0,PI0);
  SetGraphicsVersion(); /* set the graphics version using global versionflag variable */
  /* to be sure that current values are recorded */
  StoreXgc(WinNum);
  EntryCounter=Max(EntryCounter,WinNum);
  EntryCounter++;
#ifdef WITH_TK
  if (!IsTKGraphicalMode())
  {
    /* add the Edit and Insert menus */
    AddMenu(&WinNum,"Edit", EditMenusE, &ne, &menutyp, "ged", &ierr);
    
    /*AddMenu(&WinNum,"Insert", InsertMenusE, &ni, &menutyp, "ged_insert", &ierr);*/

    /* put them in grey in old style */
    refreshMenus( ScilabXgc ) ;  
  }
#endif
  XSync(dpy,0);
  
}

/* writes a message in the info widget associated to the current scilab window */

void C2F(xinfo)(char *message, integer *v2, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{
  Arg args[1];
  if ( ScilabXgc != (struct BCG *) 0 && ScilabXgc->CinfoW != (Widget) NULL)
    {
      Cardinal n = 0;
      XtSetArg(args[n], XtNlabel, message);n++;
      XtSetValues(ScilabXgc->CinfoW, args, n);
    }
}

/* 
 * the same with an easier calling sequence 
 */

#ifdef __STDC__
#include <stdarg.h>
#else
#include <varargs.h>
#endif 

#define MAXPRINTF 512

#ifdef __STDC__ 
void wininfo(char *format,...) 
#else 
void wininfo(va_alist) va_dcl
#endif 
{
  /* Extended call for C calling */
  Arg args[1];
  va_list ap;
  char buf[MAXPRINTF];
#ifdef __STDC__
  va_start(ap,format);
#else
  char *format;
  va_start(ap);
  format = va_arg(ap, char *);
#endif
  (void ) vsprintf(buf, format, ap );
  va_end(ap);
  if ( ScilabXgc != (struct BCG *) 0 && ScilabXgc->CinfoW != (Widget) NULL)
    {
      Cardinal n = 0;
      XtSetArg(args[n], XtNlabel,buf);n++;
      XtSetValues(ScilabXgc->CinfoW, args, n);
    }
}

/*
 * send a ClientMessage to XScilab
 * for scilab graphic window creation 
 */

Atom		NewGraphWindowMessageAtom;

void SendScilab(Window local, integer winnum)
{
  XClientMessageEvent ev;
  ev.type = ClientMessage;
  ev.window = local ;
  ev.message_type =NewGraphWindowMessageAtom;
  ev.format = 32;
  ev.data.l[0] = winnum;
  XSendEvent (dpy, local, False, (integer)0L, (XEvent *) &ev);
  XFlush(dpy);
}

/****************************************************************
 * Searches window named name among the sons of top  
 ****************************************************************/

#define DbugInfo0(x) /* fprintf(stderr,x) */
#define DbugInfo1(x,y)  /* fprintf(stderr,x,y) */
#define DbugInfo3(x,y,z,t)  /* fprintf(stderr,x,y,z,t) */
static int CheckWin(Window w);

Window Window_With_Name(Window top, char *name, int j, char *ResList0, char *ResList1, char *ResList2)
{
  Status status;
  Window *children,root1,parent1,w=0;
  unsigned int nchildren=0;
  integer i; 
  char *window_name;
  if ( CheckWin(top)==0) return((Window) 0);
  status=XQueryTree(dpy, top, &root1, &parent1, &children, &nchildren);
  DbugInfo1(" Status %d\n",status);
  if ( status == 0)
    {
      DbugInfo0("XQuery Tree failed \n");
      return((Window) 0);
    }
  if ( nchildren == 0 )  return((Window) 0);
  DbugInfo1("Number of children %d \n",nchildren);
  for (i= (int) nchildren-1; i >= 0 ; i--) 
    {
      if ( CheckWin(children[i])!=0)
	{
	  XFetchName(dpy, children[i], &window_name);
	  DbugInfo3("Child [%d] %s %d\n",children[i],window_name,j);
	  if ( window_name != 0 && strcmp(window_name, name)==0 )
	    {
	      w=children[i];
	      DbugInfo1("Found %s \n",window_name);
	      DbugInfo1("Level %d\n",j);
	      XFree((char *) children);
	      if (window_name) XFree(window_name);
	      break;
	    }
	  else 
	    {  
	      w=Window_With_Name(children[i],name,j+1,
				 ResList0,ResList1,ResList2);
	      if ( w != 0 )
		{
		  XFree((char *) children);
		  DbugInfo1("father was %s\n",window_name);
		  if (window_name) XFree(window_name);
		  break;
		}
	    }
	  if (window_name) XFree(window_name);
	}
    }
  return((Window) w);
}

static char *ResList[]={ VERSION,"BG","ScilabGraphic"};

Window Find_X_Scilab(void)
{
  Window w;
  DbugInfo1("Searching %s\n",VERSION);
  w=Window_With_Name(RootWindow(dpy,DefaultScreen(dpy)),
		     VERSION,0,ResList[0],ResList[0],ResList[0]);
  return(w);
}

#define STR0 "ScilabGraphic%d"

Window Find_ScilabGraphic_Window(integer i)
{
  Window w;
  char wname[sizeof(STR0)+4];
  sprintf(wname,STR0,(int) i);
  DbugInfo1("Searching %s\n",wname);
  w=Window_With_Name(RootWindow(dpy,DefaultScreen(dpy)),wname,0,
		     ResList[2],ResList[2],ResList[2]);
  return(w);
}

#define STR1 "BG%d"

Window Find_BG_Window(integer i)
{
  char wname[sizeof(STR1)+4];
  Window w;
  sprintf(wname,STR1,(int) i);
  DbugInfo1("Searching %s\n",wname);
  w=Window_With_Name(RootWindow(dpy,DefaultScreen(dpy)),wname,0,
		     ResList[1],ResList[2],ResList[2]);
  return(w);
}


#define STR2 "objfigure1"
/*** XXXXXXXXXX
extern Display *XTKdisplay;

Window Find_TK_Window(i)
     integer i;
{
  char wname[sizeof(STR2)+4];
  Window w;
  sprintf(wname,STR2,(int) i);
  DbugInfo1("Searching %s\n",wname);
  w=Window_With_Name(RootWindow(dpy,DefaultScreen(dpy)),wname,0,
			  ResList[1],ResList[2],ResList[2]);
  dpy = XTKdisplay;
  return(w);
}
**/

  /*
   * make sure that the window is valid
   * There's no function to really check this in X11
   */
#include <setjmp.h>

static jmp_buf my_env;

static void Ignore_Err(Display *d, XErrorEvent *err_ev)
{
  DbugInfo0("Ignoring Error");
  longjmp(my_env,1);
}

static int 
CheckWin(Window w)
{
  Window root_ret;
  int x, y;
  unsigned width= -1, height= -1, bw, idepth;
  int (*curh)();
  curh=XSetErrorHandler((XErrorHandler) Ignore_Err);
  if ( setjmp(my_env)) 
    {
      /** return from longjmp **/
      XSetErrorHandler((XErrorHandler) curh);
      return(0);
    }
  else
    {
      XGetGeometry (dpy, w, &root_ret, &x, &y, &width, &height, &bw, &idepth);
      XSync (dpy, 0);
      XSetErrorHandler((XErrorHandler) curh);
      return(1);}
}

/*--------------------------------------------------------
 * Initialisation of the graphic context. Used also 
 *  to come back to the default graphic state}
 *---------------------------------------------------------*/

static void 
InitMissileXgc (integer *v1, integer *v2, integer *v3, integer *v4)
{ 
  integer i,j;
  ScilabXgc->IDLastPattern = GREYNUMBER - 1;
  ScilabXgc->CurLineWidth=0 ;
  i=1;
  xset_thickness(&i,PI0,PI0,PI0);
  /** retirer le clipping **/
  i=j= -1;
  xset_unclip(PI0,PI0,PI0,PI0);
  ScilabXgc->ClipRegionSet= 0;
  xset_font((i=2,&i),(j=1,&j),PI0,PI0);
  xset_mark((i=0,&i),(j=0,&j),PI0,PI0);
  ScilabXgc->CurResizeStatus = 1 ;
  /** trace absolu **/
  i= CoordModeOrigin;
  xset_absourel(&i,PI0,PI0,PI0);
  /* initialisation des pattern dash par defaut en n&b */
  ScilabXgc->CurColorStatus = 0;
  xset_pattern((i=1,&i),PI0,PI0,PI0);
  xset_dash((i=1,&i),PI0,PI0,PI0);
  xset_hidden3d((i=1,&i),PI0,PI0,PI0);
  /* initialisation de la couleur par defaut */ 
  ScilabXgc->CurColorStatus = 1;
  set_default_colormap();
  xset_alufunction1((i=3,&i),PI0,PI0,PI0);
  xset_pixmapOn((i=0,&i),PI0,PI0,PI0);

  xset_pattern((i=ScilabXgc->NumForeground+1,&i),PI0,PI0,PI0);
  /*** XXXXX a faire aussi pour le n&b plus haut ***/
  xset_foreground((i=ScilabXgc->NumForeground+1,&i),PI0,PI0,PI0);
  xset_background((i=ScilabXgc->NumForeground+2,&i),PI0,PI0,PI0);
  xset_hidden3d((i=4,&i),PI0,PI0,PI0);
  /* Choix du mode par defaut (decide dans initgraphic) */
  getcolordef(&i);
  /** we force CurColorStatus to the opposite value of col 
      to force usecolorPos to perform initialisations 
  **/
  ScilabXgc->mafigure = (sciPointObj *)NULL;
/*   ScilabXgc->graphicsversion = versionflag; /\* NG *\/  */

  ScilabXgc->CurColorStatus = (i == 1) ? 0: 1;
  xset_usecolor(&i ,PI0,PI0,PI0);
  strcpy(ScilabXgc->CurNumberDispFormat,"%-5.2g");
  /** default scales **/
  Cscale2default();
}

static void SetGraphicsVersion()
{
  ScilabXgc->graphicsversion = versionflag; /* NG */ 
}


/* use the current ScilabXgc for reinitialization  
 * used when switching from one graphic window to an other one 
 * XXXX : remettre le foreground 
 */

static void ResetScilabXgc (void)
{ 
  integer i,j, clip[4];
  i= ScilabXgc->FontId;
  j= ScilabXgc->FontSize;
  xset_font(&i,&j,PI0,PI0);
  
  i= ScilabXgc->CurHardSymb;
  j= ScilabXgc->CurHardSymbSize;
  xset_mark(&i,&j,PI0,PI0);
  
  i= ScilabXgc->CurLineWidth;
  xset_thickness(&i,PI0,PI0,PI0);
  
  i= ScilabXgc->CurVectorStyle;
  xset_absourel(&i,PI0,PI0,PI0);
  
  i= ScilabXgc->CurDrawFunction;
  xset_alufunction1(&i,PI0,PI0,PI0);
  
  if (ScilabXgc->ClipRegionSet == 1) 
    {
      for ( i= 0 ; i < 4; i++) clip[i]=ScilabXgc->CurClipRegion[i];
      xset_clip(clip,clip+1,clip+2,clip+3);
    }
  else
    xset_unclip(PI0,PI0,PI0,PI0);

  if (ScilabXgc->CurColorStatus == 0) 
    {
      /* remise des couleurs a vide */
      ScilabXgc->CurColorStatus = 1;
      xset_pattern((i=DefaultForeground,&i),PI0,PI0,PI0);
      /* passage en n&b */
      ScilabXgc->CurColorStatus = 0;
      i= ScilabXgc->CurPattern + 1;
      xset_pattern(&i,PI0,PI0,PI0);
      i= ScilabXgc->CurDashStyle + 1;
      xset_dash(&i,PI0,PI0,PI0);
      i= ScilabXgc->NumHidden3d+1;
      xset_hidden3d(&i,PI0,PI0,PI0);
    }
  else 
    {
      /* remise a zero des patterns et dash */
      /* remise des couleurs a vide */
      ScilabXgc->CurColorStatus = 0;
      xset_pattern((i=1,&i),PI0,PI0,PI0);
      xset_dash((i=1,&i),PI0,PI0,PI0);
      /* passage en couleur  */
      ScilabXgc->CurColorStatus = 1;
      i= ScilabXgc->CurColor + 1;
      xset_pattern(&i,PI0,PI0,PI0);
      i= ScilabXgc->NumBackground+1;
      xset_background(&i,PI0,PI0,PI0);
      i= ScilabXgc->NumForeground+1;
      xset_foreground(&i,PI0,PI0,PI0);
      i= ScilabXgc->NumHidden3d+1;
      xset_hidden3d(&i,PI0,PI0,PI0);
    }
}

/*------------------------------------------------------
  Draw an axis whith a slope of alpha degree (clockwise)
  . Along the axis marks are set in the direction ( alpha + pi/2), in the 
  following way :
  \begin{itemize}
  \item   $n=<n1,n2>$,
  \begin{verbatim}
  |            |           |
  |----|---|---|---|---|---|
  <-----n1---->                 
  <-------------n2-------->
  \end{verbatim}
  $n1$and $n2$ are integer numbers for interval numbers.
  \item $size=<dl,r,coeff>$. $dl$ distance in points between 
  two marks, $r$ size in points of small mark, $r*coeff$ 
  size in points of big marks. (they are doubleing points numbers)
  \item $init$. Initial point $<x,y>$. 
  \end{itemize}
  
-------------------------------------------------------------*/

void C2F(drawaxis)(char *str, integer *alpha, integer *nsteps, integer *v2, integer *initpoint, integer *v6, integer *v7, double *size, double *dx2, double *dx3, double *dx4)
{ integer i;
 double xi,yi,xf,yf;
 double cosal,sinal;
 cosal= cos( (double)M_PI * (*alpha)/180.0);
 sinal= sin( (double)M_PI * (*alpha)/180.0);
 for (i=0; i <= nsteps[0]*nsteps[1]; i++)
   {
     if (( i % nsteps[0]) != 0)
       {
	 xi = initpoint[0]+i*size[0]*cosal;
	 yi = initpoint[1]+i*size[0]*sinal;
	 xf = xi - ( size[1]*sinal);
	 yf = yi + ( size[1]*cosal);
	 if (ScilabXgc->Cdrawable != (Drawable) 0) 
	   XDrawLine(dpy,ScilabXgc->Cdrawable,gc,inint(xi),inint(yi),inint(xf),inint(yf));
	 if (ScilabXgc->CurPixmapStatus != 1) 
	    XDrawLine(dpy,(Drawable) ScilabXgc->CWindow,gc,inint(xi),inint(yi),inint(xf),inint(yf));
       }
   }
 for (i=0; i <= nsteps[1]; i++)
   { xi = initpoint[0]+i*nsteps[0]*size[0]*cosal;
   yi = initpoint[1]+i*nsteps[0]*size[0]*sinal;
   xf = xi - ( size[1]*size[2]*sinal);
   yf = yi + ( size[1]*size[2]*cosal);
   if (ScilabXgc->Cdrawable != (Drawable) 0)
     XDrawLine(dpy,ScilabXgc->Cdrawable,gc,inint(xi),inint(yi),inint(xf),inint(yf));
   if (ScilabXgc->CurPixmapStatus != 1) 
     XDrawLine(dpy,(Drawable) ScilabXgc->CWindow,gc,inint(xi),inint(yi),inint(xf),inint(yf));
   }

 XFlush(dpy);
}

/*-----------------------------------------------------
 * Display numbers z[i] at location (x[i],y[i])
 *   with a slope alpha[i] (see displaystring), if flag==1
 *   add a box around the string, only if slope =0}
 *-----------------------------------------------------*/

void C2F(displaynumbers)(char *str, integer *x, integer *y, integer *v1, integer *v2, integer *n, integer *flag, double *z, double *alpha, double *dx3, double *dx4)
{ integer i ;
 char buf[20];
 for (i=0 ; i< *n ; i++)
   { sprintf(buf,ScilabXgc->CurNumberDispFormat,z[i]);
   C2F(displaystring)(buf,&(x[i]),&(y[i]),PI0,flag,PI0,PI0,&(alpha[i]),PD0,PD0,PD0) ;
   }
 XFlush(dpy);
}

void C2F(bitmap)(char *string, integer w, integer h)
{
  static XImage *setimage;
  setimage = XCreateImage (dpy, XDefaultVisual (dpy, DefaultScreen(dpy)),
			   1, XYBitmap, 0, string,w,h, 8, 0);	
  setimage->data = string;
  if (ScilabXgc->Cdrawable != (Drawable) 0) 
    XPutImage (dpy, ScilabXgc->Cdrawable, gc, setimage, 0, 0, 10,10,w,h);
  if (ScilabXgc->CurPixmapStatus != 1) 
    XPutImage (dpy, (Drawable) ScilabXgc->CWindow, gc, setimage, 0, 0, 10,10,w,h);

  XDestroyImage(setimage);
}


/*---------------------------------------------------------------------
 * Using X11 Fonts}
 *---------------------------------------------------------------------*/


/* FontInfoTab : information on fonts 
 *  its name and ok is set to one if the font is loaded in the Xserver 
 *  loadfamily is used for font loading 
 */

struct FontInfo { integer ok;
  char fname[100];
} FontInfoTab_[FONTNUMBER];

/** Must be of size FONTMAXSIZE **/

static char *size_[] = { "08" ,"10","12","14","18","24"};
static int i_size_[] = { 8 ,10,12,14,18,24};

/*
 * To set the current font id  and size 
 * load the fonts into X11 if necessary 
 */

typedef  struct  {
  char *alias;
  char *name;
}  FontAlias;

/* ce qui suit marche sur 75dpi ou 100dpi */

FontAlias fonttab[] ={
  {"CourR", "-adobe-courier-medium-r-normal--*-%s0-*-*-m-*-iso8859-1"},
  {"Symb", "-adobe-symbol-medium-r-normal--*-%s0-*-*-p-*-adobe-fontspecific"},
  {"TimR", "-adobe-times-medium-r-normal--*-%s0-*-*-p-*-iso8859-1"},
  {"TimI", "-adobe-times-medium-i-normal--*-%s0-*-*-p-*-iso8859-1"},
  {"TimB", "-adobe-times-bold-r-normal--*-%s0-*-*-p-*-iso8859-1"},
  {"TimBI", "-adobe-times-bold-i-normal--*-%s0-*-*-p-*-iso8859-1"},
  {"HelvR", "-adobe-helvetica-medium-r-normal--*-%s0-*-*-p-*-iso8859-1"},
  {"HelvO", "-adobe-helvetica-medium-o-normal--*-%s0-*-*-p-*-iso8859-1"},
  {"HelvB", "-adobe-helvetica-bold-r-normal--*-%s0-*-*-p-*-iso8859-1"},
  {"HelvBO", "-adobe-helvetica-bold-o-normal--*-%s0-*-*-p-*-iso8859-1"},
  {(char *) NULL,( char *) NULL}
};

int fontidscale(int fontsize)
{
  int Nsiz;
  int i;
  int isiz = i_size_[fontsize];
  double d = Min(ScilabXgc->CWindowHeight,ScilabXgc->CWindowWidth);
  
  Nsiz = (ScilabXgc != NULL) ? inint((isiz*d/400.0)) : isiz; 
  fprintf(stderr,"Scaling by -->%d %d \n",isiz,Nsiz);
  for ( i=0; i < FONTMAXSIZE ; i++) 
    {
      if (i_size_[i] >= Nsiz ) return Max(i-1,0);
    }
  return FONTMAXSIZE -1;
}

static void xset_font(integer *fontid, integer *fontsize, integer *v3, integer *v4)
{ 
  integer i,fsiz,fsiz_sca;
  i = Min(FONTNUMBER-1,Max(*fontid,0));
  fsiz = Min(FONTMAXSIZE-1,Max(*fontsize,0));
  fsiz_sca = fsiz ;/* XXX fontidscale(fsiz); Scale fonts */
  if ( FontInfoTab_[i].ok !=1 )
    { 
      if (i !=  FONTNUMBER-1 )
	{
	  C2F(loadfamily)(fonttab[i].alias,&i,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
	}
      else 
	{
	  sciprint(" The Font Id %d is not affected \r\n",(int)i);
	  Scistring(" use xlfont to set it \n");
	  return;
	}
    }
  ScilabXgc->FontId = i;
  ScilabXgc->FontSize = fsiz;
  ScilabXgc->FontXID=FontsList_[i][fsiz_sca]->fid;
  XSetFont(dpy,gc,FontsList_[i][fsiz_sca]->fid);
  XFlush(dpy);
}

/** To get the  id and size of the current font **/

static void  xget_font(integer *verbose, integer *font, integer *nargs, double *dummy)
{
  *nargs=2;
  font[0]= ScilabXgc->FontId ;
  font[1] =ScilabXgc->FontSize ;
  if (*verbose == 1) 
    {
      sciprint("\nFontId : %d ", ScilabXgc->FontId );
      sciprint("--> %s at size %s pts\r\n",
	       FontInfoTab_[ScilabXgc->FontId].fname,
	       size_[ScilabXgc->FontSize]);
    }
}

/** To set the current mark **/

static void xset_mark(integer *number, integer *size, integer *v3, integer *v4)
{ 
  ScilabXgc->CurHardSymb = Max(Min(SYMBOLNUMBER-1,*number),0);
  ScilabXgc->CurHardSymbSize = Max(Min(FONTMAXSIZE-1,*size),0);
  ;}

/** To get the current mark id **/

static void xget_mark(integer *verbose, integer *symb, integer *narg, double *dummy)
{
  *narg =2 ;
  symb[0] = ScilabXgc->CurHardSymb ;
  symb[1] = ScilabXgc->CurHardSymbSize ;
  if (*verbose == 1) 
    {
      sciprint("\nMark : %d ",ScilabXgc->CurHardSymb);
      sciprint("at size %s pts\r\n", size_[ScilabXgc->CurHardSymbSize]);
    }
}

/* Load in X11 a font at size  08 10 12 14 18 24 
 * TimR08 TimR10 TimR12 TimR14 TimR18 TimR24 
 * name is a string if it's a string containing the char % 
 *   it's suposed to be a format for a generic font in X11 string style 
 *   ex :  "-adobe-times-bold-i-normal--%s-*-75-75-p-*-iso8859-1"
 *   and the font is loaded at size 8,10,12,14,18,24
 *   else it's supposed to be an alias for a font name
 *   Ex : TimR and we shall try to load TimR08 TimR10 TimR12 TimR14 TimR18 TimR24 
 *   we first look in an internal table and transmits the string 
 *   to X11 
 */

void C2F(loadfamily)(char *name, integer *j, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{ 
  integer i,flag=1 ;
  /** generic name with % **/
  if ( strchr(name,'%') != (char *) NULL)
    {
      C2F(loadfamily_n)(name,j);
      return;
    }
  else 
    {
      /** our table of alias **/
      i=0;
      while ( fonttab[i].alias != (char *) NULL)
	{
	  if (strcmp(fonttab[i].alias,name)==0)
	    {
	      C2F(loadfamily_n)(fonttab[i].name,j);
	      return ;
	    }
	  i++;
	}
      /** Using X11 Table of aliases **/
      for ( i = 0; i < FONTMAXSIZE ; i++)
	{
	  char name1[200];
	  sprintf(name1,"%s%s",name,size_[i]);
	  FontsList_[*j][i]=XLoadQueryFont(dpy,name1);
	  if  (FontsList_[*j][i]== NULL)
	    { 
	      flag=0;
	      sciprint("\n Unknown font : %s",name1);
	      Scistring("\n I'll use font: fixed ");
	      FontsList_[*j][i]=XLoadQueryFont(dpy,"fixed");
	      if  (FontsList_[*j][i]== NULL)
		{
		  sciprint("\n Unknown font : %s\r\n","fixed");
		  Scistring("Please call an X Wizard !");
		}
	    }
	}
      FontInfoTab_[*j].ok = 1;
      if (flag != 0) 
	strcpy(FontInfoTab_[*j].fname,name);
      else
	strcpy(FontInfoTab_[*j].fname,"fixed");
    }
}

static char *size_n_[] = { "8" ,"10","12","14","18","24"};

static void C2F(loadfamily_n)(char *name, integer *j)
{ 
  char name1[200];
  integer i,flag=1 ;
  for ( i = 0; i < FONTMAXSIZE ; i++)
    {
      sprintf(name1,name,size_n_[i]);
      FontsList_[*j][i]=XLoadQueryFont(dpy,name1);
      if  (FontsList_[*j][i]== NULL)
	{ 
	  flag=0;
	  sciprint("\n Unknown font : %s",name1);
	  Scistring("\n I'll use font: fixed ");
	  FontsList_[*j][i]=XLoadQueryFont(dpy,"fixed");
	  if  (FontsList_[*j][i]== NULL)
	    {
	      sciprint("\n Unknown font : %s\r\n","fixed");
	      Scistring("  Please call an X Wizard !");
	    }
	}
    }
  FontInfoTab_[*j].ok = 1;
  if (flag != 0) 
    strcpy(FontInfoTab_[*j].fname,name);
  else
    strcpy(FontInfoTab_[*j].fname,"fixed");
}

void C2F(queryfamily)(char *name, integer *j, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{ 
  integer i ;
  name[0]='\0';
  for (i=0;i<FONTNUMBER;i++) {
    v3[i]=strlen(FontInfoTab_[i].fname);
    if (v3[i] > 0)
      strcat(name,FontInfoTab_[i].fname);
    else
      if (i < FONTNUMBER-1) {      /* ici il y avait 6 ... */
	v3[i]=strlen(fonttab[i].name);
	strcat(name,fonttab[i].name);
      }
  }
  *j=FONTNUMBER;
}



static void LoadFonts(void)
{
  integer fnum;
  C2F(loadfamily)("CourR",(fnum=0,&fnum),PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0); 
  LoadSymbFonts();
  C2F(loadfamily)("TimR",(fnum=2,&fnum),PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
  /*  the next fonts are loaded when needed       See xsetfont
      C2F(loadfamily)("TimI",(fnum=3,&fnum),PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
      C2F(loadfamily)("TimB",(fnum=4,&fnum),PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
      C2F(loadfamily)("TimBI",(fnum=5,&fnum),PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0); 
  */
}

/*
 *  We use the Symbol font  for mark plotting
 *  thus we must be able to center a Symbol character at a specified point. 
 *  
 */

typedef  struct { integer xoffset[SYMBOLNUMBER];
  integer yoffset[SYMBOLNUMBER];} Offset ;

static Offset ListOffset_[FONTMAXSIZE];
static char Marks[] = {
  /*., +,X,*,diamond(filled),diamond,triangle up,triangle down,trefle,circle*/
  (char)0x2e,(char)0x2b,(char)0xb4,(char)0xc5,(char)0xa8,
  (char)0xe0,(char)0x44,(char)0xd1,(char)0xa7,(char)0x4f};

static void 
LoadSymbFonts(void)
{ 
  XCharStruct xcs;
  integer j,k ;
  integer i;
  /** Symbol Font is loaded under Id : 1 **/
  C2F(loadfamily)("Symb",(i=1,&i),PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);

  /* We compute the char offset for several chars of the symbol font
     in order to be able to center them on a specific point 
     we need one offset per symbol
     for the font i 
     n1=FontsList_[i]->min_char_or_byte2
     info on char coded as  oxyy are stored in 
     FontsList_[i]->per_char[(char)0xyy-n1]
     
  */
  /** if symbol font was not found me must stop **/
  if (strcmp(FontInfoTab_[1].fname,fonttab[1].name) != 0) return;
  for (i =0 ; i < FONTMAXSIZE ; i++)
    {    if (FontsList_[1][i] != NULL)
      {
	k =FontsList_[1][i]->min_char_or_byte2;
	for (j=0 ; j < SYMBOLNUMBER ; j++)
	  { 
	    xcs= FontsList_[1][i]->per_char[Char2Int(Marks[j])-k];
	    (ListOffset_[i].xoffset)[j] = (xcs.rbearing+xcs.lbearing)/2;
	    (ListOffset_[i].yoffset)[j] = (xcs.ascent+xcs.descent)/2;
	  }
      }
    }
}

/*
 * The two next functions send the x and y offsets to center the current
 * symbol at point (x,y) 
 */

int C2F(CurSymbXOffset)(void)
{
  return(-(ListOffset_[ScilabXgc->CurHardSymbSize].xoffset)
	 [ScilabXgc->CurHardSymb]);
}

int C2F(CurSymbYOffset)(void)
{
  return((ListOffset_[ScilabXgc->CurHardSymbSize].yoffset)
	 [ScilabXgc->CurHardSymb]);
}

static void DrawMark(integer *x, integer *y)
{ 
  char str[1];
  str[0]=Marks[ScilabXgc->CurHardSymb];
  if (ScilabXgc->Cdrawable != (Drawable) 0)
    XDrawString(dpy,ScilabXgc->Cdrawable,gc,(int) *x+C2F(CurSymbXOffset)(),
		(int)*y+C2F(CurSymbYOffset)(),str,1);
  if (ScilabXgc->CurPixmapStatus != 1) 
    XDrawString(dpy,(Drawable) ScilabXgc->CWindow,gc,(int) *x+C2F(CurSymbXOffset)(),
		(int)*y+C2F(CurSymbYOffset)(),str,1);
  XFlush(dpy);
}

/*-------------------------------------------------------------------
 * Allocation and storing function for vectors of X11-points
 *------------------------------------------------------------------------*/

static XPoint *points;
static XPoint *get_xpoints(void) { return(points); }

int C2F(store_points)(integer n, integer *vx, integer *vy, integer onemore)
{ 
  integer i,n1 = ( onemore == 1) ? n+1 : n;
  if (ReallocVector(n1) == 1)
    {
      for (i = 0; i < n; i++){
#ifdef DEBUG
	if ( Abs(vx[i]) > int16max )
	  {
	    fprintf(stderr,"Warning store_point oustide of 16bits x=%d\n",
		    (int) vx[i]);
	  }
	if ( Abs(vy[i]) > int16max )
	  {
	    fprintf(stderr,"Warning store_point oustide of 16bits x=%d\n",
		    (int) vy[i]);
	  }
#endif
	
        points[i].x = INT_2_16B( vx[i] ) ;
        /*points[i].x = (short) vx[i];*/
        
        
        points[i].y = INT_2_16B( vy[i] ) ;
        /* points[i].y = (short) vy[i]; */

      }
      if ( onemore ) {
	points[n].x = points[0].x;
	points[n].y = points[0].y;
      }
      return(1);
    }
  else return(0);
}

static int ReallocVector(integer n)
{
  if (( points = graphic_alloc(8,n,sizeof(XPoint))) == 0) 
    { 
      sciprint(MESSAGE5); return 0;
    }
  return 1;
}

/************************************************************
 * Clipping functions 
 ************************************************************/

void XDroutine(int npts)
{
  if (ScilabXgc->Cdrawable != (Drawable) 0)
    XDrawLines (dpy, ScilabXgc->Cdrawable, gc, get_xpoints(),(int) npts,
		ScilabXgc->CurVectorStyle);
  if (ScilabXgc->CurPixmapStatus != 1) 
    XDrawLines (dpy, (Drawable) ScilabXgc->CWindow, gc, get_xpoints(),(int) npts,
		ScilabXgc->CurVectorStyle);
}

/* My own clipping routines  
 * XDrawlines with clipping on the current graphic window 
 * to avoid trouble on some X servers 
 */

static integer xleft,xright,ybot,ytop;

/* Test a single point to be within the xleft,xright,ybot,ytop bbox.
 * Sets the returned integers 4 l.s.b. as follows:
 * bit 0 if to the left of xleft.
 * bit 1 if to the right of xright.
 * bit 2 if below of ybot.
 * bit 3 if above of ytop.
 * 0 is returned if inside.
 */

static int clip_point(integer x, integer y)
{
  integer ret_val = 0;

  if (x < xleft) ret_val |= (char)0x01;
  else if (x > xright) ret_val |= (char)0x02;
  if (y < ybot) ret_val |= (char)0x04;
  else if (y > ytop) ret_val |= (char)0x08;
  return ret_val;
}

/* Clip the given line to drawing coords defined as xleft,xright,ybot,ytop.
 *   This routine uses the cohen & sutherland bit mapping for fast clipping -
 * see "Principles of Interactive Computer Graphics" Newman & Sproull page 65.
 * return 0  : segment out 
 *       1  : (x1,y1) changed 
 *	2  : (x2,y2) changed 
 *	3  : (x1,y1) and (x2,y2) changed 
 *	4  : segment in 
 */

void  set_clip_box(integer xxleft, integer xxright, integer yybot, integer yytop)
{
  xleft=xxleft;
  xright=xxright;
  ybot=yybot;
  ytop=yytop;
}

void clip_line(integer x1, integer yy1, integer x2, integer y2, integer *x1n, integer *yy1n, integer *x2n, integer *y2n, integer *flag)
{
  integer  x_intr[2], y_intr[2]; /* used to store intersection points 
				  * at most two points 
				  */
  integer x, y, dx, dy, count, pos1, pos2; 

  *x1n=x1;*yy1n=yy1;*x2n=x2;*y2n=y2;*flag=4;
  pos1 = clip_point(x1, yy1);
  pos2 = clip_point(x2, y2);
  if (pos1 || pos2) {
    if (pos1 & pos2) { *flag=0;return;}	  
    /* segment is totally out. */

    /* Here part of the segment MAy be inside. test the intersection
     * of this segment with the 4 boundaries for hopefully 2 intersections
     * in. If non found segment is totaly out.
     */
    count = 0;
    dx = x2 - x1;
    dy = y2 - yy1;

    /* Find intersections with the x parallel bbox lines: */
    if (dy != 0) {
      x = (int) (ybot - y2)  * ((double) dx / (double) dy) + x2;
      /* Test for ybot boundary. */
      if (x >= xleft && x <= xright) {
	x_intr[count] = x;
	y_intr[count++] = ybot;
      }
      x = (ytop - y2) * ((double) dx / (double) dy) + x2; 
      /* Test for ytop boundary. */
      if (x >= xleft && x <= xright) {
	x_intr[count] = x;
	y_intr[count++] = ytop;
      }
    }
    if ( count < 2 ) 
      {
	/* Find intersections with the y parallel bbox lines: */
	if (dx != 0) {
	  y = (xleft - x2) * ((double) dy / (double) dx) + y2;   
	  /* Test for xleft boundary. */
	  if (y >= ybot && y <= ytop) {
	    x_intr[count] = xleft;
	    y_intr[count++] = y;
	  }
	  if ( count < 2 ) 
	    {
	      y = (xright - x2) * ((double) dy / (double) dx) + y2;  
	      /* Test for xright boundary. */
	      if (y >= ybot && y <= ytop) {
		x_intr[count] = xright;
		y_intr[count++] = y;
	      }
	    }
	}
      }

    if (count == 2) {
      if (pos1 && pos2) {	   /* Both were out - update both */
	*x1n = x_intr[0];
	*yy1n = y_intr[0];
	*x2n = x_intr[1];
	*y2n = y_intr[1];
	*flag=3;return;
      }
      else if (pos1) {       /* Only x1/yy1 was out - update only it */
	if (dx * (x2 - x_intr[0]) + dy * (y2 - y_intr[0]) >= 0) {
	  *x1n = x_intr[0];
	  *yy1n = y_intr[0];
	  *flag=1;return;
	}
	else {
	  *x1n = x_intr[1];
	  *yy1n = y_intr[1];
	  *flag=1;return;
	}
      }
      else {	         /* Only x2/y2 was out - update only it */
	if (dx * (x_intr[0] - x1) + dy * (y_intr[0] - yy1) >= 0) {
	  *x2n = x_intr[0];
	  *y2n = y_intr[0];
	  *flag=2;return;
	}
	else {
	  *x2n = x_intr[1];
	  *y2n = y_intr[1];
	  *flag=2;return;
	}
      }
    }
    else 
      {
	/* count != 0 */
	*flag=0;return;
      }
  }
}

/* static void change_points(integer i, integer x, integer y) */
/* { */
/*   points[i].x=(short)x;   points[i].y=(short)y; */
/* } */

/* static void MyDraw(integer iib, integer iif, integer *vx, integer *vy) */
/* { */
/*   integer x1n,y1n,x11n,y11n,x2n,y2n,flag2=0,flag1=0; */
/*   integer npts; */
/*   npts= ( iib > 0) ? iif-iib+2  : iif-iib+1; */
/*   if ( iib > 0)  */
/*     { */
/*       clip_line(vx[iib-1],vy[iib-1],vx[iib],vy[iib],&x1n,&y1n,&x2n,&y2n,&flag1); */
/*     } */
/*   clip_line(vx[iif-1],vy[iif-1],vx[iif],vy[iif],&x11n,&y11n,&x2n,&y2n,&flag2); */
/*   if (C2F(store_points)(npts, &vx[Max(0,iib-1)], &vy[Max(0,iib-1)],(integer)0L)); */
/*   { */
/*     if (iib > 0 && (flag1==1||flag1==3)) change_points((integer)0L,x1n,y1n); */
/*     if (flag2==2 || flag2==3) change_points(npts-1,x2n,y2n); */
/*     XDroutine((int)npts); */

/*   } */
/* } */

/* static void My2draw(integer j, integer *vx, integer *vy) */
/* { */
/*   /\** The segment is out but can cross the box **\/ */
/*   integer vxn[2],vyn[2],flag; */
/*   integer npts=2; */
/*   clip_line(vx[j-1],vy[j-1],vx[j],vy[j],&vxn[0],&vyn[0],&vxn[1],&vyn[1],&flag); */
/*   if (flag == 3 && C2F(store_points)(npts,vxn,vyn,(integer)0L)) */
/*     { */
/* #ifdef DEBUG */
/*       sciprint("segment out mais intersecte en (%d,%d),(%d,%d)\r\n", */
/* 	       vxn[0],vyn[0],vxn[1],vyn[1]); */
/* #endif  */
/*       XDroutine((int)npts); */
 
/*     } */
/* } */

/* 
 *  returns the first (vx[.],vy[.]) point inside 
 *  xleft,xright,ybot,ytop bbox. begining at index ideb
 *  or zero if the whole polyline is out 
 */

integer first_in(integer n, integer ideb, integer *vx, integer *vy)
{
  integer i;
  for (i=ideb  ; i < n ; i++)
    {
      if (vx[i]>= xleft && vx[i] <= xright  && vy[i] >= ybot && vy[i] <= ytop)
	{
#ifdef DEBUG
	  sciprint("first in %d->%d=(%d,%d)\r\n",ideb,i,vx[i],vy[i]);
#endif
	  return(i);
	}
    }
  return(-1);
}

/* 
 *  returns the first (vx[.],vy[.]) point outside
 *  xleft,xright,ybot,ytop bbox.
 *  or zero if the whole polyline is out 
 */

integer first_out(integer n, integer ideb, integer *vx, integer *vy)
{
  integer i;
  for (i=ideb  ; i < n ; i++)
    {
      if ( vx[i]< xleft || vx[i]> xright  || vy[i] < ybot || vy[i] > ytop) 
	{
#ifdef DEBUG
	  sciprint("first out %d->%d=(%d,%d)\r\n",ideb,i,vx[i],vy[i]);
#endif
	  return(i);
	}
    }
  return(-1);
}

/* static void C2F(analyze_points)(integer n, integer *vx, integer *vy, integer onemore) */
/* {  */
/*   integer iib,iif,ideb=0,vxl[2],vyl[2]; */
/*   integer verbose=0,wd[2],narg; */
/*   xget_windowdim(&verbose,wd,&narg,vdouble); */
/*   xleft=0;xright=wd[0]; ybot=0;ytop=wd[1]; */
/* #ifdef DEBUG1 */
/*   xleft=100;xright=300; */
/*   ybot=100;ytop=300; */
/*   if (ScilabXgc->Cdrawable != (Drawable) 0) */
/*     XDrawRectangle(dpy, ScilabXgc->Cdrawable, gc,xleft,ybot,(unsigned)xright-xleft, */
/* 		   (unsigned)ytop-ybot); */
/*   if (ScilabXgc->CurPixmapStatus != 1)  */
/*     XDrawRectangle(dpy,(Drawable) ScilabXgc->CWindow , gc,xleft,ybot,(unsigned)xright-xleft, */
/* 		   (unsigned)ytop-ybot); */
/* #endif */
/* #ifdef DEBUG  */
/*   sciprint("inside analyze\r\n"); */
/* #endif */
/*   while (1)  */
/*     { integer j; */
/*     iib=first_in(n,ideb,vx,vy); */
/*     if (iib == -1)  */
/*       {  */
/* #ifdef DEBUG */
/* 	sciprint("[%d,end=%d] polyline out\r\n",(int)ideb,(int)n); */
/* 	/\* all points are out but segments can cross the box *\/ */
/* #endif  */
/* 	for (j=ideb+1; j < n; j++) My2draw(j,vx,vy); */
/* 	break; */
/*       } */
/*     else  */
/*       if ( iib - ideb > 1)  */
/* 	{ */
/* 	  /\* un partie du polygine est totalement out de ideb a iib -1 *\/ */
/* 	  /\* mais peu couper la zone *\/ */
/* 	  for (j=ideb+1; j < iib; j++) My2draw(j,vx,vy); */
/* 	}; */
/*     iif=first_out(n,iib,vx,vy); */
/*     if (iif == -1) { */
/*       /\* special case the polyligne is totaly inside *\/ */
/*       if (iib == 0)  */
/* 	{ */
/* 	  if (C2F(store_points)(n,vx,vy,onemore)) */
/* 	    { */
/* 	      int n1 ; */
/* 	      if (onemore == 1) n1 = n+1;else n1= n; */
/* 	      XDroutine(n1); */
/* 	      return; */
/* 	    } */
/* 	  else */
/* 	    return; */
/* 	} */
/*       else  */
/* 	MyDraw(iib,n-1,vx,vy); */
/*       break; */
/*     } */
/* #ifdef DEBUG */
/*     sciprint("Analysed : [%d,%d]\r\n",(int)iib,(int)iif); */
/* #endif  */
/*     MyDraw(iib,iif,vx,vy); */
/*     ideb=iif; */
/*     } */
/*   if (onemore == 1) { */
/*     /\* The polyligne is closed we consider the closing segment *\/ */
/*     integer x1n,y1n,x2n,y2n,flag1=0; */
/*     vxl[0]=vx[n-1];vxl[1]=vx[0];vyl[0]=vy[n-1];vyl[1]=vy[0]; */
/*     clip_line(vxl[0],vyl[0],vxl[1],vyl[1],&x1n,&y1n,&x2n,&y2n,&flag1); */
/*     if ( flag1==0) return ; */
/*     if ( C2F(store_points)((integer)2L,vxl,vyl,(integer)0L)) */
/*       { */
/* 	if (flag1==1||flag1==3) change_points((integer)0L,x1n,y1n); */
/* 	if (flag1==2||flag1==3) change_points((integer)1L,x2n,y2n); */
/* 	XDroutine(2); */
/*       } */
/*   } */
/* } */

int CheckScilabXgc(void)
{
  return( ScilabXgc != (struct BCG *) 0);
}

