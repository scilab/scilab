#include "wgraph.h"
#include "resource.h"
#include "Events.h"

#include "Messages.h"
#include "Warnings.h"
#include "Errors.h"
#include "WindowList.h"
#include "periScreen.h"
#include "Actions.h"
#include "periScreen.h"
#include "cluni0.h"

#include "win_mem_alloc.h" /* MALLOC */
/*-----------------------------------------------------------------------------------*/
extern HINSTANCE hdllInstance;
extern TW textwin;
extern GW graphwin;
extern void sci_pixmapclear(HDC hdc_c, struct BCG *ScilabGC );
extern void sci_pixmapclear_rect(HDC hdc_c, struct BCG *ScilabGC,int w,int h); 
extern void sci_pixmap_resize(struct BCG * ScilabGC, int x, int y) ;
extern void   set_no_delete_win_mode() ;
extern void DebugGW (char *fmt,...);
extern void DebugGW1 (char *fmt,...);
extern int check_pointer_win __PARAMS ((int *x1,int *y1,int *win));
extern void delete_sgwin_entities(int win_num);
extern EXPORT LRESULT CALLBACK WndGraphProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
extern EXPORT LRESULT CALLBACK WndParentGraphProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
extern void ExportBMP(struct BCG *ScilabGC,char *pszflname);
extern void Setscig_buzyState(BOOL state);
extern Scig_deletegwin_handler scig_deletegwin_handler;
extern Scig_deletegwin_handler set_scig_deletegwin_handler(Scig_deletegwin_handler f);
extern void SetGHdc(HDC lhdc,int width,int height);
/*-----------------------------------------------------------------------------------*/
static void sci_extra_margin(HDC hdc , struct BCG *ScilabGC);
/*-----------------------------------------------------------------------------------*/
int C2F (deletewin) (integer * number)
{
  double dv=0;
  int v=0;
  int num = *number;

  /* destroying recorded graphic commands */
  scig_erase (num);
  /* delete the windows and resources */
  DeleteObjs(num);
  scig_deletegwin_handler (num);
  DeleteSGWin (num); /* Here we 1) destroy the ScilabXgc (set to NULL) if it is the last window in the list */
                        /*         2) or reset the ScilabXgc to the next one see DeleteSGWin*/

  
  /* So, we use another flag named v_flag :*/
  delete_sgwin_entities(num);
  return (0);
}
/*-----------------------------------------------------------------------------------*/
/****************************************************
 * Debug Function 
 ****************************************************/
extern void DebugGW (char *fmt,...)
{
#ifdef DEBUG
  int count;
  char buf[MAXPRINTF];
  va_list args;
  va_start (args, fmt);
  count = vsprintf (buf, fmt, args);
  MessageBox (textwin.hWndParent, (LPSTR) buf,
	      textwin.Title, MB_ICONEXCLAMATION);
  va_end (args);
#endif
}
/*-----------------------------------------------------------------------------------*/
extern void DebugGW1 (char *fmt,...)
{
  int  count;
  char buf[MAXPRINTF];
  va_list args;
  va_start (args, fmt);
  count = vsprintf (buf, fmt, args);
  MessageBox (textwin.hWndParent, (LPSTR) buf,
	      textwin.Title, MB_ICONEXCLAMATION);
  va_end (args);
}
/*-----------------------------------------------------------------------------------*/
/**SciViewportMove
 *@description: used to move the panner and the viewport interactively 
 *              through scilab command.
 *
 *@input: struct BCG *ScilabGC : structure associated to a Scilab 
Graphic window
 *        int x,y : the x,y point of the graphic window to be moved at 
 *        the up-left position of the viewport
 *
 *@output: NONE
 *
 *@author: Matthieu PHILIPPE
 *@date: Dec 1999
 **/
void SciViewportMove (struct BCG *ScilabGC,int x, int y)
{
  SCROLLINFO vertsi;
  SCROLLINFO horzsi;

  if (ScilabGC != NULL)
    {
      sciGetScrollInfo (ScilabGC, SB_VERT, &vertsi);
      sciGetScrollInfo (ScilabGC, SB_HORZ, &horzsi);
      /* MAJ D.ABDEMOUCHE*/
      if (ScilabGC->CurResizeStatus == 0)
	{
	  horzsi.nPos = max (horzsi.nMin, min (horzsi.nMax, x));
	  sciSetScrollInfo (ScilabGC, SB_HORZ, &horzsi, TRUE);
	  vertsi.nPos = min (vertsi.nMax, max (vertsi.nMin, y));
	  sciSetScrollInfo (ScilabGC, SB_VERT, &vertsi, TRUE);
	  InvalidateRect (ScilabGC->CWindow, (LPRECT) NULL, FALSE);
	}
    }
}
/*-----------------------------------------------------------------------------------*/
/**SciViewportGet
 *@description: used to get panner position through scilab command.
 *
 *@input: struct BCG *ScilabGC : structure associated to a Scilab 
Graphic window
 *        int x,y : the x,y point of the graphic window to be moved at 
 *        the up-left position of the viewport
 *
 *@output: NONE
 *
 *@author: Matthieu PHILIPPE
 *@date: Dec 1999
 **/
void SciViewportGet (struct BCG *ScilabXgc,int *x,int *y)
{
  if (ScilabXgc != NULL)
    {
      *x = ScilabXgc->horzsi.nPos;
      *y = ScilabXgc->vertsi.nPos;
    }
  else
    {
      *x = 0;
      *y = 0;
    }
}
/*-----------------------------------------------------------------------------------*/
/**GPopupResize
 *@description: a little beat different to windowdim. GPopupResize sets
 * the visible window (parents dimension)
 *
 *@see: setwindowdim
 *
 **/
void GPopupResize (struct BCG * ScilabXgc,int * width,int * height)
{
  RECT rect, rect1;
  int xof, yof;
  SCROLLINFO vertsi;
  SCROLLINFO horzsi;
  int *x = width; /* F.Leray 01.07.04 copy the setwindowdim function philosohpy (see periwin.c)*/
  int *y = height;

 if ( (ScilabXgc->CWindow != NULL) && (ScilabXgc->hWndParent != NULL) )
  {
    /* initialisation des variables SCROLLINFO*/
    sciGetScrollInfo(ScilabXgc, SB_VERT, &vertsi);
    sciGetScrollInfo(ScilabXgc, SB_HORZ, &horzsi);
    
    ScilabXgc->CWindowWidth = Max((int) *x,50);
    ScilabXgc->CWindowHeight =Max((int) *y,50);
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
/****************************************************
 * Drawing graphic window 
 ****************************************************/
/* utility for backing store simulation */
/* A améliorer pour que le bitmap ne soit pas plus grand que nécessaire 
 * ou au moins qu'il utilise rcpaint comme clipregion. 
 */
static void ScilabPaintWithBitmap(HWND hwnd,HDC hdc , struct BCG *ScilabGC)
{
  static HDC hdc_compat = NULL; 
  static HBITMAP hbmTemp = NULL; 
  static int hbm_width=0, hbm_height = 0;
  int win_w,win_h;
  HBITMAP hbmSave;

  if ( hdc_compat == NULL) 
    {
      if (( hdc_compat = CreateCompatibleDC (hdc)) == NULL)
	{
	  sciprint(MSG_WARNING17);
	  return ;
	}
      SetMapMode(hdc_compat, MM_TEXT);
      SetBkMode(hdc_compat,TRANSPARENT);
      SetTextAlign(hdc_compat, TA_LEFT|TA_BOTTOM);
    } 
  /* be sure that a previous clip does not remain */
  win_w= Max(ScilabGC->CWindowWidth,ScilabGC->CWindowWidthView);
  win_h= Max(ScilabGC->CWindowHeight,ScilabGC->CWindowHeightView);
  if ( hbmTemp != NULL ) 
    {
      if ( hbm_width < win_w || hbm_height < win_h )
	{
	  hbm_width = win_w;
	  hbm_height = win_h;
	  hbmTemp =CreateCompatibleBitmap (hdc,hbm_width,hbm_height);
	}
    }
  else
    {
      hbm_width = win_w;
      hbm_height = win_h;
      hbmTemp =CreateCompatibleBitmap (hdc,hbm_width,hbm_height);
    }
  
  if (!hbmTemp)
    {
      sciprint(MSG_WARNING18);
      return ; 
    }
  hbmSave = SelectObject ( hdc_compat, hbmTemp);
  if ( hbmSave != NULL) DeleteObject ( hbmSave ); hbmSave = NULL;

  SelectClipRgn(hdc_compat,NULL);
  sci_pixmapclear(hdc_compat,ScilabGC); 
  scig_replay_hdc('W',ScilabGC->CurWindow,hdc_compat,ScilabGC->CWindowWidth,
		  ScilabGC->CWindowHeight,1);
  SelectClipRgn(hdc_compat,NULL);
  /* painting extra space in gray if necessary */ 
  sci_extra_margin(hdc_compat ,ScilabGC);

  /* be sure that there's no active clip set by graphics */ 
  if ( ScilabGC->ClipRegionSet == 1 )  SelectClipRgn(hdc,NULL);
  /* the grahics */
  BitBlt(hdc,ScilabGC->horzsi.nPos,ScilabGC->vertsi.nPos,
	 ScilabGC->CWindowWidthView, ScilabGC->CWindowHeightView,
	 hdc_compat,ScilabGC->horzsi.nPos,ScilabGC->vertsi.nPos,
	 SRCCOPY); 
}
/*-----------------------------------------------------------------------------------*/
static void sci_extra_margin(HDC hdc_c , struct BCG *ScilabGC)
{
  if (  ScilabGC->CWindowWidth-ScilabGC->vertsi.nPos < ScilabGC->CWindowWidthView) 
    {
      RECT rect;
      HBRUSH hBrush =  GetStockBrush(GRAY_BRUSH); 
      rect.left   = ScilabGC->CWindowWidth;
      rect.top    = ScilabGC->vertsi.nPos ;
      rect.right  = ScilabGC->horzsi.nPos + ScilabGC->CWindowWidthView;
      rect.bottom = ScilabGC->vertsi.nPos + ScilabGC->CWindowHeightView;
      FillRect( hdc_c , &rect,hBrush );
    }
  if (  ScilabGC->CWindowHeight-ScilabGC->vertsi.nPos < ScilabGC->CWindowHeightView) 
    {
      RECT rect;
      HBRUSH hBrush =  GetStockBrush(GRAY_BRUSH); 
      rect.left   = ScilabGC->horzsi.nPos ;
      rect.top    = ScilabGC->CWindowHeight;
      rect.right  = ScilabGC->horzsi.nPos + ScilabGC->CWindowWidthView;
      rect.bottom = ScilabGC->vertsi.nPos + ScilabGC->CWindowHeightView;
      FillRect( hdc_c , &rect,hBrush );
    }
}
/*-----------------------------------------------------------------------------------*/
/* the paint function */
void ScilabPaint (HWND hwnd, struct BCG *ScilabGC)
{
  /* static paint = 0; */
  HDC hdc;
  PAINTSTRUCT ps;
  
  Setscig_buzyState(TRUE);
	hdc = BeginPaint(hwnd, &ps);  
  
  if (ScilabGC->Inside_init == 1) goto paint_end;
  
  if ( ScilabGC->in_sizemove == 1) goto paint_end;

  SetMapMode (hdc, MM_TEXT);
  SetBkMode (hdc, TRANSPARENT);
  /* GetClientRect (hwnd, &rect);
   * SetViewportExtEx(hdc, rect.right, rect.bottom,NULL);*/
  SetViewportExtEx(hdc, ScilabGC->CWindowWidthView,ScilabGC->CWindowHeightView, NULL);
  /* MAJ D.ABDEMOUCHE*/
  if (ScilabGC->CurResizeStatus == 0)
    {
      /* xViewport = xWindow  + xViewOrg 
       * the viewport = [ScilabGC->horzsi.nPos,ScilabGC->vertsi.nPos,
       *                 ScilabGC->CWindowWidthView,ScilabGC->CWindowHeightView]
       */
      SetViewportOrgEx (hdc, -ScilabGC->horzsi.nPos,-ScilabGC->vertsi.nPos, NULL);
    }
  if (  ScilabGC->CurPixmapStatus == 1 )
    {
      /* pixmap status is on we use it for redrawing */ 
      if (ScilabGC->CurResizeStatus == 1)/* MAJ D.ABDEMOUCHE*/
	{
	  SelectClipRgn(ScilabGC->hdcCompat,NULL);
	  scig_replay_hdc('W',ScilabGC->CurWindow,ScilabGC->hdcCompat,
			  ScilabGC->CWindowWidth, ScilabGC->CWindowHeight,
			  1);
	}
      /* be sure that there's no active clip set by graphics */ 
      if ( ScilabGC->ClipRegionSet == 1 ) SelectClipRgn(hdc,NULL);
      BitBlt(hdc,ScilabGC->horzsi.nPos,ScilabGC->vertsi.nPos,
	     ScilabGC->CWindowWidthView,ScilabGC->CWindowHeightView,
	     ScilabGC->hdcCompat,ScilabGC->horzsi.nPos,ScilabGC->vertsi.nPos,
	     SRCCOPY); 
      sci_extra_margin(hdc, ScilabGC);
    }
  else 
    {
      /* no backing store thus we must redraw */
      if (  emulate_backing_store == 1 ) 
	{
	  ScilabPaintWithBitmap (hwnd,hdc,ScilabGC);
	}
      else 
	{
	  if ( ScilabGC->ClipRegionSet == 1 )  SelectClipRgn(hdc,NULL);
	  scig_replay_hdc('W',ScilabGC->CurWindow,hdc,
			  ScilabGC->CWindowWidth, ScilabGC->CWindowHeight,
			  1);
	  if ( ScilabGC->ClipRegionSet == 1 )  SelectClipRgn(hdc,NULL);
	  /* painting extra space in gray if necessary */ 
	  sci_extra_margin(hdc ,ScilabGC);
	}
    }
  paint_end : 
    {
      EndPaint(hwnd, &ps);
      Setscig_buzyState(FALSE);
    }
}
/*-----------------------------------------------------------------------------------*/
static void ScilabNoPaint (HWND hwnd, struct BCG *ScilabGC)
{
  HDC hdc;
  PAINTSTRUCT ps;
  hdc = BeginPaint(hwnd, &ps);  
  EndPaint(hwnd, &ps);
}
/*-----------------------------------------------------------------------------------*/
/****************************************************
 * Resize 
 ****************************************************/
int ScilabGResize (HWND hwnd, struct BCG *ScilabGC, WPARAM wParam)
{
  /** We do not paint just check if we must resize the pixmap  **/
  if ( ScilabGC->Inside_init != 1
      && ((wParam == SIZE_MAXIMIZED) || (wParam == SIZE_RESTORED)))
    {
      HBITMAP hbmTemp;
      HBITMAP  hbmSave;
      HDC hdc1;
      hdc1 = TryToGetDC (ScilabGC->CWindow);
      hbmTemp = CreateCompatibleBitmap (hdc1,ScilabGC->CWindowWidth,
					ScilabGC->CWindowHeight);
	  ReleaseDC(ScilabGC->CWindow,hdc1);
      if (hbmTemp == NULL )
	{
	  sciprint(MSG_WARNING19);
	  return FALSE;
	}
      hbmSave = SelectObject ( ScilabGC->hdcCompat, hbmTemp);
      if ( ScilabGC->hbmCompat != NULL)
	DeleteObject (ScilabGC->hbmCompat);
      ScilabGC->hbmCompat = hbmTemp;
      if ( ScilabGC->ClipRegionSet == 1 )  SelectClipRgn(ScilabGC->hdcCompat,NULL);
      sci_pixmapclear(ScilabGC->hdcCompat,ScilabGC); 
      
      return TRUE;
    }
  return FALSE;
}
/*-----------------------------------------------------------------------------------*/
/********************************************************
 * A special Replay for win95 
 * we want to replay with a specific hdc 
 * and reset the hdc to its current value when leaving 
 * we must also protect the alufunction current value 
 *  ( for application like gr_menu or scicos : where 
 *    a redraw can occurs while we are using a alufunction 
 *    in a graphic mode without xtape )
 ********************************************************/
void scig_replay_hdc (char c, integer win_num, HDC hdc, int width,int height,  int scale)
{
  integer verb = 0, cur, na;
  char name[4];
  integer alu, narg, verbose = 0;
  GetDriver1 (name, PI0, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0);
  /** Warning : We use a driver which does not touch to hdc **/
  C2F (SetDriver) ("Int", PI0, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0);
  C2F (dr) ("xget", "window", &verb, &cur, &na, PI0, PI0, PI0, PD0, 
	    PD0, PD0, PD0, 0L, 0L);
  C2F (dr) ("xset", "window", &win_num, PI0, PI0, PI0, PI0, PI0, PD0, 
	    PD0, PD0, PD0, 0L, 0L);
  C2F (dr) ("xget", "alufunction", &verbose, &alu, &narg, PI0, PI0, 
	    PI0, PD0, PD0, PD0, PD0, 0L, 0L);
  SetGHdc (hdc, width, height);
  /** new font for printers **/
  if (c == 'P')
    SciG_Font_Printer (scale);
  /** the create default font/brush etc... in hdc */
  ResetScilabXgc ();
  /** xclear will properly upgrade background if necessary **/
  if (sciGetPixmapStatus() != 1)
    C2F (dr) ("xclear", "v", PI0, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, 
	      PD0, 0L, 0L);

  sciRedrawF(&win_num); /* NG */
  C2F (dr1) ("xset", "alufunction", &alu, PI0, PI0, PI0, PI0, PI0, PD0, 
	     PD0, PD0, PD0, 0L, 0L);
  C2F (dr) ("xset", "window", &cur, PI0, PI0, PI0, PI0, PI0, PD0, PD0, 
	    PD0, PD0, 0L, 0L);
  C2F (SetDriver) (name, PI0, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, 
		   PD0);
  SetGHdc ((HDC) 0, 600, 400);
  /** back to usual font size **/
  SciG_Font ();
  SwitchWindow (&cur);
}
/*-----------------------------------------------------------------------------------*/
/* A.C Aout 2004 */
int GetScreenProperty(char *prop, char *value)
{
  POINT Ecran = {GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)} ;
  HDC hdc=GetDC(NULL);
  if(!strcmp(prop,"screensize_px"))
    {
      sprintf(value,"%f|%f|%f|%f",(float)1,(float)1,
	      (float)Ecran.x,
	      (float)Ecran.y);
    }
  else if(!strcmp(prop,"screensize_mm"))
    {
      sprintf(value,"%f|%f|%f|%f",(float)0,(float)0,
	      (float)GetDeviceCaps(hdc,HORZSIZE),
	      (float)GetDeviceCaps(hdc,VERTSIZE));
    }
  else if(!strcmp(prop,"screensize_cm"))
    {
      sprintf(value,"%f|%f|%f|%f",(float)0,(float)0,
	      (float)GetDeviceCaps(hdc,HORZSIZE)/10,
	      (float)GetDeviceCaps(hdc,VERTSIZE)/10);
    }
  else if(!strcmp(prop,"screensize_in"))
    {
      sprintf(value,"%f|%f|%f|%f",(float)0,(float)0,
								  (float)GetDeviceCaps(hdc,HORZSIZE)/25.4,
								  (float)GetDeviceCaps(hdc,VERTSIZE)/25.4);
    }
  else if(!strcmp(prop,"screensize_pt"))
    {
      sprintf(value,"%f|%f|%f|%f",(float)0,(float)0,
								  (float)GetDeviceCaps(hdc,HORZSIZE)/25.4*72,
								  (float)GetDeviceCaps(hdc,VERTSIZE)/25.4*72);
    }
  else if(!strcmp(prop,"screensize_norm"))
    {
      sprintf(value,"%f|%f|%f|%f",(float)0,(float)0,(float)1,(float)1);
    }
  else if(!strcmp(prop,"screendepth"))
    {
      sprintf(value,"%f",(float)GetDeviceCaps(hdc,BITSPIXEL));
    }
  else
    {
      return -1;
    }
  ReleaseDC (NULL, hdc);
  return 0;
}


/*-----------------------------------------------------------------------------------*/
/* Scilab get the DPI (root properties) */
/* F.Leray 08.03.05 */
/* return the x/y DPI */
int GetScreenDPI(int *ixres, int *iyres)
{
  POINT Ecran = {GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)} ;
  HDC hdc=GetDC(NULL);

  *ixres = GetDeviceCaps(hdc, LOGPIXELSX);
  *iyres = GetDeviceCaps(hdc, LOGPIXELSY);
  
  ReleaseDC (NULL, hdc);
  return 0;
}

/*-----------------------------------------------------------------------------------*/
integer GetCurrentFigureWindows(void)
{
	integer verb = 0;
	integer CurrentWindow=0;
	integer na=0;
	/* Backup Current Figure */
	C2F (dr) ("xget", "window", &verb, &CurrentWindow, &na, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 0L, 0L);
	return CurrentWindow;
}
/*-----------------------------------------------------------------------------------*/
void SetCurrentFigureWindows(integer win)
{
	scig_sel (win);
}
/*-----------------------------------------------------------------------------------*/
