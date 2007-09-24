/*------------------------------------------------------------------------
 *    Graphic library
 *    Copyright (C) 1998-2003 Enpc/Jean-Philippe Chancelier
 *    jpc@cermics.enpc.fr 
 *    Fabrice Leray 2004
 *    Allan CORNET 2004
 *--------------------------------------------------------------------------*/

/***************************************************************** 
 *  Windows driver 
 *****************************************************************/

#ifndef STRICT 
#define STRICT
#endif 

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdarg.h>

#include "../wsci/wresource.h"
#include "../wsci/resource.h"
#include "../wsci/wcommon.h"
#include "../wsci/wgraph.h"
#include "../wsci/wtext.h"
#include "Events.h"

#include "bcg.h"
#include "periWin.h" 
#include "../version.h"
#include "color.h" 
#include "Graphics.h"
#include "scigraphic.h"
#include "../machine.h"
#include "../wsci/GetOS.h"


#ifdef WITH_TK
#include "tcl.h"
#include "tk.h"
#include "tkPlatDecls.h"

extern Tcl_Interp *TCLinterp;
extern Tk_Window TKmainWindow;

extern void flushTKEvents ();
extern int tcl_check_one_event();
#endif

#include "../os_specific/win_mem_alloc.h" /* MALLOC */


#define M_PI	3.14159265358979323846
#define CoordModePrevious 1
#define CoordModeOrigin 0

extern BOOL IsTKGraphicalMode(void);
#ifndef WITH_TK
#define GXxor 6
#endif

/** 
    Warning : the following code won't work if the win.a library is 
    replaced by a dll. The way to find WndGraphProc should be changed 

    Warning : Potential loop 
    It's dangerous to use sciprint in the following code 
    if the argument string to sciprint contains a \n
    because this will call the TextMessage function (
    which will enter a message loop).
    One can enter an infinite loop if TexMessage is activated 
    while inside WndGraphProc and all the following function 
    can be called while inside WndGraphProc.
**/

LRESULT CALLBACK WndGraphProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndParentGraphProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
void setcolormapg(struct BCG *Xgc,integer *v1, integer *v2, double *a, integer *v3);
static void C2F(setscilabFigure)(integer *v1,integer *v2,integer *v3,integer *v4,integer *v5,integer *v6,double *figure);
static void C2F(getscilabFigure)(integer *verbose, integer *x,integer *narg, double *figure);
static void C2F(setscilabVersion)(integer *vers, integer *v2, integer *v3, integer *v4);
static void C2F(getscilabVersion)(integer *verbose, integer *vers, integer *narg, double *dummy);
static void C2F(setscilabxgc)(integer *v1, integer *v2, integer *v3, integer *v4);
static void C2F(getscilabxgc)(integer *verbose, integer *x,integer *narg, double *dummy);
static void C2F(getcolormapsize)(integer *verbose, integer *num, integer *narg, double *val);

/* Initialization values - Guess Now Scale later */

#define WIN_XMAX (2400)
#define WIN_YMAX (1800)
#define WIN_HCHAR (WIN_XMAX/75) 
#define WIN_VCHAR (WIN_YMAX/25)

/* relative or absolute mode for drawing */

#define MESSAGE4 "Can't allocate point vector"
#define MESSAGE5 "Can't re-allocate point vector" 
#define Char2Int(x)   ( x & 0x000000ff )

static double *vdouble = 0; /* used when a double argument is needed */
/* These DEFAULTNUMCOLORS colors come from Xfig 
   used in periPos.c and periFig.c
*/

unsigned short default_colors[] = {
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

LPSTR szGraphClass = "wscilab_graph";
LPSTR szParentGraphClass = "wscilab_parent_graph";


#define MAXDASH 5
static integer DashTab[MAXDASH] = { PS_SOLID,PS_DASH,PS_DOT,PS_DASHDOT,PS_DASHDOTDOT};


/** 
    We need to provide a hdc for each graphic operation 
    but hdc can be changed to be set to a window a printer a metafile etc...
    Thus hdc is kept as a global variable 
    which will be set to what we need : see Xcall.c 
**/

extern GW graphwin; /** keeps information for the current graphic window **/
extern TW textwin; /** keeps information for the current scilab window **/
extern int versionflag;

/** XXX a mettre ailleurs **/

static  POINT *C2F(ReturnPoints)();
static HFONT getcurfont();
int XorString(integer x,integer y, char *string,int fWidth,int fHeight);

static int screencolor = 1 ; /* default screen color or not :initgraphic_*/
static COLORREF DefaultBackground = RGB(255,255,255);
static COLORREF DefaultForeground = RGB(0,0,0);

#define COLORREF COLORREF

/** Structure to keep the graphic state  **/

struct BCG MissileXgc ;

/* structure for Window List  */

typedef  struct 
{
  struct BCG winxgc;
  struct WindowList *next;
} WindowList  ;

static WindowList *The_List  = (WindowList *) NULL;
static integer deleted_win = -1;
struct BCG *ScilabXgc = (struct BCG *) 0;

/** functions **/

HWND GetWindowNumber();
struct BCG *GetWinXgc();
struct BCG *GetWindowXgcNumber();
struct BCG *AddNewWindow();
struct BCG *AddNewWindowToList();

int SwitchWindow(integer *intnum);

static int ReallocVector();
static void DrawMark(),LoadFonts(), LoadSymbFonts();
static void C2F(loadfamily_n)();
static void CreateGraphClass();
static BOOL RegisterParentWindowGraphClass (void);
static BOOL RegisterWindowGraphClass (void);
static void XDrawPoints();
static BOOL SciPalette(int iNumClr);
static void set_current_clip (void);
static void set_clip_after_scroll (void) ;


extern void UpdateFileGraphNameMenu(struct BCG *ScilabGC);
extern void CreateGraphToolBar(struct BCG * ScilabGC); 
extern void CreateGedMenus(struct BCG * ScilabGC); 
extern void SetIsASingleClickToFalse(void);
extern BOOL IsASingleClickToFalse(void);
extern BOOL IsWindowInterface(void);
extern void RefreshGraphToolBar(struct BCG * ScilabGC) ;
/************************************************
 * dealing with hdc : when using the Rec driver 
 * each command in xcall is ``encadree'' with 
 * SetWinhdc and ReleaseWinHdc 
 * when replaying SetGHdc and  are used to provide 
 * the proper hdc for replaying ( see Rec) 
 * XXXX : voir xbasr ?? 
 * XXXX bien verifier ds les fonction qui suivent si 
 * ScilabXgc peut etre un pointeur nul 
 *************************************************/

static HDC  hdc = (HDC) 0 ; 
static HDC  hdc1 = (HDC) 0 ;

integer sciGetwresize(void);

/*-----------------------------------------------------------------------------------*/
int sciGetScrollInfo(struct BCG *Scilabgc, int sb_ctl, SCROLLINFO *si)
{
  SCROLLINFO totosi;
  switch (sb_ctl) 
    {
    case SB_VERT:
      /* definition des scroll bars verticalles */
      si->cbSize = Scilabgc->vertsi.cbSize;
      si->fMask  = Scilabgc->vertsi.fMask;
      si->nMin   = Scilabgc->vertsi.nMin;
      si->nMax   = Scilabgc->vertsi.nMax;
      si->nPage  = Scilabgc->vertsi.nPage;
      si->nPos   = Scilabgc->vertsi.nPos;
      break;
    case SB_HORZ:
      /* definition des scroll bars horizontalles */
      si->cbSize = Scilabgc->horzsi.cbSize;
      si->fMask  = Scilabgc->horzsi.fMask;
      si->nMin   = Scilabgc->horzsi.nMin;
      si->nMax   = Scilabgc->horzsi.nMax;
      si->nPage  = Scilabgc->horzsi.nPage;
      si->nPos   = Scilabgc->horzsi.nPos;
      break;
    default:
      break;
    }
  /* force le rafraichissement de l'affichage des SB !!! */
  GetScrollInfo(Scilabgc->CWindow, sb_ctl, &totosi);
  return 0;
}
/*-----------------------------------------------------------------------------------*/
int sciSetScrollInfo(struct BCG *Scilabgc, int sb_ctl, SCROLLINFO *si, BOOLEAN bRedraw)
{
  int inttmp = si->nMax;
  SCROLLINFO totosi;
  
  switch (sb_ctl) {
  case SB_VERT:
    /* definition des scroll bars verticalles */
    Scilabgc->vertsi.cbSize = si->cbSize;
    Scilabgc->vertsi.fMask  = si->fMask;
    Scilabgc->vertsi.nMin   = si->nMin;
    Scilabgc->vertsi.nMax   = si->nMax;
    Scilabgc->vertsi.nPage  = si->nPage;
    if ( Scilabgc->CurResizeStatus == 1) 
      {
	si->nMax = 0; /* on effectue un swapp pour ne pas changer la val (pointeur !!) */
	/* 0 permet de faire disparaitre les scrollbars */
      }
    Scilabgc->vertsi.nPos   = si->nPos;	
    break;
  case SB_HORZ:
    /* definition des scroll bars horizontalles */
    Scilabgc->horzsi.cbSize = si->cbSize;
    Scilabgc->horzsi.fMask  = si->fMask;
    Scilabgc->horzsi.nMin   = si->nMin;
    Scilabgc->horzsi.nMax   = si->nMax;
    Scilabgc->horzsi.nPage  = si->nPage;
    if ( Scilabgc->CurResizeStatus == 1 ) {  si->nMax = 0;} 
    Scilabgc->horzsi.nPos   = si->nPos;
    break;
  default:
    break;
  }
  /* reset clip region after a scroll */
  /* set_clip_after_scroll() ; */
  /* ? */  
  SetScrollInfo(Scilabgc->CWindow, sb_ctl, si, bRedraw);
  if (  Scilabgc->CurResizeStatus == 1)
    si->nMax = inttmp;
  /* force le rafraichissement de l'affichage des SB !!! */
  sciGetScrollInfo(Scilabgc, sb_ctl, &totosi);
  return 0;
}
/*-----------------------------------------------------------------------------------*/
int sciInitScrollBar(struct BCG *Scilabgc)
{
  SCROLLINFO si;

  /* definition des scroll bars verticalles a la creationS*/
  si.cbSize = sizeof(SCROLLINFO);
  si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS;
  si.nMin   = 0;
  si.nMax   = Scilabgc->CWindowHeight;
  si.nPage  = Scilabgc->CWindowHeightView;
  si.nPos   = 0;
  sciSetScrollInfo(ScilabXgc,SB_VERT, &si, TRUE);

  /* definition des scroll bars horizontalles */
  si.cbSize = sizeof(SCROLLINFO);
  si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS;
  si.nMin   = 0;
  si.nMax   = Scilabgc->CWindowWidth;
  si.nPage  = Scilabgc->CWindowWidthView;
  si.nPos   = 0;
  sciSetScrollInfo(ScilabXgc,SB_HORZ, &si, TRUE);
  return 0;
}
/*-----------------------------------------------------------------------------------*/
void  SetWinhdc()
{
  if ( ScilabXgc != (struct BCG *) 0 && ScilabXgc->CWindow != (HWND) 0)
    {
      if ( sciGetPixmapStatus() == 1) 
	hdc = ScilabXgc->hdcCompat;
      else
	hdc=TryToGetDC(ScilabXgc->CWindow);
    }
}
/*-----------------------------------------------------------------------------------*/
int  MaybeSetWinhdc()
{
  int DiD = GetDriverId(); 	/* F.Leray 26.07.04 */
  if(  DiD != 0)
    return 0;

  /** a clarifier XXXX faut-il aussi un test **/
  if ( hdc == (HDC) 0)  
    {
      if ( sciGetPixmapStatus() == 1) 
	{
	  hdc	= ScilabXgc->hdcCompat;

	  /* 09.02.04 et 16.02.04 Leray*/
	  /* IL FAUDRA RETOURNER UNE VALEUR MAIS LAQUELLE ???*/
	  // return( ?? );
	  // je mets un return(1) pour le moment pour rester cohérent
	  // avec SetWinhdc (cf. ci-dessus).
	  return(1);
	}
      else
	{
	  hdc=TryToGetDC(ScilabXgc->CWindow);
	  return(1);
	}
    }
  else 
    {
      return(0);
    }
}
/*-----------------------------------------------------------------------------------*/
void  ReleaseWinHdc()
{
  int DiD = GetDriverId(); 	/* F.Leray 26.07.04 */
  if(  DiD != 0)
    { /* Nothing to do */
    }
  else
    {
      if ( ScilabXgc != (struct BCG *) 0 && ScilabXgc->CWindow != (HWND) 0)
	{
	  if ( sciGetPixmapStatus() != 1) 
	    ReleaseDC(ScilabXgc->CWindow,hdc);
	  hdc = (HDC) 0;
	}
    }
}
/*-----------------------------------------------------------------------------------*/
/****************************
 * used when replaying with 
 * printers or memory hdc 
 ***************************/

void SetGHdc(lhdc,width,height)
     HDC lhdc;
     int width,height;
{
  if ( lhdc != (HDC) 0)
    {
      hdc1= hdc ;
      if ( hdc != (HDC) 0) ReleaseWinHdc();
      hdc = lhdc;
      ScilabXgc->CWindowWidth  = width;
      ScilabXgc->CWindowHeight = height;
    }
  else
    {
      if ( hdc1 != (HDC) 0 && ScilabXgc != (struct BCG *) 0 
	   && ScilabXgc->CWindow != (HWND) 0 )
	{
	  RECT rect;
	  hdc=TryToGetDC(ScilabXgc->CWindow);
	  /* get back the dimensions   */
	  GetClientRect(ScilabXgc->CWindow,&rect);
	  ScilabXgc->CWindowWidthView  = rect.right-rect.left;
	  ScilabXgc->CWindowHeightView = rect.bottom-rect.top;
	}
    }
}
/*-----------------------------------------------------------------------------------*/
/** Allocating colors in BCG struct */

int XgcAllocColors(xgc,m)
     struct BCG *xgc;
     int m;
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
  if (!(xgc->Colors = (COLORREF *) MALLOC(mm*sizeof(COLORREF)))) {
    Scistring("XgcAllocColors: unable to alloc\n");
    FREE(xgc->Red);
    FREE(xgc->Green);
    FREE(xgc->Blue);
    return 0;
  }
  return 1;
}
/*-----------------------------------------------------------------------------------*/
int XgcFreeColors(xgc)
     struct BCG *xgc;
{
  FREE(xgc->Red); xgc->Red = (float *) 0;
  FREE(xgc->Green);xgc->Green = (float  *) 0;
  FREE(xgc->Blue); xgc->Blue = (float *) 0;
  FREE(xgc->Colors); xgc->Colors = (COLORREF *) 0;
  return(0);
}
/*-----------------------------------------------------------------------------------*/
/** Pixmap routines **/

void sci_pixmapclear_rect(HDC hdc_c, struct BCG *ScilabGC,int ww,int hh)
{
  RECT rect;
  COLORREF px;
  HBRUSH hBrush;
  if ( hdc_c  == NULL) return ;
  px = (ScilabGC->Colors == NULL)? DefaultBackground 
    :  ScilabGC->Colors[ScilabGC->NumBackground];
  SetBkColor( hdc_c , px );
  rect.top    = 0;
  rect.bottom = ww ;
  rect.left   = 0;
  rect.right  = hh ;
  hBrush = CreateSolidBrush(px);
  FillRect( hdc_c , &rect,hBrush );
  DeleteObject(hBrush);
}
/*-----------------------------------------------------------------------------------*/
void sci_pixmapclear(HDC hdc_c, struct BCG *ScilabGC )
{
  sci_pixmapclear_rect(hdc_c,ScilabGC, 
		       ScilabGC->CWindowHeight,ScilabGC->CWindowWidth);
}
/*-----------------------------------------------------------------------------------*/
void C2F(pixmapclear)( integer *v1, integer *v2, integer *v3, integer *v4)
{
  sci_pixmapclear(ScilabXgc->hdcCompat, ScilabXgc);
}
/*-----------------------------------------------------------------------------------*/
void C2F(show)( integer *v1, integer *v2, integer *v3, integer *v4)
{
  if ( ScilabXgc->hdcCompat)
    {
      HDC hdc1=TryToGetDC(ScilabXgc->CWindow);
      BitBlt (hdc1,0,0,ScilabXgc->CWindowWidth,ScilabXgc->CWindowHeight,
	      ScilabXgc->hdcCompat,0,0,SRCCOPY);
      /* ScilabXgc->horzsi.nPos,ScilabXgc->vertsi.nPos,SRCCOPY); */
      ReleaseDC(ScilabXgc->CWindow,hdc1);
      RefreshGraphToolBar(ScilabXgc);
    }

}
/*-----------------------------------------------------------------------------------*/

/** 
 *  Resize the pixmap associated to CurWindow and store it 
 *  back in the window List 
 **/

void sci_pixmap_resize(struct BCG * ScilabGC, int x, int y) 
{
  HDC hdc1;
  HBITMAP hbmTemp;
  hdc1=TryToGetDC(ScilabGC->CWindow);
  hbmTemp = CreateCompatibleBitmap (hdc1,x,y);
  ReleaseDC(ScilabGC->CWindow,hdc1);
  if (!hbmTemp)
    {
      sciprint("Can't resize pixmap\r\n");
      return;
    }
  else
    {
      HBITMAP  hbmSave;
      hbmSave = SelectObject ( ScilabGC->hdcCompat, hbmTemp);
      if ( ScilabGC->hbmCompat != NULL)
	DeleteObject (ScilabGC->hbmCompat);
      ScilabGC->hbmCompat = hbmTemp;
      sci_pixmapclear(ScilabGC->hdcCompat, ScilabGC);
      /* XXX C2F(show)(PI0,PI0,PI0,PI0); */ 
    }
}
/*-----------------------------------------------------------------------------------*/
void CPixmapResize(int x,int y)
{
  void sci_pixmap_resize(ScilabXgc, x, y) ;
}
/*-----------------------------------------------------------------------------------*/
/* 
 * Resize the Pixmap according to window size change 
 * But only if there's a pixmap 
 */
void CPixmapResize1()
{
  if ( sciGetPixmapStatus() == 1 )
    sci_pixmap_resize(ScilabXgc,ScilabXgc->CWindowWidth,ScilabXgc->CWindowHeight);
}

/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------
  \encadre{General routines}
  -----------------------------------------------------*/

/** To select (raise on the screen )the current graphic Window  **/
/** If there's no graphic window then select creates one **/

void C2F(xselgraphic)(v1, v2, v3, v4, v5, v6, v7, dv1, dv2, dv3, dv4)
     char *v1;
     integer *v2;
     integer *v3;
     integer *v4;
     integer *v5;
     integer *v6;
     integer *v7;
     double *dv1;
     double *dv2;
     double *dv3;
     double *dv4;
{ 
  integer ierr;
  if (ScilabXgc == (struct BCG *)0 || ScilabXgc->CWindow == (HWND) NULL) 
    C2F(initgraphic)("",PI0,&ierr,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
  if (IsIconic(ScilabXgc->hWndParent)) 
    ShowWindow(ScilabXgc->hWndParent, SW_SHOWNORMAL);
  BringWindowToTop(ScilabXgc->hWndParent);
}
/*-----------------------------------------------------------------------------------*/
/** End of graphic (do nothing)  **/
void C2F(xendgraphic)()
{
} 
/*-----------------------------------------------------------------------------------*/
void C2F(xend)(v1, v2, v3, v4, v5, v6, v7, dv1, dv2, dv3, dv4)
     char *v1;
     integer *v2;
     integer *v3;
     integer *v4;
     integer *v5;
     integer *v6;
     integer *v7;
     double *dv1;
     double *dv2;
     double *dv3;
     double *dv4;
{
  /** Nothing in Windows **/
}
/*-----------------------------------------------------------------------------------*/
/** Clear the current graphic window     **/

void C2F(clearwindow)(v1, v2, v3, v4, v5, v6, v7, dv1, dv2, dv3, dv4)
     char *v1;
     integer *v2,*v3,*v4,*v5,*v6,*v7;
     double *dv1,*dv2,*dv3,*dv4;
{
  RECT rect;
  static COLORREF px;
  HBRUSH hBrush;
  if ( ScilabXgc->ClipRegionSet == 1) 
    SelectClipRgn(hdc,NULL);
  if ( sciGetPixmapStatus() == 1) 
    {
      C2F(pixmapclear)(PI0,PI0,PI0,PI0);
      C2F(show)(PI0,PI0,PI0,PI0);      
    }
  else
    {
      px = (ScilabXgc->Colors == NULL)? DefaultBackground 
	:  ScilabXgc->Colors[ScilabXgc->NumBackground];
      SetBkColor(hdc, px );
      GetClientRect(ScilabXgc->CWindow, &rect);
      /** verifier ce qui se passe si on est en Xor ? XXXXXXXX **/
      hBrush = CreateSolidBrush(px);
      /* on met a jour la couleur de fond */
      rect.top    = 0;
      rect.left   = 0;
      rect.right  = ScilabXgc->CWindowWidth;
      rect.bottom = ScilabXgc->CWindowHeight;
      FillRect(hdc, &rect,hBrush );
      DeleteObject(hBrush);
    }
  /* reset the clip region using current data */
  set_current_clip ();

  if (! IsTKGraphicalMode()) 
  {
	  RefreshGraphToolBar(ScilabXgc);
  }
	
}
/*-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------
  \encadre{To generate a pause, in seconds}
  ------------------------------------------------------------*/

void C2F(xpause)(str, sec_time, v3, v4, v5, v6, v7, dv1, dv2, dv3, dv4)
     char *str;
     integer *sec_time;
     integer *v3;
     integer *v4;
     integer *v5;
     integer *v6;
     integer *v7;
     double *dv1;
     double *dv2;
     double *dv3;
     double *dv4;
{ 
  int ms = (*sec_time)/1000; /** time is specified in microseconds in scilab**/
  if (*sec_time > 0)
    {
#ifdef WIN32
      {
	if (ms > 0) Sleep(ms); /* Number of milliseconds to sleep. */
      }
#else
      {
#ifdef HAVE_USLEEP
	{ usleep(*sec_time); }
#else
#ifdef HAVE_SLEEP
	{  sleep(*sec_time); }
#endif
#endif
      }
#endif
    }
  else
    {
      Scierror(999,"xpause: error time must be >0.\r\n");
      return; 
    }
}


/*-----------------------------------------------------------------------------------*/
/*************************************************************
 * Changes the popupname 
 *************************************************************/

void Setpopupname(string)
     char *string;
{ 
  /* set the window title if exists */
  SetWindowText(ScilabXgc->hWndParent, string);
}
/*-----------------------------------------------------------------------------------*/

void C2F(setpopupname)(x0, v2, v3, v4, v5, v6, v7, dv1, dv2, dv3, dv4)
     char *x0;
     integer *v2,*v3,*v4,*v5,*v6,*v7;
     double *dv1,*dv2,*dv3,*dv4;
{
  Setpopupname(x0);
}
/*-----------------------------------------------------------------------------------*/
/****************************************************************
 Wait for mouse click in graphic window 
   send back mouse location  (x1,y1)  and button number  
   0,1,2}
   There's just a pb if the window is iconified when we try to click 
   in this case we return i= -1
****************************************************************/

static int check_mouse(MSG *msg,integer *ibutton,integer *x1,integer *yy1,
		       int xofset,int yofset,
		       int getmouse,int getrelease);
/*-----------------------------------------------------------------------------------*/
int check_pointer_win(int *x1,int *yy1,int *win)
{
  RECT lpRect;
  HWND hwnd_window_pointed;
  WindowList *listptr = The_List;
  POINT Point;
  integer iwin = -1;
  /* where if the pointer  */
  GetCursorPos(&Point);
  /* over which window */
  hwnd_window_pointed = WindowFromPoint(Point);
  /* le curseur est bien sur une fenetre */
  if (hwnd_window_pointed != NULL)
    {
      iwin = -1;
      listptr = The_List;
      while (listptr != (WindowList  *) 0 )
	{
	  if (hwnd_window_pointed == listptr->winxgc.CWindow 
	      || hwnd_window_pointed == listptr->winxgc.hWndParent)
	    {
	      iwin = listptr->winxgc.CurWindow;
	      break;
	    }
	  listptr =  (WindowList *)listptr->next;
	}
      /* si la fenetre pointee est une fenetre scilab */
      if ( iwin != -1 )
	{
	  /* quelle est la dimension de la fenetre */
	  GetWindowRect(listptr->winxgc.CWindow , &lpRect);
	  /* on calcule la position relative du click */
	  *x1  = Point.x - lpRect.left + listptr->winxgc.horzsi.nPos;
	  *yy1 = Point.y - lpRect.top  + listptr->winxgc.vertsi.nPos;
	  *win = iwin;
	  return 1;
	}
    }
  return 0;
}
/*-----------------------------------------------------------------------------------*/
/* used by xclick_any and xclick */ 
extern void set_wait_click(int val); 
extern void set_event_select(int val); 
/*-----------------------------------------------------------------------------------*/
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
/*-----------------------------------------------------------------------------------*/
void C2F(xclick_any)(char *str,integer *ibutton,integer* x1,integer * yy1, integer *iwin,integer *iflag,integer *istr,double * dv1, double *dv2,double * dv3,double * dv4)
{
  integer win;
  integer wincount;
  integer lstr ;
  int motion,release,ok;
  // reinitialisation à -1
  // correction bg
  // plot2d();  => fermer la fenêtre du plot avec la croix.
  // plot2d();
  // [c_i,c_x,c_y,c_w,c_m]=xclick()
  // ==> xclick renvoie le code -100 pour le bouton ce qui correspond à une fermeture de fenêtre
  deleted_win = -1;

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
  

  if ( *iflag ==0 )  ClearClickQueue(-1); /*clear the queue if required*/
 
  /** first check if an event has been store in the queue while wait_for_click was 0 **/
  set_wait_click(1); /*disable event handler if any */
  set_event_select(1+4); /* select press and release events*/
  while (1) 
    {
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
      Sleep(1);

      if ( deleted_win != -1 )
	{
	  /* a graphic window was deleted we quit */
	  *iwin = deleted_win;
	  deleted_win = -1;
	  *x1= 0;
	  *yy1 = 0;
	  *ibutton    = -100;
	  set_wait_click(0);
	  set_event_select(1+2+4); 
	  return ;
	}

      /** Check menu activation **/
      if ( *istr==1 && C2F(ismenu)()==1 ) 
	{
	  int entry;
	  C2F(getmen)(str,&lstr,&entry);
	  *ibutton = -2;
	  *istr=lstr;
	  *x1=0;
	  *yy1=0;
	  *iwin=-1;
	  break;
	}

    }
  set_wait_click(0);
  set_event_select(1+2+4); 
}
/*-----------------------------------------------------------------------------------*/
void C2F(xclick)(str, ibutton, x1, yy1, iflag,istr, v7, dv1, dv2, dv3, dv4)
     char *str;
     integer *ibutton,*x1,*yy1,*iflag,*istr,*v7;
     double *dv1;
     double *dv2;
     double *dv3;
     double *dv4;
{
  integer lstr ;
  SciClick(ibutton,x1, yy1,iflag,0,0,*istr,str,&lstr);
  if ( *istr == 1) 
    {
      if (*ibutton == -2) 
	{
	  /*sciprint("Menu activated %s %d",str,lstr);*/
	  *istr = lstr;
	}
      else
	*istr = 0;
    }
}
/*-----------------------------------------------------------------------------------*/
void C2F(xgetmouse)(str, ibutton, x1, yy1,iflag, v6, v7, dv1, dv2, dv3, dv4)
     char *str;
     integer *ibutton,*x1,*yy1,*iflag,*v6,*v7;
     double *dv1;
     double *dv2;
     double *dv3;
     double *dv4;
{
  SciClick(ibutton,x1, yy1,iflag,v6[0],v6[1],0,(char *) 0,(integer *)0);
}
/*-----------------------------------------------------------------------------------*/
void SciMouseCapture()
{
  SetCapture(ScilabXgc->CWindow);
}
/*-----------------------------------------------------------------------------------*/
void SciMouseRelease()
{
  ReleaseCapture();
}
/*-----------------------------------------------------------------------------------*/


/*****************************************
 * general function for mouse click or 
 * dynamic menu activation 
 * 
 * if iflag = 0 : clear previous mouse click 
 * if iflag = 1 : don't 
 * if getmouse = 1 : check also mouse move 
 * if dyn_men = 1 ; check also dynamic menus 
 *              ( return the buton code in str )
 *****************************************/
static int check_mouse(MSG *msg,integer *ibutton,integer *x1,integer *yy1,
		       int xofset,int yofset,
		       int getmouse,int getrelease)
{
  if (  msg->message == WM_LBUTTONDOWN )
    {
      *x1=LOWORD(msg->lParam) + xofset;
      *yy1= HIWORD(msg->lParam) + yofset;
      *ibutton=0; 
    }
  else if (  msg->message == WM_MBUTTONDOWN )
    {
      *x1 = LOWORD(msg->lParam) + xofset;
      *yy1= HIWORD(msg->lParam) + yofset;
      *ibutton=1; 
    }
  else if (  msg->message == WM_RBUTTONDOWN )
    {
      *x1 = LOWORD(msg->lParam) + xofset;
      *yy1= HIWORD(msg->lParam) + yofset;
      *ibutton=2; 
    }
  else if ( getmouse == 1 && msg->message == WM_MOUSEMOVE )
    {
      *x1 = LOWORD(msg->lParam) + xofset;
      *yy1= HIWORD(msg->lParam) + yofset;
      *ibutton=-1; /** 0 for left button **/
    }
  else if ( getrelease == 1 &&  msg->message == WM_LBUTTONUP )
    {
      *x1 = LOWORD(msg->lParam) + xofset;
      *yy1= HIWORD(msg->lParam) + yofset;
      *ibutton= -5 ; 
    }
  else if (getrelease == 1 &&  msg->message == WM_MBUTTONUP )
    {
      *x1 = LOWORD(msg->lParam) + xofset;
      *yy1= HIWORD(msg->lParam) + yofset;
      *ibutton= -4 ;
    }
  else if ( getrelease == 1 && msg->message == WM_RBUTTONUP )
    {
      *x1 = LOWORD(msg->lParam) + xofset;
      *yy1= HIWORD(msg->lParam) + yofset;
      *ibutton= -3;
    }
  else 
    {
      return 0; 
    }
  return 1;
}
/*-----------------------------------------------------------------------------------*/
void SciClick(integer *ibutton, integer *x1, integer *yy1, integer *iflag, int getmouse, int getrelease, int dyn_men, char *str, integer *lstr)
{
	integer ok,choice,motion,release;
  int win;
  
  choice=(dyn_men>1); /* depending on lhs */
  if ( ScilabXgc == (struct BCG *) 0 || ScilabXgc->CWindow == (HWND) 0)
    {
      *x1   =  -1;
      *yy1  =  -1;
      *ibutton = -100;
      return;
    }

  win = ScilabXgc->CurWindow;
  
  if ( *iflag ==0 )  ClearClickQueue(ScilabXgc->CurWindow);

  /** Pas necessaire en fait voir si c'est mieux ou moins bien **/
  if (IsIconic(ScilabXgc->hWndParent)) ShowWindow(ScilabXgc->hWndParent, SW_SHOWNORMAL);
  BringWindowToTop(ScilabXgc->hWndParent);

  set_wait_click(1);
  set_event_select(1+2*getmouse+4*getrelease);

  while ( 1 ) 
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
      Sleep(1);

      if ( ScilabXgc == (struct BCG *) 0 || ScilabXgc->CWindow == (HWND) 0)
	{
	  /* graphic window was deleted */
	  *x1= 0 ;
	  *yy1= 0;
	  *ibutton=-100; 
	  set_event_select(1+2+4);
	  set_wait_click(0);
	  return;
	}

      if ( dyn_men == 1 &&  C2F(ismenu)()==1 ) 
	{
	  int entry;
	  C2F(getmen)(str,lstr,&entry);
	  *ibutton = -2;
	  *x1=0;
	  *yy1=0;
	  set_event_select(1+2+4);
	  set_wait_click(0);
	  return ;
	}

      if ( CtrlCHit(&textwin) == 1) 
	{
	  *x1= 0 ;
	  *yy1= 0;
	  *ibutton=0;
	  break ;
	}
    }
  set_event_select(1+2+4);
  set_wait_click(0);
}
 
/*-----------------------------------------------------------------------------------*/
/*------------------------------------------------
  \encadre{Clear a rectangle }
  -------------------------------------------------*/

void C2F(cleararea)(str, x, y, w, h, v6, v7, dv1, dv2, dv3, dv4)
     char *str;
     integer *x;
     integer *y;
     integer *w;
     integer *h;
     integer *v6;
     integer *v7;
     double *dv1;
     double *dv2;
     double *dv3;
     double *dv4;
{
  HBRUSH hBrush;
  RECT rect;
  static COLORREF px;
  px = (ScilabXgc->Colors == NULL)? DefaultBackground 
    :  ScilabXgc->Colors[ScilabXgc->NumBackground];
  SetRect(&rect,(int) *x,(int) *y,(int) *x+*w,(int) *y+*h);
  /** XXXXXX : verifier en Xor **/
  hBrush = CreateSolidBrush(px);
  FillRect(hdc, &rect,hBrush );
  DeleteObject(hBrush);
  /** XXXX : mettre la background brush ds ScilabXgc pour ne pas 
      la cr'eer a chaque fois **/
}

/*-----------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------
  \section{Function for graphic context modification}
  ------------------------------------------------------------------------*/

/** to get the window upper-left point coordinates on the screen  **/

void C2F(getwindowpos)(verbose, x, narg,dummy)
     integer *verbose;
     integer *x;
     integer *narg;
     double *dummy;
{
  RECT rect;
  GetWindowRect(ScilabXgc->hWndParent,&rect);
  *narg = 2;
  x[0]=rect.left; x[1] = rect.top;
  if (*verbose == 1) 
    sciprint("\n ScilabXgc->CWindow position :%d,%d\r\n",x[0],x[1]);
}
/*-----------------------------------------------------------------------------------*/
/** to set the window upper-left point position on the screen **/
void C2F(setwindowpos)(x, y, v3, v4)
     integer *x;
     integer *y;
     integer *v3;
     integer *v4;
{
  SetWindowPos(ScilabXgc->hWndParent,HWND_TOP,*x,*y,0,0,
	       SWP_NOSIZE | SWP_NOZORDER );
}
/*-----------------------------------------------------------------------------------*/
/** To get the window size **/
void C2F(getwindowdim)(verbose, x, narg,dummy)
     integer *verbose;
     integer *x;
     integer *narg;
     double *dummy;
{     
  *narg = 2;
  /* on renvoie la taille de la fenetre virtuelle */
  /* windows devrait s'occupper de tout afficher correctement */
  x[0]= ScilabXgc->CWindowWidth;
  x[1]= ScilabXgc->CWindowHeight;
  if (*verbose == 1) 
    sciprint("\n CWindow dim :%d,%d\r\n",(int) x[0],(int) x[1]);
} 
/*-----------------------------------------------------------------------------------*/
/* To change the window size
 * on redimensionne la dimension virtuelle
 * si le resizing et off, la vue (View) et la virtuelle sont egales
 * voir dans le resize de wgraph.c
 * @see: GPopupResize
 */
void C2F(setwindowdim)(x, y, v3, v4)
     integer *x;
     integer *y;
     integer *v3;
     integer *v4;
{
  RECT rect,rect1;
  int xof,yof;
  SCROLLINFO vertsi;
  SCROLLINFO horzsi;
  if (ScilabXgc->CWindow != (HWND) NULL) 
    {
      /* initialisation des variables SCROLLINFO*/
      sciGetScrollInfo(ScilabXgc, SB_VERT, &vertsi);
      sciGetScrollInfo(ScilabXgc, SB_HORZ, &horzsi);
    
      ScilabXgc->CWindowWidth = Max((int) *x,10);
      ScilabXgc->CWindowHeight =Max((int) *y,10);
      if ( sciGetwresize() == 1 ) {
	ScilabXgc->CWindowWidthView  = ScilabXgc->CWindowWidth;
	ScilabXgc->CWindowHeightView = ScilabXgc->CWindowHeight;
	vertsi.nPos   = 0;
	horzsi.nPos   = 0;
      }

      if ( ScilabXgc->CWindowWidthView > ScilabXgc->CWindowWidth)
	ScilabXgc->CWindowWidthView = ScilabXgc->CWindowWidth;

      if (ScilabXgc->CWindowHeightView > ScilabXgc->CWindowHeight)
	ScilabXgc->CWindowHeightView = ScilabXgc->CWindowHeight;
      CPixmapResize1();

      GetWindowRect (ScilabXgc->hWndParent, &rect) ;
      GetWindowRect (ScilabXgc->CWindow, &rect1) ;
      xof = (rect.right-rect.left)- (rect1.right - rect1.left);
      yof = (rect.bottom-rect.top)- (rect1.bottom -rect1.top );
      if (sciGetwresize () == 0)
	{
	  SetWindowPos (ScilabXgc->hWndParent, HWND_TOP, 0, 0,
			ScilabXgc->CWindowWidthView + xof +
			GetSystemMetrics (SM_CXVSCROLL),
			ScilabXgc->CWindowHeightView + yof +
			GetSystemMetrics (SM_CYHSCROLL),
			SWP_NOMOVE | SWP_NOZORDER);
	}
      else
	{
	  SetWindowPos(ScilabXgc->hWndParent,HWND_TOP,0,0,
		       ScilabXgc->CWindowWidthView  + xof,
		       ScilabXgc->CWindowHeightView + yof,
		       SWP_NOMOVE | SWP_NOZORDER );
	}
      vertsi.nMax   = ScilabXgc->CWindowHeight;
      vertsi.nPage  = ScilabXgc->CWindowHeightView;
      horzsi.nMax   = ScilabXgc->CWindowWidth;
      horzsi.nPage  = ScilabXgc->CWindowWidthView;  
      sciSetScrollInfo(ScilabXgc,SB_VERT, &(vertsi), TRUE);
      sciSetScrollInfo(ScilabXgc,SB_HORZ, &(horzsi), TRUE);
      if ( sciGetwresize() == 0 ) 
	{
	  /* need to force a rdraw in that case */
	  if ( ScilabXgc->horzsi.nPos + ScilabXgc->CWindowWidthView < 
	       ScilabXgc->CWindowWidth 
	       &&  ScilabXgc->vertsi.nPos + ScilabXgc->CWindowHeightView < 
	       ScilabXgc->CWindowHeight) 
	    InvalidateRect(ScilabXgc->CWindow,NULL,FALSE);
	}
    }
}
/*-----------------------------------------------------------------------------------*/
/** To get the popup  window size **/
void C2F(getpopupdim)(verbose, x, narg,dummy)
     integer *verbose;
     integer *x;
     integer *narg;
     double *dummy;
{
  x[0]= ScilabXgc->CWindowWidthView; 
  x[1]= ScilabXgc->CWindowHeightView;
  *narg = 2;
  if (*verbose == 1) 
    sciprint("\n ScilabXgc->CWindow dim :%d,%d\r\n",(int) x[0],(int) x[1]);
}

/*-----------------------------------------------------------------------------------*/
/**C2F(setpopupdim)
 *@description: To change the popup window size  (visible borders)
 *              it's used by xget('wpdim') and xset('wpdim',x,y) in Scilab langage
 **/
void C2F(setpopupdim)(x, y, v3, v4)
     integer *x;
     integer *y;
     integer *v3;
     integer *v4;
{
  /*  int x1= Min((int) *x, ScilabXgc->CWindowWidth);
      int x2= Min((int) *y, ScilabXgc->CWindowHeight);

      GPopupResize(ScilabXgc,&x1,&x2);*/

  GPopupResize(ScilabXgc,x,y);
}
/*-----------------------------------------------------------------------------------*/
/** To change the window view  **/
void C2F(setviewport)(x, y, v3, v4)
     integer *x;
     integer *y;
     integer *v3;
     integer *v4;
{
  if ( sciGetwresize() == 0) 
    SciViewportMove(ScilabXgc,*x,*y);
}
/*-----------------------------------------------------------------------------------*/
/**C2F(getviewport)
 *@description: To get the viewport Upper/Left point Position 
 **/
void C2F(getviewport)(verbose, x, narg,dummy)
     integer *verbose;
     integer *x;
     integer *narg;
     double *dummy;
{     
  *narg = 2;
  if ( sciGetwresize() == 0) 
    {	
      SciViewportGet(ScilabXgc,x,x+1);
    }
  else 
    { 
      x[0]=0;
      x[1]=0;
    }
  if (*verbose == 1) 
    sciprint("\n Viewport position:%d,%d\r\n",(int) x[0],(int) x[1]);
} 
/*-----------------------------------------------------------------------------------*/
/********************************************
 * select window intnum as the current window 
 * window is created if necessary 
 ********************************************/
void C2F(setcurwin)(intnum, v2, v3, v4)
     integer *intnum;
     integer *v2;
     integer *v3;
     integer *v4;
{ 
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
      RECT rect;
      GetClientRect(ScilabXgc->CWindow,&rect);
      ScilabXgc->CWindowWidthView  = rect.right-rect.left;
      ScilabXgc->CWindowHeightView = rect.bottom-rect.top;
      /* ajout pour compatibilite */
      ScilabXgc->CWindowWidth  = ScilabXgc->CWindowWidth;
      ScilabXgc->CWindowHeight = ScilabXgc->CWindowHeight;
    }
}
/*-----------------------------------------------------------------------------------*/
/* used in the previous function to set back the graphic scales 
   when changing form one window to an other 
   Also used in scig_tops : to force a reset of scilab graphic scales 
   after a print in Postscript or Xfig 
*/
int SwitchWindow(integer *intnum)
{
  /** trying to get window *intnum **/
  struct BCG *SXgc;
  integer ierr;
  SXgc = GetWindowXgcNumber(*intnum);
  if ( SXgc != (struct BCG *) 0 ) 
    {
      /** releasing previous hdc **/
      /** wininfo("quit window %d with alu %d\r\n",
	  ScilabXgc->CurWindow,
	  ScilabXgc->CurDrawFunction); 
      **/
      ReleaseWinHdc();
      ScilabXgc = SXgc;
      SetWinhdc();
      ResetScilabXgc ();
      /** wininfo("switching to window %d with alu %d\r\n",*intnum,
	  ScilabXgc->CurDrawFunction); **/
      get_window_scale(*intnum,NULL);
    }
  else 
    {
      /** Create window **/
      C2F(initgraphic)("",intnum,&ierr,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
    }
  return(0);
}
/*-----------------------------------------------------------------------------------*/
/** Get the id number of the Current Graphic Window **/
/**
   Get the id number of the Current Graphic Window 
   In all the other functions we are sure that ScilabXgc exists 
   when we call them ( see sciwin in matdes.f ) 
   exept for this function which is called in sciwin and the previous one 
**/
void C2F(getcurwin)(verbose, intnum, narg,dummy)
     integer *verbose;
     integer *intnum;
     integer *narg;
     double *dummy;
{
  *narg =1 ;
  *intnum = (ScilabXgc != (struct BCG *) 0) ? ScilabXgc->CurWindow : 0;
  if (*verbose == 1) 
    sciprint("\nCurrent Graphic Window :%d\r\n",(int) *intnum);
}
/*-----------------------------------------------------------------------------------*/
/** Set a clip zone (rectangle ) **/
void C2F(setclip)(x, y, w, h)
     integer *x;
     integer *y;
     integer *w;
     integer *h;
{
  ScilabXgc->ClipRegionSet = 1;
  ScilabXgc->CurClipRegion[0]= *x;
  ScilabXgc->CurClipRegion[1]= *y;
  ScilabXgc->CurClipRegion[2]= *w;
  ScilabXgc->CurClipRegion[3]= *h;
  set_current_clip();
}
/*-----------------------------------------------------------------------------------*/
static void set_current_clip()
{
  HRGN hRgn;
  if (ScilabXgc->ClipRegionSet == 0) return;
  if (sciGetwresize() == 1) {
    hRgn = CreateRectRgn(ScilabXgc->CurClipRegion[0],
			 ScilabXgc->CurClipRegion[1],
			 ScilabXgc->CurClipRegion[2] 
			 + ScilabXgc->CurClipRegion[0],
			 ScilabXgc->CurClipRegion[3]  
			 +  ScilabXgc->CurClipRegion[1]);    
  }
  else {
    hRgn = CreateRectRgn(ScilabXgc->CurClipRegion[0] - GetScrollPos(ScilabXgc->CWindow,SB_HORZ),
			 ScilabXgc->CurClipRegion[1] - GetScrollPos(ScilabXgc->CWindow,SB_VERT),
			 ScilabXgc->CurClipRegion[2] 
			 + ScilabXgc->CurClipRegion[0],
			 ScilabXgc->CurClipRegion[3]  
			 +  ScilabXgc->CurClipRegion[1]);    
  }
  SelectClipRgn(hdc, hRgn);
  DeleteObject(hRgn);
}
/*-----------------------------------------------------------------------------------*/
static void set_clip_after_scroll() 
{  
  HRGN hRgn;
  if (ScilabXgc->ClipRegionSet == 1 && sciGetwresize() == 0) 
    {
      hRgn = CreateRectRgn(ScilabXgc->CurClipRegion[0] -  GetScrollPos(ScilabXgc->CWindow,SB_HORZ),
			   ScilabXgc->CurClipRegion[1] - GetScrollPos(ScilabXgc->CWindow,SB_VERT),
			   ScilabXgc->CurClipRegion[2] 
			   + ScilabXgc->CurClipRegion[0],
			   ScilabXgc->CurClipRegion[3]  
			   +  ScilabXgc->CurClipRegion[1]);    
      SelectClipRgn(hdc, hRgn);
      DeleteObject(hRgn);
    }
}
/*-----------------------------------------------------------------------------------*/
/** unset clip zone **/
void C2F(unsetclip)(v1, v2, v3, v4)
     integer *v1;
     integer *v2;
     integer *v3;
     integer *v4;
{
  ScilabXgc->ClipRegionSet = 0;
  SelectClipRgn(hdc,NULL);
}

/** Get the boundaries of the current clip zone **/
void C2F(getclip)(verbose, x, narg,dummy)
     integer *verbose;
     integer *x;
     integer *narg;
     double *dummy;
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
  if (*verbose == 1) {
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
/*-----------------------------------------------------------------------------------*/
/*----------------------------------------------------------
  \encadre{For the drawing functions dealing with vectors of 
  points, the following routine is used to select the mode 
  absolute or relative }
  Absolute mode if *num==0, relative mode if *num != 0
  ------------------------------------------------------------*/
/** to set absolute or relative mode **/
void C2F(setabsourel)(num, v2, v3, v4)
     integer *num;
     integer *v2;
     integer *v3;
     integer *v4;
{
  if (*num == CoordModeOrigin )
    ScilabXgc->CurVectorStyle =  CoordModeOrigin;
  else 
    ScilabXgc->CurVectorStyle =  CoordModePrevious ;
}
/*-----------------------------------------------------------------------------------*/
/** to get information on absolute or relative mode **/
void C2F(getabsourel)(verbose, num, narg,dummy)
     integer *verbose;
     integer *num;
     integer *narg;
     double *dummy;
{
  *narg = 1;
  *num = ScilabXgc->CurVectorStyle  ;
  if (*verbose == 1) {
    if (ScilabXgc->CurVectorStyle == CoordModeOrigin)
      Scistring("\nTrace Absolu");
    else 
      Scistring("\nTrace Relatif");
  }
}
/*-----------------------------------------------------------------------------------*/
/** The alu function for drawing : Works only with X11 **/
/** Not in Postscript **/
/** All the possibilities : Read The X11 manual to get more informations **/
static struct alinfo { 
  char *name;
  int id;
  char *info;} AluStruc_[] =
    { 
      {"GXclear" ,R2_WHITE," 0 "},
      {"GXand" ,R2_MASKPEN," src AND dst "},
      {"GXandReverse" ,R2_MASKPENNOT," src AND NOT dst "},
      {"GXcopy" ,R2_COPYPEN," src "},
      {"GXandInverted" ,R2_MASKNOTPEN," NOT src AND dst "},
      {"GXnoop" ,R2_NOP," dst "},
      {"GXxor" ,R2_XORPEN," src XOR dst "},
      {"GXor" ,R2_MERGEPEN," src OR dst "},
      {"GXnor" ,R2_NOTMERGEPEN," NOT src AND NOT dst "},
      {"GXequiv" ,R2_NOTXORPEN," NOT src XOR dst "},
      {"GXinvert" ,R2_NOT," NOT dst "},
      {"GXorReverse" ,R2_MERGEPENNOT," src OR NOT dst "},
      {"GXcopyInverted" ,R2_NOTCOPYPEN," NOT src "}, 
      {"GXorInverted" ,R2_MERGENOTPEN," NOT src OR dst "},
      {"GXnand" ,R2_NOTMASKPEN," NOT src OR NOT dst "},
      {"GXset" ,R2_BLACK," 1 "}
    };
/*-----------------------------------------------------------------------------------*/
static void idfromname(name1, num)
     char *name1;
     integer *num;
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
/*-----------------------------------------------------------------------------------*/
void C2F(setalufunction)(string)
     char *string;
{     
  integer value;
  idfromname(string,&value);
  if ( value != -1)
    {
      SetROP2(hdc,(int) value);
      ScilabXgc->CurDrawFunction = value;
      set_c(ScilabXgc->CurColor);
    }
}
/*-----------------------------------------------------------------------------------*/
void C2F(setalufunction1)(num, v2, v3, v4)
     integer *num;
     integer *v2;
     integer *v3;
     integer *v4;
{     
  int value;
  ScilabXgc->CurDrawFunction = Min(15,Max(0,*num));
  value=AluStruc_[ScilabXgc->CurDrawFunction].id;
  SetROP2(hdc,(int) value);
  if ( ScilabXgc->CurColorStatus == 1 )
    {
      /** we force here the computation of current color  **/
      /** since the way color are computed changes according **/
      /** to alufunction mode **/
      set_c(ScilabXgc->CurColor);
    }
}
/*-----------------------------------------------------------------------------------*/
void C2F(getalufunction)(verbose, value, narg,dummy)
     integer *verbose;
     integer *value;
     integer *narg;
     double *dummy;
{ 
  *narg =1 ;
  *value = ScilabXgc->CurDrawFunction ;
  if (*verbose ==1 ) 
    { 
      sciprint("\nThe Alufunction is %s -> <%s>\r\n",
	       AluStruc_[*value].name,
	       AluStruc_[*value].info);}
}
/*-----------------------------------------------------------------------------------*/
/** to set the thickness of lines : 0 is a possible value **/
/** give the thinest line (0 and 1 the same for X11 but   **/
/** with diferent algorithms ) **/
/** defaut value is 1 **/
void C2F(setthickness)(integer *value,integer *v2,integer *v3,integer *v4)
{ 
  HPEN hpen=NULL ;
	HPEN hpenOld=NULL ;

  int width =Max(0, *value);
  int style = DashTab[ScilabXgc->CurDashStyle];
  int OS = SciWinGetPlatformId();

  ScilabXgc->CurLineWidth = width ;
  
  if ( ScilabXgc->CurColorStatus == 1 ) 
  {
		COLORREF px = DefaultForeground ;
		if (ScilabXgc->Colors != NULL)
		{
			if ( ScilabXgc->CurDrawFunction !=  GXxor ) px= ScilabXgc->Colors[ScilabXgc->CurColor];
		  else px = ScilabXgc->Colors[ScilabXgc->CurColor] ^ ScilabXgc->Colors[ScilabXgc->NumBackground];
  	}
        
		if(OS == VER_PLATFORM_WIN32_NT)
    {
				if ( (style == PS_SOLID) || (width<=1) )
				{
					hpen = CreatePen(style,ScilabXgc->CurLineWidth,px);
				}
				else
				{
					LOGBRUSH logbrush;
					logbrush.lbStyle = BS_SOLID;
					logbrush.lbColor = px;
					logbrush.lbHatch = 0;
					hpen = ExtCreatePen(PS_GEOMETRIC|PS_ENDCAP_FLAT|style,width,&logbrush,0,NULL);
				}
			  
		}
	  else
		{
			 hpen = CreatePen(PS_SOLID,ScilabXgc->CurLineWidth,px);
		}
	}
  else 
  {
	  if(OS == VER_PLATFORM_WIN32_NT)
	  {
			if ( (style == PS_SOLID) || (width<=1) )
			{
				hpen = CreatePen(style,width,RGB(0,0,0));
			}
			else
			{
				LOGBRUSH logbrush;
				logbrush.lbStyle = BS_SOLID;
				logbrush.lbColor = RGB(0,0,0);
				logbrush.lbHatch = 0;
				hpen = ExtCreatePen(PS_GEOMETRIC|PS_ENDCAP_FLAT|style,width,&logbrush,0,NULL);
			}
	  }
	  else
	  {
        /** warning win95 only uses dot or dash with linewidth <= 1 **/
        width = ( style != PS_SOLID) ? 0 : ScilabXgc->CurLineWidth ;
        hpen = CreatePen(style,width,RGB(0,0,0));
	  }
  }
  hpenOld=SelectObject(hdc,hpen);
  if ( hpenOld ) DeleteObject(hpenOld);
  ScilabXgc->hPen = hpen;
}
/*-----------------------------------------------------------------------------------*/
/** to get the thickness value **/
void C2F(getthickness)(integer *verbose,integer *value,integer *narg,double *dummy)
{
  *narg =1 ;
  *value = ScilabXgc->CurLineWidth ;
  if (*verbose ==1 ) sciprint("\nLine Width:%d\r\n", ScilabXgc->CurLineWidth ) ;
}
/*-----------------------------------------------------------------------------------*/
/** To set grey level for filing areas **/
/** from black (*num =0 ) to white     **/

#define GREYNUMBER 17
/** maybe not so useful ???? XXXX **/

HBRUSH Tabpix_[GREYNUMBER];

static WORD grey0[GREYNUMBER][8]={
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
  {0x00, 0x00, 0x44, 0x00, 0x00, 0x00, 0x44, 0x00},
  {0x00, 0x44, 0x00, 0x22, 0x08, 0x40, 0x01, 0x20},
  {0x00, 0x92, 0x00, 0x25, 0x00, 0x92, 0x00, 0xa4},
  {0x55, 0x00, 0xaa, 0x00, 0x55, 0x00, 0xaa, 0x00},
  {0xad, 0x00, 0x5b, 0x00, 0xda, 0x00, 0x6d, 0x00},
  {0x6d, 0x02, 0xda, 0x08, 0x6b, 0x10, 0xb6, 0x20},
  {0x6d, 0x22, 0xda, 0x0c, 0x6b, 0x18, 0xb6, 0x24},
  {0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa},
  {0x92, 0xdd, 0x25, 0xf3, 0x94, 0xe7, 0x49, 0xdb},
  {0x92, 0xfd, 0x25, 0xf7, 0x94, 0xef, 0x49, 0xdf},
  {0x52, 0xff, 0xa4, 0xff, 0x25, 0xff, 0x92, 0xff},
  {0xaa, 0xff, 0x55, 0xff, 0xaa, 0xff, 0x55, 0xff},
  {0xff, 0x6d, 0xff, 0xda, 0xff, 0x6d, 0xff, 0x5b},
  {0xff, 0xbb, 0xff, 0xdd, 0xf7, 0xbf, 0xfe, 0xdf},
  {0xff, 0xff, 0xbb, 0xff, 0xff, 0xff, 0xbb, 0xff},
  {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
};
/*-----------------------------------------------------------------------------------*/
void C2F(CreatePatterns)()
{ 
  integer i ;
  for ( i=0 ; i < GREYNUMBER ; i++)
    {
      HBITMAP hBitmap;
      hBitmap = CreateBitmap(8,8,1,1,grey0[i]);
      Tabpix_[i] =CreatePatternBrush(hBitmap);
      DeleteObject(hBitmap);
    }
}
/*-----------------------------------------------------------------------------------*/
void C2F(setpattern)(num, v2, v3, v4)
     integer *num;
     integer *v2;
     integer *v3;
     integer *v4;
{ integer i ; 
 if ( ScilabXgc->CurColorStatus == 1 ) 
   {
     set_c(*num-1);
   }
 else 
   {
     i= Max(0,Min(*num-1,GREYNUMBER-1));
     ScilabXgc->CurPattern = i;
     SelectObject(hdc,Tabpix_[i]);
   }
}
/*-----------------------------------------------------------------------------------*/
/** To get the id of the current pattern  **/
void C2F(getpattern)(verbose, num, narg,dummy)
     integer *verbose;
     integer *num;
     integer *narg;
     double *dummy;
{ 
  *narg=1;
  if ( ScilabXgc->CurColorStatus == 1 ) 
    *num = ScilabXgc->CurColor +1 ;
  else 
    *num = ScilabXgc->CurPattern +1 ;
  if (*verbose == 1) sciprint("\n Pattern : %d\r\n",ScilabXgc->CurPattern+1);
}
/*-----------------------------------------------------------------------------------*/
/** To get the id of the last pattern **/
void C2F(getlast)(verbose, num, narg,dummy)
     integer *verbose;
     integer *num;
     integer *narg;
     double *dummy;
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
/*-----------------------------------------------------------------------------------*/
/*--------------------------------------
  \encadre{Line style }
  ---------------------------------------*/

/**  use a table of dashes and set default X11-dash style to **/
/**  one of the possible value. value points **/
/**  to a strictly positive integer **/
/**  if *value == 0 -> Solid line   **/
/**  else Dashed Line **/

/* old version of setdash retained for compatibility */
void C2F(set_dash_or_color)(value, v2, v3, v4)
     integer *value;
     integer *v2;
     integer *v3;
     integer *v4;
{
  if ( ScilabXgc->CurColorStatus == 1) set_c(*value-1);
  else C2F(setdash)(value, v2, v3, v4);
}
/*-----------------------------------------------------------------------------------*/
void C2F(setdash)(integer *value,integer *v2,integer *v3,integer *v4)
{
  static integer l3 ;
  COLORREF col=RGB(0,0,0) ;
  HPEN hpen=NULL;
	HPEN hpenOld=NULL;
  int id=0,width=0;
  int OS = SciWinGetPlatformId();

  l3 = Max(0,Min(MAXDASH - 1,*value - 1));

  if(OS == VER_PLATFORM_WIN32_NT) width = ScilabXgc->CurLineWidth ;
  
  if ( ScilabXgc->CurColorStatus == 1) 
	{
    id = ScilabXgc->CurColor;
    if ( ScilabXgc->CurDrawFunction !=  GXxor ) col = ScilabXgc->Colors[id];
    else col = ScilabXgc->Colors[id] ^ ScilabXgc->Colors[ScilabXgc->NumBackground];
  }
  else col=RGB(0,0,0);
 
  if(OS == VER_PLATFORM_WIN32_NT)
  {
		int style = DashTab[l3];
		if ( (style == PS_SOLID) || (width<=1) )
		{
			hpen = CreatePen(style,width,col); /** warning win95 only uses dot or dash with linewidth <= 1 **/
		}
		else
		{
			LOGBRUSH logbrush;
			logbrush.lbStyle = BS_SOLID;
			logbrush.lbColor = col;
			logbrush.lbHatch = 0;
			hpen = ExtCreatePen(PS_GEOMETRIC|PS_ENDCAP_FLAT|style,width,&logbrush,0,NULL);
		}
	}
  else  hpen = CreatePen(DashTab[l3],width,col); /** warning win95 only uses dot or dash with linewidth <= 1 **/
   
  hpenOld=SelectObject(hdc,hpen);
  if ( hpenOld ) DeleteObject(hpenOld);
  ScilabXgc->hPen = hpen;
  ScilabXgc->CurDashStyle = l3;
}
/*-----------------------------------------------------------------------------------*/
static void C2F(set_dash_and_color)(value, v2, v3, v4)
     integer *value;
     integer *v2;
     integer *v3;
     integer *v4;
{
  C2F(setdash)(value, v2, v3, v4);
  C2F(setpattern)(value+6, v2, v3, v4);
}
/*-----------------------------------------------------------------------------------*/
static void C2F(set_line_style)(value, v2, v3, v4)
     integer *value;
     integer *v2;
     integer *v3;
     integer *v4;
{
  integer j;
  if (ScilabXgc->CurColorStatus == 0) 
    C2F(setdash)(value,PI0,PI0,PI0);
  else {
    j= ScilabXgc->CurDashStyle + 1;
    C2F(setdash)(&j,PI0,PI0,PI0);
    C2F(setpattern)(value,PI0,PI0,PI0);
  }
}
/*-----------------------------------------------------------------------------------*/
/** to get the current dash-style **/
/* old version of getdash retained for compatibility */
void C2F(get_dash_or_color)(verbose, value, narg,dummy)
     integer *verbose;
     integer *value;
     integer *narg;
     double *dummy; 
{

  if ( ScilabXgc->CurColorStatus == 1) 
    {
      *narg =1 ;
      *value =ScilabXgc->CurColor+1;
      if (*verbose == 1) sciprint("Color %d",(int)*value);
      return;
    }
  C2F(getdash)(verbose, value, narg,dummy);
}

void C2F(getdash)(verbose, value, narg,dummy)
     integer *verbose;
     integer *value;
     integer *narg;
     double *dummy; 
{
  *narg =1 ;
  *value =ScilabXgc->CurDashStyle+1;
  if ( *verbose == 1) 
    {
      switch ( *value )
	{
	case 0: Scistring("\nLine style = Line Solid\n"); break ;
	case 1: Scistring("\nLine style = DASH\n"); break ;
	case 2: Scistring("\nLine style = DOT\n"); break ;
	case 3: Scistring("\nLine style = DASHDOT\n"); break ;
	case 4: Scistring("\nLine style = DASHDOTDOT\n"); break ;
	}
    }
}
/*-----------------------------------------------------------------------------------*/
static void C2F(get_dash_and_color)(verbose, value, narg,dummy)
     integer *verbose;
     integer *value;
     integer *narg;
     double *dummy;
{
  /*may be improved replacing 6 by narg */
  C2F(getdash)(verbose, value, narg,dummy);
  C2F(getpattern)(verbose, value+6, narg,dummy);
  *narg = 6;
}
/*-----------------------------------------------------------------------------------*/
/* basculement eventuel de couleur a n&b */
void C2F(usecolor)(num, v1, v2, v3)
     integer *num;
     integer *v1;
     integer *v2;
     integer *v3;
{
  integer i;
  i =  Min(Max(*num,0),1);
  if ( ScilabXgc->CurColorStatus != (int) i) 
    {
      if (ScilabXgc->CurColorStatus == 1) 
	{
	  /* je passe de Couleur a n&b */
	  /* remise des couleurs a vide */
	  ScilabXgc->CurColorStatus = 1;
	  C2F(setpattern)((i=1,&i),PI0,PI0,PI0);
	  /* passage en n&b */
	  ScilabXgc->CurColorStatus = 0;
	  i= ScilabXgc->CurPattern + 1;
	  C2F(setpattern)(&i,PI0,PI0,PI0);
	  i= ScilabXgc->CurDashStyle + 1;
	  C2F(setdash)(&i,PI0,PI0,PI0);
	  ScilabXgc->IDLastPattern = GREYNUMBER - 1;
	}
      else 
	{
	  /* je passe en couleur */
	  /* remise a zero des patterns et dash */
	  /* remise des couleurs a vide */
	  ScilabXgc->CurColorStatus = 0;
	  C2F(setpattern)((i=1,&i),PI0,PI0,PI0);
	  C2F(setdash)((i=1,&i),PI0,PI0,PI0);
	  /* passage en couleur  */
	  ScilabXgc->CurColorStatus = 1;
	  i= ScilabXgc->CurColor + 1;
	  C2F(setpattern)(&i,PI0,PI0,PI0);
	  ScilabXgc->IDLastPattern = ScilabXgc->Numcolors - 1;
	}
    }
}
/*-----------------------------------------------------------------------------------*/
void C2F(getusecolor)(verbose, num, narg,dummy)
     integer *verbose;
     integer *num;
     integer *narg;
     double *dummy;
{
  *num = ScilabXgc->CurColorStatus;
  if (*verbose == 1) 
    sciprint("\n Use color %d\r\n",(int)*num);
  *narg=1;
}
/*-----------------------------------------------------------------------------------*/
/** Change the status of a Graphic Window **/
/** adding or removing a Background Pixmap to it **/
void C2F(setpixmapOn)(num, v2, v3, v4)
     integer *num;
     integer *v2;
     integer *v3;
     integer *v4;
{
  integer num1= Min(Max(*num,0),1);
  if ( sciGetPixmapStatus() == num1 ) return;
  if ( num1 == 1 )
    {
      /** I add a Background Pixmap to the window **/
      C2F(xinfo)("Animation mode is on,( xset('pixmap',0) to leave)",
		 PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
      if ((  ScilabXgc->hdcCompat = CreateCompatibleDC (hdc)) == NULL)
	{
	  sciprint("Setting pixmap on is impossible \r\n");
	  return;
	}
      else
	{
	  HBITMAP hbmTemp ;
	  SetMapMode(ScilabXgc->hdcCompat, MM_TEXT);
	  SetBkMode(ScilabXgc->hdcCompat,TRANSPARENT);
	  SetTextAlign(ScilabXgc->hdcCompat, TA_LEFT|TA_BOTTOM);
	  hbmTemp =CreateCompatibleBitmap (hdc,
					   ScilabXgc->CWindowWidth,
					   ScilabXgc->CWindowHeight);
	  /* ajout */
	  if (!hbmTemp)
	    {
	      sciprint("Setting pixmap on is impossible \r\n");
	      return;
	    }
	  else
	    {
	      HBITMAP  hbmSave;
	      hbmSave = SelectObject ( ScilabXgc->hdcCompat, hbmTemp);
	      if ( ScilabXgc->hbmCompat != NULL)
        	DeleteObject (ScilabXgc->hbmCompat);
	      ScilabXgc->hbmCompat = hbmTemp;
	      ScilabXgc->CurPixmapStatus = 1;
	      C2F(pixmapclear)(PI0,PI0,PI0,PI0); /* background color */
	      /** the create default font/brush etc... in hdc */ 
	      SetGHdc(ScilabXgc->hdcCompat,ScilabXgc->CWindowWidth,
		      ScilabXgc->CWindowHeight);
	      ResetScilabXgc ();
	      SetGHdc((HDC)0,ScilabXgc->CWindowWidth,
		      ScilabXgc->CWindowHeight);
	      /* ajout */
	      C2F(show)(PI0,PI0,PI0,PI0);
	    }
	}
    }
  if ( num1 == 0 )
    {
      /** I remove the Background Pixmap to the window **/
      ScilabXgc->CurPixmapStatus = 0;
      /** XXXX **/
      if ( ScilabXgc->hdcCompat)
	SelectObject (ScilabXgc->hdcCompat, NULL) ;
      if ( ScilabXgc->hbmCompat)
	DeleteObject (ScilabXgc->hbmCompat);
      if ( ScilabXgc->hdcCompat)
	{
	  if ( hdc == ScilabXgc->hdcCompat)
	    hdc=TryToGetDC(ScilabXgc->CWindow);
	  DeleteDC(ScilabXgc->hdcCompat);
	}
      ScilabXgc->hbmCompat = (HBITMAP) 0;
      ScilabXgc->hdcCompat = (HDC) 0;
    }
}
/*-----------------------------------------------------------------------------------*/
void C2F(getpixmapOn)(integer *verbose, integer *value, integer *narg, double *dummy)
{
  *value=sciGetPixmapStatus();
  *narg =1 ;
  if (*verbose == 1) sciprint("Color %d",(int)*value);
}
/*-----------------------------------------------------------------------------------*/
integer sciGetPixmapStatus()
{
  return ScilabXgc->CurPixmapStatus;
}
/*-----------------------------------------------------------------------------------*/
/** Change the status of a Graphic Window **/
/** follow or dont follow the viewport resize  **/
void C2F(setwresize)( integer *num, integer *v2, integer *v3, integer *v4)
{
  integer num1= Min(Max(*num,0),1);
  integer xtmp=0;
  integer ytmp=0;
  if ( ScilabXgc->CurResizeStatus == num1 ) 
    {
      static int count=0; 
      wininfo(" ");
      return;
    }
  ScilabXgc->CurResizeStatus = num1; /* a faire avant setwindowdim */
  C2F(setwindowdim)((xtmp = ScilabXgc->CWindowWidthView, &xtmp),
		    (ytmp=ScilabXgc->CWindowHeightView,&ytmp), PI0, PI0);
  SetViewportOrgEx(hdc,-ScilabXgc->horzsi.nPos,-ScilabXgc->vertsi.nPos,NULL);
  InvalidateRect(ScilabXgc->CWindow,NULL,FALSE);
}
/*-----------------------------------------------------------------------------------*/
void C2F(getwresize)(verbose, value, narg,dummy)
     integer *verbose;
     integer *value;
     integer *narg;
     double *dummy;
{
  *value = sciGetwresize();
  *narg =1 ;
  if (*verbose == 1) sciprint("Resize status %d",(int)*value);
}
/*-----------------------------------------------------------------------------------*/
/*
 * Cette fonction renvoie le status wresize
 */
integer sciGetwresize()
{
  return ScilabXgc->CurResizeStatus;
}
/*-----------------------------------------------------------------------------------*/
static int set_default_colormap_flag = 1;
/*-----------------------------------------------------------------------------------*/
int C2F(sedeco)(flag) 
     int *flag;
{
  set_default_colormap_flag = *flag;
  return(0);
}
/*-----------------------------------------------------------------------------------*/
/* set_default_colormap is called when raising a window for the first 
   timeby xset('window',...) or by getting back to default by 
   xset('default',...) */

void set_default_colormap()
{
  int i,m;
  unsigned long maxcol;
  COLORREF *c = (COLORREF *) NULL;
  float *r = (float *) NULL;
  float *g = (float *) NULL;
  float *b = (float *) NULL;
  /** XXXXX Trouver une doc sur les pallettes **/
  int iPlanes = GetDeviceCaps(hdc,PLANES);
  int iBitsPixel = GetDeviceCaps(hdc,BITSPIXEL);
  /* int numcolors = GetDeviceCaps(hdc,NUMCOLORS_SCI);*/
  /** to avoid overflow in maxcol **/
  /** must be improved for 32bit color display **/
  if ( iBitsPixel > 24 ) iBitsPixel = 24;
  maxcol = 1 << ( iPlanes*iBitsPixel);

  /* we don't want to set the default colormap at window creation 
     if the scilab command was xset("colormap"); */

  if (set_default_colormap_flag == 0) return;
  if ( DEFAULTNUMCOLORS > maxcol) {
    sciprint("No enough colors for default colormap. Maximum is %d\r\n",
	     maxcol);
    return;
  }
  m = DEFAULTNUMCOLORS;

  /* Save old color vectors */
  if(ScilabXgc->Colors != (COLORREF *) NULL) // F.Leray
    c = ScilabXgc->Colors;
  
  if(ScilabXgc->Red != (float *) NULL)
    r = ScilabXgc->Red;
  
  if(ScilabXgc->Green != (float *) NULL)
    g = ScilabXgc->Green;

  if(ScilabXgc->Blue != (float *) NULL)
    b = ScilabXgc->Blue;
  
  if (!XgcAllocColors(ScilabXgc,m)) {
    ScilabXgc->Colors = c;
    ScilabXgc->Red = r;
    ScilabXgc->Green = g;
    ScilabXgc->Blue = b;
    return;
  }

  /* Getting RGB values */
  for (i = 0; i < m; i++) {
    ScilabXgc->Red[i] = ((float)default_colors[3*i])/(float)255.0;
    ScilabXgc->Green[i] = (float)default_colors[3*i+1]/(float)255.0;
    ScilabXgc->Blue[i] = (float)default_colors[3*i+2]/(float)255.0;  
    ScilabXgc->Colors[i] = RGB(default_colors[3*i],
			       default_colors[3*i+1],
			       default_colors[3*i+2]);
  }
  /* Black */
  ScilabXgc->Red[m] = ScilabXgc->Green[m] =   ScilabXgc->Blue[m] =(float) 0;
  ScilabXgc->Colors[m]= RGB(0,0,0);

  /* White */
  ScilabXgc->Red[m+1] =   ScilabXgc->Green[m+1] =   ScilabXgc->Blue[m+1] = (float)1;
  ScilabXgc->Colors[m+1]= RGB(255,255,255);

  ScilabXgc->Numcolors = m;
  ScilabXgc->IDLastPattern = m - 1;
  ScilabXgc->CmapFlag = 1;
  /* Black and white pixels */

  ScilabXgc->NumForeground = m;
  ScilabXgc->NumBackground = m + 1;
  FREE(c); FREE(r); FREE(g); FREE(b);
}
/*-----------------------------------------------------------------------------------*/
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
void C2F(setcolormap)(v1,v2,v3,v4,v5,v6,a)
     integer *v1,*v2;
     integer *v3;
     integer *v4,*v5,*v6;
     double *a;
{
  unsigned long maxcol;
  int iPlanes = GetDeviceCaps(hdc,PLANES);
  int iBitsPixel = GetDeviceCaps(hdc,BITSPIXEL);

  *v3 = 0;

  /** must be improved for 32bit color display **/
  if ( iBitsPixel > 24 ) iBitsPixel = 24;
  maxcol = 1 << ( iPlanes*iBitsPixel);
 
  /** to avoid overflow in maxcol **/
  if (*v2 != 3 || (unsigned long) *v1 > maxcol || *v1 < 0) {
    sciprint("Colormap must be a m x 3 array with m <= %d\r\n",maxcol);
    *v3 = 1;
    return;
  }
  setcolormapg(ScilabXgc,v1,v2,a,v3);
}
/*-----------------------------------------------------------------------------------*/
void C2F(setgccolormap)(v1,v2,a,XGC,v3)
     integer *v1,*v2;
     double *a;
     struct BCG *XGC;
     integer *v3;
{

  setcolormapg(XGC,v1,v2,a,v3);
}
/*-----------------------------------------------------------------------------------*/
void setcolormapg(struct BCG *Xgc,integer *v1, integer *v2, double *a, integer *v3)
{
  int i,palstatus ,m;
  unsigned long maxcol;
  COLORREF  *c = (COLORREF *) NULL;
  float *r = (float *) NULL;
  float *g = (float *) NULL;
  float *b = (float *) NULL;
  /** XXXXX Trouver une doc sur les pallettes **/
  int iPlanes = GetDeviceCaps(hdc,PLANES);
  int iBitsPixel = GetDeviceCaps(hdc,BITSPIXEL);

  if ( ScilabXgc->CurPixmapStatus == 1) 
    hdc = Xgc->hdcCompat;
  else
    hdc=TryToGetDC(Xgc->CWindow); /*?????*/

  /** must be improved for 32bit color display **/
  if ( iBitsPixel > 24 ) iBitsPixel = 24;
  maxcol = 1 << ( iPlanes*iBitsPixel);
  palstatus= (GetDeviceCaps(hdc, RASTERCAPS) & RC_PALETTE);

 
  m = *v1;

  /* Save old color vectors */
  if(Xgc->Colors != (COLORREF *) NULL) // F.Leray
    c = Xgc->Colors;
  
  if(Xgc->Red != (float *) NULL)
    r = Xgc->Red;
  
  if(Xgc->Green != (float *) NULL)
    g = Xgc->Green;

  if(Xgc->Blue != (float *) NULL)
    b = Xgc->Blue;

  if (!XgcAllocColors(Xgc,m)) {
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
    Xgc->Colors[i] = RGB((unsigned short) (255.0*a[i]),
			 (unsigned short) (255.0*a[i+m]),
			 (unsigned short) (255.0*a[i+2*m]));
  }
  /* Black */
  Xgc->Red[m] = Xgc->Green[m] =  Xgc->Blue[m] = (float) 0;
  Xgc->Colors[m]= RGB(0,0,0);

  /* White */
  Xgc->Red[m+1] =  Xgc->Green[m+1] =  Xgc->Blue[m+1] = (float) 0;
  Xgc->Colors[m+1]= RGB(255,255,255);

  Xgc->Numcolors = m;
  Xgc->IDLastPattern = m - 1;
  Xgc->CmapFlag = 0;
  Xgc->NumForeground = m;
  Xgc->NumBackground = m + 1;
  C2F(usecolor)((i=1,&i) ,PI0,PI0,PI0);
  /** we must change the current pattern before the alufunction **/
  C2F(setpattern)((i=Xgc->NumForeground+1,&i),PI0,PI0,PI0);  
  C2F(setalufunction1)(&Xgc->CurDrawFunction,PI0,PI0,PI0);
  C2F(setforeground)((i=Xgc->NumForeground+1,&i),PI0,PI0,PI0);
  C2F(setbackground)((i=Xgc->NumForeground+2,&i),PI0,PI0,PI0);
  FREE(c); FREE(r); FREE(g); FREE(b);
}
/*-----------------------------------------------------------------------------------*/
/*** unfinished : a version with palettes **/
void C2F(pal_setcolormap)(v1,v2,v3,v4,v5,v6,a)
     integer *v1,*v2;
     integer *v3;
     integer *v4,*v5,*v6;
     double *a;
{
  int i,m,maxcol;
  COLORREF  *c = (COLORREF *) NULL;
  float *r = (float *) NULL;
  float *g = (float *) NULL;
  float *b = (float *) NULL;
  int iPlanes = GetDeviceCaps(hdc,PLANES);
  int iBitsPixel = GetDeviceCaps(hdc,BITSPIXEL);

  *v3 = 0;

  /** to avoid overflow in maxcol **/
  /** must be improved for 32bit color display **/
  if ( iBitsPixel > 24 ) iBitsPixel = 24;
  maxcol = 1 << ( iPlanes*iBitsPixel);

  if (*v2 != 3 || *v1 > maxcol || *v1 < 0) {
    sciprint("Colormap must be a m x 3 array with m <= %d\r\n",maxcol);
    *v3 = 1;
    return;
  }
  m = *v1;

  /* Save old color vectors */
  if(ScilabXgc->Colors != (COLORREF *) NULL) // F.Leray
    c = ScilabXgc->Colors;
  
  if(ScilabXgc->Red != (float *) NULL)
    r = ScilabXgc->Red;
  
  if(ScilabXgc->Green != (float *) NULL)
    g = ScilabXgc->Green;

  if(ScilabXgc->Blue != (float *) NULL)
    b = ScilabXgc->Blue;
 
  if (!XgcAllocColors(ScilabXgc,m)) {
    ScilabXgc->Colors = c;
    ScilabXgc->Red = r;
    ScilabXgc->Green = g;
    ScilabXgc->Blue = b;
    *v3 = 1;
    return;
  }

  /* Checking RGB values */
  for (i = 0; i < m; i++) {
    if (a[i] < 0 || a[i] > 1 || a[i+m] < 0 || a[i+m] > 1 ||
	a[i+2*m] < 0 || a[i+2*m]> 1) {
      Scistring("RGB values must be between 0 and 1\n");
      ScilabXgc->Colors = c;
      ScilabXgc->Red = r;
      ScilabXgc->Green = g;
      ScilabXgc->Blue = b;
      *v3 = 1;
      return;
    }
    ScilabXgc->Red[i] = (float)a[i];
    ScilabXgc->Green[i] = (float)a[i+m];
    ScilabXgc->Blue[i] = (float)a[i+2*m];  
    ScilabXgc->Colors[i] = RGB((unsigned short) (255.0*a[i]),
			       (unsigned short) (255.0*a[i+m]),
			       (unsigned short) (255.0*a[i+2*m]));
  }
  /* Black */
  ScilabXgc->Red[m] = ScilabXgc->Green[m] =  ScilabXgc->Blue[m] = (float) 0;
  ScilabXgc->Colors[m]= RGB(0,0,0);

  /* White */
  ScilabXgc->Red[m+1] =  ScilabXgc->Green[m+1] =  ScilabXgc->Blue[m+1] = (float) 0;
  ScilabXgc->Colors[m+1]= RGB(255,255,255);

  if ((GetDeviceCaps(hdc, RASTERCAPS)) & RC_PALETTE )
    {
      if ( SciPalette(m) == FALSE )
	{
	  for (i = 0; i < m; i++) {
	    ScilabXgc->Colors[i] = RGB((unsigned short) (255.0*a[i]),
				       (unsigned short) (255.0*a[i+m]),
				       (unsigned short) (255.0*a[i+2*m]));
	  }
	  ScilabXgc->Colors[m]= RGB(0,0,0);
	  ScilabXgc->Colors[m+1]= RGB(255,255,255);
	}
    }
  ScilabXgc->Numcolors = m;
  ScilabXgc->IDLastPattern = m - 1;
  ScilabXgc->CmapFlag = 0;
  ScilabXgc->NumForeground = m;
  ScilabXgc->NumBackground = m + 1;
  C2F(usecolor)((i=1,&i) ,PI0,PI0,PI0);
  /** we must change the current pattern before the alufunction **/
  C2F(setpattern)((i=ScilabXgc->NumForeground+1,&i),PI0,PI0,PI0);  
  C2F(setalufunction1)(&ScilabXgc->CurDrawFunction,PI0,PI0,PI0);
  C2F(setpattern)((i=ScilabXgc->NumForeground+1,&i),PI0,PI0,PI0);  
  C2F(setforeground)((i=ScilabXgc->NumForeground+1,&i),PI0,PI0,PI0);
  C2F(setbackground)((i=ScilabXgc->NumForeground+2,&i),PI0,PI0,PI0);
  FREE(c); FREE(r); FREE(g); FREE(b);
}
/*-----------------------------------------------------------------------------------*/
static BOOL SciPalette(int iNumClr)
{
  static HPALETTE hPal=NULL;
  LOGPALETTE    *plogPal;
  UINT          uiSizPal;
  INT           i;
  uiSizPal = sizeof(WORD)*2 + sizeof(PALETTEENTRY)*iNumClr;
  if ((plogPal = (LOGPALETTE *) LocalAlloc(LMEM_FIXED,uiSizPal)) == NULL) {
    sciprint("Fail in Allocating palette!\r\n");
    hPal = NULL;
    return FALSE;
  }
  plogPal->palVersion = 0x300;
  plogPal->palNumEntries = (WORD) iNumClr;

  for (i=0; i<iNumClr; i++) 
    {
      plogPal->palPalEntry[i].peRed   =(unsigned char) (255.0* ScilabXgc->Red[i]);
      plogPal->palPalEntry[i].peGreen =(unsigned char) (255.0* ScilabXgc->Green[i]);
      plogPal->palPalEntry[i].peBlue  =(unsigned char) (255.0* ScilabXgc->Blue[i]);
      plogPal->palPalEntry[i].peFlags = PC_RESERVED;
      ScilabXgc->Colors[i]=PALETTERGB(plogPal->palPalEntry[i].peRed,
				      plogPal->palPalEntry[i].peGreen,
				      plogPal->palPalEntry[i].peBlue);
    }

  if ( hPal != (HPALETTE) NULL) DeleteObject(hPal);
  hPal = CreatePalette((LPLOGPALETTE)plogPal);
  if ((hPal) == NULL) {
    sciprint("Fail in creating palette!\r\n");
    return FALSE;
  }
  SelectPalette(hdc, hPal, FALSE);
  RealizePalette(hdc);
  /** UpdateColors(hdc) **/
  GlobalFree(plogPal);
  return TRUE;
}
/*-----------------------------------------------------------------------------------*/
/* getting the colormap size */
void  C2F(getcolormapsize)(integer *verbose, integer *num, integer *narg, double *val)
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

void C2F(getcolormap)(verbose,num,narg,val)
     integer *verbose;
     integer *num;
     integer *narg;
     double *val;
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
/*-----------------------------------------------------------------------------------*/
/** set and get the number of the background or foreground */
void C2F(setbackground)(num, v2, v3, v4)
     integer *num;
     integer *v2;
     integer *v3;
     integer *v4;
{
  if (ScilabXgc->CurColorStatus == 1)
    {
      COLORREF px;
      ScilabXgc->NumBackground = Max(0,Min(*num - 1,ScilabXgc->Numcolors + 1));
      C2F(setalufunction1)(&ScilabXgc->CurDrawFunction,PI0,PI0,PI0);
      px = (ScilabXgc->Colors == NULL) ? DefaultBackground :  ScilabXgc->Colors[ScilabXgc->NumBackground];

      /** A finir XXXX 
	  if (ScilabXgc->Cdrawable != (Drawable) ScilabXgc->CWindow ) 
	  {
	  XSetWindowBackground(dpy, ScilabXgc->CWindow,px);
	  }
      **/
    }
}
/*-----------------------------------------------------------------------------------*/
void C2F(getbackground)(verbose, num, narg,dummy)
     integer *verbose;
     integer *num;
     integer *narg;
     double *dummy;
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
/*-----------------------------------------------------------------------------------*/
/** set and get the number of the background or foreground */
void C2F(setforeground)(num, v2, v3, v4)
     integer *num;
     integer *v2;
     integer *v3;
     integer *v4;
{
  if (ScilabXgc->CurColorStatus == 1)
    {
      COLORREF px;
      ScilabXgc->NumForeground = Max(0,Min(*num - 1,ScilabXgc->Numcolors + 1));
      C2F(setalufunction1)(&ScilabXgc->CurDrawFunction,PI0,PI0,PI0);
      px = (ScilabXgc->Colors == NULL) ? DefaultForeground 
	:  ScilabXgc->Colors[ScilabXgc->NumForeground];
      /** XX inutile **/
    }
}
/*-----------------------------------------------------------------------------------*/
void C2F(getforeground)(verbose, num, narg,dummy)
     integer *verbose;
     integer *num;
     integer *narg;
     double *dummy;
{
  *narg=1;
  if ( ScilabXgc->CurColorStatus == 1 )
    {
      *num = ScilabXgc->NumForeground + 1;
    }
  else
    {
      *num =  1; /** the foreground is a solid line style in b&w */
    }
  if (*verbose == 1)
    sciprint("\n Foreground : %d\r\n",*num);
}
/*-----------------------------------------------------------------------------------*/
/** set and get the number of the hidden3d color */
void C2F(sethidden3d)(num, v2, v3, v4)
     integer *num;
     integer *v2;
     integer *v3;
     integer *v4;
{
  if (ScilabXgc->CurColorStatus == 1)
    {
      /* S Mottelet: Max(-1,... -> Max(-2,...  to take into account the value -1 */
      ScilabXgc->NumHidden3d = Max(-2,Min(*num - 1,ScilabXgc->Numcolors + 1));
    }
}
/*-----------------------------------------------------------------------------------*/
void C2F(gethidden3d)(verbose, num, narg,dummy)
     integer *verbose;
     integer *num;
     integer *narg;
     double *dummy;
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
  if (*verbose == 1) sciprint("\n Hidden3d : %d\r\n",*num);
}
/*-----------------------------------------------------------------------------------*/
/*****************************************************
 * return 1 : if the current window exists 
 *            and its colormap is not the default 
 *            colormap (the number of colors is returned in m
 * else return 0 
 *****************************************************/
int CheckColormap(m)
     int *m;
{
  if (  ScilabXgc != (struct BCG *) 0 )
    {
      *m =  ScilabXgc->Numcolors;
      if ( ScilabXgc->CmapFlag  != 1) 
	return 1;
      else 
	return 0;
    }
  else {
    *m=0;
    return(0);}
}
/*-----------------------------------------------------------------------------------*/
void get_r(i,r) 
     int i;
     float *r;
{
  *r = ScilabXgc->Red[i];
}
/*-----------------------------------------------------------------------------------*/
void get_g(i,g) 
     int i;
     float *g;
{
  *g = ScilabXgc->Green[i];
}
/*-----------------------------------------------------------------------------------*/
void get_b(i,b) 
     float *b;
     int i;
{
  *b = ScilabXgc->Blue[i];
}
/*-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------
  \encadre{general routines accessing the  set<> or get<>
  routines } 
  -------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/
static void InitMissileXgc();
static void SetGraphicsVersion();
/*-----------------------------------------------------------------------------------*/

void C2F(sempty)(verbose, v2, v3, v4)
     integer *verbose;
     integer *v2;
     integer *v3;
     integer *v4;
{
  if ( *verbose ==1 ) Scistring("\n No operation ");
}
/*-----------------------------------------------------------------------------------*/
void C2F(gempty)(verbose, v2, v3,dummy)
     integer *verbose;
     integer *v2;
     integer *v3;
     double *dummy;
{
  if ( *verbose ==1 ) Scistring("\n No operation ");
}
/*-----------------------------------------------------------------------------------*/
#define NUMSETFONC 33
/*-----------------------------------------------------------------------------------*/
/** Table in lexicographic order **/

static struct bgc { char *name ;
  void  (*setfonc )() ;
  void  (*getfonc )() ;}

MissileGCTab_[] = {
  {"alufunction",C2F(setalufunction1),C2F(getalufunction)},
  {"background",C2F(setbackground),C2F(getbackground)},
  {"clipoff",C2F(unsetclip),C2F(getclip)},
  {"clipping",C2F(setclip),C2F(getclip)},
  {"cmap_size",C2F(sempty),C2F(getcolormapsize)},
  {"color",C2F(setpattern),C2F(getpattern)},
  {"colormap",C2F(setcolormap),C2F(getcolormap)},
  {"dashes",C2F(set_dash_or_color),C2F(get_dash_or_color)}, /* obsolet */
  {"default",InitMissileXgc, C2F(gempty)},
  {"figure",C2F(setscilabFigure),C2F(getscilabFigure)},/* NG */
  {"font",C2F(xsetfont),C2F(xgetfont)},
  {"foreground",C2F(setforeground),C2F(getforeground)},
  {"gc",C2F(setscilabxgc),C2F(getscilabxgc)},/* NG */
  {"gccolormap",C2F(setgccolormap),C2F(getcolormap)}, /* NG */
  {"hidden3d",C2F(sethidden3d),C2F(gethidden3d)},
  {"lastpattern",C2F(sempty),C2F(getlast)},
  {"line mode",C2F(setabsourel),C2F(getabsourel)},
  {"line style",C2F(setdash),C2F(getdash)},
  {"mark",C2F(xsetmark),C2F(xgetmark)},
  {"pattern",C2F(setpattern),C2F(getpattern)},
  {"pixmap",C2F(setpixmapOn),C2F(getpixmapOn)},
  {"thickness",C2F(setthickness),C2F(getthickness)},
  {"use color",C2F(usecolor),C2F(getusecolor)},
  {"version",C2F(setscilabVersion),C2F(getscilabVersion)},/* NG */
  {"viewport", C2F(setviewport), C2F(getviewport)},
  {"wdim",C2F(setwindowdim),C2F(getwindowdim)},
  {"white",C2F(sempty),C2F(getlast)},
  {"window",C2F(setcurwin),C2F(getcurwin)},
  {"wpdim",C2F(setpopupdim),C2F(getpopupdim)},
  {"wpos",C2F(setwindowpos),C2F(getwindowpos)},
  {"wresize",C2F(setwresize),C2F(getwresize)},
  {"wshow",C2F(show),C2F(gempty)},
  {"wwpc",C2F(pixmapclear),C2F(gempty)}
};

#ifdef lint 
/*-----------------------------------------------------------------------------------*/
/* pour forcer lint a verifier ca */

static 
test(str,flag,verbose,x1,x2,x3,x4,x5)
     char str[];
     integer flag ;
     integer  *verbose,*x1,*x2,*x3,*x4,*x5;
{ 
  double *dv;
  C2F(setalufunction1)(x1,x2,x3,x4);C2F(getalufunction)(verbose,x1,x2,dv);
  C2F(setclip)(x1,x2,x3,x4);C2F(getclip)(verbose,x1,x2,dv);
  C2F(unsetclip)(x1,x2,x3,x4);C2F(getclip)(verbose,x1,x2,dv);
  C2F(setdash)(x1,x2,x3,x4);C2F(getdash)(verbose,x1,x2,dv);
  InitMissileXgc(x1,x2,x3,x4); C2F(gempty)(verbose,x1,x2,dv);
  C2F(xsetfont)(x1,x2,x3,x4);C2F(xgetfont)(verbose,x1,x2,dv);
  C2F(setabsourel)(x1,x2,x3,x4);C2F(getabsourel)(verbose,x1,x2,dv);
  C2F(xsetmark)(x1,x2,x3,x4);C2F(xgetmark)(verbose,x1,x2,dv);
  C2F(setpattern)(x1,x2,x3,x4);C2F(getpattern)(verbose,x1,x2,dv);
  C2F(setpixmapOn)(x1,x2,x3,x4);C2F(getpixmapOn)(verbose,x1,x2,dv);
  C2F(setthickness)(x1,x2,x3,x4);C2F(getthickness)(verbose,x1,x2,dv);
  C2F(usecolor)(x1,x2,x3,x4);C2F(gempty)(verbose,x1,x2,dv);
  C2F(setwindowdim)(x1,x2,x3,x4);C2F(getwindowdim)(verbose,x1,x2,dv);
  C2F(sempty)(x1,x2,x3,x4);C2F(getwhite)(verbose,x1,x2,dv);
  C2F(setcurwin)(x1,x2,x3,x4);C2F(getcurwin)(verbose,x1,x2,dv);
  C2F(setwindowpos)(x1,x2,x3,x4);C2F(getwindowpos)(verbose,x1,x2,dv);
  C2F(show)(x1,x2,x3,x4);C2F(gempty)(verbose,x1,x2,dv);
  C2F(pixmapclear)(x1,x2,x3,x4);gempty(verbose,x1,x2,dv);
}

#endif 
/*-----------------------------------------------------------------------------------*/
void C2F(MissileGCget)(str, verbose, x1, x2, x3, x4, x5,dv1, dv2, dv3, dv4)
     char *str; 
     integer *verbose;
     integer *x1; integer *x2; integer *x3; integer *x4;
     integer *x5; double *dv1; double *dv2; double *dv3; double *dv4;
{ 
  int x6=0;
  C2F(MissileGCGetorSet)(str,1L,verbose,x1,x2,x3,x4,x5,&x6,dv1);
}
/*-----------------------------------------------------------------------------------*/
void C2F(MissileGCset)(str, x1, x2, x3, x4, x5, x6, dv1, dv2, dv3, dv4)
     char *str;
     integer *x1;
     integer *x2;
     integer *x3;
     integer *x4;
     integer *x5;
     integer *x6;
     double *dv1;
     double *dv2; 
     double *dv3;
     double *dv4;
{
  integer verbose=0 ;
  C2F(MissileGCGetorSet)(str,0L,&verbose,x1,x2,x3,x4,x5,x6,dv1);
}
/*-----------------------------------------------------------------------------------*/
void C2F(MissileGCGetorSet)(char *str,integer flag,integer *verbose,integer *x1,integer *x2,integer *x3,integer *x4,integer *x5,integer *x6,double  *dv1)
{
	integer i ;
	for (i=0; i < NUMSETFONC ; i++)
	{
		integer j;
		j = strcmp(str,MissileGCTab_[i].name);
		if ( j == 0 ) 
		{
			#ifdef _DEBUG
			if (*verbose == 1)	 sciprint("\nGetting Info on %s\r\n",str);
			#endif
			if (flag == 1)	 (MissileGCTab_[i].getfonc)(verbose,x1,x2,dv1);
			else  (MissileGCTab_[i].setfonc)(x1,x2,x3,x4,x5,x6,dv1);
			return;
		}
		else 
		{
			if ( j <= 0)
			{
				sciprint("\nUnknow X operator <%s>\r\n",str);
				*x1=1;*x2=0;
				return;
			}
		}
	}
	sciprint("\n Unknow X operator <%s>\r\n",str);
	*x1=1;
	*x2=0;
}
/*-----------------------------------------------------------------------------------*/
/*-------------------------------------------------------
  \section{Functions for drawing}
  ---------------------------------------------------------*/

/*----------------------------------------------------
  \subsection{String display}

  \encadre{display of a string
  at (x,y) position whith slope angle alpha in degree . 
  Angle are given clockwise. 
  If *flag ==1 and angle is z\'ero a framed box is added 
  around the string}.
  -----------------------------------------------------*/

void C2F(displaystring)(string, x, y, v1, flag, v6, v7, angle, dv2, dv3, dv4)
     char *string;
     integer *x,*y,*v1,*flag,*v6,*v7;
     double *angle,*dv2,*dv3,*dv4;
{ 
  if ( Abs(*angle) <= 0.1) 
    {
      if ( ScilabXgc->CurDrawFunction ==  GXxor )
	{
	  SIZE size ;
	  GetTextExtentPoint32(hdc,string,strlen(string),&size);
	  XorString(*x,*y,string,size.cx,size.cy);
	}
      else
	{
	  SetBkMode(hdc, TRANSPARENT); 
	  TextOut(hdc,(int) *x,(int) *y,string,strlen(string));
	  SetBkMode(hdc, OPAQUE); 
	}
      if ( *flag == 1) 
	{
	  integer rect[4];
	  C2F(boundingbox)(string,x,y,rect,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
	  C2F(drawrectangle)(string,rect,rect+1,rect+2,rect+3,PI0,PI0,PD0,PD0,PD0,PD0);
	}
    }
  else 
    {
      C2F(DispStringAngle)(x,y,string,angle);
    }
}

/* F.Leray 22/06/05 */
/* Move here the font structure definition */
/* I need the definition of the font structure fonttab now to create new rotated fonts */

/***********************************
 * Fonts for the graphic windows 
 ***********************************/

#define FONTNUMBER 10 // F.Leray FONTNUMBER set to 10 pb not solved: font Id 9 has always the same size
                      // and looks like font fId 8 Size 5... Pb here F.Leray 25.04.02
#define FONTMAXSIZE 6
#define SYMBOLNUMBER 10

/* structure to keep track of fonts 
   Dans FontInfoTab : on se garde des information sur les 
   fonts la fonts i a pour nom fname et ok vaut 1 si 
   elle a ete chargee ds le serveur 
   c'est loadfamily qui se charge de charger une font a diverses 
   taille ds le serveur.
   The font i at size fsiz is stored at position FontInfoTab[i].hf[fsiz]
*/

typedef struct tagFontInfo { 
  integer ok;
  char fname[100];
  HFONT hf[FONTMAXSIZE];
} FontInfoT[FONTNUMBER];

static int scale_font_size = 1;
static FontInfoT FontInfoTab;           /** for screen **/ 
static FontInfoT FontInfoTabPrinter;    /** for printer **/ 

static FontInfoT *FontTab = &FontInfoTab;

static char *size_[] = { "08" ,"10","12","14","18","24"};
static int size_n_[] = {8,10,12,14,18,24};

/** We use the Symbol font  for mark plotting **/
/** so we want to be able to center a Symbol character at a specified point **/

typedef  struct { integer xoffset[FONTMAXSIZE][SYMBOLNUMBER];
  integer yoffset[FONTMAXSIZE][SYMBOLNUMBER];} Offset ;
static Offset ListOffset;
static Offset ListOffsetPrint;
static Offset *SymbOffset = &ListOffset;

static char Marks[] = {
  /*., +,X,*,diamond(filled),diamond,triangle up,triangle down,trefle,circle*/
  (char)46,(char)43,(char)180,(char)42, (char)168,(char)224,
  (char)196,(char)209,(char)167,(char)176,};

/** To set the current font id  and size **/
/** load the fonts into X11 if necessary **/

typedef  struct  {
  char *alias;  char *name;  char *Winname;
}  FontAlias;

/** ce qui suit marche sur 75dpi ou 100dpi **/

FontAlias fonttab[] ={
  {"CourR", "-adobe-courier-medium-r-normal--*-%s0-*-*-m-*-iso8859-1","Courier New"},
  {"Symb", "-adobe-symbol-medium-r-normal--*-%s0-*-*-p-*-adobe-fontspecific","Symbol"},
  {"TimR", "-adobe-times-medium-r-normal--*-%s0-*-*-p-*-iso8859-1","Times New Roman"},
  {"TimI", "-adobe-times-medium-i-normal--*-%s0-*-*-p-*-iso8859-1","Times New Roman Italic"},
  {"TimB", "-adobe-times-bold-r-normal--*-%s0-*-*-p-*-iso8859-1","Times New Roman Bold"},
  {"TimBI", "-adobe-times-bold-i-normal--*-%s0-*-*-p-*-iso8859-1","Times New Roman Bold Italic"},
  {"HelvR", "-adobe-helvetica-medium-r-normal--*-%s0-*-*-p-*-iso8859-1","Arial"},
  {"HelvO", "-adobe-helvetica-medium-o-normal--*-%s0-*-*-p-*-iso8859-1","Arial Italic"},
  {"HelvB", "-adobe-helvetica-bold-r-normal--*-%s0-*-*-p-*-iso8859-1", "Arial Bold"},
  {"HelvBO","-adobe-helvetica-bold-o-normal--*-%s0-*-*-p-*-iso8859-1", "Arial Bold Italic"},
  {(char *) NULL,( char *) NULL}
};

/* F.Leray: the algo. changed. to allow nicer rotation for the font characters string */
/* For more explanations, see http://msdn.microsoft.com/library/default.asp?url=/library/en-us/gdi/fontext_2tf8.asp */
/*-----------------------------------------------------------------------------------*/
void C2F(DispStringAngle)(x0, yy0, string, angle)
     integer *x0;
     integer *yy0;
     char *string;
     double *angle;
{

  HFONT hfnt, hfntPrev;
  size_t pcch;
  LOGFONT lf;
  char *p;
  
 memset(&lf, 0, sizeof(LOGFONT));
 strncpy(lf.lfFaceName,(*FontTab)[ScilabXgc->FontId].fname,LF_FACESIZE);

 lf.lfHeight  = - size_n_[ScilabXgc->FontSize]*scale_font_size;
 lf.lfCharSet = DEFAULT_CHARSET;
 if ( (p = strstr((*FontTab)[ScilabXgc->FontId].fname," Italic")) != (LPSTR)NULL ) {
   lf.lfFaceName[ (unsigned int)(p- (*FontTab)[ScilabXgc->FontId].fname) ] = '\0';
   lf.lfItalic = TRUE;
 }
 if ( (p = strstr((*FontTab)[ScilabXgc->FontId].fname," Bold")) != (LPSTR)NULL ) {
   lf.lfFaceName[ (unsigned int)(p- (*FontTab)[ScilabXgc->FontId].fname) ] = '\0';
   lf.lfWeight = FW_BOLD;
 }
   
 SetBkMode(hdc, TRANSPARENT); 
 
 /* hfnt = CreateFontIndirect((LOGFONT FAR *)&lf);*/
 
 lf.lfEscapement = (int) (-(*angle)*10);

 hfnt = CreateFontIndirect((LOGFONT FAR *)&lf); 
 hfntPrev = SelectObject(hdc, hfnt);
	
 pcch = strlen(string);

 TextOut(hdc, *x0, *yy0, string, pcch); 
 SelectObject(hdc, hfntPrev); 

 DeleteObject(hfnt); 
 
 // Reset the background mode to its default. 
 
 SetBkMode(hdc, OPAQUE); 
 
}
/*-----------------------------------------------------------------------------------*/
int XorString(x,y,string,fWidth,fHeight)
     integer x,y;
     char *string;
     int fWidth,fHeight;
{
  COLORREF col ;
  /** HPEN hpenOld;
      HBRUSH hbrushOld; **/
  HFONT hfont,hfontOld;
  HDC hdcMem;
  HBITMAP hbitmap, hbitmapOld;
  hfont=getcurfont();
  hdcMem = CreateCompatibleDC (hdc);
  if (hdcMem) {
    hbitmap = CreateCompatibleBitmap (hdc,fWidth,fHeight);
    if (hbitmap) {
      SetMapMode(hdcMem, MM_TEXT);
      SetBkMode(hdcMem,TRANSPARENT);
      SetTextAlign(hdcMem, TA_LEFT|TA_BOTTOM);
      hbitmapOld = SelectObject (hdcMem, hbitmap);
      BitBlt (hdcMem, 0, 0,fWidth,fHeight, NULL, 0, 0, WHITENESS);
      /** unused : 
	  hpenOld=SelectObject(hdcMem,ScilabXgc->hPen);
	  hbrushOld=SelectObject(hdcMem,ScilabXgc->hBrush); 
      **/
      hfontOld=SelectObject(hdcMem,hfont);
      if (ScilabXgc->Colors != NULL) 
	{
	  /** see set_c **/
	  col = ScilabXgc->Colors[ScilabXgc->CurColor];
	  if ( ScilabXgc->CurDrawFunction !=  GXxor )
	    col = col ^ ScilabXgc->Colors[ScilabXgc->NumBackground];
	  SetTextColor(hdcMem,col); 
	}
      if (TextOut (hdcMem,0,fHeight,string,strlen(string)))
	{
	  /** see raster ops in VC++ **/
	  BitBlt(hdc, x,y-fHeight,fWidth,fHeight,hdcMem,0,0,0x990066);
	} else {
	  MessageBox (GetFocus(),
		      "Unable to perform TextOut", "DisplayGlyph", MB_OK);
	}
      SelectObject (hdcMem, hbitmapOld);
      /** SelectObject (hdcMem, hpenOld);
	  SelectObject (hdcMem, hbrushOld); **/
      SelectObject (hdcMem, hfontOld);
      DeleteObject (hbitmap);
    } else {
      MessageBox (GetFocus(), "Unable To create Bitmap", "DisplayGlyph", MB_OK);
    }
    DeleteDC (hdcMem);
  } else {
    MessageBox (GetFocus(), "Unable to create DC", "DisplayGlyph", MB_OK);
  }
  return 0;
}

/*-----------------------------------------------------------------------------------*/
/** To get the bounding rectangle of a string **/

void C2F(boundingbox)(string, x, y, rect, v5, v6, v7, dv1, dv2, dv3, dv4)
     char *string;
     integer *x,*y,*rect,*v5,*v6,*v7;
     double *dv1,*dv2,*dv3,*dv4;
{ 
  SIZE size ;
  /** text mode is supposed to be bottom  **/
  GetTextExtentPoint32(hdc,(LPCTSTR)string,(int)strlen(string),&size);
  rect[0]= *x ;
  rect[1]= *y - size.cy ;
  rect[2]= size.cx;
  rect[3]= size.cy;
}
/*-----------------------------------------------------------------------------------*/
/*------------------------------------------------
  subsection{ Segments and Arrows }
  -------------------------------------------------*/

void C2F(drawline)(x1, yy1, x2, y2)
     integer *x1;
     integer *yy1;
     integer *x2;
     integer *y2;
{
   
  MoveToEx(hdc,(int) *x1,(int) *yy1,NULL);
  LineTo(hdc,(int) *x2,(int) *y2);
  
}
/*-----------------------------------------------------------------------------------*/
/** Draw a set of segments **/
/** segments are defined by (vx[i],vy[i])->(vx[i+1],vy[i+1]) **/
/** for i=0 step 2 **/
/** n is the size of vx and vy **/

void C2F(drawsegments)(str, vx, vy, n, style, iflag, v7, dv1, dv2, dv3, dv4)
     char *str;
     integer *vx;
     integer *vy;
     integer *n;
     integer *style;
     integer *iflag;
     integer *v7;
     double *dv1;
     double *dv2;
     double *dv3;
     double *dv4;
{
  integer verbose=0,Dnarg,Dvalue[10],NDvalue;
  integer i ;

  C2F(get_dash_and_color)(&verbose,Dvalue,&Dnarg,vdouble);


  if ( (int) *iflag == 1) { /* one style per segment */
    for (i=0 ; i < *n/2 ; i++) {
      NDvalue = style[i];
      C2F(set_line_style)(&NDvalue,PI0,PI0,PI0);
      MoveToEx(hdc,(int) vx[2*i],(int) vy[2*i],NULL);
      LineTo(hdc,(int) vx[2*i+1],(int) vy[2*i+1]);
    }
  }
  else {
    if (*style >= 1) /* set color */
      C2F(set_line_style)(style,PI0,PI0,PI0);

    for (i=0 ; i < *n/2 ; i++) {
      MoveToEx(hdc,(int) vx[2*i],(int) vy[2*i],NULL);
      LineTo(hdc,(int) vx[2*i+1],(int) vy[2*i+1]);
    }
  }
  C2F(set_dash_and_color)( Dvalue,PI0,PI0,PI0);
}
/*-----------------------------------------------------------------------------------*/
/** Draw a set of arrows **/
/** arrows are defined by (vx[i],vy[i])->(vx[i+1],vy[i+1]) **/
/** for i=0 step 2 **/
/** n is the size of vx and vy **/
/** as is 10*arsize (arsize) the size of the arrow head in pixels **/

void C2F(drawarrows)(str, vx, vy, n, as, style, iflag, dv1, dv2, dv3, dv4)
     char *str;
     integer *vx;
     integer *vy;
     integer *n;
     integer *as;
     integer *style;
     integer *iflag;
     double *dv1;
     double *dv2;
     double *dv3;
     double *dv4;
{ 
  integer verbose=0,Dnarg,Dvalue[10],NDvalue,i;
  double cos20=cos(20.0*M_PI/180.0);
  double sin20=sin(20.0*M_PI/180.0);
  integer polyx[4],polyy[4];
  C2F(get_dash_and_color)(&verbose,Dvalue,&Dnarg,vdouble);
  for (i=0 ; i < *n/2 ; i++)
    { 
      double dx,dy,norm;
      if ( (int) *iflag == 1) 
	NDvalue = style[i];
      else 
	NDvalue=(*style < 1) ?  Dvalue[0] : *style;
      C2F(set_line_style)(&NDvalue,PI0,PI0,PI0);
      
      dx=( vx[2*i+1]-vx[2*i]);
      dy=( vy[2*i+1]-vy[2*i]);
      norm = sqrt(dx*dx+dy*dy);
      if ( Abs(norm) >  SMDOUBLE ) 
	{ integer nn=1,p=3;
	dx=(*as/10.0)*dx/norm;dy=(*as/10.0)*dy/norm;
	polyx[0]= polyx[3]=vx[2*i+1];/* correction bug 607 (bruno 18 nov 2004) */
	polyx[1]= inint(polyx[0]  - cos20*dx -sin20*dy );
	polyx[2]= inint(polyx[0]  - cos20*dx + sin20*dy);
	polyy[0]= polyy[3]=vy[2*i+1];/* correction bug 607 (bruno 18 nov 2004) */
	polyy[1]= inint(polyy[0] + sin20*dx -cos20*dy) ;
	polyy[2]= inint(polyy[0] - sin20*dx - cos20*dy) ;
	C2F(fillpolylines)("v",polyx,polyy,&NDvalue, &nn,&p,PI0,PD0,PD0,PD0,PD0);
	}
      MoveToEx(hdc,(int) vx[2*i],(int) vy[2*i],NULL);
      LineTo(hdc,(int) (vx[2*i+1]-dx*cos20),(int) (vy[2*i+1]-dy*cos20));
      

    }
  C2F(set_dash_and_color)( Dvalue,PI0,PI0,PI0);
}
/*-----------------------------------------------------------------------------------*/
/** Draw or fill a set of rectangle **/
/** rectangle i is specified by (vect[i],vect[i+1],vect[i+2],vect[i+3]) **/
/** for x,y,width,height **/
/** for i=0 step 4 **/
/** (*n) : number of rectangles **/
/** fillvect[*n] : specify the action  **/
/** if fillvect[i] is > 0 then fill the rectangle i **/
/** if fillvect[i] is == 0  then only draw the rectangle i **/
/**                         with the current drawing style **/
/** if fillvect[i] is < 0 then draw the  rectangle with -fillvect[i] **/

void C2F(drawrectangles)(str, vects, fillvect, n, v5, v6, v7, dv1, dv2, dv3, dv4)
     char *str;
     integer *vects;
     integer *fillvect;
     integer *n;
     integer *v5;
     integer *v6;
     integer *v7;
     double *dv1;
     double *dv2;
     double *dv3;
     double *dv4;
{
  integer i,cpat,verbose=0,num,cd[10];
  C2F(getpattern)(&verbose,&cpat,&num,vdouble);

  C2F(get_dash_and_color)(&verbose,cd,&num,vdouble);
  for (i=0 ; i< *n ; i++)
    {
      if (fillvect[i] < 0)
	{
	  int dash = - fillvect[i];
	  C2F(set_line_style)(&dash,PI0,PI0,PI0);
	  C2F(drawrectangle)(str,vects+4*i,vects+4*i+1,vects+4*i+2,vects+4*i+3
			     ,PI0,PI0,PD0,PD0,PD0,PD0);
	}
      else if (fillvect[i] == 0)
	{
	  /* C2F(set_line_style)(&cd,PI0,PI0,PI0);*/
	  C2F(drawrectangle)(str,vects+4*i,vects+4*i+1,vects+4*i+2,vects+4*i+3
			     ,PI0,PI0,PD0,PD0,PD0,PD0);
	}
      else
	{
	  C2F(setpattern)(&(fillvect[i]),PI0,PI0,PI0);
	  C2F(fillrectangle)(str,vects+4*i,vects+4*i+1,vects+4*i+2,vects+4*i+3,PI0,PI0,PD0,PD0,PD0,PD0);
	}
    }
  C2F(set_dash_and_color)(&(cd[0]),PI0,PI0,PI0);
}
/*-----------------------------------------------------------------------------------*/
/** Draw one rectangle with current line style **/

void C2F(drawrectangle)(str, x, y, width, height, v6, v7, dv1, dv2, dv3, dv4)
     char *str;
     integer *x;
     integer *y;
     integer *width;
     integer *height;
     integer *v6;
     integer *v7;
     double *dv1;
     double *dv2;
     double *dv3;
     double *dv4;
{ 
  SelectObject(hdc,GetStockObject(NULL_BRUSH));
  /* +1 added for correct 2d axis necessary !! **/
  Rectangle(hdc,(int) *x,(int) *y,(int) *width+*x+1 ,(int) *height+*y+1);
  if ( ScilabXgc->hBrush != (HBRUSH) 0) 
    SelectObject(hdc,ScilabXgc->hBrush);
}
/*-----------------------------------------------------------------------------------*/
/** fill one rectangle, with current pattern **/

void C2F(fillrectangle)(str, x, y, width, height, v6, v7, dv1, dv2, dv3, dv4)
     char *str;
     integer *x;
     integer *y;
     integer *width;
     integer *height;
     integer *v6;
     integer *v7;
     double *dv1;
     double *dv2;
     double *dv3;
     double *dv4;
{ 
  /** Rectangle with current pen and brush **/
  Rectangle(hdc,(int) *x,(int) *y,(int) *width + *x+1 ,(int) *height + *y+1 );
}
/*-----------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------
 * draw a set of rectangles, provided here to accelerate GraySquare for X11 device 
 *  x : of size n1 gives the x-values of the grid 
 *  y : of size n2 gives the y-values of the grid 
 *  z : is the value of a function on the grid defined by x,y 
 *  on each rectangle the average value of z is computed 
 *----------------------------------------------------------------------------------*/

void fill_grid_rectangles(x, y, z, n1, n2)
     integer x[],y[];
     double *z;
     integer n1,n2;
{
  double zmoy,zmax,zmin,zmaxmin;
  integer i,j,verbose=0,whiteid,narg,fill[1],cpat,xz[2];
  int flag;

  zmin=Mini(z,(n1)*(n2));
  zmax=Maxi(z,(n1)*(n2));
  zmaxmin=zmax-zmin;
  if (zmaxmin <= SMDOUBLE) zmaxmin=SMDOUBLE;
  /* WARNING: this routine is provided here to accelerate 
   * multiple rectangles drawing 
   * since it is not called the usual way i.e through 
   * dr we must add here SetWinhdc and ReleaseWinHdc 
   */ 
  flag=MaybeSetWinhdc();
  C2F(getlast)(&verbose,&whiteid,&narg,vdouble);
  C2F(getpattern)(&verbose,&cpat,&narg,vdouble);
  C2F(getwindowdim)(&verbose,xz,&narg,vdouble);

  for (i = 0 ; i < (n1)-1 ; i++)
    for (j = 0 ; j < (n2)-1 ; j++)
      {
	integer w,h;
	zmoy=1/4.0*(z[i+n1*j]+z[i+n1*(j+1)]+z[i+1+n1*j]+z[i+1+n1*(j+1)]);
	fill[0]=1 + inint((whiteid-1)*(zmoy-zmin)/(zmaxmin));
	C2F(setpattern)(fill,PI0,PI0,PI0);
        w=Abs(x[i+1]-x[i]);h=Abs(y[j+1]-y[j]);
	/* We don't trace rectangle which are totally out **/
	if ( w != 0 && h != 0 && x[i] < xz[0] && y[j+1] < xz[1] && x[i]+w > 0 && y[j+1]+h > 0 )
	  Rectangle(hdc,(int) x[i],(int) y[j+1],(int) w + x[i]+1 ,(int) h + y[j+1]+1 );
      }
  C2F(setpattern)(&cpat,PI0,PI0,PI0);
  if ( flag == 1) ReleaseWinHdc();
}
/*-----------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------
 * draw a set of rectangles, provided here to accelerate GraySquare1 for X11 device 
 *  x : of size n1 gives the x-values of the grid 
 *  y : of size n2 gives the y-values of the grid 
 *  z : of size (n1-1)*(n2-1)  gives the f-values on the middle 
 *  of each rectangle. 
 *  z[i,j] is the value on the middle of rectangle 
 *        P1= x[i],y[j] x[i+1],y[j+1]
 *----------------------------------------------------------------------------------*/

void fill_grid_rectangles1(x, y, z, n1, n2)
     integer *x;
     integer *y;
     double *z;
     integer n1;
     integer n2;
{
  integer i,j,verbose=0,narg,fill[1],cpat,xz[2];
  int flag;

  /* WARNING: this routine is provided here to accelerate 
   * multiple rectangles drawing 
   * since it is not called the usual way i.e through 
   * dr we must add here SetWinhdc and ReleaseWinHdc 
   */ 
  flag=MaybeSetWinhdc();
  C2F(getpattern)(&verbose,&cpat,&narg,vdouble);
  C2F(getwindowdim)(&verbose,xz,&narg,vdouble);
  for (i = 0 ; i < (n1)-1 ; i++)
    for (j = 0 ; j < (n2)-1 ; j++)
      {
	integer w,h;
	fill[0]= (int) z[i+(n1-1)*j];
	C2F(setpattern)(fill,PI0,PI0,PI0);
	w=Abs(x[j+1]-x[j]);
	h=Abs(y[i+1]-y[i]);
	/* We don't trace rectangle which are totally out **/
	if ( w != 0 && h != 0 && x[j] < xz[0] && y[i] < xz[1] && x[j]+w > 0 && y[i]+h > 0 )
	  Rectangle(hdc,(int) x[j],(int) y[i],(int) w + x[j]+1 ,(int) h + y[i]+1 );
      }
  C2F(setpattern)(&cpat,PI0,PI0,PI0);
  if ( flag == 1) ReleaseWinHdc();
}
/*-----------------------------------------------------------------------------------*/
/*----------------------
  \subsection{Circles and Ellipsis }
  ------------------------*/
/** Draw or fill a set of ellipsis or part of ellipsis **/
/** Each is defined by 6-parameters, **/
/** ellipsis i is specified by $vect[6*i+k]_{k=0,5}= x,y,width,height,angle1,angle2$ **/
/** <x,y,width,height> is the bounding box **/
/** angle1,angle2 specifies the portion of the ellipsis **/
/** caution : angle=degreangle*64          **/
/** if fillvect[i] is in [0,whitepattern] then  fill the ellipsis i **/
/** with pattern fillvect[i] **/
/** if fillvect[i] is > whitepattern  then only draw the ellipsis i **/
/** The drawing style is the current drawing **/

void C2F(fillarcs)(str, vects, fillvect, n, v5, v6, v7, dv1, dv2, dv3, dv4)
     char *str;
     integer *vects;
     integer *fillvect;
     integer *n;
     integer *v5;
     integer *v6;
     integer *v7;
     double *dv1;
     double *dv2;
     double *dv3;
     double *dv4;
{
  integer i,cpat,verb,num;
  verb=0;
  C2F(getpattern)(&verb,&cpat,&num,vdouble);
  for (i=0 ; i< *n ; i++)
    {
      if (fillvect[i] > ScilabXgc->IDLastPattern + 1)
	{
	  C2F(setpattern)(&(cpat),PI0,PI0,PI0);
	  C2F(drawarc)(str,vects+6*i,vects+6*i+1,
		       vects+6*i+2,vects+6*i+3,
		       vects+6*i+4,vects+6*i+5,PD0,PD0,PD0,PD0);
	}
      else
	{
	  C2F(setpattern)(&(fillvect[i]),PI0,PI0,PI0);
	  C2F(fillarc)(str,vects+6*i,vects+6*i+1,
		       vects+6*i+2,vects+6*i+3,
		       vects+6*i+4,vects+6*i+5,PD0,PD0,PD0,PD0);
	}
    }
  C2F(setpattern)(&(cpat),PI0,PI0,PI0);
}

/*-----------------------------------------------------------------------------------*/
/** Draw a set of ellipsis or part of ellipsis **/
/** Each is defined by 6-parameters, **/
/** ellipsis i is specified by $vect[6*i+k]_{k=0,5}= x,y,width,height,angle1,angle2$ **/
/** <x,y,width,height> is the bounding box **/
/** angle1,angle2 specifies the portion of the ellipsis **/
/** caution : angle=degreangle*64          **/

void C2F(drawarcs)(str, vects, style, n, v5, v6, v7, dv1, dv2, dv3, dv4)
     char *str;
     integer *vects;
     integer *style;
     integer *n;
     integer *v5;
     integer *v6;
     integer *v7;
     double *dv1;
     double *dv2;
     double *dv3;
     double *dv4;
{
  integer verbose=0,Dnarg,Dvalue[10],NDvalue,i;
  /* store the current values */
  C2F(get_dash_and_color)(&verbose,Dvalue,&Dnarg,vdouble);
  for (i=0 ; i< *n ; i++)
    {
      NDvalue = style[i];
      C2F(set_line_style)(&NDvalue,PI0,PI0,PI0);
      C2F(drawarc)(str,vects+6*i,vects+6*i+1,
		   vects+6*i+2,vects+6*i+3,
		   vects+6*i+4,vects+6*i+5,PD0,PD0,PD0,PD0);
    }
  C2F(set_dash_and_color)( Dvalue,PI0,PI0,PI0);
}
/*-----------------------------------------------------------------------------------*/
/** Draw a single ellipsis or part of it **/

void C2F(drawarc)(str, x, y, width, height, angle1, angle2, dv1, dv2, dv3, dv4)
     char *str;
     integer *x, *y, *width,*height, *angle1, *angle2;
     double *dv1,*dv2,*dv3,*dv4;
{ 
  int xmid= *x + *width/2;
  int ymid= *y + *height/2;
  int lg= Max(*width,*height);
  SelectObject(hdc,GetStockObject(NULL_BRUSH));
  /** Ellipse(hdc,(int) *x,(int) *y,(int) *width + *x ,(int) *height + *y ); **/
  Arc(hdc,(int) *x,(int) *y,(int) *width + *x ,(int) *height + *y, 
      xmid + (int) (lg*cos(*angle1*M_PI/11520.00)),
      ymid - (int) (lg*sin(*angle1*M_PI/11520.00)),
      xmid + (int)  (lg*cos((*angle1+*angle2)*M_PI/11520.00)),
      ymid - (int)(lg*sin((*angle1+*angle2)*M_PI/11520.00))); /** 180*64 **/
  if ( ScilabXgc->hBrush != (HBRUSH) 0) SelectObject(hdc,ScilabXgc->hBrush);
}
/*-----------------------------------------------------------------------------------*/
/** Fill a single elipsis or part of it with current pattern **/
void C2F(fillarc)(str, x, y, width, height, angle1, angle2, dv1, dv2, dv3, dv4)
     char *str;
     integer *x, *y, *width,*height, *angle1, *angle2;
     double *dv1,*dv2,*dv3,*dv4;
{
  int xmid= *x + *width/2;
  int ymid= *y + *height/2;
  int lg= Max(*width,*height);
  /**Ellipse(hdc,(int) *x,(int) *y,(int)*width+*x ,(int)*height+*y ); **/
  Pie(hdc,(int) *x,(int) *y,(int) *width + *x ,(int) *height + *y, 
      xmid +(int)  (lg*cos(*angle1*M_PI/11520.00)),
      ymid - (int) (lg*sin(*angle1*M_PI/11520.00)),
      xmid+(int) (lg*cos((*angle1+*angle2)*M_PI/11520.00)),
      ymid - (int) (lg*sin((*angle1+*angle2)*M_PI/11520.00))); /** 180*64 **/
}
/*-----------------------------------------------------------------------------------*/
/*--------------------------------------------------------------
  \encadre{Filling or Drawing Polylines and Polygons}
  ---------------------------------------------------------------*/

/** Draw a set of (*n) polylines (each of which have (*p) points) **/
/** with lines or marks **/
/** drawvect[i] >= 0 use a mark for polyline i **/
/** drawvect[i] < 0 use a line style for  i **/

void C2F(drawpolylines)(str, vectsx, vectsy, drawvect, n, p, v7, dv1, dv2, dv3, dv4)
     char *str;
     integer *vectsx;
     integer *vectsy;
     integer *drawvect;
     integer *n;
     integer *p;
     integer *v7;
     double *dv1;
     double *dv2;
     double *dv3;
     double *dv4;
{ integer verbose=0 ,symb[2],Mnarg,Dnarg,Dvalue[10],NDvalue,i,close;
/* store the current values */
 C2F(xgetmark)(&verbose,symb,&Mnarg,vdouble);
 C2F(get_dash_and_color)(&verbose,Dvalue,&Dnarg,vdouble);
 for (i=0 ; i< *n ; i++)
   {
     if (drawvect[i] <= 0)
       { /** we use the markid : drawvect[i] **/
	 NDvalue = - drawvect[i] ;
	 C2F(xsetmark)(&NDvalue,symb+1,PI0,PI0);
	 C2F(setpattern)(Dvalue+6,PI0,PI0,PI0);
	  
	 C2F(drawpolymark)(str,p,vectsx+(*p)*i,vectsy+(*p)*i,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
       }
     else
       {/** we use the line-style number abs(drawvect[i])  **/
	 C2F(set_line_style)(drawvect+i,PI0,PI0,PI0);
	 close = 0;
	 C2F(drawpolyline)(str,p,vectsx+(*p)*i,vectsy+(*p)*i,&close,
			   PI0,PI0,PD0,PD0,PD0,PD0);
       }
   }
 /** back to default values **/
 C2F(set_dash_and_color)( Dvalue,PI0,PI0,PI0);
 C2F(xsetmark)(symb,symb+1,PI0,PI0);
}
/*-----------------------------------------------------------------------------------*/
/** fill a set of polygons each of which is defined by 
    (*p) points (*n) is the number of polygons 
    the polygon is closed by the routine 
    fillvect[*n] :         
    fillvect[*n] :         
    if fillvect[i] == 0 draw the boundaries with current color 
    if fillvect[i] > 0  draw the boundaries with current color 
    then fill with pattern fillvect[i]
    if fillvect[i] < 0  fill with pattern - fillvect[i]
**/

void C2F(fillpolylines)(str, vectsx, vectsy, fillvect, n, p, v7, dv1, dv2, dv3, dv4)
     char *str;
     integer *vectsx;
     integer *vectsy;
     integer *fillvect;
     integer *n;
     integer *p;
     integer *v7;
     double *dv1;
     double *dv2;
     double *dv3;
     double *dv4;
{
  integer Dnarg,Dvalue[10];
  integer i,cpat,verbose=0,num,close=1,pattern;
  C2F(getpattern)(&verbose,&cpat,&num,vdouble);
  C2F(get_dash_and_color)(&verbose,Dvalue,&Dnarg,vdouble);
  for (i=0 ; i< *n ; i++)
    {
      if (fillvect[i] > 0) 
	{ 
	  /** on peint puis on fait un contour ferme **/
	  C2F(setpattern)(&fillvect[i],PI0,PI0,PI0);
	  C2F(fillpolyline)(str,p,vectsx+(*p)*i,vectsy+(*p)*i,(close=1,&close),
			    PI0,PI0,PD0,PD0,PD0,PD0);
          C2F(set_line_style)(Dvalue,PI0,PI0,PI0);
	  C2F(setpattern)(&(cpat),PI0,PI0,PI0);
	  C2F(drawpolyline)(str,p,vectsx+(*p)*i,vectsy+(*p)*i,(close=1,&close)
			    ,PI0,PI0,PD0,PD0,PD0,PD0);
	}
      else  if (fillvect[i] == 0 )
	{
	  C2F(set_line_style)(Dvalue,PI0,PI0,PI0);
	  C2F(setpattern)(&cpat,PI0,PI0,PI0);
	  C2F(drawpolyline)(str,p,vectsx+(*p)*i,vectsy+(*p)*i,(close=0,&close)
			    ,PI0,PI0,PD0,PD0,PD0,PD0);
	}
      else 
	{
          pattern = -fillvect[i] ;
	  C2F(setpattern)(&pattern,PI0,PI0,PI0);
	  C2F(fillpolyline)(str,p,vectsx+(*p)*i,vectsy+(*p)*i,(close=0,&close)
			    ,PI0,PI0,PD0,PD0,PD0,PD0);
	}
    }
  C2F(set_dash_and_color)(Dvalue,PI0,PI0,PI0); 
}
/*-----------------------------------------------------------------------------------*/
/** Only draw one polygon  with current line style **/
/** according to *closeflag : it's a polyline or a polygon **/
/** n is the number of points of the polyline */

void C2F(drawpolyline)(str, n, vx, vy, closeflag, v6, v7, dv1, dv2, dv3, dv4)
     char *str;
     integer *n;
     integer *vx;
     integer *vy;
     integer *closeflag;
     integer *v6;
     integer *v7;
     double *dv1;
     double *dv2;
     double *dv3;
     double *dv4;
{ 
  integer n1;
  if (*closeflag == 1) n1 = *n+1;else n1= *n;
  if (n1 >= 2) 
    {
      if (C2F(store_points)(*n, vx, vy,*closeflag))
	{
	  Polyline(hdc,C2F(ReturnPoints)(),(int) n1);
	} 
    }
}
/*-----------------------------------------------------------------------------------*/
/** Fill the polygon or polyline **/
/** according to *closeflag : the given vector is a polyline or a polygon **/
void C2F(fillpolyline)(str, n, vx, vy, closeflag, v6, v7, dv1, dv2, dv3, dv4)
     char *str;
     integer *n;
     integer *vx;
     integer *vy;
     integer *closeflag;
     integer *v6;
     integer *v7;
     double *dv1;
     double *dv2;
     double *dv3;
     double *dv4;
{
  integer n1;
  if (*closeflag == 1) n1 = *n+1;else n1= *n;
  if (C2F(store_points)(*n, vx, vy,*closeflag))
    {
      Polygon(hdc,C2F(ReturnPoints)(), n1);
    }
}
/*-----------------------------------------------------------------------------------*/
/** Draw the current mark centred at points defined **/
/** by vx and vy (vx[i],vy[i]) **/
void C2F(drawpolymark)(str, n, vx, vy, v5, v6, v7, dv1, dv2, dv3, dv4)
     char *str;
     integer *n;
     integer *vx;
     integer *vy;
     integer *v5;
     integer *v6;
     integer *v7;
     double *dv1;
     double *dv2;
     double *dv3;
     double *dv4;
{
  if ( ScilabXgc->CurHardSymb == 0 )
    {
      if (C2F(store_points)(*n, vx, vy,0L))		
	{
	  XDrawPoints (hdc,C2F(ReturnPoints)(), *n);
	}
    }
  else 
    { 
      integer i,keepid,keepsize,hds;
      i=1; /** the symbol font **/
      keepid =  ScilabXgc->FontId;
      keepsize= ScilabXgc->FontSize;
      hds= ScilabXgc->CurHardSymbSize;
      C2F(xsetfont)(&i,&hds,PI0,PI0);
      for ( i=0; i< *n ;i++) DrawMark(hdc,vx+i,vy+i);
      C2F(xsetfont)(&keepid,&keepsize,PI0,PI0);
    }
}
/*-----------------------------------------------------------------------------------*/
static void XDrawPoints(lhdc, points, Npoints)
     HDC lhdc;
     POINT *points;
     integer Npoints;
{
  int i ;
  for ( i=0; i < Npoints;i++) 
    {
      /** XXX SetPixel plutot **/
      MoveToEx(hdc,points[i].x,points[i].y,NULL);
      LineTo(hdc,points[i].x+1,points[i].y);
    }
}
/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------
  \encadre{List of Window id}
  -----------------------------------------*/
/*
 * Adds a new entry at the end of the Window List
 * and returns a pointer to that entry
 */

struct BCG *AddNewWindowToList()
{
  return( AddNewWindow(&The_List));
}
/*-----------------------------------------------------------------------------------*/
struct BCG *AddNewWindow(listptr)
     WindowList **listptr;
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
	  (*listptr)->winxgc.CWindow = (HWND) NULL;
	  (*listptr)->winxgc.CurWindow = 0;
	  (*listptr)->winxgc.in_sizemove = 0;
	  (*listptr)->winxgc.Red = (float *) 0;
	  (*listptr)->winxgc.Green = (float *) 0;
	  (*listptr)->winxgc.Blue = (float *) 0;
	  (*listptr)->winxgc.Colors = (COLORREF *) 0;
	  (*listptr)->winxgc.CmapFlag  = 1;
	  (*listptr)->winxgc.EventHandler[0]='\0'; 
	  (*listptr)->winxgc.lpgw = &graphwin;
	  (*listptr)->winxgc.hPen  = (HPEN) 0;
	  (*listptr)->winxgc.hBrush  = (HBRUSH) 0;
	  (*listptr)->winxgc.hbmCompat = (HBITMAP) 0;
	  (*listptr)->winxgc.hdcCompat = (HDC) 0;
	  (*listptr)->winxgc.CurColor = 32; // Adding here F.Leray set the current color to black (i.e.: 32+1=33)
	  (*listptr)->next = (struct WindowList *) NULL ;

	  /* used by new menus */
	  (*listptr)->winxgc.IDM_Count=0;
	  (*listptr)->winxgc.hMenuRoot=NULL;

	  return(&((*listptr)->winxgc));
	}
    }
  else
    {
      return( AddNewWindow((WindowList **) &((*listptr)->next)));
    }
}
/*-----------------------------------------------------------------------------------*/
/** destruction d'une fenetre **/
void DeleteSGWin(intnum)
     integer intnum;
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
/*-----------------------------------------------------------------------------------*/
/** detruit la fenetre num dans la liste des fenetres */
void DeleteWindowToList(num)
     integer num;
{
  WindowList *L1,*L2;
  L1 = The_List;
  L2 = The_List;
  while ( L1 != (WindowList *) NULL)
    {
      if ( L1->winxgc.CurWindow == num )
	{
	  /** destroying windows **/
	  /** XXXX : if there's a pixmap we must free it **/
	  deleted_win = num;
	  DestroyWindow(L1->winxgc.hWndParent);
	  DestroyWindow(L1->winxgc.CWindow);
	  DestroyWindow(L1->winxgc.Statusbar);
	  CloseGraphMacros(&(L1->winxgc));
          XgcFreeColors(&(L1->winxgc));
	  if ( L1->winxgc.CurPixmapStatus == 1) 
	    {
	      /** Freeing bitmaps  **/
	      if ( L1->winxgc.hdcCompat)
		SelectObject (L1->winxgc.hdcCompat, NULL) ;
	      if ( L1->winxgc.hbmCompat)
		DeleteObject (L1->winxgc.hbmCompat);
	      if ( L1->winxgc.hdcCompat)
		{
		  DeleteDC(L1->winxgc.hdcCompat);
		}
	    }
	  /** The window was found **/
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
/*-----------------------------------------------------------------------------------*/
/********************************************
 * Get Window number wincount ( or 0 )
 ********************************************/

HWND GetWindowNumber(wincount)
     int wincount;
{
  struct BCG *bcg;
  bcg = GetWindowXgcNumber(wincount);
  if ( bcg != (struct BCG *) 0)
    return( bcg->CWindow);
  else
    return( (HWND) 0);
}
/*-----------------------------------------------------------------------------------*/
/********************************************
 * returns the graphic context of window i
 * or 0 if this window does not exists
 ********************************************/

struct BCG *GetWindowXgcNumber(i)
     integer i;
{
  return( GetWinXgc(The_List,Max(0,i)));
}
/*-----------------------------------------------------------------------------------*/
struct BCG *GetWinXgc(listptr, i)
     WindowList *listptr;
     integer i;
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

/*-----------------------------------------------------------------------------------*/
/***************************
 * get ids of scilab windows
 * in array Ids,
 * Num gives the number of windows
 * flag == 1 ==> get the Ids 
 * flag == 0 ==> just get the Number Num 
 ***************************/

void C2F(getwins)(Num,Ids,flag)
     integer *Num,Ids[],*flag;
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
/*-----------------------------------------------------------------------------------*/
/*--------------------------------------------------------------
  \encadre{Routine for initialisation : string is a display name }
  --------------------------------------------------------------*/
void set_c(integer coli)
{
  int i=0,id=0, width=0;
  COLORREF col=RGB(0,0,0);
  HBRUSH hBrush=NULL;
	HBRUSH hBrushOld=NULL;
  HPEN hpen=NULL;
	HPEN hpenOld=NULL;
	int style=0;
	int OS = SciWinGetPlatformId();

  if (ScilabXgc->Colors == NULL)  return;

  i= Max(0,Min(coli,ScilabXgc->Numcolors + 1));
  ScilabXgc->CurColor = i ;

  if ( ScilabXgc->CurDrawFunction !=  GXxor ) col = ScilabXgc->Colors[i];
  else col = ScilabXgc->Colors[i] ^ ScilabXgc->Colors[ScilabXgc->NumBackground];

  hBrush=CreateSolidBrush(col);
  hBrushOld=SelectObject(hdc,hBrush);
	if ( hBrushOld ) DeleteObject(hBrushOld);
	ScilabXgc->hBrush = hBrush;

  id =  ScilabXgc->CurDashStyle;
	style=DashTab[id];

	width = ScilabXgc->CurLineWidth;

	if(OS == VER_PLATFORM_WIN32_NT)
	{
		if ( (style == PS_SOLID) || (width<=1) )
		{
			hpen = CreatePen(style,ScilabXgc->CurLineWidth,col);
		}
		else
		{
			LOGBRUSH logbrush;
			logbrush.lbStyle = BS_SOLID;
			logbrush.lbColor = col;
			logbrush.lbHatch = 0;
			hpen = ExtCreatePen(PS_GEOMETRIC|PS_ENDCAP_FLAT|style,width,&logbrush,0,NULL);
		}
	}
	else
	{
		width = ( DashTab[id] != PS_SOLID) ?  0 : ScilabXgc->CurLineWidth ;
		hpen = CreatePen(PS_SOLID,ScilabXgc->CurLineWidth,col);
	}

  hpenOld=SelectObject(hdc,hpen);
  if (hpenOld) DeleteObject(hpenOld);
  ScilabXgc->hPen = hpen;

	SetTextColor(hdc,col); 
}

 
/*-----------------------------------------------------------------------------------*/
/** Initialyze the dpy connection and creates graphic windows **/
/** If v2 is not a nul pointer *v2 is the window number to create **/
/** EntryCounter is used to check for first Entry + to now an available number **/

void C2F(initgraphic)(string, v2, v3, v4, v5, v6, v7, dv1, dv2, dv3, dv4)
     char *string;
     integer *v2;
     integer *v3;
     integer *v4;
     integer *v5;
     integer *v6;
     integer *v7;
     double *dv1;
     double *dv2;
     double *dv3;
     double *dv4;
{ 
	static char popupname[sizeof("ScilabGraphic")+4];
	static char winname[sizeof("BG")+4];
	struct BCG *NewXgc ;
	RECT rect,rect1;
	static integer EntryCounter = 0;
	integer WinNum;
	static HMENU sysmenu;
	SCROLLINFO vertsi;
	SCROLLINFO horzsi;

	if ( v2 != (integer *) NULL && *v2 != -1 )
	{
		WinNum= *v2;
	}
	else
	{
		WinNum= EntryCounter;
	}

	if (EntryCounter == 0)
	{
		/** XXXXXX : pour l'instant couleur par defaut */
		screencolor = 1;
		if (C2F(AllocVectorStorage)()==0) return;
		graphwin.xmax = WIN_XMAX;
		graphwin.ymax = WIN_YMAX;
		if (! graphwin.hPrevInstance) /** XXX : utiliser EntryCounter ??? **/
		{
			CreateGraphClass();
		}
		/** Read or use default values **/
	}

	if (( NewXgc = AddNewWindowToList()) == (struct BCG *) 0)
	{
		Scistring("initgraphics: unable to alloc\n");
		return;
	}
	else
	{
		ScilabXgc= NewXgc;
	}
	/** ReadRegistryGraph takes care of graphwin.Origin and  graphwin.Size **/
	/** ScilabXgc is send to CreateWindow and this information is used 
	in WndGraphProc **/

	/* Probleme graphwin peut etre locké par une autre fonction */
	/* Faire une copie serait preferable */
	ScilabXgc->lpgw = &graphwin;

	sprintf(popupname,"ScilabGraphic%d", (int)WinNum);
	if (EntryCounter == 0)
	{
		ReadRegistryGraph(ScilabXgc);
	}
	ScilabXgc->Inside_init=1; /** to know that we are inside init code **/

	if (IsTKGraphicalMode())
	{
	#ifdef WITH_TK
		Tk_Window  win;
		Window    windowId;
		/* TKmainWindow est initialise dans tclsci.c  Tk_CreateMainWindow */
		if ( TKmainWindow != 0)
		{
			win = Tk_NameToWindow(TCLinterp, string,TKmainWindow);
			if (win != 0)
			{
				windowId = Tk_WindowId(win);
				ScilabXgc->CWindow = (HWND) Tk_GetHWND( windowId );
				if (ScilabXgc->CWindow ==NULL) MessageBox((HWND)NULL,"Couldn't open graphic window",(LPSTR)NULL, MB_ICONHAND | MB_SYSTEMMODAL);
				ScilabXgc->hWndParent=NULL;
				GetClientRect(ScilabXgc->hWndParent, &rect);
				ScilabXgc->CWindowWidth =  rect.right;
				ScilabXgc->CWindowHeight = rect.bottom;
				ScilabXgc->CWindowWidthView  = ScilabXgc->CWindowWidth;
				ScilabXgc->CWindowHeightView = ScilabXgc->CWindowHeight;
			}
		}
	#endif	
		SetWinhdc();
		SetMapMode(hdc, MM_TEXT);
		SetBkMode(hdc,TRANSPARENT);
		GetClientRect(ScilabXgc->CWindow, &rect);
		SetViewportExtEx(hdc, rect.right, rect.bottom,NULL);
		SetTextAlign(hdc, TA_LEFT|TA_BOTTOM);
		SetFocus( ScilabXgc->CWindow);
		if (EntryCounter == 0)
		{
			C2F(CreatePatterns)();
			LoadFonts();
		} 
		InitMissileXgc(PI0,PI0,PI0,PI0);/* a laisser ici */
		/* to be sure that current values are recorded */
		SetGraphicsVersion(); /* set the graphics version using global versionflag variable */

		StoreXgc(WinNum);
		EntryCounter=Max(EntryCounter,WinNum);
		EntryCounter++;
	}
	else
	{
		ScilabXgc->hWndParent = CreateWindow(szParentGraphClass, popupname,
			WS_OVERLAPPEDWINDOW,
			graphwin.Origin.x, graphwin.Origin.y,
			graphwin.Size.x, graphwin.Size.y,
			NULL, NULL, graphwin.hInstance, 
			NewXgc);

		if (ScilabXgc->hWndParent == (HWND)NULL)
		{
			MessageBox((HWND)NULL,"Couldn't open parent graph window",(LPSTR)NULL, MB_ICONHAND | MB_SYSTEMMODAL);
			return;
		}
		ShowWindow(ScilabXgc->hWndParent,  SW_SHOWNORMAL);
		ScilabXgc->Statusbar =  InitStatusBar (ScilabXgc->hWndParent);
		ShowWindow(ScilabXgc->Statusbar,  SW_SHOWNORMAL);
		GetWindowRect (ScilabXgc->Statusbar, &rect1) ;
		GetClientRect(ScilabXgc->hWndParent, &rect);

		MoveWindow(ScilabXgc->Statusbar, 0, rect.bottom -( rect1.bottom - rect1.top),
		rect.right,  ( rect1.bottom - rect1.top), TRUE) ;

		sprintf((char *)winname,"BG%d", (int)WinNum);

		ScilabXgc->CWindowWidth =  rect.right;
		ScilabXgc->CWindowHeight = rect.bottom - ( rect1.bottom - rect1.top);

		ScilabXgc->CWindow = CreateWindow(szGraphClass, winname,
			WS_CHILD | WS_VSCROLL | WS_HSCROLL,
			0, graphwin.ButtonHeight,
			ScilabXgc->CWindowWidth,
			ScilabXgc->CWindowHeight,
			ScilabXgc->hWndParent,
			NULL, graphwin.hInstance,
			NewXgc);

		ScilabXgc->CurResizeStatus = 1;
		ScilabXgc->CWindowWidthView  = ScilabXgc->CWindowWidth;
		ScilabXgc->CWindowHeightView = ScilabXgc->CWindowHeight;

		/* definition des scroll bars verticalles */
		vertsi.cbSize = sizeof(SCROLLINFO);
		vertsi.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS;
		vertsi.nMin   = 0;
		vertsi.nMax   = ScilabXgc->CWindowHeight;
		vertsi.nPage  = ScilabXgc->CWindowHeightView;
		vertsi.nPos   = 0;
		sciSetScrollInfo(ScilabXgc,SB_VERT, &(vertsi), TRUE);
		sciGetScrollInfo(ScilabXgc,SB_VERT, &vertsi);

		/* definition des scroll bars horizontalle */
		horzsi.cbSize = sizeof(SCROLLINFO);
		horzsi.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS;
		horzsi.nMin   = 0;
		horzsi.nMax   = ScilabXgc->CWindowWidth;
		horzsi.nPage  = ScilabXgc->CWindowWidthView;
		horzsi.nPos   = 0;
		sciSetScrollInfo(ScilabXgc,SB_HORZ, &horzsi, TRUE);	  
		sciGetScrollInfo(ScilabXgc,SB_HORZ, &horzsi);
	
		if (ScilabXgc->CWindow == (HWND)NULL) 
		{
			MessageBox((HWND)NULL,"Couldn't open graphic window",(LPSTR)NULL, MB_ICONHAND | MB_SYSTEMMODAL);
			return;
		}
        
		ShowWindow(ScilabXgc->CWindow, SW_SHOWNORMAL);
		ShowWindow(ScilabXgc->hWndParent,  SW_SHOWNORMAL);
		graphwin.resized = FALSE;

		/*UpdateFileGraphNameMenu( ScilabXgc);
		LoadGraphMacros( ScilabXgc);*/
		/** Default value is without Pixmap **/
		ScilabXgc->CurPixmapStatus = 0;
		ScilabXgc->CurResizeStatus = 1;
		ScilabXgc->CurWindow = WinNum;
		/* on fait un SetWinhdc car on vient de creer la fenetre */
		/* le release est fait par Xcall.c */
		SetWinhdc();
		SetMapMode(hdc, MM_TEXT);
		SetBkMode(hdc,TRANSPARENT);
		GetClientRect(ScilabXgc->CWindow, &rect);
		SetViewportExtEx(hdc, rect.right, rect.bottom,NULL);
		SetTextAlign(hdc, TA_LEFT|TA_BOTTOM);
		SetFocus( ScilabXgc->CWindow);
		SetActiveWindow( ScilabXgc->CWindow);

		if (EntryCounter == 0)
		{
			C2F(CreatePatterns)();
			LoadFonts();
		} 
		InitMissileXgc(PI0,PI0,PI0,PI0);/* a laisser ici */
		/* to be sure that current values are recorded */  
		SetGraphicsVersion(); /* set the graphics version using global versionflag variable */

		StoreXgc(WinNum);
		EntryCounter=Max(EntryCounter,WinNum);
		EntryCounter++;
		
		/* see ../wsci/wgmenu.c */
		CreateGedMenus(ScilabXgc);	  
		CreateGraphToolBar(ScilabXgc);	  
	}

	ScilabXgc->Inside_init=0;
}
/*-----------------------------------------------------------------------------------*/
static void CreateGraphClass()
{
  RegisterParentWindowGraphClass();
  RegisterWindowGraphClass();
}
/*-----------------------------------------------------------------------------------*/
static BOOL RegisterParentWindowGraphClass (void)
{
/** The parent window **/
  BOOL bOK=FALSE;
  static WNDCLASS ParentGraphwndclass;
	
  ParentGraphwndclass.style = CS_HREDRAW | CS_VREDRAW;
  ParentGraphwndclass.lpfnWndProc = WndParentGraphProc;
  ParentGraphwndclass.cbClsExtra = 0;
  ParentGraphwndclass.cbWndExtra = 4 * sizeof(void *);
  ParentGraphwndclass.hInstance = graphwin.hInstance;
  ParentGraphwndclass.hIcon =  LoadIcon (graphwin.hInstance, (LPCSTR)TEXT(IDI_PUFFIN));
  ParentGraphwndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
  ParentGraphwndclass.hbrBackground = NULL;
  ParentGraphwndclass.lpszMenuName = NULL;
  ParentGraphwndclass.lpszClassName = szParentGraphClass;

  if (!RegisterClass(&ParentGraphwndclass))
    {
      DWORD dwLastError = GetLastError();
      char buff1[1000], buff2[1000];

      FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwLastError, 0, buff1, sizeof (buff1), NULL);
      sprintf(buff2, "ParentGraphwndclass Win32 error %ld occured", dwLastError);

      MessageBox(NULL, buff1, buff2, MB_ICONERROR);
      bOK=FALSE;
    }
  else bOK=TRUE;

  return bOK;
}
/*-----------------------------------------------------------------------------------*/
static BOOL RegisterWindowGraphClass (void)
{
  BOOL bOK=FALSE;
  static WNDCLASS Graphwndclass;
  /** each Graphic window owns is DC : CS_OWNDC **/
  Graphwndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC |CS_DBLCLKS ;
  Graphwndclass.lpfnWndProc = WndGraphProc;
  Graphwndclass.cbClsExtra = 0;
  Graphwndclass.cbWndExtra = 4 * sizeof(void *);
  Graphwndclass.hInstance = graphwin.hInstance;
  Graphwndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  Graphwndclass.hCursor =  LoadCursor(NULL, IDC_CROSS);
  Graphwndclass.hbrBackground = NULL;
  Graphwndclass.lpszMenuName = NULL;
  Graphwndclass.lpszClassName = szGraphClass;

  if (!RegisterClass(&Graphwndclass))
    {
      DWORD dwLastError = GetLastError();
      char buff1[1000], buff2[1000];

      FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwLastError, 0, buff1, sizeof (buff1), NULL);
      sprintf(buff2, "ParentGraphwndclass Win32 error %ld occured", dwLastError);

      MessageBox(NULL, buff1, buff2, MB_ICONERROR);
      bOK=FALSE;
    }
  else bOK=TRUE;

  return bOK;
}
/*-----------------------------------------------------------------------------------*/
/* Writes a message in the Label info part of the Graphicwindow  */

void C2F(xinfo)(message, v2, v3, v4, v5, v6, v7, dv1, dv2, dv3, dv4)
     char *message;
     integer *v2,*v3,*v4,*v5,*v6,*v7;
     double *dv1,*dv2,*dv3,*dv4;
{
  if ( ScilabXgc != (struct BCG *) 0 && ScilabXgc->Statusbar != (HWND) 0)
    {
      (BOOL)SendMessage(ScilabXgc->Statusbar, SB_SETTEXT, (WPARAM) 0, 
			(LPARAM) (LPSTR) message );
    }
}
/*-----------------------------------------------------------------------------------*/
/* Extended call for C calling */
#define MAXPRINTF 512
/*-----------------------------------------------------------------------------------*/
void wininfo(char *fmt,...)
{
  int count;
  char buf[MAXPRINTF];
  va_list args;
  va_start(args,fmt);
  count = vsprintf(buf,fmt,args);
  if ( ScilabXgc != (struct BCG *) 0 && ScilabXgc->Statusbar != (HWND) 0)
    {
      (BOOL)SendMessage(ScilabXgc->Statusbar, SB_SETTEXT, (WPARAM) 0, 
			(LPARAM) (LPSTR) buf);
    }
}
/*-----------------------------------------------------------------------------------*/
/*************************************************
 * Initialize the graphic context. Used also 
 * to come back to the default graphic state
 *************************************************/

static void InitMissileXgc (integer *v1,integer *v2,integer *v3,integer *v4)
{ 
  integer i,j;
  ScilabXgc->IDLastPattern = GREYNUMBER - 1;
  ScilabXgc->CurLineWidth=0 ;
  C2F(setalufunction1)((i=3,&i),PI0,PI0,PI0);
  /** retirer le clipping **/
  i=j= -1;
  C2F(unsetclip)(PI0,PI0,PI0,PI0);
  ScilabXgc->ClipRegionSet= 0;
  C2F(xsetfont)((i=2,&i),(j=1,&j),PI0,PI0);
  C2F(xsetmark)((i=0,&i),(j=0,&j),PI0,PI0);
  /** trac\'e absolu **/
  i= CoordModeOrigin ;
  C2F(setabsourel)(&i,PI0,PI0,PI0);
  /* initialisation des pattern dash par defaut en n&b */
  ScilabXgc->CurColorStatus =0;
  C2F(setpattern)((i=1,&i),PI0,PI0,PI0);
  C2F(setdash)((i=1,&i),PI0,PI0,PI0);
  C2F(sethidden3d)((i=1,&i),PI0,PI0,PI0);
  /** attention setthickness : depend de la couleur en Win95 **/
  C2F(setthickness)((i=0,&i),PI0,PI0,PI0);
  /* initialisation de la couleur par defaut */ 
  ScilabXgc->CurColorStatus = 1;
  set_default_colormap(); //F.Leray This call initializes ScilabXgc->NumForeground = 32 and ScilabXgc->NumBackground = 33
  C2F(setalufunction1)((i=3,&i),PI0,PI0,PI0);
  C2F(setpattern)((i=DefaultForeground,&i),PI0,PI0,PI0);
  C2F(setpattern)((i=ScilabXgc->NumForeground+1,&i),PI0,PI0,PI0);
  C2F(setthickness)((i=1,&i),PI0,PI0,PI0);
  /*** XXXXX a faire aussi pour le n&b plus haut ***/
  C2F(setforeground)((i=ScilabXgc->NumForeground+1,&i),PI0,PI0,PI0);
  C2F(setbackground)((i=ScilabXgc->NumForeground+2,&i),PI0,PI0,PI0); // F.Leray Check if OK here may be Pb here... 04.03.04
  C2F(sethidden3d)((i=4,&i),PI0,PI0,PI0);
  /* Choix du mode par defaut (decide dans initgraphic) */
  getcolordef(&i);
  /** we force CurColorStatus to the opposite value of col
   *  to force usecolorPos to perform initialisations
   **/
  ScilabXgc->mafigure = (sciPointObj *)NULL;
  /*   ScilabXgc->graphicsversion = versionflag; /\* NG *\/  */
  ScilabXgc->CurColorStatus = (i == 1) ? 0: 1;
  C2F(usecolor)(&i ,PI0,PI0,PI0);
  strcpy(ScilabXgc->CurNumberDispFormat,"%-5.2g");

  /** default scales **/
  Cscale2default();

  /* This is a very bad idea to try to put a 
   * C2F(setwresize)((i = 1, &i), PI0,PI0,PI0);
   * here. It leads to an infinite paint loop 
   * since setwresize start a InvalidateRect 
   * and InitMissileXgc if started from xset('default') 
   * is a recorded graphic 
   * xset('default') -> setwresize -> InvalidateRect 
   * then paint -> xset('default')
   */ 
  
  C2F(setpixmapOn)((i = 0,&i),PI0,PI0,PI0);
}

static void SetGraphicsVersion()
{
  ScilabXgc->graphicsversion = versionflag; /* NG */ 
}

/*-----------------------------------------------------------------------------------*/
/* returns the current color status */

void getcolordef(screenc)
     integer *screenc;
{
  *screenc= screencolor;
}
/*-----------------------------------------------------------------------------------*/
void setcolordef(screenc)
     integer screenc;
{
  screencolor = screenc;
}
/*-----------------------------------------------------------------------------------*/
/* Utilise le ScilabXgc courant pour reinitialiser le gc XWindow */
/* cela est utilis'e quand on change de fenetre graphique        */

void ResetScilabXgc ()
{ 
  integer i,j, clip[4];
  i= ScilabXgc->FontId;
  j= ScilabXgc->FontSize;
  C2F(xsetfont)(&i,&j,PI0,PI0);
  /** wininfo("Reset Scilab Xgc avec %d %d\r\n",i,j); **/
  i= ScilabXgc->CurHardSymb;
  j= ScilabXgc->CurHardSymbSize;
  C2F(xsetmark)(&i,&j,PI0,PI0);
  
  i= ScilabXgc->CurLineWidth;
  C2F(setthickness)(&i,PI0,PI0,PI0);
  
  i= ScilabXgc->CurVectorStyle;
  C2F(setabsourel)(&i,PI0,PI0,PI0);
  
  i= ScilabXgc->CurDrawFunction;
  C2F(setalufunction1)(&i,PI0,PI0,PI0);
  
  if (ScilabXgc->ClipRegionSet == 1) 
    {
      for ( i= 0 ; i < 4; i++) clip[i]=ScilabXgc->CurClipRegion[i];
      C2F(setclip)(clip,clip+1,clip+2,clip+3);
    }
  else
    C2F(unsetclip)(PI0,PI0,PI0,PI0);

  if (ScilabXgc->CurColorStatus == 0) 
    {
      /* remise des couleurs a vide */
      ScilabXgc->CurColorStatus = 1;
      C2F(setpattern)((i=DefaultForeground,&i),PI0,PI0,PI0);
      /* passage en n&b */
      ScilabXgc->CurColorStatus = 0;
      i= ScilabXgc->CurPattern + 1;
      C2F(setpattern)(&i,PI0,PI0,PI0);
      i= ScilabXgc->CurDashStyle + 1;
      C2F(setdash)(&i,PI0,PI0,PI0);
      i= ScilabXgc->NumHidden3d+1;
      C2F(sethidden3d)(&i,PI0,PI0,PI0);
    }
  else 
    {
      /* remise a zero des patterns et dash */
      /* remise des couleurs a vide */
      ScilabXgc->CurColorStatus = 0;
      C2F(setpattern)((i=1,&i),PI0,PI0,PI0);
      C2F(setdash)((i=1,&i),PI0,PI0,PI0);
      /* passage en couleur  */
      ScilabXgc->CurColorStatus = 1;
      i= ScilabXgc->CurColor + 1;
      C2F(setpattern)(&i,PI0,PI0,PI0);
      i= ScilabXgc->NumBackground+1;
      C2F(setbackground)(&i,PI0,PI0,PI0);
      i= ScilabXgc->NumForeground+1;
      C2F(setforeground)(&i,PI0,PI0,PI0);
      i= ScilabXgc->NumHidden3d+1;
      C2F(sethidden3d)(&i,PI0,PI0,PI0);
    }
}
/*-----------------------------------------------------------------------------------*/
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

void C2F(drawaxis)(str, alpha, nsteps, v2, initpoint, v6, v7, size, dx2, dx3, dx4)
     char *str;
     integer *alpha;
     integer *nsteps;
     integer *v2;
     integer *initpoint;
     integer *v6;
     integer *v7;
     double *size;
     double *dx2;
     double *dx3;
     double *dx4;
{ integer i;
 double xi,yi,xf,yf;
 double cosal,sinal;
 cosal= cos( (double)M_PI * (*alpha)/180.0);
 sinal= sin( (double)M_PI * (*alpha)/180.0);
 for (i=0; i <= nsteps[0]*nsteps[1]; i++)
   {
     if ( ( i % nsteps[0]) != 0) 
       {
	 xi = initpoint[0]+i*size[0]*cosal;
	 yi = initpoint[1]+i*size[0]*sinal;
	 xf = xi - ( size[1]*sinal);
	 yf = yi + ( size[1]*cosal);
	 MoveToEx(hdc,inint(xi),inint(yi),NULL);
	 LineTo(hdc,inint(xf),inint(yf));
       }
   }
 for (i=0; i <= nsteps[1]; i++)
   { xi = initpoint[0]+i*nsteps[0]*size[0]*cosal;
   yi = initpoint[1]+i*nsteps[0]*size[0]*sinal;
   xf = xi - ( size[1]*size[2]*sinal);
   yf = yi + ( size[1]*size[2]*cosal);
   MoveToEx(hdc,inint(xi),inint(yi),NULL);
   LineTo(hdc,inint(xf),inint(yf));
   }
 /**
    xi = initpoint[0]; yi= initpoint[1];
    xf = initpoint[0]+ nsteps[0]*nsteps[1]*size[0]*cosal;
    yf = initpoint[1]+ nsteps[0]*nsteps[1]*size[0]*sinal;
    MoveToEx(hdc,inint(xi),inint(yi),NULL);
    LineTo(hdc,inint(xf),inint(yf));
 **/
}
/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------
  \encadre{Display numbers z[i] at location (x[i],y[i])
  with a slope alpha[i] (see displaystring), if flag==1
  add a box around the string, only if slope =0}
  -----------------------------------------------------*/

void C2F(displaynumbers)(str, x, y, v1, v2, n, flag, z, alpha, dx3, dx4)
     char *str;
     integer *x;
     integer *y;
     integer *v1;
     integer *v2;
     integer *n;
     integer *flag;
     double *z;
     double *alpha;
     double *dx3;
     double *dx4;
{ 
  char buf[20];
  integer i ;
  for (i=0 ; i< *n ; i++)
    {
      sprintf(buf,ScilabXgc->CurNumberDispFormat,z[i]);
      C2F(displaystring)(buf,&(x[i]),&(y[i]),PI0,flag,PI0,PI0,&(alpha[i]),PD0,PD0,PD0) ;
    }
}

void C2F(bitmap)(string, w, h)
     char *string;
     integer w;
     integer h;
{
  /** 
      static XImage *setimage;
      setimage = XCreateImage (dpy, XDefaultVisual (dpy, DefaultScreen(dpy)),
      1, XYBitmap, 0, string,w,h, 8, 0);	
      setimage->data = string;
      XPutImage (dpy, Cdrawable, gc, setimage, 0, 0, 10,10,w,h);
      XDestroyImage(setimage);
  **/
}

/*-----------------------------------------------------------------------------------*/
/***********************************
 * set current font to font fontid at size 
 * fontsize ( <<load>> the font if necessary )
 ***********************************/

void C2F(xsetfont)(fontid, fontsize, v3, v4)
     integer *fontid;
     integer *fontsize;
     integer *v3;
     integer *v4;
{ 
  integer i,fsiz;
  i = Min(FONTNUMBER-1,Max(*fontid,0));
  fsiz = Min(FONTMAXSIZE-1,Max(*fontsize,0));
  if ( (*FontTab)[i].ok !=1 )
    { 
      // if (i != FONTNUMBER-1 ) F.Leray test here 26.02.04
      if (i != FONTNUMBER )
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
  SelectFont(hdc, (*FontTab)[i].hf[fsiz]);
}
/*-----------------------------------------------------------------------------------*/
static HFONT getcurfont()
{
  return( (*FontTab)[ScilabXgc->FontId].hf[ScilabXgc->FontSize]);
}
/*-----------------------------------------------------------------------------------*/
/*********************************************
 * To get the  id and size of the current font 
 **********************************************/

void  C2F(xgetfont)(verbose, font, nargs,dummy)
     integer *verbose;
     integer *font;
     integer *nargs;
     double *dummy;
{
  *nargs=2;
  font[0]= ScilabXgc->FontId ;
  font[1] =ScilabXgc->FontSize ;
  if (*verbose == 1) 
    {
      sciprint("\r\nFontId : %d ", ScilabXgc->FontId );
      sciprint("--> %s at size %s pts\r\n",
	       (*FontTab)[ScilabXgc->FontId].fname,
	       size_[ScilabXgc->FontSize]);
    }
}
/*-----------------------------------------------------------------------------------*/
/*********************************************
 * To set the current mark ( a symbol in font symbol)
 **********************************************/

void C2F(xsetmark)(number, size, v3, v4)
     integer *number;
     integer *size;
     integer *v3;
     integer *v4;
{ 
  ScilabXgc->CurHardSymb = Max(Min(SYMBOLNUMBER-1,*number),0);
  ScilabXgc->CurHardSymbSize = Max(Min(FONTMAXSIZE-1,*size),0);
}

/*-----------------------------------------------------------------------------------*/
/*********************************************
 * To get the current mark id 
 **********************************************/

void C2F(xgetmark)(verbose, symb, narg,dummy)
     integer *verbose;
     integer *symb;
     integer *narg;
     double *dummy;
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
/*-----------------------------------------------------------------------------------*/
/**************************************************
 * loadfamily Loads a font at size  08 10 12 14 18 24 
 * for example TimR08 TimR10 TimR12 TimR14 TimR18 TimR24 
 * name is a string 
 *  ( X11 only : if it's a string containing the char % 
 *    it's suposed to be a format for a generic font in X11 string style 
 *    "-adobe-times-bold-i-normal--%s-*-75-75-p-*-iso8859-1" ) 
 * it's supposed to be an alias for a font name
 * Ex : TimR and we shall try to load TimR08 TimR10 TimR12 TimR14 TimR18 TimR24 
 **************************************************/

void C2F(loadfamily)(name, j, v3, v4, v5, v6, v7, dv1, dv2, dv3, dv4)
     char *name;
     integer *j,*v3,*v4,*v5,*v6,*v7;
     double *dv1, *dv2,*dv3,*dv4;
{ 
  integer i;
  /** our table of alias **/
  i=0;
  while ( fonttab[i].alias != (char *) NULL)
    {
      if (strcmp(fonttab[i].alias,name)==0)
	{
	  C2F(loadfamily_n)(fonttab[i].Winname,j);
	  return ;
	}
      i++;
    }
  C2F(loadfamily_n)(name,j);
}
/*-----------------------------------------------------------------------------------*/
void C2F(queryfamily)(name, j, v3, v4, v5, v6, v7, dv1, dv2, dv3, dv4)
     char *name;
     integer *j;
     integer *v3;
     integer *v4;
     integer *v5;
     integer *v6;
     integer *v7;
     double *dv1;
     double *dv2;
     double *dv3;
     double *dv4;
{ 
  integer i ;
  name[0]='\0';
  // for (i=0;i<FONTNUMBER;i++) { //Pb here: fonttab size is 10 and i goes from 0 to 10 included 
  // fonttab[10] is "{(char *) NULL,( char *) NULL}" F.Leray 24.02.04
  // Pb fixed by setting FONTNUMBER to 10
  
  for (i=0;i<FONTNUMBER;i++) {
    v3[i]=strlen((*FontTab)[i].fname);
    if (v3[i] > 0)
      strcat(name,(*FontTab)[i].fname);
    else {
      v3[i]=strlen(fonttab[i].Winname);
      strcat(name,fonttab[i].Winname);
    }
  }
  *j=FONTNUMBER;
}
/*-----------------------------------------------------------------------------------*/
/** creates a font **/
void SciMakeFont(name,size,hfont)
     char *name;
     int size;
     HFONT *hfont;
{
  LOGFONT lf;
  char *p;
  memset(&lf, 0, sizeof(LOGFONT));
  strncpy(lf.lfFaceName,name,LF_FACESIZE);
  /** lf.lfHeight = - MulDiv( size, GetDeviceCaps(hdc, LOGPIXELSY), 72);  **/
  lf.lfHeight  = - size*scale_font_size;
  lf.lfCharSet = DEFAULT_CHARSET;
  if ( (p = strstr(name," Italic")) != (LPSTR)NULL ) {
    lf.lfFaceName[ (unsigned int)(p- name) ] = '\0';
    lf.lfItalic = TRUE;
  }
  if ( (p = strstr(name," Bold")) != (LPSTR)NULL ) {
    lf.lfFaceName[ (unsigned int)(p- name) ] = '\0';
    lf.lfWeight = FW_BOLD;
  }
  *hfont = CreateFontIndirect((LOGFONT FAR *)&lf);
}
/*-----------------------------------------------------------------------------------*/
static void C2F(loadfamily_n)(name, j)
     char *name;
     integer *j;
{ 
  integer i,flag=1 ;
  for ( i = 0; i < FONTMAXSIZE ; i++)
    {
      SciMakeFont(name,size_n_[i], &((*FontTab)[*j].hf[i]));
      if  (  (*FontTab)[*j].hf[i] == (HFONT) 0 )
	{ 
	  flag=0;
	  sciprint("Unknown font : %s\r\n",name);
	  Scistring("I'll use font: Courier New\r\n");
	  SciMakeFont("Courier New",size_n_[i], &((*FontTab)[*j].hf[i]));
	  if  ( (*FontTab)[*j].hf[i] == (HFONT) 0)
	    {
	      sciprint("Unknown font : %s\r\n","Courier New");
	      Scistring("  Please send a bug report !\r\n");
	    }
	}
    }
  (*FontTab)[*j].ok = 1;
  if (flag != 0) 
    strcpy((*FontTab)[*j].fname,name);
  else
    strcpy((*FontTab)[*j].fname,"Courier New");
}
/*-----------------------------------------------------------------------------------*/
/********************************************
 * switch to printer font 
 ********************************************/

void SciG_Font_Printer(int scale)
{
  static int last_scale= -1;
  FontTab = &FontInfoTabPrinter;
  SymbOffset = &ListOffsetPrint;
  if ( last_scale != -1 && last_scale != scale ) 
    CleanFonts();
  scale_font_size = last_scale = scale;
  LoadFonts();
}
/*-----------------------------------------------------------------------------------*/
void SciG_Font(void) 
{
  FontTab = &FontInfoTab;
  SymbOffset = &ListOffset;
  scale_font_size = 1; 
}
/*-----------------------------------------------------------------------------------*/
void CleanFonts()
{
  int i,j;
  for ( j= 0 ; j < FONTNUMBER  ; j++ )
    {
      if ( (*FontTab)[j].ok == 1) 
	{
	  (*FontTab)[j].ok = 0;
	  for ( i = 0; i < FONTMAXSIZE ; i++)
	    {
	      DeleteObject( (*FontTab)[j].hf[i]) ;
	      (*FontTab)[j].hf[i] = (HFONT) 0;
	    }
	}
    }
}
/*-----------------------------------------------------------------------------------*/
/********************************************
 * Initial set of font loaded at startup 
 ********************************************/

static void LoadFonts()
{
  integer fnum;
  C2F(loadfamily)("CourR",(fnum=0,&fnum),PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0); 
  LoadSymbFonts();
  C2F(loadfamily)("TimR",(fnum=2,&fnum),PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
  /*  
      XXX : we load fonts when we need see xsetfonts
      C2F(loadfamily)("TimI",(fnum=3,&fnum),PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
      C2F(loadfamily)("TimB",(fnum=4,&fnum),PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
      C2F(loadfamily)("TimBI",(fnum=5,&fnum),PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0); 
      See xsetfont
  */
}

/*-----------------------------------------------------------------------------------*/
static void LoadSymbFonts()
{
  /** XCharStruct xcs;**/
  integer j,fid;
  integer i;
  /** Symbol Font is loaded under Id : 1 **/
  C2F(loadfamily)("Symb",(i=1,&i),PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
  /* We compute the char offset for several chars of the symbol font    */
  /** if symbol font was not found me must stop  **/
  fid=1; /** the symbol font **/
  for (i =0 ; i < FONTMAXSIZE ; i++)
    {
      if ( (*FontTab)[1].hf[i] != NULL)
        {
	  SelectFont(hdc, (*FontTab)[fid].hf[i]);
          for (j=0 ; j < SYMBOLNUMBER ; j++)
            {
	      SIZE size;
              char str[1];
              str[0]=Marks[j];
	      GetTextExtentPoint32(hdc,str,1,&size);
              SymbOffset->xoffset[i][j] = size.cx /2;
              SymbOffset->yoffset[i][j] = size.cy /2;
            }
        }
    }
}
/*-----------------------------------------------------------------------------------*/
/** The two next functions send the x and y offsets to center the current **/
/** symbol at point (x,y) **/
int C2F(CurSymbXOffset)()
{
  return(- SymbOffset->xoffset[ScilabXgc->CurHardSymbSize]
	 [ScilabXgc->CurHardSymb]);
}
/*-----------------------------------------------------------------------------------*/
int C2F(CurSymbYOffset)()
{
  return( SymbOffset->yoffset[ScilabXgc->CurHardSymbSize]
	  [ScilabXgc->CurHardSymb]);
}
/*-----------------------------------------------------------------------------------*/
/********************************************
 * Draws the current mark centred at position 
 * x,y
 ********************************************/
static void DrawMark(lhdc,x, y)
     HDC lhdc;
     integer *x;
     integer *y;
{
  char str[2];

  str[0]=Marks[ScilabXgc->CurHardSymb];
  str[1]='\0';
  if ( ScilabXgc->CurDrawFunction ==  GXxor )
    {
      SIZE size ;
      GetTextExtentPoint32(hdc,str,1,&size);
      XorString(*x+C2F(CurSymbXOffset)(),*y+C2F(CurSymbYOffset)(),
		str,size.cx,size.cy);
    }
  else
    {
      TextOut(hdc,*x+C2F(CurSymbXOffset)(),*y+C2F(CurSymbYOffset)(),str,1); 
    }

}
/*-----------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------
  \subsection{Allocation and storing function for vectors of X11-points}
  ------------------------------------------------------------------------*/

static POINT *points;
static unsigned nbpoints;
#define NBPOINTS 256 
/*-----------------------------------------------------------------------------------*/
int C2F(store_points)(n, vx, vy, onemore)
     integer n;
     integer *vx;
     integer *vy;
     integer onemore;
{ 
  integer i,n1;
  if ( onemore == 1) n1=n+1;
  else n1=n;
  if (ReallocVector(n1) == 1)
    {
      for (i = 0; i < n; i++){
//#ifdef DEBUG
//	if ( Abs(vx[i]) > int16max )
//	  {
//	    wsprintf(stderr,"Warning store_point oustide of 16bits x=%d\n",
//		    (int) vx[i]);
//	  }
//	if ( Abs(vy[i]) > int16max )
//	  {
//	    wsprintf(stderr,"Warning store_point oustide of 16bits x=%d\n",
//		    (int) vy[i]);
//	  }
//#endif
	points[i].x = INT_2_16B( vx[i] ) ;
	points[i].y = INT_2_16B( vy[i] ) ;
      }
      if (onemore == 1) {
	points[n].x=(short) points[0].x;
	points[n].y=(short) points[0].y;
      }
      return(1);
    }
  else return(0);
}
/*-----------------------------------------------------------------------------------*/
static int ReallocVector(n)
     integer n;
{
  while ( (unsigned) n > nbpoints){
    nbpoints = 2 * nbpoints ;

    points = (POINT *) REALLOC(points,(unsigned)
			       nbpoints * sizeof (POINT));
    if (points == 0) 
      { 
	sciprint(MESSAGE5);
	return (0);
      }
  }
  return(1);
}
/*-----------------------------------------------------------------------------------*/
int C2F(AllocVectorStorage)()
{
  nbpoints = NBPOINTS;
  points = (POINT *) MALLOC( nbpoints * sizeof (POINT)); 
  if ( points == 0) { sciprint(MESSAGE4);return(0);}
  else return(1);
}
/*-----------------------------------------------------------------------------------*/
static POINT *C2F(ReturnPoints)() { return(points); }

/**  Clipping functions **/
/** XXXX a isoler des p'eripheriques car c'est utilis'e dans plot3d **/

/* My own clipping routines  
   XDrawlines with clipping on the current graphic window 
   to ovoid trouble on some X servers **/

static integer xleft,xright,ybot,ytop;

/* Test a single point to be within the xleft,xright,ybot,ytop bbox.
 * Sets the returned integers 4 l.s.b. as follows:
 * bit 0 if to the left of xleft.
 * bit 1 if to the right of xright.
 * bit 2 if below of ybot.
 * bit 3 if above of ytop.
 * 0 is returned if inside.
 */
/*-----------------------------------------------------------------------------------*/
static int clip_point( integer x,integer y)
{
  integer ret_val = 0;
  if (x < xleft) ret_val |= (char)0x01;
  else if (x > xright) ret_val |= (char)0x02;
  if (y < ybot) ret_val |= (char)0x04;
  else if (y > ytop) ret_val |= (char)0x08;
  return ret_val;
}
/*-----------------------------------------------------------------------------------*/
/* Clip the given line to drawing coords defined as xleft,xright,ybot,ytop.
 *   This routine uses the cohen & sutherland bit mapping for fast clipping -
 * see "Principles of Interactive Computer Graphics" Newman & Sproull page 65.
 return 0  : segment out 
 1  : (x1,y1) changed 
 2  : (x2,y2) changed 
 3  : (x1,y1) and (x2,y2) changed 
 4  : segment in 
*/


void set_clip_box(xxleft,xxright,yybot,yytop)
     integer xxleft,xxright,yybot,yytop;
{
  xleft=xxleft;
  xright=xxright;
  ybot=yybot;
  ytop=yytop;
}
/*-----------------------------------------------------------------------------------*/
// Prototype de clip_line : cf. Graphics.h F.Leray 18.02.04
/*
  extern void clip_line  __PARAMS((integer,integer,integer ,integer,integer *,integer *,integer *,integer *, integer *));  
*/

void
clip_line(x1, yy1, x2, y2, x1n, yy1n, x2n, y2n, flag)
     integer x1, yy1, x2, y2, *flag, *x1n, *yy1n, *x2n, *y2n;
{
  integer x, y, dx, dy, x_intr[2], y_intr[2], count, pos1, pos2;

  *x1n=x1; *yy1n=yy1; *x2n=x2; *y2n=y2; *flag=4;
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
      x = (int) ((ybot - y2)  * ((double) dx / (double) dy) + x2);
      /* Test for ybot boundary. */
      if (x >= xleft && x <= xright) {
	x_intr[count] = x;
	y_intr[count++] = ybot;
      }
      x = (int) ((ytop - y2) * ((double) dx / (double) dy) + x2); 
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
	  y = (int) ((xleft - x2) * ((double) dy / (double) dx) + y2);   
	  /* Test for xleft boundary. */
	  if (y >= ybot && y <= ytop) {
	    x_intr[count] = xleft;
	    y_intr[count++] = y;
	  }
	  if ( count < 2 ) 
	    {
	      y = (int) ((xright - x2) * ((double) dy / (double) dx) + y2);  
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


/*-----------------------------------------------------------------------------------*/

/* 
 *  returns the first (vx[.],vy[.]) point inside 
 *  xleft,xright,ybot,ytop bbox. begining at index ideb
 *  or zero if the whole polyline is out 
 */

integer first_in(n, ideb, vx, vy)
     integer n;
     integer ideb;
     integer *vx;
     integer *vy;
{
  integer i;
  for (i=ideb  ; i < n ; i++)
    {
      if (vx[i]>= xleft && vx[i] <= xright  && vy[i] >= ybot && vy[i] <= ytop)
			{
				return(i);
			}
    }
  return(-1);
}
/*-----------------------------------------------------------------------------------*/
/* 
 *  returns the first (vx[.],vy[.]) point outside
 *  xleft,xright,ybot,ytop bbox.
 *  or zero if the whole polyline is out 
 */

integer first_out(n, ideb, vx, vy)
     integer n;
     integer ideb;
     integer *vx;
     integer *vy;
{
  integer i;
  for (i=ideb  ; i < n ; i++)
    {
      if ( vx[i]< xleft || vx[i]> xright  || vy[i] < ybot || vy[i] > ytop) return(i);
    }
  return(-1);
}
/*-----------------------------------------------------------------------------------*/
int CheckScilabXgc()
{
  return( ScilabXgc != (struct BCG *) 0);
}
/*-----------------------------------------------------------------------------------*/
/* NG beg */
static void C2F(setscilabFigure)(integer *v1,integer *v2,integer *v3,integer *v4,integer *v5,integer *v6,double *figure)
{
  figure=(double *)ScilabXgc->mafigure;
}
/*-----------------------------------------------------------------------------------*/
static void C2F(getscilabFigure)(integer *verbose, integer *x,integer *narg, double *figure)
{   
  figure=(double *)ScilabXgc->mafigure;
}
/*-----------------------------------------------------------------------------------*/
static void C2F(setscilabVersion)(integer *vers, integer *v2, integer *v3, integer *v4)
{
  ScilabXgc->graphicsversion=*vers;
}
/*-----------------------------------------------------------------------------------*/
static void C2F(getscilabVersion)(integer *verbose, integer *vers, integer *narg, double *dummy)
{   
  *vers = ScilabXgc->graphicsversion;
}
/*-----------------------------------------------------------------------------------*/
static void C2F(setscilabxgc)(integer *v1, integer *v2, integer *v3, integer *v4)
{

}
/*-----------------------------------------------------------------------------------*/
static void C2F(getscilabxgc)(integer *verbose, integer *x,integer *narg, double *dummy)
{   
  double **XGC;
  XGC=(double **)dummy;
  *XGC= (double *)ScilabXgc;
}
/*-----------------------------------------------------------------------------------*/
