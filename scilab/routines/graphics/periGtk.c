/*------------------------------------------------------------------------
 *    Graphic library
 *    Copyright (C) 2001-2003 Enpc/Jean-Philippe Chancelier
 *    jpc@cermics.enpc.fr 
 --------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------
 *    Gtk  Driver.
 *--------------------------------------------------------------------------*/

#ifndef nsp 
#define Sciprintf sciprint
#endif 

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <gtk/gtk.h>
#include "Math.h"
#include "bcg.h"
#include "../version.h"
#include "color.h"
#include "Events.h"

extern int  Scierror __PARAMS((int iv,char *fmt,...));
extern integer C2F(ismenu)();
extern int C2F(getmen)(char * btn_cmd,integer * lb, integer * entry);
extern void * graphic_initial_menu(int winid);
extern void MenuFixCurrentWin __PARAMS(( int ivalue));
extern void GPopupResize __PARAMS((struct BCG *ScilabXgc,int *,int *));
extern int versionflag;

/** Global variables to deal with X11 **/

static double *vdouble = 0; /* used when a double argument is needed */
static unsigned long maxcol;
void setcolormapg(struct  BCG *XGC,integer *v1,integer *v2, double *a, integer *v3);/* NG */

/* These DEFAULTNUMCOLORS colors come from Xfig */

unsigned short  default_colors[] = {
  0,   0,   0, /* Black: DEFAULTBLACK */
  0,   0, 255, /* Blue */
  0, 255,   0, /* Green */
  0, 255, 255, /* Cyan */
  255,   0,   0, /* Red */
  255,   0, 255, /* Magenta */
  255,   0,   0, /* Yellow */
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

GtkWidget *GetWindowNumber  __PARAMS((int));
GtkWidget *GetBGWindowNumber __PARAMS((int));

struct BCG *GetWindowXgcNumber __PARAMS((int));
static struct BCG *AddNewWindowToList __PARAMS((void));

static void ResetScilabXgc __PARAMS((void));
static void DrawMark(integer *x, integer *y),LoadFonts(void), LoadSymbFonts(void), C2F(analyze_points)(integer n, integer *vx, integer *vy, integer onemore);
static void DrawMark(integer *x, integer *y), My2draw(integer j, integer *vx, integer *vy), MyDraw(integer iib, integer iif, integer *vx, integer *vy), change_points(integer i, integer x, integer y);
static void C2F(loadfamily_n)(char *name, integer *j);
static void PixmapClear   __PARAMS((int x,int y,int w,int h));
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

/* declaration for xget('name',...) functions */

typedef void (xget_f) __PARAMS((integer *verbose, integer *x,integer *narg, double *dummy));
static xget_f xget_windowpos,xget_windowdim,xget_popupdim,xget_viewport,xget_curwin ,xget_clip;
static xget_f xget_absourel,xget_alufunction,xget_thickness,xget_pattern,xget_last,xget_dash;
static xget_f xget_usecolor,xget_pixmapOn,xget_wresize,xget_colormap,xget_background;
static xget_f xget_foreground,xget_hidden3d;
static xget_f xget_font,xget_mark,xget_dash_or_color;
static xget_f xget_scilabxgc,xget_scilabFigure,xget_scilabVersion;/* NG */
static xget_f xget_colormap_size;

/* utility for points allocations */

static GdkPoint *gtk_get_xpoints(void);
static int GtkReallocVector __PARAMS((integer n));
static int gtk_store_points __PARAMS((integer n, integer *vx,integer *vy,int  onemore));

typedef void (*r_c) __PARAMS((int x,int y,int w,int h));
static void RectangleClear   __PARAMS((int x,int y,int w,int h,int clipflag,r_c f ));
static void R_clear  __PARAMS((int x,int y,int w,int h));

static void CreateGtkGWindow (struct BCG *ScilabXgc) ;
void create_graphic_window_menu(struct BCG *dd);
void start_sci_gtk();

/* Allocating colors in BCG struct */

#define PIXEL_FROM_RGB(r,g,b) gdk_rgb_xpixel_from_rgb(((guchar)r << 16)|((guchar)g << 8)|((guchar)b))
#define PIXEL_FROM_CMAP(i) PIXEL_FROM_RGB(ScilabXgc->Red[i],ScilabXgc->Green[i],ScilabXgc->Blue[i])


void C2F(initgraphic) (char *string, integer *v2, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
void C2F(xinfo) (char *message, integer *v2, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
void C2F(MissileGCGetorSet) (char *str, integer flag, integer *verbose, integer *x1, integer *x2, integer *x3, integer *x4, integer *x5, integer *x6, double *dv1);
void C2F(drawrectangle) (char *str, integer *x, integer *y, integer *width, integer *height, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
void C2F(DispStringAngle) (integer *x0, integer *yy0, char *string, double *angle);
void C2F(boundingbox) (char *string, integer *x, integer *y, integer *rect, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
void C2F(fillpolylines) (char *str, integer *vectsx, integer *vectsy, integer *fillvect, integer *n, integer *p, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
void C2F(fillrectangle) (char *str, integer *x, integer *y, integer *width, integer *height, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
void C2F(drawarc) (char *str, integer *x, integer *y, integer *width, integer *height, integer *angle1, integer *angle2, double *dv1, double *dv2, double *dv3, double *dv4);
void C2F(fillarc) (char *str, integer *x, integer *y, integer *width, integer *height, integer *angle1, integer *angle2, double *dv1, double *dv2, double *dv3, double *dv4);
void C2F(drawpolymark) (char *str, integer *n, integer *vx, integer *vy, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
void C2F(drawpolyline) (char *str, integer *n, integer *vx, integer *vy, integer *closeflag, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
void C2F(fillpolyline) (char *str, integer *n, integer *vx, integer *vy, integer *closeflag, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);
void C2F(loadfamily) (char *name, integer *j, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4);

static int XgcAllocColors(struct BCG *xgc, int m)
{
  /* don't forget black and white */
  int mm = m + 2;
  if ( (!(xgc->Red = (float *) MALLOC(mm*sizeof(float))))
       || (!(xgc->Green = (float *) MALLOC(mm*sizeof(float))))
       || (!(xgc->Blue = (float *) MALLOC(mm*sizeof(float)))) ) 
    {
      Sciprintf("XgcAllocColors: unable to alloc\n");
      FREE(xgc->Red);FREE(xgc->Green);FREE(xgc->Blue);
      return 0;
    }
  return 1;
}



/*---------------------------------------------------------
 * Pixmap routines: 
 *---------------------------------------------------------*/

static void xset_pixmapclear(integer *v1, integer *v2, integer *v3, integer *v4)
{
  PixmapClear(0,0,ScilabXgc->CWindowWidth,ScilabXgc->CWindowHeight);
}

static void xset_show(integer *v1, integer *v2, integer *v3, integer *v4)
{
  if ( ScilabXgc->Cdrawable != ScilabXgc->drawing->window) 
    {
      /* copy pixmap ScilabXgc->Cdrawable to  ScilabXgc->drawing->window
       * and expose 
       */
      gdk_gc_set_background(ScilabXgc->stdgc, &ScilabXgc->gcol_bg);
      /* drawing to the window and to the backing store pixmap */
      gdk_draw_pixmap(ScilabXgc->drawing->window,ScilabXgc->stdgc, ScilabXgc->Cdrawable,0,0,
		      0,0,ScilabXgc->CWindowWidth, ScilabXgc->CWindowHeight);
      gdk_draw_pixmap(ScilabXgc->pixmap, ScilabXgc->stdgc, ScilabXgc->Cdrawable,0,0,0,0,
		      ScilabXgc->CWindowWidth, ScilabXgc->CWindowHeight);
      /* force expose */
      /* gtk_widget_queue_draw(ScilabXgc->private->drawing);*/
    }
}

/* 
 * resize the extra pixmap associated to the window x,y is the new size 
 */

void CPixmapResize(int x, int y)
{
  /** create a new pixmap **/
  GdkDrawable *temp = (GdkDrawable *) gdk_pixmap_new(ScilabXgc->drawing->window,x,y,-1);
  if ( temp  == NULL ) 
    {
      Sciprintf("No more space to create Pixmaps\r\n");
      return;
    }
  gdk_pixmap_unref((GdkPixmap *) ScilabXgc->Cdrawable);
  ScilabXgc->Cdrawable = temp;
  PixmapClear(0,0,x,y);
}

/*
 * Pixmap clear: clear the extra pixmap associated to the window 
 */

static void PixmapClear(int x, int y, int w, int h)
{
  gdk_gc_set_background(ScilabXgc->stdgc, &ScilabXgc->gcol_bg);
  gdk_draw_rectangle(ScilabXgc->Cdrawable,ScilabXgc->stdgc, TRUE,
		     0,0,ScilabXgc->CWindowWidth, ScilabXgc->CWindowHeight);
}

/* 
 * Resize the Pixmap according to window size change 
 * But only if there's a pixmap 
 */

void CPixmapResize1(void)
{
  if (ScilabXgc->Cdrawable != (GdkDrawable *) ScilabXgc->drawing->window)
    CPixmapResize(ScilabXgc->CWindowWidth, ScilabXgc->CWindowHeight);
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
  /* Test not really usefull: see sciwin in matdes.f */
  integer ierr;
  if ( ScilabXgc == (struct BCG *)0 || ScilabXgc->window ==  NULL) 
    C2F(initgraphic)("",PI0,&ierr,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
  gdk_window_show(ScilabXgc->window->window);
  gdk_flush();
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
  /* we use the stdgc graphic context */
  gdk_gc_set_foreground(ScilabXgc->stdgc, &ScilabXgc->gcol_bg);
  gdk_draw_rectangle(ScilabXgc->Cdrawable, ScilabXgc->stdgc, TRUE, 0, 0,
		     ScilabXgc->CWindowWidth, ScilabXgc->CWindowHeight);
  if ( ScilabXgc->Cdrawable == ScilabXgc->drawing->window) 
    gdk_draw_rectangle(ScilabXgc->pixmap, ScilabXgc->stdgc, TRUE, 0, 0,
		       ScilabXgc->CWindowWidth, ScilabXgc->CWindowHeight);
}

/* generates a pause, in seconds */

#if defined(__STDC__) || defined(_IBMR2)
/** for usleep **/
#include <unistd.h> 
#endif 

void C2F(xpause)(char *str, integer *sec_time, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{ 
  unsigned useconds;
  useconds=(unsigned) *sec_time;
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

/*-----------------------------------------------------------------
 * Changes the graphic window popupname 
 *-----------------------------------------------------------------*/

/* XXXX */
extern char *sci_convert_to_utf8(char *str, int *alloc);

void Setpopupname(char *string)
{ 
  int alloc ;
  char *str_new = sci_convert_to_utf8(string,&alloc);
  gtk_window_set_title(GTK_WINDOW(ScilabXgc->window),str_new);
  if ( alloc == TRUE ) g_free (str_new);
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

typedef struct _GTK_locator_info GTK_locator_info;

struct _GTK_locator_info {
  guint win, x,y, ok;
  int getrelease,getmouse,getmen,getkey, button;
  int sci_click_activated;
  guint timer;
  char *str;
  int  lstr;
};

static GTK_locator_info info = { -1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0};

static GdkEventButton last_press={0};/* initialized to zero */

static gboolean locator_button_press(GtkWidget *widget,
				     GdkEventButton *event,
				     BCG *gc)
{
  int id;
  switch (event->type) 
    {
    case GDK_2BUTTON_PRESS : 
      /* fprintf(stderr,"In the double \n"); */
      /* extra event for double click */
      id= event->button-1 +10; /* double click code */
      break;
    case GDK_3BUTTON_PRESS : return;break;
    case GDK_BUTTON_PRESS :
    default: 
      last_press = *event;
      id= event->button-1; /* press code */
      break;
    }
  if ( info.sci_click_activated == 0) 
    {
      PushClickQueue( gc->CurWindow,event->x, event->y,id,0,0);
    }
  else 
    {
      info.ok = 1; info.win=  gc->CurWindow; info.x = event->x; info.y = event->y; 
      info.button = id;
      gtk_main_quit();
    }
  return TRUE;
}

/* here we detect if the release can be considered 
 * to be the release associated to a click (press-release)
 * In that case we return a click i.e a click will 
 * generate at scilab level two events a press and a click !
 * the release is not returned. 
 * a double click will generate : press/click/press/double_click/click
 *
 */

static gboolean locator_button_release(GtkWidget *widget,
				       GdkEventButton *event,
				       BCG *gc)
{
  static GdkDisplay *display=NULL;

/* to compile with gdk<2.4 */
#if GTK_MAJOR_VERSION==2 &&  GTK_MINOR_VERSION>=4
  int display_double_click_distance = display->double_click_distance;
#else
  int display_double_click_distance=5;
#endif

  if ( display == NULL) display=gdk_display_get_default();
  if ((event->time < (last_press.time + 2*display->double_click_time)) &&
      (event->window == last_press.window) &&
      (event->button == last_press.button) &&
      (ABS (event->x - last_press.x) <= display_double_click_distance) &&
      (ABS (event->y - last_press.y) <= display_double_click_distance))
    {
      /* fprintf(stderr,"This is a click\n"); */
      /* return a click */
      if ( info.sci_click_activated == 0 ) 
	{
	  PushClickQueue( gc->CurWindow,event->x, event->y,event->button+2 ,0,0);
	}
      else 
	{
	  info.ok =1 ; info.win=  gc->CurWindow; info.x = event->x;  info.y = event->y;
	  info.button = event->button +2 ;
	  gtk_main_quit();
	}
    }
  else 
    {
      /* return a release */
      if ( info.sci_click_activated == 0 || info.getrelease == 0 ) 
	{
	  PushClickQueue( gc->CurWindow,event->x, event->y,event->button-6 ,0,1);
	}
      else 
	{
	  info.ok =1 ; info.win=  gc->CurWindow; info.x = event->x;  info.y = event->y;
	  info.button = event->button -6;
	  gtk_main_quit();
	}
    }
  return TRUE;
}

static gboolean locator_button_motion(GtkWidget *widget,
				      GdkEventMotion *event,
				      BCG *gc)
{
  gint x,y; 
  GdkModifierType state;
  if (event->is_hint)
    { 
      gdk_window_get_pointer (event->window, &x, &y, &state);
    }
  else 
    {
      x= event->x; y = event->y;
    }
  if ( info.sci_click_activated == 0 || info.getmouse == 0 ) 
    {
      PushClickQueue( gc->CurWindow,x, y,-1 ,1,0);
    }
  else 
    {
      info.ok =1 ;  info.win=  gc->CurWindow; info.x = x;  info.y = y;
      info.button = -1;
      gtk_main_quit();
    }
  return TRUE;
}


static gint key_press_event (GtkWidget *widget, GdkEventKey *event, BCG *gc)
{
  /* modified 30/10/02 to get cursor location and register  key_press_event in queue' SS */
  gint x,y; 
  GdkModifierType state;

  if ( info.sci_click_activated == 0 || info.getkey == 0 ) 
    {
      gdk_window_get_pointer (gc->drawing->window, &x, &y, &state);
      PushClickQueue( gc->CurWindow,x, y,event->keyval ,0,0);      
    }
  else 
    {
      /* fprintf(stderr,"key = %d\n",event->keyval); */
      if ( (event->keyval >= 0x20) && (event->keyval <= 0xFF))
	{
	  /* since Alt-keys and Ctrl-keys are stored in menus I want to ignore them here */
	  gdk_window_get_pointer (gc->drawing->window, &x, &y, &state);
	  info.x=x ; info.y=y;
	  info.ok =1 ;  info.win=  gc->CurWindow; 
	  /* This is not good we should return modifiers 
	   * as extra data 
	   */
	  /* 1000 + key : Ctrl-key 
	   * 2000 + key : Ctrl-Alt-key 
	   * 3000 + key : Alt-key 
	   */
	  if ( event->state & GDK_CONTROL_MASK ) 
	    {
	      info.button = ( event->state & GDK_MOD1_MASK ) ? 2000: 1000;
	    }
	  else
	    {
	      info.button = ( event->state & GDK_MOD1_MASK ) ? 3000: 0;
	    }
	  info.button += event->keyval ;
	  gtk_main_quit();
	}
    }
  return FALSE; /* want also the menu to receive the key pressed */
}


static gboolean sci_destroy_window (GtkWidget *widget, GdkEventKey *event,  BCG *gc)
{
  if (  get_delete_win_mode() == 1 )
    {
      wininfo("Cannot destroy window while acquiring zoom rectangle ");
      return TRUE;
    }
  if ( info.sci_click_activated != 0 ) 
    {
      info.ok =1 ;  info.win=  gc->CurWindow; info.x = 0 ;  info.y = 0;
      info.button = -100;
      C2F(deletewin)(&gc->CurWindow);
      /* DeleteSGWin(gc->CurWindow); */
      gtk_main_quit();
    }
  else 
    {
      C2F(deletewin)(&gc->CurWindow);
      /* DeleteSGWin(gc->CurWindow); */
    }

  return TRUE;
}

static gint timeout_test (BCG *gc)
{
  if ( info.getmen  == 1 &&  C2F(ismenu)()==1 ) 
    {
      int entry;
      /*sciprint("je suis ds le timeout un menu est active");*/
      C2F(getmen)(info.str,&info.lstr,&entry);
      info.ok = 1 ; info.x = 0 ; info.y =0 ; info.button =  -2;
      info.win = gc->CurWindow; /* XXXX à revoir */
      gtk_main_quit();
    }
  return TRUE;
}

  
#ifdef WITH_TK

extern int flushTKEvents();

static gint timeout_tk (void *v)
{
  flushTKEvents();
  return TRUE;
}

#endif


void C2F(xclick_any)(char *str, integer *ibutton, integer *x1, 
		     integer *yy1, integer *iwin, integer *iflag,
		     integer *istr, double *dv1, double *dv2, 
		     double *dv3, double *dv4)
{
  guint timer_tk;
  GTK_locator_info rec_info ; 
  int win = -1,i;
  int motion,release,ok;
  int wincount = GetWinsMaxId()+1;
  if (wincount == 0) 
    {
      *x1=0;  *yy1=0;  *iwin=0;  *ibutton = -100;  *istr = 0;
      return; 
    }
  
  if ( *iflag ==0 )   ClearClickQueue(-1);  /*clear the queue if required*/
  /* checks  if we already have something on the queue **/ 
 
    win=-1;
    ok=0;
    while (CheckClickQueue(&win,x1,yy1,ibutton,&motion,&release) == 1) {
      if ((motion==0) && (release==0)) {
	*iwin = win ;
	ok=1;
	break;
      }

    }

  /* change the cursors */ 

  for (i=0; i < wincount ; i++ ) 
    {
      struct BCG *bcg =  GetWindowXgcNumber(i);
      if ( bcg  != NULL)
	gdk_window_set_cursor(bcg->drawing->window,bcg->gcursor);
    }

  /* save info in local variable  */
  rec_info = info;
  /* set info */ 
  info.ok = 0 ; 
  info.getrelease = 0 ;
  info.getmouse   = 0 ; 
  info.getkey     = 1 ; 
  info.getmen     = 1 ; 
  info.sci_click_activated = 1;

  if (  info.getmen == 1  ) 
    {
      /*  Check soft menu activation during xclick_any */ 
      info.timer = gtk_timeout_add(100, (GtkFunction) timeout_test, ScilabXgc);
      info.str   = str;
    }

  /* take care of tck/tk */
  
#ifdef WITH_TK
  timer_tk=  gtk_timeout_add(100,  (GtkFunction) timeout_tk , NULL);
#endif

  while (1) 
    {
      /* take care of window destroy during this .....XXXXXX */
      gtk_main();
      /* be sure that gtk_main_quit was activated by proper event */
      if ( info.ok == 1 ) break;
    }
  
  *x1 = info.x; 
  *yy1 = info.y;
  *ibutton = info.button;
  *iwin = info.win;
  *istr = info.lstr;

  /* remove timer if it was set by us */ 
  if ( info.getmen == 1 )  gtk_timeout_remove (info.timer);
  
#ifdef WITH_TK
  gtk_timeout_remove(timer_tk);
#endif

  /* take care of recursive calls i.e restore info  */
  info = rec_info ; 

  for (i=0; i < wincount ; i++ ) 
    {
      struct BCG *bcg =  GetWindowXgcNumber(i);
      if ( bcg  != NULL) 
	gdk_window_set_cursor(bcg->drawing->window,bcg->ccursor);
    }
}

void C2F(xclick)(char *str, integer *ibutton, integer *x1,
		 integer *yy1, integer *iflag, integer *istr,
		 integer *v7, double *dv1, double *dv2,
		 double *dv3, double *dv4)
{
  integer lstr ;
  SciClick(ibutton,x1, yy1,iflag,0,0,*istr,str,&lstr);
  if ( *istr == 1) 
    {
      if (*ibutton == -2) 
	{
	  /*	  Sciprintf("Menu activated %s %d",str,lstr);*/
	  *istr = lstr;
	}
      else
	*istr = 0;
    }
}

void C2F(xgetmouse)(char *str, integer *ibutton, integer *x1, 
		    integer *yy1, integer *iflag, integer *v6, 
		    integer *v7, double *dv1, double *dv2,
		    double *dv3, double *dv4)
{
  integer lstr ;
  SciClick(ibutton,x1, yy1,iflag,v6[0],v6[1],0,(char *) 0,&lstr);
}

/*------------------------------------------------------------------------------
 * wait for events: mouse motion and mouse press and release 
 *                  and dynamic menu activation through a timeout 
 * 
 * if iflag = 0 : clear previous mouse click 
 * if iflag = 1 : don't 
 * if getmouse = 1 : check also mouse move 
 * if getrelease=1 : check also mouse release 
 * if dyn_men = 1 ; check also dynamic menus (returns the menu code in str )
 * return value : 0,1,2 if button pressed 
 *                -5,-4,-3: if button release
 *                -100 : error 
 *                -2   : menu activated 
 *------------------------------------------------------------------------------*/

/* XXXX
 * A finir pour tenir compte des control C de l'utilisateur  */

void SciClick(integer *ibutton, integer *x1, integer *yy1, integer *iflag, 
	      int getmouse, int getrelease, int dyn_men, char *str, integer *lstr)
{
  guint timer_tk;
  GTK_locator_info rec_info ; 
  int win,motion,release,ok;
  if ( ScilabXgc == (struct BCG *) 0 || ScilabXgc->Cdrawable == NULL ) {
    *ibutton = -100;     return;
  }
  win = ScilabXgc->CurWindow;
  if ( *iflag ==1 && CheckClickQueue(&win,x1,yy1,ibutton,&motion,&release) == 1) 
    { 
      /* sciprint("ds la queue %f %f \n",(double) *x1,(double) *yy1);*/ /* XXXX */
      return ;
    }
  if ( *iflag ==0 )  ClearClickQueue(ScilabXgc->CurWindow);

  gdk_window_set_cursor (ScilabXgc->drawing->window,ScilabXgc->gcursor);
  
  /* save info in local variable  */
  rec_info = info;
  /* set info */ 
  info.ok = 0 ; 
  info.getrelease = getrelease ; 
  info.getmouse   = getmouse ;
  info.getmen     = dyn_men;
  info.getkey     = 1; /*  check keys in xclick */
  info.sci_click_activated = 1;

  if ( info.getmen == 1 && info.timer == 0 ) 
    {
      info.timer = gtk_timeout_add (100, (GtkFunction) timeout_test, ScilabXgc);
      info.str   = str;
    }

  
#ifdef WITH_TK
  timer_tk=  gtk_timeout_add(100,  (GtkFunction) timeout_tk , NULL);
#endif

  
  while (1) 
    {
      gtk_main();
      /* be sure that gtk_main_quit was activated by proper event */
      if ( info.ok == 1 &&  info.win == win  ) break;
    }

#ifdef WITH_TK
  gtk_timeout_remove(timer_tk);
#endif

  *x1 = info.x;
  *yy1 = info.y;
  *ibutton = info.button;
  *lstr = info.lstr;
  
  /* remove timer */ 
  if ( dyn_men == 1 && rec_info.timer == 0 )  gtk_timeout_remove (info.timer);

  /* take care of recursive calls i.e restore info  */
  info = rec_info ; 

  if ( ScilabXgc != (struct BCG *) 0 && ScilabXgc->Cdrawable != NULL ) {
    gdk_window_set_cursor (ScilabXgc->drawing->window,ScilabXgc->ccursor);
  }

}



/*******************************************************
 * clear a rectangle zone 
 *******************************************************/

static void R_clear(int x, int y, int w, int h)
{
  gdk_draw_rectangle(ScilabXgc->Cdrawable, ScilabXgc->wgc, TRUE,x,y,w,h);
  if ( ScilabXgc->Cdrawable == ScilabXgc->drawing->window) 
    gdk_draw_rectangle(ScilabXgc->pixmap, ScilabXgc->wgc, TRUE,x,y,w,h);
}

static void RectangleClear(int x, int y, int w, int h, int clipflag, r_c F)
{
  /* switch to a clear gc */
  int cur_alu = ScilabXgc->CurDrawFunction;
  int clear = 0 ; /* 0 is the Xclear alufunction */;
  if ( cur_alu != clear ) xset_alufunction1(&clear,PI0,PI0,PI0); 
  if ( clipflag == 1 && ScilabXgc->ClipRegionSet == 1) 
    {
      static GdkRectangle clip_rect = { 0,0,int16max,  int16max};
      gdk_gc_set_clip_rectangle(ScilabXgc->wgc, &clip_rect);
    }
  (*F)(x,y,w,h);
  if ( cur_alu != clear )
    xset_alufunction1(&cur_alu,PI0, PI0,PI0);   /* back to current value */ 
  if ( clipflag == 1 && ScilabXgc->ClipRegionSet == 1) 
    {
      /* restor clip */
      GdkRectangle clip_rect = { ScilabXgc->CurClipRegion[0],
				 ScilabXgc->CurClipRegion[1],
				 ScilabXgc->CurClipRegion[2],
				 ScilabXgc->CurClipRegion[3]};
      gdk_gc_set_clip_rectangle(ScilabXgc->wgc, &clip_rect);
    }
}

void C2F(cleararea)(char *str, integer *x, integer *y, integer *w, integer *h, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{
  RectangleClear(*x,*y,*w,*h,0,R_clear);
}



/************************************************************************
 * graphic context modifications 
 ************************************************************************/

/** to get the window upper-left point coordinates on the screen  **/

static void xget_windowpos(integer *verbose, integer *x, integer *narg, double *dummy)
{
  gint xx,yy;
  gdk_window_get_position (ScilabXgc->window->window,&xx,&yy);
  *narg = 2; x[0]=xx;x[1]=yy;
  if (*verbose == 1) 
    Sciprintf("\n ScilabXgc->CWindow position :%d,%d\r\n",x[0],x[1]);
}


/** to set the window upper-left point position on the screen **/

static void xset_windowpos(integer *x, integer *y, integer *v3, integer *v4)
{
  integer ierr;
  if (ScilabXgc == NULL || ScilabXgc->window ==  NULL) 
    C2F(initgraphic)("",PI0,&ierr,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
  gdk_window_move (ScilabXgc->window->window, *x,*y);
}

/** To get the drawbox  window size : used by periGif **/

void C2F(getwindowdim) (integer *verbose, integer *x, integer *narg, double *dummy)
{   
  xget_windowdim(verbose, x, narg,dummy);
}

/** To get the drawbox  window size **/

static void xget_windowdim(integer *verbose, integer *x, integer *narg, double *dummy)
{     
  /* the two dimensions are always updated */
  *narg = 2;
  x[0]= ScilabXgc->CWindowWidth;
  x[1]= ScilabXgc->CWindowHeight;
  if (*verbose == 1) 
    Sciprintf("\n ScilabXgc->CWindow dim :%d,%d\r\n",(int) x[0],(int) x[1]);
} 

/*
 * To change the drawbox window size. 
 * Here this function set the min size of the graphic window 
 * which means that if the scrolled window 
 * is smaller than the min size scrollbar will be drawn 
 * but if the scrolled window is greater then drawbox will follow 
 */

static void xset_windowdim(integer *x, integer *y, integer *v3, integer *v4)
{
  /* XXXX: not so easy !!! */
  if (ScilabXgc == NULL || ScilabXgc->window ==  NULL) return ;
  gtk_widget_set_usize(ScilabXgc->drawing,*x,*y);
  /* XXXXX gdk_window_resize(ScilabXgc->drawing->window,*x,*y); */
  gdk_flush();
}

/** To get the popup  window size **/

static void xget_popupdim(integer *verbose, integer *x, integer *narg, double *dummy)
{ 
  gint xx,yy;
  gdk_window_get_size (ScilabXgc->window->window,&xx,&yy);
  *narg = 2;  x[0]= xx ;   x[1]= yy ; 
  if (*verbose == 1) 
    Sciprintf("\n ScilabXgc->CWindow dim :%d,%d\r\n",(int) x[0],(int) x[1]);
} 

/** To change the popup window size  **/

static void xset_popupdim(integer *x, integer *y, integer *v3, integer *v4)
{
  gdk_window_resize(ScilabXgc->window->window,*x,*y);
}

/** To get the viewport Upper/Left point Position **/

static void xget_viewport(integer *verbose, integer *x, integer *narg, double *dummy)
{     
  GtkAdjustment * H, *V;
  *narg = 2; 
  if ( ScilabXgc->CurResizeStatus == 0) 
    {
      /* get the horizontal and vertival adjustments of the ? */
      H = gtk_scrolled_window_get_hadjustment (GTK_SCROLLED_WINDOW (ScilabXgc->scrolled));
      V = gtk_scrolled_window_get_vadjustment (GTK_SCROLLED_WINDOW (ScilabXgc->scrolled));
      x[0] = (int) H->value;
      x[1] = (int) V->value;
    }
  else 
    { 
      x[0]=x[1]=0;
    }
  if (*verbose == 1) 
    Sciprintf("\n Viewport position:%d,%d\r\n",(int) x[0],(int) x[1]);
} 

/** To change the window size  **/

static void xset_viewport(integer *x, integer *y, integer *v3, integer *v4)
{
  if ( ScilabXgc->CurResizeStatus == 0) 
    {
      gtk_adjustment_set_value( gtk_scrolled_window_get_hadjustment (GTK_SCROLLED_WINDOW (ScilabXgc->scrolled)),
				(gfloat) *x);
      gtk_adjustment_set_value( gtk_scrolled_window_get_vadjustment (GTK_SCROLLED_WINDOW (ScilabXgc->scrolled)),
				(gfloat) *y);      
    }
}

/********************************************
 * select window intnum as the current window 
 * window is created if necessary 
 ********************************************/

static void xset_curwin(integer *intnum, integer *v2, integer *v3, integer *v4)
{ 
  struct BCG *bcgk;
  bcgk =  ScilabXgc ;
  integer ierr;
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
      /* XWindowAttributes war;
      XGetWindowAttributes(dpy,ScilabXgc->CWindow,&war); 
      ScilabXgc->CWindowWidth = war.width;
      ScilabXgc->CWindowHeight = war.height;
      */
    }
}

/* used in the previous function to set back the graphic scales 
 * when changing form one window to an other 
 * Also used in scig_tops : to force a reset of scilab graphic scales 
 * after a print in Postscript or Xfig 
 */

void SwitchWindow(integer *intnum)
{
  /** trying to get window *intnum **/
  struct BCG *SXgc =  GetWindowXgcNumber(*intnum);
  integer ierr;
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
    Sciprintf("\nCurrent Graphic Window :%d\r\n",(int) *intnum);
}

/** Set a clip zone (rectangle ) **/

static void xset_clip(integer *x, integer *y, integer *w, integer *h)
{
  GdkRectangle clip_rect ={ *x,*y,*w,*h};
  ScilabXgc->ClipRegionSet = 1;
  ScilabXgc->CurClipRegion[0]= clip_rect.x;
  ScilabXgc->CurClipRegion[1]= clip_rect.y;
  ScilabXgc->CurClipRegion[2]= clip_rect.width;
  ScilabXgc->CurClipRegion[3]= clip_rect.height;
  gdk_gc_set_clip_rectangle(ScilabXgc->wgc, &clip_rect);
}

/** unset clip zone **/

static void xset_unclip(integer *v1, integer *v2, integer *v3, integer *v4)
{
  static GdkRectangle clip_rect = { 0,0,int16max,  int16max};
  ScilabXgc->ClipRegionSet = 0;
  gdk_gc_set_clip_rectangle(ScilabXgc->wgc, &clip_rect);
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
	Sciprintf("\nThere's a Clip Region :x:%d,y:%d,w:%d,h:%d\r\n",
		 ScilabXgc->CurClipRegion[0],
		 ScilabXgc->CurClipRegion[1],
		 ScilabXgc->CurClipRegion[2],
		 ScilabXgc->CurClipRegion[3]);
      else 
	Sciprintf("\nNo Clip Region");
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
  if (*num == 0 )
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
	Sciprintf("\nTrace Absolu");
      else 
	Sciprintf("\nTrace Relatif");
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
    {"GXclear" , GDK_CLEAR," 0 "},
    {"GXand" , GDK_AND," src AND dst "},
    {"GXandReverse" , GDK_AND_REVERSE," src AND NOT dst "},
    {"GXcopy" , GDK_COPY," src "},
    {"GXandInverted" , GDK_AND_INVERT," NOT src AND dst "},
    {"GXnoop" , GDK_NOOP," dst "},
    {"GXxor" , GDK_XOR," src XOR dst "},
    {"GXor" , GDK_OR," src OR dst "},
    {"GXnor" , GDK_OR," NOT src AND NOT dst "}, /*  GDK_NOR:  XXX missing in gdk */
    {"GXequiv" , GDK_EQUIV," NOT src XOR dst "},
    {"GXinvert" , GDK_INVERT," NOT dst "},
    {"GXorReverse" , GDK_OR_REVERSE," src OR NOT dst "},
    {"GXcopyInverted" , GDK_COPY_INVERT," NOT src "},
    {"GXorInverted" , GDK_OR_INVERT," NOT src OR dst "},
    {"GXnand" , GDK_NAND," NOT src OR NOT dst "},
    {"GXset" , GDK_SET," 1 "}
  };

static void idfromname(char *name1, integer *num)
{int i;
 *num = -1;
 for ( i =0 ; i < 16;i++)
   if (strcmp(AluStruc_[i].name,name1)== 0)  *num=i;
 if (*num == -1 ) 
   {
     Sciprintf("\n Use the following keys (integer in scilab");
     for ( i=0 ; i < 16 ; i++)
       Sciprintf("\nkey %s   -> %s\r\n",AluStruc_[i].name,
		AluStruc_[i].info);
   }
}

void xset_alufunction(char *string)
{   
  integer value;
  idfromname(string,&value);
  if ( value != -1)
    {
      ScilabXgc->CurDrawFunction = value;
      gdk_gc_set_function(ScilabXgc->wgc, AluStruc_[value].id);
    }
}

static void xset_alufunction1(integer *num, integer *v2, integer *v3, integer *v4)
{   
  int value ; 
  GdkColor temp = {0,0,0,0};
  ScilabXgc->CurDrawFunction = Min(15,Max(0,*num));
  value = AluStruc_[ScilabXgc->CurDrawFunction].id;
  switch (value) 
    {
    case GDK_CLEAR : 
      gdk_gc_set_foreground(ScilabXgc->wgc, &ScilabXgc->gcol_bg);
      gdk_gc_set_background(ScilabXgc->wgc, &ScilabXgc->gcol_bg);
      gdk_gc_set_function(ScilabXgc->wgc,GDK_COPY);
      break;
    case GDK_XOR   : 
      temp.pixel = ScilabXgc->gcol_fg.pixel ^ ScilabXgc->gcol_bg.pixel ;
      gdk_gc_set_foreground(ScilabXgc->wgc, &temp);
      gdk_gc_set_background(ScilabXgc->wgc, &ScilabXgc->gcol_bg);
      gdk_gc_set_function(ScilabXgc->wgc,GDK_XOR);
      break;
    default :
      gdk_gc_set_foreground(ScilabXgc->wgc, &ScilabXgc->gcol_fg);
      gdk_gc_set_background(ScilabXgc->wgc, &ScilabXgc->gcol_bg);
      gdk_gc_set_function(ScilabXgc->wgc,value);
      break;
    }
  if ( value == GDK_XOR  && ScilabXgc->CurColorStatus == 1 )
    {
      /** the way colors are computed changes if we are in Xor mode **/
      /** so we force here the computation of current color  **/
      set_c(ScilabXgc->CurColor);
    }
}


static void xget_alufunction(integer *verbose, integer *value, integer *narg, double *dummy)
{ 
  *narg =1 ;
  *value = ScilabXgc->CurDrawFunction ;
  if (*verbose ==1 ) 
    { 
      Sciprintf("\nThe Alufunction is %s -> <%s>\r\n",
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
  int val =  ScilabXgc->CurDashStyle + 1;
  ScilabXgc->CurLineWidth =Max(0, *value);
  /* when line thickness changes we must change the dash style */
  xset_dash(&val,PI0,PI0,PI0);
}

/** to get the thickness value **/

static void xget_thickness(integer *verbose, integer *value, integer *narg, double *dummy)
{
  *narg =1 ;
  *value = ScilabXgc->CurLineWidth ;
  if (*verbose ==1 ) 
    Sciprintf("\nLine Width:%d\r\n", ScilabXgc->CurLineWidth ) ;
}

/** To set grey level for filing areas **/
/** from black (*num =0 ) to white     **/

/* Pixmap  Tabpix_[GREYNUMBER]; */

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
/* 
void C2F(CreatePatterns)(whitepixel, blackpixel)
     Pixel whitepixel;
     Pixel blackpixel;
{ 
  
  integer i ;
  for ( i=0 ; i < GREYNUMBER ; i++)
    Tabpix_[i] =XCreatePixmapFromBitmapData(dpy, root,grey0[i] ,8,8,whitepixel
					   ,blackpixel,XDefaultDepth (dpy,DefaultScreen(dpy)));
 
}
*/

static void xset_pattern(integer *num, integer *v2, integer *v3, integer *v4)
{ 
  if (ScilabXgc->CurColorStatus == 1) 
    {
      set_c(*num-1);
    }
  else 
    {
      /* 
	 integer i ; 
      i= Max(0,Min(*num - 1,GREYNUMBER - 1));
      ScilabXgc->CurPattern = i;
      XSetTile (dpy, gc, Tabpix_[i]); 
      if (i ==0)
	XSetFillStyle(dpy,gc,FillSolid);
      else 
	XSetFillStyle(dpy,gc,FillTiled);
      */
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
    Sciprintf("\n Pattern : %d\r\n",ScilabXgc->CurPattern + 1);
}

/** To get the id of the last pattern **/

static void xget_last(integer *verbose, integer *num, integer *narg, double *dummy)
{
  if ( ScilabXgc->CurColorStatus == 1 ) 
    {
      *num = ScilabXgc->IDLastPattern + 1;
      if (*verbose == 1) 
	Sciprintf("\n Id of Last Color %d\r\n",(int)*num);
    }
  else 
    {
      *num = ScilabXgc->IDLastPattern + 1;
      if (*verbose == 1) 
	Sciprintf("\n Id of Last Pattern %d\r\n",(int)*num);
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
  {2,5,2,5}, {5,2,5,2},  {5,3,2,3}, {8,3,2,3},
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
  if ( *value == 0) 
    {
      gdk_gc_set_line_attributes(ScilabXgc->wgc,
				 (ScilabXgc->CurLineWidth <= 1) ? 0 : ScilabXgc->CurLineWidth,
				 GDK_LINE_SOLID,
				 GDK_CAP_BUTT,
				 GDK_JOIN_ROUND);
    }
  else 
    {
      gint8 buffdash[18];
      int i;
      for ( i =0 ; i < *n ; i++) buffdash[i]=xx[i];
      gdk_gc_set_dashes(ScilabXgc->wgc, 0, buffdash, *n);
      gdk_gc_set_line_attributes(ScilabXgc->wgc, 
				 (ScilabXgc->CurLineWidth == 0 ) ? 1 : ScilabXgc->CurLineWidth,
				 GDK_LINE_ON_OFF_DASH,
				 GDK_CAP_BUTT,
				 GDK_JOIN_ROUND);
    }
}


/** to get the current dash-style **/
/* old version of xget_dash retained for compatibility */

static void xget_dash_or_color(integer *verbose, integer *value, integer *narg, double *dummy)
{
  if ( ScilabXgc->CurColorStatus ==1) 
    {
      *narg =1 ;
      *value = ScilabXgc->CurColor + 1;
      if (*verbose == 1) Sciprintf("Color %d",(int)*value);
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
    { if (*verbose == 1) Sciprintf("\nLine style = Line Solid");}
  else 
    {
      value[1]=4;
      *narg = value[1]+2;
      for (i = 0 ; i < value[1]; i++) value[i+2]=DashTab[*value-2][i];
      if (*verbose ==1) 
	{
	  Sciprintf("\nDash Style %d:<",(int)*value - 1);
	  for (i = 0 ; i < value[1]; i++)
	    Sciprintf("%d ",(int)value[i+2]);
	  Sciprintf(">\n");
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
    Sciprintf("\n Use color %d\r\n",(int)*num);
  *narg=1;
}

/* Change the pixmap status of a Graphic Window. 
 * adding or removing a Background Pixmap to it 
 */

static void xset_pixmapOn(integer *num, integer *v2, integer *v3, integer *v4)
{ 
  int num1= Min(Max(*num,0),1);
  if ( ScilabXgc->CurPixmapStatus == num1 ) return;
  if ( num1 == 1 )
    {
      GdkDrawable *temp ;
      /** create a new pixmap **/
      temp = (GdkDrawable *) gdk_pixmap_new(ScilabXgc->drawing->window,
			    ScilabXgc->CWindowWidth, ScilabXgc->CWindowHeight,
			    -1);
      if ( temp  == NULL ) 
	{
	  C2F(xinfo)("Not enough space to switch to Animation mode",
		     PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
	}
      else 
	{
	  C2F(xinfo)("Animation mode is on,( xset('pixmap',0) to leave)",
		     PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
	  ScilabXgc->Cdrawable = temp;
	  ScilabXgc->CurPixmapStatus = 1;
	  PixmapClear(0,0,ScilabXgc->CWindowWidth,ScilabXgc->CWindowHeight);
	}
    }
  else 
    {
      /** I remove the extra pixmap to the window **/
      C2F(xinfo)(" ",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
      gdk_pixmap_unref((GdkPixmap *) ScilabXgc->Cdrawable);
      ScilabXgc->Cdrawable = (GdkDrawable *)ScilabXgc->drawing->window;
      ScilabXgc->CurPixmapStatus = 0; 
    }
}

static void xget_pixmapOn(integer *verbose, integer *value, integer *narg, double *dummy)
{
  *value=ScilabXgc->CurPixmapStatus;
  *narg =1 ;
  if (*verbose == 1) Sciprintf("Pixmap status %d",(int)*value);
}

/* Change the status of a Graphic Window
 * i.e follows or dont follow the viewport resize
 * Here the behaviour is different 
 * If we want the graphic window to follow the viewport resize 
 * (i.e we dont want to see scrollbars) then we fix the minimum 
 * size of the grahic window to very small values 
 */

static void xset_wresize(integer *num, integer *v2, integer *v3, integer *v4)
{
  integer num1= Min(Max(*num,0),1);
  if ( num1 != ScilabXgc->CurResizeStatus && num1 == 1) 
    {
      /* we want here that the graphic window follows the viewport resize */
      /* we set the min size of graphic window to the min accepted size */
      gtk_widget_set_usize(ScilabXgc->drawing,200,100);
      ScilabXgc->CurResizeStatus = num1 ;
      return ; 
    }
  ScilabXgc->CurResizeStatus = num1 ;
}

static void xget_wresize(integer *verbose, integer *value, integer *narg, double *dummy)
{
  *value=ScilabXgc->CurResizeStatus;
  *narg =1 ;
  if (*verbose == 1) Sciprintf("Resize status %d",(int)*value);
}

/* setting the default colormap with colors defined in color.h */

static int set_default_colormap_flag = 1;

int C2F(sedeco)(int *flag)
{
  set_default_colormap_flag = *flag;
  return(0);
}


/* set_default_colormap is called when raising a window for the first 
 *  time by xset('window',...) or by getting back to default by 
 *  xset('default',...) 
 */

#define SETCOLOR(i,r,g,b)  ScilabXgc->Red[i]=(float)r;ScilabXgc->Green[i]=(float)g;ScilabXgc->Blue[i]=(float)b ; 

void set_default_colormap(void)
{
  int i;
  float *r, *g, *b;
  int m;
  /* we don't want to set the default colormap at window creation 
     if the scilab command was xset("colormap"); 
  */
  if (set_default_colormap_flag == 0) return;
  if (DEFAULTNUMCOLORS > maxcol) {
    Sciprintf("Not enough colors for default colormap. Maximum is %d\r\n",
	     maxcol);
    return;
  }
  m = DEFAULTNUMCOLORS;
  /* Save old color vectors */
  r = ScilabXgc->Red;  g = ScilabXgc->Green;  b = ScilabXgc->Blue;

  if (!XgcAllocColors(ScilabXgc,m)) {
    ScilabXgc->Red = r;    ScilabXgc->Green = g;    ScilabXgc->Blue = b;
    return;
  }
  /* Getting RGB values */
  for (i = 0; i < m; i++) {
    SETCOLOR(i, default_colors[3*i], default_colors[3*i+1], default_colors[3*i+2]);
  }
  /* Black */
  SETCOLOR(m, 0,0,0);
  /* White */
  SETCOLOR(m+1, 255,255,255);
  ScilabXgc->Numcolors = m;
  ScilabXgc->IDLastPattern = m - 1;
  ScilabXgc->CmapFlag = 1;
  /* Black and white pixels */
  ScilabXgc->NumForeground = m;
  ScilabXgc->NumBackground = m + 1;
  FREE(r); FREE(g); FREE(b);
}

/* Setting the colormap 
 *   a must be a m x 3 double RGB matrix: 
 *   a[i] = RED
 *   a[i+m] = GREEN
 *   a[i+2*m] = BLUE
 * v2 gives the value of m and *v3 must be equal to 3 
 */
/* add *v3 (OUT) to know if colormap allocation has succeeded: */
/* 0: succeed */
/* 1: failed */
/* NG beg*/

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
  *v3 = 0;
  setcolormapg(XGC,v1,v2,a,v3);
}
/* NG end*/

void setcolormapg(struct  BCG *XGC,integer *v1,integer *v2, double *a, integer *v3)
{
  int i,m;
  float *r, *g, *b;
  /* 2 colors reserved for black and white */
  if (*v2 != 3 || *v1 < 0 || *v1 > maxcol - 2) {
    Scierror(999,"Colormap must be a m x 3 array with m <= %ld\r\n",
	    maxcol-2);
    *v3 = 1;
    return;
  }
  m = *v1;
  /* Save old color vectors */
  r = XGC->Red;
  g = XGC->Green;
  b = XGC->Blue;

  if (!XgcAllocColors(XGC,m)) {
    XGC->Red = r;
    XGC->Green = g;
    XGC->Blue = b;
    *v3 = 1;
    return;
  }
  /* Checking RGB values */
  for (i = 0; i < m; i++) {
    if (a[i] < 0 || a[i] > 1 || a[i+m] < 0 || a[i+m] > 1 ||
	a[i+2*m] < 0 || a[i+2*m]> 1) {
      Sciprintf("RGB values must be between 0 and 1\n");
      XGC->Red = r;
      XGC->Green = g;
      XGC->Blue = b;
      *v3 = 1;
      return;
    }
    SETCOLOR(i, (float)  (a[i]*255), (float)(a[i+m]*255),(float) (a[i+2*m]*255));
  }

  /* Black */
  SETCOLOR(m, 0,0,0);
  /* White */
  SETCOLOR(m+1, 255,255,255);
  XGC->Numcolors = m;
  XGC->IDLastPattern = m - 1;
  XGC->CmapFlag = 0;
  XGC->NumForeground = m;
  XGC->NumBackground = m + 1;
  xset_usecolor((i=1,&i) ,PI0,PI0,PI0);
  xset_alufunction1(&XGC->CurDrawFunction,PI0,PI0,PI0);
  xset_pattern((i=XGC->NumForeground+1,&i),PI0,PI0,PI0);  
  xset_foreground((i=XGC->NumForeground+1,&i),PI0,PI0,PI0);
  xset_background((i=XGC->NumForeground+2,&i),PI0,PI0,PI0);
  FREE(r); FREE(g); FREE(b);
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
    val[i] = (double)ScilabXgc->Red[i]/255.0;
    val[i+m] = (double)ScilabXgc->Green[i]/255.0;
    val[i+2*m] = (double)ScilabXgc->Blue[i]/255.0;
  }
  if (*verbose == 1) Sciprintf("Size of colormap: %d colors\r\n",m);
}

/** set and get the number of the background or foreground */


static void xset_background(integer *num, integer *v2, integer *v3, integer *v4)
{ 
  if (ScilabXgc->CurColorStatus == 1) 
    {
      int bg = ScilabXgc->NumBackground =  Max(0,Min(*num - 1,ScilabXgc->Numcolors + 1));
      if (ScilabXgc->Red != NULL )
	{
	  /* we fix the default background in ScilabXgc->gcol_bg */
	  ScilabXgc->gcol_bg.red = 0;
	  ScilabXgc->gcol_bg.green = 0;
	  ScilabXgc->gcol_bg.blue = 0;
	  ScilabXgc->gcol_bg.pixel = PIXEL_FROM_CMAP(bg);
	}
      /* 
       * if we change the background of the window we must change 
       * the gc ( with alufunction ) and the window background 
       */
      xset_alufunction1(&ScilabXgc->CurDrawFunction,PI0,PI0,PI0);
      gdk_window_set_background(ScilabXgc->drawing->window, &ScilabXgc->gcol_bg);
    }
}
 
static void xget_background(integer *verbose, integer *num, integer *narg, double *dummy)
{ 
  *narg=1;
  *num = ( ScilabXgc->CurColorStatus == 1 ) ? ScilabXgc->NumBackground + 1 : 1;
  if (*verbose == 1) 
    Sciprintf("\n Background : %d\r\n",*num);
}

/* set and get the number of the background or foreground */

static void xset_foreground(integer *num, integer *v2, integer *v3, integer *v4)
{ 
  if (ScilabXgc->CurColorStatus == 1) 
    {
      int fg = ScilabXgc->NumForeground = Max(0,Min(*num - 1,ScilabXgc->Numcolors + 1));
      if (ScilabXgc->Red != NULL )
	{
	  ScilabXgc->gcol_fg.red = 0;
	  ScilabXgc->gcol_fg.green = 0;
	  ScilabXgc->gcol_fg.blue = 0;
	  ScilabXgc->gcol_fg.pixel = PIXEL_FROM_CMAP(fg);
	  xset_alufunction1(&ScilabXgc->CurDrawFunction,PI0,PI0,PI0);
	}
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
    Sciprintf("\n Foreground : %d\r\n",*num);
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
    Sciprintf("\n Hidden3d : %d\r\n",*num);
}

/**********************************************************
 * Used in xsetm()
 *    to see the colormap of current graphic window
 ******************************************************/

int IsPrivateCmap(void) { return 0 ;} 

void set_cmap(void * w)
{
  /* XXX
  if ( ScilabXgc != (struct BCG *) 0 && ScilabXgc->Cmap != (Colormap)0)
    XSetWindowColormap(dpy,w,ScilabXgc->Cmap);
  */
}

int get_pixel(int i)
{
  /* XXX
  if ( ScilabXgc != (struct BCG *) 0 && ScilabXgc->Cmap != (Colormap)0)
    return(ScilabXgc->Colors[Max(Min(i,ScilabXgc->Numcolors + 1),0)]);
  else 
  */
  return(0);
}
/* 
Pixmap get_pixmap(i) 
     int i;
{
  return(Tabpix_[ Max(0,Min(i - 1,GREYNUMBER - 1))]);
}
*/

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
  *r = ScilabXgc->Red[i]/255.0;
}

void  get_g(int i, float *g)
{
  *g = ScilabXgc->Green[i]/255.0;
}
void get_b(int i, float *b)
{
  *b = ScilabXgc->Blue[i]/255.0;
}

/*-----------------------------------------------------------
 * general routines accessing the previous  set<> or get<> 
 * 
 *-----------------------------------------------------------*/

static void InitMissileXgc(integer *v1, integer *v2, integer *v3, integer *v4);
static void SetGraphicsVersion();

static void xset_empty(integer *verbose, integer *v2, integer *v3, integer *v4)
{
  if ( *verbose ==1 ) Sciprintf("\n No operation ");
}

static void xget_empty(integer *verbose, integer *v2, integer *v3, double *dummy)
{
  if ( *verbose ==1 ) Sciprintf("\n No operation ");
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
	  Sciprintf("\nGettting Info on %s\r\n",str);
	if (flag == 1)
	  (MissileGCTab_[i].getfonc)(verbose,x1,x2,dv1);
	else 
	  (MissileGCTab_[i].setfonc)(x1,x2,x3,x4,x5,x6,dv1);
	return;}
      else 
	{ if ( j <= 0)
	  {
	    Sciprintf("\nUnknow X operator <%s>\r\n",str);
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
  Sciprintf("\n Unknow X operator <%s>\r\n",str);
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
      gint lbearing, rbearing, iascent, idescent, iwidth;
      gdk_string_extents(ScilabXgc->font,"X", &lbearing, &rbearing,
			 &iwidth, &iascent, &idescent);
      gdk_draw_text(ScilabXgc->Cdrawable,ScilabXgc->font,ScilabXgc->wgc, 
		    *x, *y - idescent , string, strlen(string));
      if ( ScilabXgc->Cdrawable == ScilabXgc->drawing->window) 
	gdk_draw_text(ScilabXgc->pixmap,ScilabXgc->font,ScilabXgc->wgc, 
		      *x, *y - idescent , string, strlen(string));
      if ( *flag == 1) 
	{
	  integer rect[] = { *x , *y- iascent - idescent, 
			     gdk_string_width(ScilabXgc->font, string),
			     iascent+idescent};
	  C2F(drawrectangle)(string,rect,rect+1,rect+2,rect+3,
			     PI0,PI0,PD0,PD0,PD0,PD0);
	}
    }
  else 
    {
      C2F(DispStringAngle)(x,y,string,angle);
    }
}

void C2F(DispStringAngle)(integer *x0, integer *yy0, char *string, double *angle)
{
  int i,flag=0, rect[4],x,y;
  double xd,yd, sina ,cosa,l,zero=0.0;
  char str1[2];
  str1[1]='\0';
  xd=x= *x0;
  yd=y= *yy0;
  sina= sin(*angle * M_PI/180.0);
  cosa= cos(*angle * M_PI/180.0);
  for ( i = 0 ; i < (int)strlen(string); i++)
    { 
      str1[0]=string[i];
      C2F(displaystring)(str1,&x,&y,PI0,&flag,PI0,PI0,&zero,PD0,PD0,PD0);
      /* XDrawString(dpy,ScilabXgc->Cdrawable,gc,(int) x,(int) y ,str1,1); */
      C2F(boundingbox)(str1,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
      if (0 && cosa <= 0.0 && i < (int)strlen(string)-1)
	{ 
	  char str2[2];
	  /** si le cosinus est negatif le deplacement est a calculer **/
	  /** sur la boite du caractere suivant **/
	  str2[1]='\0';str2[0]=string[i+1];
	  C2F(boundingbox)(str2,&x,&y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
	  fprintf(stderr,"cos is negative\n");
	}
      /* C2F(drawrectangle)(string,rect,rect+1,rect+2,rect+3,  PI0,PI0,PD0,PD0,PD0,PD0); */
      if ( Abs(cosa) >= 1.e-8 )
	{
	  if ( Abs(sina/cosa) <= Abs(((double)rect[3])/((double)rect[2])))
	    l = Abs(rect[2]/cosa);
	  else 
	    l = Abs(rect[3]/sina);
	}
      else 
	l = Abs(rect[3]/sina);
      xd +=  cosa*l*1.1;
      yd +=  sina*l*1.1;
      x = inint(xd);
      y = inint(yd);
    }
}

/** To get the bounding rectangle of a string **/

void C2F(boundingbox)(char *string, integer *x, integer *y, integer *rect, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{ 
  gint lbearing, rbearing, iascent, idescent, iwidth;
  gdk_string_extents(ScilabXgc->font,"X", &lbearing, &rbearing, &iwidth, &iascent, &idescent);
  rect[0]= *x ;
  rect[1]= *y - iascent - idescent;
  rect[2]= gdk_string_width(ScilabXgc->font, string);
  rect[3]= iascent + idescent;
}

/*------------------------------------------------
 * line segments arrows 
 *-------------------------------------------------*/

void C2F(drawline)(integer *x1, integer *yy1, integer *x2, integer *y2)
{
  gdk_draw_line(ScilabXgc->Cdrawable,ScilabXgc->wgc, *x1, *yy1, *x2, *y2);
  if ( ScilabXgc->Cdrawable == ScilabXgc->drawing->window) 
    gdk_draw_line(ScilabXgc->pixmap,ScilabXgc->wgc, *x1, *yy1, *x2, *y2);
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
      gdk_draw_line(ScilabXgc->Cdrawable,ScilabXgc->wgc,vx[2*i],vy[2*i],vx[2*i+1],vy[2*i+1]);
      if ( ScilabXgc->Cdrawable == ScilabXgc->drawing->window) 
	gdk_draw_line(ScilabXgc->pixmap,ScilabXgc->wgc,vx[2*i],vy[2*i],vx[2*i+1],vy[2*i+1]);
    }
  }
  else {
    if (*style >= 1) /* set color */
      xset_line_style(style,PI0,PI0,PI0);

    for (i=0 ; i < *n/2 ; i++) {
      if ( ScilabXgc->Cdrawable == ScilabXgc->drawing->window) 
	gdk_draw_line(ScilabXgc->pixmap,ScilabXgc->wgc,vx[2*i],vy[2*i],vx[2*i+1],vy[2*i+1]);
      gdk_draw_line(ScilabXgc->Cdrawable,ScilabXgc->wgc,vx[2*i],vy[2*i],vx[2*i+1],vy[2*i+1]);
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
	polyx[0]= polyx[3]=vx[2*i+1];/* +dx*cos20;*/
	polyx[1]= inint(polyx[0]  - cos20*dx -sin20*dy );
	polyx[2]= inint(polyx[0]  - cos20*dx + sin20*dy);
	polyy[0]= polyy[3]=vy[2*i+1];/*+dy*cos20;*/
	polyy[1]= inint(polyy[0] + sin20*dx -cos20*dy) ;
	polyy[2]= inint(polyy[0] - sin20*dx - cos20*dy) ;
	C2F(fillpolylines)("v",polyx,polyy,&NDvalue,&nn,&p,PI0,PD0,PD0,PD0,PD0);
	}
      /* xset_pattern(&NDvalue,PI0,PI0,PI0); commented out 13/09/00 ss */
      if ( ScilabXgc->Cdrawable == ScilabXgc->drawing->window) 
	gdk_draw_line(ScilabXgc->pixmap,ScilabXgc->wgc,vx[2*i],vy[2*i],vx[2*i+1]-dx*cos20,vy[2*i+1]-dy*cos20);
      gdk_draw_line(ScilabXgc->Cdrawable,ScilabXgc->wgc,vx[2*i],vy[2*i],vx[2*i+1]-dx*cos20,vy[2*i+1]-dy*cos20);
    }
  xset_dash_and_color( Dvalue,PI0,PI0,PI0);
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

void C2F(drawrectangle)(char *str, integer *x, integer *y, integer *width, integer *height, 
			integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{ 
  gdk_draw_rectangle(ScilabXgc->Cdrawable, ScilabXgc->wgc, FALSE,*x,*y,*width,*height);
  if ( ScilabXgc->Cdrawable == ScilabXgc->drawing->window) 
    gdk_draw_rectangle(ScilabXgc->pixmap,ScilabXgc->wgc, FALSE,*x,*y,*width,*height);
}

/** fill one rectangle, with current pattern **/

void C2F(fillrectangle)(char *str, integer *x, integer *y, integer *width, integer *height,
			integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{ 
  gdk_draw_rectangle(ScilabXgc->Cdrawable, ScilabXgc->wgc, TRUE,*x,*y,*width,*height);
  if ( ScilabXgc->Cdrawable == ScilabXgc->drawing->window) 
    gdk_draw_rectangle(ScilabXgc->pixmap,ScilabXgc->wgc, TRUE,*x,*y,*width,*height);
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
	xset_pattern(fill,PI0,PI0,PI0);
        w=Abs(x[i+1]-x[i]);h=Abs(y[j+1]-y[j]);
	/* We don't trace rectangle which are totally out **/
	if ( w != 0 && h != 0 && x[i] < xz[0] && y[j+1] < xz[1] && x[i]+w > 0 && y[j+1]+h > 0 )
	  if ( Abs(x[i]) < int16max && Abs(y[j+1]) < int16max && w < uns16max && h < uns16max)
	    {
	      gdk_draw_rectangle(ScilabXgc->Cdrawable, ScilabXgc->wgc, 
				 TRUE,x[i],y[j+1],w,h);
	      if ( ScilabXgc->Cdrawable == ScilabXgc->drawing->window) 
		gdk_draw_rectangle(ScilabXgc->pixmap,ScilabXgc->wgc,
				   TRUE,x[i],y[j+1],w,h);
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
	  if ( Abs(x[j]) < int16max && Abs(y[i+1]) < int16max && w < uns16max && h < uns16max)
	    {
	      gdk_draw_rectangle(ScilabXgc->Cdrawable, ScilabXgc->wgc, 
				 TRUE,x[j],y[i],w,h);
	      if ( ScilabXgc->Cdrawable == ScilabXgc->drawing->window) 
		gdk_draw_rectangle(ScilabXgc->pixmap,ScilabXgc->wgc,
				   TRUE,x[j],y[i],w,h);
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


void C2F(fillarcs)(char *str, integer *vects, integer *fillvect, integer *n, integer *v5,
		   integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
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


void C2F(drawarcs)(char *str, integer *vects, integer *style, integer *n, integer *v5,
		   integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
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

void C2F(drawarc)(char *str, integer *x, integer *y, integer *width, integer *height,
		  integer *angle1, integer *angle2, double *dv1, double *dv2, double *dv3, double *dv4)
{ 
  gdk_draw_arc(ScilabXgc->Cdrawable, ScilabXgc->wgc,FALSE,*x,*y,*width,*height,*angle1, *angle2);
  if ( ScilabXgc->Cdrawable == ScilabXgc->drawing->window) 
    gdk_draw_arc(ScilabXgc->pixmap,ScilabXgc->wgc,FALSE,*x,*y,*width,*height,*angle1, *angle2);
}

/** Fill a single elipsis or part of it with current pattern **/

void C2F(fillarc)(char *str, integer *x, integer *y, integer *width, integer *height,
		  integer *angle1, integer *angle2, double *dv1, double *dv2, double *dv3, double *dv4)
{ 
  gdk_draw_arc(ScilabXgc->Cdrawable, ScilabXgc->wgc,TRUE,*x,*y,*width,*height,*angle1, *angle2);
  if ( ScilabXgc->Cdrawable == ScilabXgc->drawing->window) 
    gdk_draw_arc(ScilabXgc->pixmap,ScilabXgc->wgc,TRUE,*x,*y,*width,*height,*angle1, *angle2);
}

/*
 * Filling or Drawing Polylines and Polygons
 */

/* 
 * Draw a set of (*n) polylines (each of which have (*p) points) 
 * with lines or marks 
 * drawvect[i] <= 0 use a mark for polyline i
 * drawvect[i] >  0 use a line style for polyline i 
 */

void C2F(drawpolylines)(char *str, integer *vectsx, integer *vectsy, integer *drawvect,
			integer *n, integer *p, integer *v7, double *dv1, double *dv2,
			double *dv3, double *dv4)
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

void C2F(fillpolylines)(char *str, integer *vectsx, integer *vectsy, integer *fillvect,
			integer *n, integer *p, integer *v7, double *dv1, double *dv2,
			double *dv3, double *dv4)
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
	  xset_line_style(Dvalue,PI0,PI0,PI0);
	  xset_pattern(&cpat,PI0,PI0,PI0);
	  C2F(drawpolyline)(str,p,vectsx+(*p)*i,vectsy+(*p)*i,(close=1,&close)
			    ,PI0,PI0,PD0,PD0,PD0,PD0);
	}
      else 
	{
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

void C2F(drawpolyline)(char *str, integer *n, integer *vx, integer *vy, integer *closeflag,
		       integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{ 
  integer n1;
  if (*closeflag == 1) n1 = *n+1;else n1= *n;
  if (n1 >= 2) 
    {
      /* 
	 C2F(analyze_points)(*n, vx, vy,*closeflag); 
	 gdk_flush();
      */
      if ( gtk_store_points(*n, vx, vy, *closeflag)) 
	{
	  gdk_draw_lines(ScilabXgc->Cdrawable,ScilabXgc->wgc, gtk_get_xpoints(), n1);
	  if ( ScilabXgc->Cdrawable == ScilabXgc->drawing->window) 
	    gdk_draw_lines(ScilabXgc->pixmap, ScilabXgc->wgc, gtk_get_xpoints(), n1);
	}
    }
}

/* 
 * Fill the polygon or polyline 
 * according to *closeflag : the given vector is a polyline or a polygon 
 */

void C2F(fillpolyline)(char *str, integer *n, integer *vx, integer *vy,
		       integer *closeflag, integer *v6, integer *v7, double *dv1,
		       double *dv2, double *dv3, double *dv4)
{
  integer n1;
  if (*closeflag == 1) n1 = *n+1;else n1= *n;
  /* 
  if (gtk_store_points(*n, vx, vy,*closeflag)){
    XFillPolygon (dpy, ScilabXgc->Cdrawable, gc, get_xpoints(), n1,
		  Complex, ScilabXgc->CurVectorStyle);
  }
  gdk_flush();
  */
  if ( gtk_store_points(*n, vx, vy, *closeflag)) 
    {
      gdk_draw_polygon(ScilabXgc->Cdrawable,ScilabXgc->wgc,TRUE,gtk_get_xpoints(), n1);
      if ( ScilabXgc->Cdrawable == ScilabXgc->drawing->window) 
	gdk_draw_polygon(ScilabXgc->pixmap, ScilabXgc->wgc,TRUE,gtk_get_xpoints(), n1); 
    }
}

/* 
 * Draw the current mark centred at points defined
 * by vx and vy (vx[i],vy[i]) 
 */

void C2F(drawpolymark)(char *str, integer *n, integer *vx, integer *vy,
		       integer *v5, integer *v6, integer *v7, double *dv1,
		       double *dv2, double *dv3, double *dv4)
{
  if ( ScilabXgc->CurHardSymb == 0 )
    {
      if (gtk_store_points(*n, vx, vy,(integer)0L))
	{
	  gdk_draw_points(ScilabXgc->Cdrawable,
			  ScilabXgc->wgc,gtk_get_xpoints(), *n);
	  if ( ScilabXgc->Cdrawable == ScilabXgc->drawing->window) 
	    gdk_draw_points(ScilabXgc->pixmap, 
			    ScilabXgc->wgc,gtk_get_xpoints(), *n); 
	}
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

static struct BCG *AddNewWindowToList(void)
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
	  Sciprintf("AddNewWindow No More Place ");
	  return((struct BCG *) 0);
	}
      else 
	{ 
	  (*listptr)->winxgc.window = NULL;
	  (*listptr)->winxgc.item_factory = NULL;
	  (*listptr)->winxgc.drawing =  NULL;
	  (*listptr)->winxgc.pixmap =   NULL;
	  (*listptr)->winxgc.CinfoW =   NULL ;
	  (*listptr)->winxgc.CurWindow = 0;
	  (*listptr)->winxgc.Red = (float *) 0;
	  (*listptr)->winxgc.Green = (float *) 0;
	  (*listptr)->winxgc.Blue = (float *) 0;
	  (*listptr)->winxgc.CmapFlag  = 1;
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
	  /* the animation pixmap */
	  if ( L1->winxgc.CurPixmapStatus == 1 ) 
	    gdk_pixmap_unref(L1->winxgc.Cdrawable);
	  /* backing store pixmap */
	  gdk_pixmap_unref(L1->winxgc.pixmap);
	  /* destroy window */
	  gtk_widget_destroy(L1->winxgc.window);
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

GtkWidget *GetWindowNumber(int wincount)
{
  struct BCG *bcg;
  bcg = GetWindowXgcNumber(wincount);
  if ( bcg != (struct BCG *) 0) 
    return( bcg->window);
  else 
    return( NULL);
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

GtkWidget *GetBGWindowNumber(int wincount)
{
  struct BCG *bcg;
  bcg = GetWindowXgcNumber(wincount);
  if ( bcg != (struct BCG *) 0) 
    return( bcg->window );
  else 
    return( NULL ) ;
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
  /* Sciprintf("Max Id : %d \r\n",Num); */
  return(Num);
}

/********************************************
 * Routines for initialization : string is a display name 
 ********************************************/

void set_c(integer col)
{
  int value = AluStruc_[ScilabXgc->CurDrawFunction].id;
  GdkColor temp = {0,0,0,0};
  /* colors from 1 to ScilabXgc->Numcolors */
  ScilabXgc->CurColor = col = Max(0,Min(col,ScilabXgc->Numcolors + 1));
  if (ScilabXgc->Red  == NULL) return;
  temp.pixel = PIXEL_FROM_CMAP(col);
  switch (value) 
    {
    case GDK_CLEAR : 
      break;
    case GDK_XOR   : 
      temp.pixel = temp.pixel ^ ScilabXgc->gcol_bg.pixel ;
      gdk_gc_set_foreground(ScilabXgc->wgc, &temp);
      break;
    default :
      gdk_gc_set_foreground(ScilabXgc->wgc, &temp);
      break;
    }
}

/*
 * initgraphic : initialize graphic window
 * If v2 is not a nul pointer *v2 is the window number to create 
 * EntryCounter is used to check for first Entry + to know the next 
 * available window number 
 */

void C2F(initgraphic)(char *string, integer *v2, integer *v3, integer *v4,
		      integer *v5, integer *v6, integer *v7, double *dv1, 
		      double *dv2, double *dv3, double *dv4)
{ 
  struct BCG *NewXgc ;
#ifdef WITH_TK
  integer ne=7, menutyp=2, ierr;
  char *EditMenus[]={"_Select figure as current","_Redraw figure",\
                     "_Erase figure",/*"_Copy object","_Paste object",\
                     "_Move object","_Delete object",*/"_Figure properties",\
                     "_Current axes properties","Start _entity picker",\
                     "Stop e_ntity picker"};
  /*integer ni=6;
  char *InsertMenus[]={"--- _Line","^v^  _Polyline","---> _Arrow",\
                       "_Text","[]   _Rectangle","O   _Circle"};*/
#else
  integer ne=3, menutyp=2, ierr;
  char *EditMenus[]={"_Select as current","_Redraw figure","_Erase figure"};
#endif
  static integer EntryCounter = 0;
  integer WinNum;
  *v3 = 0;
  if ( v2 != (integer *) NULL && *v2 != -1 )
    WinNum= *v2;
  else
    WinNum= EntryCounter;
  NewXgc = AddNewWindowToList();
  if ( NewXgc == (struct BCG *) 0) 
    {
      Sciprintf("initgraphics: unable to alloc\n");
      return;
    }
  else 
    {
      ScilabXgc= NewXgc;
    }
  ScilabXgc->CurPixmapStatus = 0; 
  ScilabXgc->CurResizeStatus = 0; 
  ScilabXgc->CurWindow = WinNum;
  CreateGtkGWindow(ScilabXgc);
  if (EntryCounter == 0)
    {
      maxcol = 1 << 16; /* XXXXX : to be changed */
      LoadFonts();
    }
  /** Default value is without Pixmap **/
  ScilabXgc->Cdrawable = (GdkDrawable *) ScilabXgc->drawing->window;
  InitMissileXgc(PI0,PI0,PI0,PI0);
  SetGraphicsVersion(); /* set the graphics version using global versionflag variable */

  /* to be sure that current values are recorded */
  StoreXgc(WinNum);
  EntryCounter=Max(EntryCounter,WinNum);
  EntryCounter++;
  AddMenu(&WinNum,"_Edit", EditMenus, &ne, &menutyp, "ged", &ierr);
#ifdef WITH_TK
  /*AddMenu(&WinNum,"_Insert", InsertMenus, &ni, &menutyp, "ged_insert", &ierr);*/
#endif
  gdk_flush();
}


/* writes a message in the info widget associated to the current scilab window */

void C2F(xinfo)(char *message, integer *v2, integer *v3, integer *v4, integer *v5, integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{
  if ( ScilabXgc != (struct BCG *) 0 && ScilabXgc->CinfoW != NULL)
    {
      gtk_statusbar_pop ((GtkStatusbar *) ScilabXgc->CinfoW, 1);
      gtk_statusbar_push ((GtkStatusbar *) ScilabXgc->CinfoW, 1, message);
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
  /* Arg args[1];*/
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
  if ( ScilabXgc != (struct BCG *) 0 && ScilabXgc->CinfoW != NULL)
    {
      gtk_statusbar_pop ((GtkStatusbar *) ScilabXgc->CinfoW, 1);
      gtk_statusbar_push ((GtkStatusbar *) ScilabXgc->CinfoW, 1,buf);
    }
}



/*--------------------------------------------------------
 * Initialisation of the graphic context. Used also 
 *  to come back to the default graphic state}
 *---------------------------------------------------------*/

static void InitMissileXgc (integer *v1, integer *v2, integer *v3, integer *v4)
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
  ScilabXgc->CurPixmapStatus =0 ;
  ScilabXgc->CurResizeStatus =0 ;
  xset_pixmapOn((i=0,&i),PI0,PI0,PI0);
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
  xset_pattern((i=ScilabXgc->NumForeground+1,&i),PI0,PI0,PI0);
  /*** XXXXX a faire aussi pour le n&b plus haut ***/
  xset_foreground((i=ScilabXgc->NumForeground+1,&i),PI0,PI0,PI0);
  xset_background((i=ScilabXgc->NumForeground+2,&i),PI0,PI0,PI0);
  xset_hidden3d((i=4,&i),PI0,PI0,PI0);
  /* Choix du mode par defaut (decide dans initgraphic) */
  getcolordef(&i);
  /** we force CurColorStatus to the opposite value of col 
   * to force usecolorPos to perform initialisations 
   **/
  ScilabXgc->mafigure = (sciPointObj *)NULL;
/*   ScilabXgc->graphicsversion = versionflag; /\* NG *\/ */

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
      xset_pattern((i=1,&i),PI0,PI0,PI0);
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
	 gdk_draw_line(ScilabXgc->Cdrawable,ScilabXgc->wgc, xi,yi,xf,yf) ;
	 if ( ScilabXgc->Cdrawable == ScilabXgc->drawing->window) 
	   gdk_draw_line(ScilabXgc->pixmap,ScilabXgc->wgc, xi,yi,xf,yf);
       }
   }
 for (i=0; i <= nsteps[1]; i++)
   { xi = initpoint[0]+i*nsteps[0]*size[0]*cosal;
   yi = initpoint[1]+i*nsteps[0]*size[0]*sinal;
   xf = xi - ( size[1]*size[2]*sinal);
   yf = yi + ( size[1]*size[2]*cosal);
   gdk_draw_line(ScilabXgc->Cdrawable,ScilabXgc->wgc, xi,yi,xf,yf) ;
   if ( ScilabXgc->Cdrawable == ScilabXgc->drawing->window) 
     gdk_draw_line(ScilabXgc->pixmap,ScilabXgc->wgc, xi,yi,xf,yf);
   }
}

/*-----------------------------------------------------
 * Display numbers z[i] at location (x[i],y[i])
 *   with a slope alpha[i] (see displaystring), if flag==1
 *   add a box around the string, only if slope =0}
 *-----------------------------------------------------*/

void C2F(displaynumbers)(char *str, integer *x, integer *y, integer *v1, integer *v2, integer *n, integer *flag, double *z, double *alpha, double *dx3, double *dx4)
{
  integer i ;
  static char buf[56];
  for (i=0 ; i< *n ; i++)
    { 
      sprintf(buf,ScilabXgc->CurNumberDispFormat,z[i]);
      C2F(displaystring)(buf,&(x[i]),&(y[i]),PI0,flag,PI0,PI0,&(alpha[i]),PD0,PD0,PD0) ;
    }
}

void C2F(bitmap)(char *string, integer w, integer h)
{
  /* 
  static XImage *setimage;
  setimage = XCreateImage (dpy, XDefaultVisual (dpy, DefaultScreen(dpy)),
			   1, XYBitmap, 0, string,w,h, 8, 0);	
  setimage->data = string;
  XPutImage (dpy, ScilabXgc->Cdrawable, gc, setimage, 0, 0, 10,10,w,h);
  XDestroyImage(setimage);
  */
}


/*---------------------------------------------------------------------
 * Using X11 Fonts
 *---------------------------------------------------------------------*/

#define FONTNUMBER 11 
#define FONTMAXSIZE 6
#define SYMBOLNUMBER 10

/* FontsList : storing font informations
 *             the font i with size fsiz is stored at 
 *             FontsList_[i][fsiz]->fid
 */

GdkFont *FontsList_[FONTNUMBER][FONTMAXSIZE];

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
  {"Symb",  "-adobe-symbol-medium-r-normal--*-%s0-*-*-p-*-adobe-fontspecific"},
  {"TimR",  "-adobe-times-medium-r-normal--*-%s0-*-*-p-*-iso8859-1"},
  {"TimI",  "-adobe-times-medium-i-normal--*-%s0-*-*-p-*-iso8859-1"},
  {"TimB",  "-adobe-times-bold-r-normal--*-%s0-*-*-p-*-iso8859-1"},
  {"TimBI", "-adobe-times-bold-i-normal--*-%s0-*-*-p-*-iso8859-1"},
  {"HelvR", "-adobe-helvetica-medium-r-normal--*-%s0-*-*-p-*-iso8859-1"},
  {"HelvO", "-adobe-helvetica-medium-o-normal--*-%s0-*-*-p-*-iso8859-1"},
  {"HelvB", "-adobe-helvetica-bold-r-normal--*-%s0-*-*-p-*-iso8859-1"},
  {"HelvBO","-adobe-helvetica-bold-o-normal--*-%s0-*-*-p-*-iso8859-1"},
  {(char *) NULL,( char *) NULL}
};

int fontidscale(int fontsize)
{
  int nnsiz,i;
  int isiz = i_size_[fontsize];
  double d = Min(ScilabXgc->CWindowHeight,ScilabXgc->CWindowWidth);
  nnsiz = (ScilabXgc != NULL) ? inint((isiz*d/400.0)) : isiz; 
  /* fprintf(stderr,"Scaling by -->%d %d \n",isiz,nnsiz); */
  for ( i=0; i < FONTMAXSIZE ; i++) 
    {
      if (i_size_[i] >= nnsiz ) return Max(i-1,0);
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
      if (i != FONTNUMBER-1 )   /* on charge la fonte par defaut correspondante */
	{
	  C2F(loadfamily)(fonttab[i].alias,&i,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
	}
      else
	{
	  Sciprintf(" The Font Id %d is not affected \r\n",(int)i);
	  Sciprintf(" use xlfont to set it \n");
	  return;
	}
    }
  ScilabXgc->FontId = i;
  ScilabXgc->FontSize = fsiz;
  ScilabXgc->font = FontsList_[i][fsiz_sca];
  /* 
     XSetFont(dpy,gc,FontsList_[i][fsiz_sca]->fid);
     gdk_flush();
  */
}

/** To get the  id and size of the current font **/

static void  xget_font(integer *verbose, integer *font, integer *nargs, double *dummy)
{
  *nargs=2;
  font[0]= ScilabXgc->FontId ;
  font[1] =ScilabXgc->FontSize ;
  if (*verbose == 1) 
    {
      Sciprintf("\nFontId : %d ", ScilabXgc->FontId );
      Sciprintf("--> %s at size %s pts\r\n",
	       FontInfoTab_[ScilabXgc->FontId].fname,
	       size_[ScilabXgc->FontSize]);
    }
}

/** To set the current mark **/

static void xset_mark(integer *number, integer *size, integer *v3, integer *v4)
{ 
  ScilabXgc->CurHardSymb = Max(Min(SYMBOLNUMBER-1,*number),0);
  ScilabXgc->CurHardSymbSize = Max(Min(FONTMAXSIZE-1,*size),0);
}

/** To get the current mark id **/

static void xget_mark(integer *verbose, integer *symb, integer *narg, double *dummy)
{
  *narg =2 ;
  symb[0] = ScilabXgc->CurHardSymb ;
  symb[1] = ScilabXgc->CurHardSymbSize ;
  if (*verbose == 1) 
    {
      Sciprintf("\nMark : %d ",ScilabXgc->CurHardSymb);
      Sciprintf("at size %s pts\r\n", size_[ScilabXgc->CurHardSymbSize]);
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

void C2F(loadfamily)(char *name, integer *j, integer *v3, integer *v4, integer *v5, 
		     integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
{ 
  integer i,flag=1;

  if ( *j < 0  ||  *j > FONTNUMBER-1 )
    {
      Scierror(999,"xlfont: font-id must be an integer between 0 and %d \r\n",  FONTNUMBER-1);
      return;
    }

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
	  FontsList_[*j][i]=  gdk_font_load(name1);
	  if  (FontsList_[*j][i]== NULL)
	    { 
	      flag=0;
	      Sciprintf("\n Unknown font : %s",name1);
	      Sciprintf("\n I'll use font: fixed ");
	      FontsList_[*j][i]=  gdk_font_load(name1);
	      if  (FontsList_[*j][i]== NULL)
		{
		  Sciprintf("\n Unknown font : %s\r\n","fixed");
		  Sciprintf("Please call an X Wizard !");
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
      FontsList_[*j][i]=  gdk_font_load(name1);
      if  (FontsList_[*j][i]== NULL)
	{ 
	  flag=0;
	  Sciprintf("\n Unknown font : %s",name1);
	  Sciprintf("\n I'll use font: fixed ");
	  FontsList_[*j][i]= gdk_font_load(name1);
	  if  (FontsList_[*j][i]== NULL)
	    {
	      Sciprintf("\n Unknown font : %s\r\n","fixed");
	      Sciprintf("  Please call an X Wizard !");
	    }
	}
    }
  FontInfoTab_[*j].ok = 1;
  if (flag != 0) 
    strcpy(FontInfoTab_[*j].fname,name);
  else
    strcpy(FontInfoTab_[*j].fname,"fixed");
}

void C2F(queryfamily)(char *name, integer *j, integer *v3, integer *v4, integer *v5, 
		      integer *v6, integer *v7, double *dv1, double *dv2, double *dv3, double *dv4)
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
  integer j, i;
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
    {    
      if (FontsList_[1][i] != NULL)
	{
	  for (j=0 ; j < SYMBOLNUMBER ; j++)
	    { 
	      gint lbearing, rbearing, iascent, idescent, iwidth;
	      gchar tmp[2] = { (gchar) Marks[j],0};
	      gdk_string_extents(FontsList_[1][i], tmp,
				 &lbearing, &rbearing,
				 &iwidth, &iascent, &idescent);
	      (ListOffset_[i].xoffset)[j] = (rbearing+lbearing)/2;/* ou iwidth/2 ? */
	      (ListOffset_[i].yoffset)[j] = (iascent+idescent)/2;
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
  gdk_draw_text(ScilabXgc->Cdrawable,ScilabXgc->font,ScilabXgc->wgc, 
		*x+C2F(CurSymbXOffset)(), *y +C2F(CurSymbYOffset)(),str,1);
  if ( ScilabXgc->Cdrawable == ScilabXgc->drawing->window) 
    gdk_draw_text(ScilabXgc->pixmap,ScilabXgc->font,ScilabXgc->wgc, 
		  *x+C2F(CurSymbXOffset)(), *y +C2F(CurSymbYOffset)(),str,1);
}


/*-------------------------------------------------------------------
 * Allocation and storing function for vectors of GtkPoints 
 *------------------------------------------------------------------------*/

static GdkPoint *gtk_points = NULL;

static GdkPoint *gtk_get_xpoints(void) { return(gtk_points); }

static int gtk_store_points(integer n, integer *vx, integer *vy, integer onemore)
{ 
  integer i,n1 = ( onemore == 1) ? n+1 : n;
  if (GtkReallocVector(n1) == 1)
    {
      for (i = 0; i < n; i++){
	gtk_points[i].x =(gint16) Min(Max(-int16max,vx[i]),int16max);
	gtk_points[i].y =(gint16) Min(Max(-int16max,vy[i]),int16max);
      }
      if (onemore == 1) {
	gtk_points[n].x=(gint16) gtk_points[0].x;
	gtk_points[n].y=(gint16) gtk_points[0].y;
      }
      return(1);
    }
  else return(0);
}

#define MESSAGE5 "Can't re-allocate point vector" 

static int GtkReallocVector(integer n)
{
  if (( gtk_points = graphic_alloc(8,n,sizeof(GdkPoint))) == 0) 
    { 
      Sciprintf(MESSAGE5); return 0;
    }
  return 1;
}

/*----------------------------------------------------------
 * Clipping functions 
 *----------------------------------------------------------*/

static void XDroutine(int npts)
{
  gdk_draw_lines(ScilabXgc->Cdrawable,ScilabXgc->wgc, gtk_get_xpoints(), npts);
  if ( ScilabXgc->Cdrawable == ScilabXgc->drawing->window) 
    gdk_draw_lines(ScilabXgc->pixmap, ScilabXgc->wgc, gtk_get_xpoints(), npts);
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
  integer x, y, dx, dy, x_intr[2], y_intr[2], count, pos1, pos2;
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
	  if (count < 2 ) 
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

static void change_points(integer i, integer x, integer y)
{
  gtk_points[i].x=(gint16)x;   gtk_points[i].y=(gint16)y;
}

static void MyDraw(integer iib, integer iif, integer *vx, integer *vy)
{
  integer x1n,y1n,x11n,y11n,x2n,y2n,flag2=0,flag1=0;
  integer npts;
  npts= ( iib > 0) ? iif-iib+2  : iif-iib+1;
  if ( iib > 0) 
    {
      clip_line(vx[iib-1],vy[iib-1],vx[iib],vy[iib],&x1n,&y1n,&x2n,&y2n,&flag1);
    }
  clip_line(vx[iif-1],vy[iif-1],vx[iif],vy[iif],&x11n,&y11n,&x2n,&y2n,&flag2);
  if (gtk_store_points(npts, &vx[Max(0,iib-1)], &vy[Max(0,iib-1)],(integer)0L));
  {
    if (iib > 0 && (flag1==1||flag1==3)) change_points((integer)0L,x1n,y1n);
    if (flag2==2 || flag2==3) change_points(npts-1,x2n,y2n);
    XDroutine((int)npts);
  }
}

static void My2draw(integer j, integer *vx, integer *vy)
{
  /** The segment is out but can cross the box **/
  integer vxn[2],vyn[2],flag;
  integer npts=2;
  clip_line(vx[j-1],vy[j-1],vx[j],vy[j],&vxn[0],&vyn[0],&vxn[1],&vyn[1],&flag);
  if (flag == 3 && gtk_store_points(npts,vxn,vyn,(integer)0L))
    {
#ifdef DEBUG
      Sciprintf("segment out mais intersecte en (%d,%d),(%d,%d)\r\n",
	       vxn[0],vyn[0],vxn[1],vyn[1]);
#endif 
      XDroutine((int)npts);
    }
}

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
	  Sciprintf("first in %d->%d=(%d,%d)\r\n",ideb,i,vx[i],vy[i]);
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
	  Sciprintf("first out %d->%d=(%d,%d)\r\n",ideb,i,vx[i],vy[i]);
#endif
	  return(i);
	}
    }
  return(-1);
}

static void C2F(analyze_points)(integer n, integer *vx, integer *vy, integer onemore)
{ 
  integer iib,iif,ideb=0,vxl[2],vyl[2];
  integer verbose=0,wd[2],narg;
  xget_windowdim(&verbose,wd,&narg,vdouble);
  xleft=0;xright=wd[0]; ybot=0;ytop=wd[1];
#ifdef DEBUG1
  xleft=100;xright=300;
  ybot=100;ytop=300;
  XDrawRectangle(dpy, ScilabXgc->Cdrawable, gc,xleft,ybot,(unsigned)xright-xleft,
		 (unsigned)ytop-ybot);
#endif
#ifdef DEBUG 
  Sciprintf("inside analyze\r\n");
#endif
  while (1) 
    { integer j;
    iib=first_in(n,ideb,vx,vy);
    if (iib == -1) 
      { 
#ifdef DEBUG
	Sciprintf("[%d,end=%d] polyline out\r\n",(int)ideb,(int)n);
	/* all points are out but segments can cross the box */
#endif 
	for (j=ideb+1; j < n; j++) My2draw(j,vx,vy);
	break;
      }
    else 
      if ( iib - ideb > 1) 
	{
	  /* un partie du polygine est totalement out de ideb a iib -1 */
	  /* mais peu couper la zone */
	  for (j=ideb+1; j < iib; j++) My2draw(j,vx,vy);
	};
    iif=first_out(n,iib,vx,vy);
    if (iif == -1) {
      /* special case the polyligne is totaly inside */
      if (iib == 0) 
	{
	  if (gtk_store_points(n,vx,vy,onemore))
	    {
	      int n1 ;
	      if (onemore == 1) n1 = n+1;else n1= n;
	      XDroutine(n1);
	      return;
	    }
	  else
	    return;
	}
      else 
	MyDraw(iib,n-1,vx,vy);
      break;
    }
#ifdef DEBUG
    Sciprintf("Analysed : [%d,%d]\r\n",(int)iib,(int)iif);
#endif 
    MyDraw(iib,iif,vx,vy);
    ideb=iif;
    }
  if (onemore == 1) {
    /* The polyligne is closed we consider the closing segment */
    integer x1n,y1n,x2n,y2n,flag1=0;
    vxl[0]=vx[n-1];vxl[1]=vx[0];vyl[0]=vy[n-1];vyl[1]=vy[0];
    clip_line(vxl[0],vyl[0],vxl[1],vyl[1],&x1n,&y1n,&x2n,&y2n,&flag1);
    if ( flag1==0) return ;
    if ( gtk_store_points((integer)2L,vxl,vyl,(integer)0L))
      {
	if (flag1==1||flag1==3) change_points((integer)0L,x1n,y1n);
	if (flag1==2||flag1==3) change_points((integer)1L,x2n,y2n);
	XDroutine(2);
      }
  }
}

int CheckScilabXgc(void)
{
  return( ScilabXgc != (struct BCG *) 0);
}

/*--------------------------------------------------------------------------
 * Create Graphic widget 
 *--------------------------------------------------------------------------*/

/* Infos 
 *  width = gdk_screen_width();
 *  gdk_screen_width_mm();
 *  height = gdk_screen_height();
 *  heightMM = gdk_screen_height_mm();
 *  gtk_widget_destroy(dd->window);
 *  gdk_pixmap_unref(dd->pixmap);
 *
 */

/* a revoir XXXX */

#define R_RED(col)	(((col)	   )&255) 
#define R_GREEN(col)	(((col)>> 8)&255)
#define R_BLUE(col)	(((col)>>16)&255)

/* set the r, g, b, and pixel values of gcol to color */

void SetRgBColor(BCG *dd,int red,int green,int blue)
{
  GdkColor gcol = { gdk_rgb_xpixel_from_rgb((red << 16)|(green << 8)|(blue)),0,0,0};
  gdk_gc_set_foreground(dd->wgc, &gcol);
}

static void SetColor(GdkColor *gcol, int color)
{
  int red, green, blue;
  red = R_RED(color);
  green = R_GREEN(color);
  blue = R_BLUE(color);
  gcol->red = 0;
  gcol->green = 0;
  gcol->blue = 0;
  gcol->pixel = gdk_rgb_xpixel_from_rgb((red << 16)|(green << 8)|(blue));
}


/* signal functions */

static gint realize_event(GtkWidget *widget, gpointer data)
{
  BCG *dd = (BCG *) data;
  g_return_val_if_fail(dd != NULL, FALSE);
  g_return_val_if_fail(dd->drawing != NULL, FALSE);
  g_return_val_if_fail(GTK_IS_DRAWING_AREA(dd->drawing), FALSE);
  /* create gc */
  dd->wgc = gdk_gc_new(dd->drawing->window);
  /* standard gc : for pixmap copies */
  /* this gc could be shared by all windows */
  dd->stdgc = gdk_gc_new(dd->drawing->window);
  /* set the cursor */
  dd->gcursor = gdk_cursor_new(GDK_CROSSHAIR);
  dd->ccursor = gdk_cursor_new(GDK_TOP_LEFT_ARROW);
  gdk_window_set_cursor(dd->drawing->window, dd->ccursor);
  /* set window bg */
  gdk_window_set_background(dd->drawing->window, &dd->gcol_bg);
  return FALSE;
}

static gint configure_event(GtkWidget *widget, GdkEventConfigure *event, gpointer data)
{
  BCG *dd = (BCG *) data;
  g_return_val_if_fail(dd != NULL, FALSE);
  g_return_val_if_fail(dd->drawing != NULL, FALSE);
  g_return_val_if_fail(GTK_IS_DRAWING_AREA(dd->drawing), FALSE);

  /* check for resize */
  if( (GTK_WIDGET_REALIZED(dd->drawing)) 
      && ((dd->CWindowWidth != event->width) || (dd->CWindowHeight != event->height))) {
    dd->CWindowWidth = event->width;
    dd->CWindowHeight = event->height;
    dd->resize = 1;
  }
  return FALSE;
}

static gint expose_event(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
  BCG *dd = (BCG *) data;
  g_return_val_if_fail(dd != NULL, FALSE);
  g_return_val_if_fail(dd->drawing != NULL, FALSE);
  g_return_val_if_fail(GTK_IS_DRAWING_AREA(dd->drawing), FALSE);
  if(dd->resize != 0) { 
      dd->resize = 0;
      gdk_pixmap_unref(dd->pixmap);
      dd->pixmap = gdk_pixmap_new(dd->drawing->window,
					   dd->CWindowWidth, dd->CWindowHeight,
					   -1);

      /* fill private background with background */
      gdk_gc_set_background(dd->stdgc, &dd->gcol_bg);
      gdk_draw_rectangle(dd->pixmap,dd->stdgc, TRUE,0,0,dd->CWindowWidth, dd->CWindowHeight);
      /* On lance l'action standard de resize + redessin  */
      scig_resize(dd->CurWindow);
      gdk_draw_pixmap(dd->drawing->window, dd->stdgc, dd->pixmap,0,0,0,0,
		      dd->CWindowWidth, dd->CWindowHeight);
    }
  else 
    {
      gdk_draw_pixmap(dd->drawing->window, dd->stdgc, dd->pixmap,
		      event->area.x, event->area.y, event->area.x, event->area.y,
 		      event->area.width, event->area.height);
    }
  return FALSE;
}

#define R_RGB(r,g,b)	((r)|((g)<<8)|((b)<<16))

static int GTK_Open(struct BCG *dd, char *dsp, double w, double h)
{
  static char gwin_name[100];
  gint iw, ih;
  GtkWidget *scrolled_window;
  GtkWidget *vbox;
  /* initialise pointers */
  dd->drawing = NULL;
  dd->wgc = NULL;
  dd->gcursor = NULL;
  dd->ccursor = NULL;
  gdk_rgb_init();
  gtk_widget_push_visual(gdk_rgb_get_visual());
  gtk_widget_push_colormap(gdk_rgb_get_cmap());

  /* create window etc */

  dd->CWindowWidth = iw = w ; /*  / pixelWidth(); */
  dd->CWindowHeight = ih = h; /*  pixelHeight(); */

  dd->window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  sprintf( gwin_name, "Graphic Window %d", dd->CurWindow );
  gtk_window_set_title (GTK_WINDOW (dd->window),  gwin_name);

  gtk_window_set_policy(GTK_WINDOW(dd->window), TRUE, TRUE, FALSE);

  gtk_widget_realize(dd->window);

  vbox = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (dd->window), vbox);
  gtk_widget_show (vbox);

  dd->vbox =  gtk_vbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (vbox), dd->vbox, FALSE, TRUE, 0);
  dd->menu_entries = graphic_initial_menu(dd->CurWindow );
  dd->menubar = NULL;
  create_graphic_window_menu(dd);
  dd->CinfoW = gtk_statusbar_new ();
  gtk_box_pack_start (GTK_BOX (vbox), dd->CinfoW, FALSE, TRUE, 0);

  /* create a new scrolled window. */
  dd->scrolled = scrolled_window = gtk_scrolled_window_new (NULL, NULL);
  gtk_container_set_border_width (GTK_CONTAINER (scrolled_window),0);

  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
                                    GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

  /* fix min size of the scrolled window */
  gtk_widget_set_usize(scrolled_window,iw,ih);
  
  /* place and realize the scrolled window  */

  gtk_box_pack_start (GTK_BOX (vbox), scrolled_window, TRUE, TRUE, 0);

  gtk_widget_realize(scrolled_window);

  /* create drawingarea */

  dd->drawing = gtk_drawing_area_new();

  gtk_signal_connect(GTK_OBJECT(dd->drawing), "button-press-event",
		     (GtkSignalFunc) locator_button_press, (gpointer) dd);
  gtk_signal_connect(GTK_OBJECT(dd->drawing), "button-release-event",
		     (GtkSignalFunc) locator_button_release, (gpointer) dd);
  gtk_signal_connect(GTK_OBJECT(dd->drawing), "motion-notify-event",
		     (GtkSignalFunc) locator_button_motion, (gpointer) dd);
  gtk_signal_connect(GTK_OBJECT(dd->drawing), "realize",
		     (GtkSignalFunc) realize_event, (gpointer) dd);
  /* 
  gtk_signal_connect (GTK_OBJECT (dd->drawing), "key_press_event",
		      (GtkSignalFunc) key_press_event, (gpointer) dd);
  */
  gtk_widget_set_events(dd->drawing, GDK_EXPOSURE_MASK 
			| GDK_BUTTON_PRESS_MASK 
			| GDK_KEY_PRESS_MASK 
			| GDK_BUTTON_RELEASE_MASK
			| GDK_POINTER_MOTION_MASK
			| GDK_POINTER_MOTION_HINT_MASK
			| GDK_LEAVE_NOTIFY_MASK );

  /* drawingarea properties */
  /* min size of the graphic window */
  gtk_widget_set_usize(dd->drawing, iw, ih);

  /* setup background color */
  dd->bg = R_RGB(255, 255, 255);
  SetColor(&dd->gcol_bg, dd->bg);

  /* setup foreground color */
  dd->bg = R_RGB(0,0,0);
  SetColor(&dd->gcol_fg, dd->fg);

  /* place and realize the drawing area */
  gtk_scrolled_window_add_with_viewport (
      GTK_SCROLLED_WINDOW (scrolled_window),dd->drawing);
  gtk_widget_realize(dd->drawing);

  /* connect to signal handlers, etc */
  gtk_signal_connect(GTK_OBJECT(dd->drawing), "configure_event",
		     (GtkSignalFunc) configure_event, (gpointer) dd);
  gtk_signal_connect(GTK_OBJECT(dd->drawing), "expose_event",
		     (GtkSignalFunc) expose_event, (gpointer) dd);

  gtk_signal_connect(GTK_OBJECT(dd->window), "delete_event",
		     (GtkSignalFunc)  sci_destroy_window, (gpointer) dd);

  gtk_signal_connect (GTK_OBJECT (dd->window), "key_press_event",
      (GtkSignalFunc) key_press_event, (gpointer) dd);

  /* show everything */
  gtk_widget_realize(dd->window);
  gtk_widget_show_all(dd->window);

  /* create offscreen drawable */
  dd->pixmap = gdk_pixmap_new(dd->drawing->window,
			      dd->CWindowWidth, dd->CWindowHeight,
			      -1);

  gdk_gc_set_foreground(dd->stdgc, &dd->gcol_bg);
  gdk_draw_rectangle(dd->pixmap, dd->stdgc, TRUE, 0, 0,
		     dd->CWindowWidth, dd->CWindowHeight);

  /* let other widgets use the default colour settings */
  gtk_widget_pop_visual();
  gtk_widget_pop_colormap();
  return 1;
}

static void CreateGtkGWindow(struct BCG *ScilabXgc) {
  start_sci_gtk(); /* be sure that gtk is started */
  GTK_Open(ScilabXgc,"unix:0",600,400);
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

static void xset_scilabxgc(integer *v1, integer *v2, integer *v3, integer *v4)
{

}
static void xget_scilabxgc(integer *verbose, integer *x,integer *narg, double *dummy)
{   
  double **XGC;
  
  XGC=(double **)dummy;
  *XGC= (double *)ScilabXgc;
  
}
/* NG end */


