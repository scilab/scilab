/*******************************************
 * Original source : GNUPLOT - win/wgraph.c
 * modified for Scilab 
 *******************************************
 *
 * Copyright (C) 1992   Maurice Castro, Russell Lang
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
 * AUTHORS (GNUPLOT) 
 *   Maurice Castro
 *   Russell Lang
 *
 * Modifications for Scilab 
 *   Jean-Philipe Chancelier 
 *   Bugs and mail : Scilab@inria.fr 
 */
#ifndef STRICT
#define STRICT
#endif
#include <windows.h>
#include <windowsx.h>
#include <commdlg.h>
#include <shellapi.h>

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>

#include "wresource.h"
#include "wcommon.h"
#include "../graphics/scigraphic.h"
#include "../graphics/Graphics.h"
#include "wgraph.h"
extern void SetGHdc __PARAMS ((HDC lhdc, int width, int height));
static void scig_replay_hdc (char c, integer win_num, HDC hdc, int width, int height,
			     int scale);
extern int check_pointer_win __PARAMS ((int *x1,int *y1,int *win));
extern TW textwin;

EXPORT LRESULT CALLBACK WndGraphProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
void ReadGraphIni (struct BCG *ScilabGC);
void WriteGraphIni (struct BCG *ScilabGC);

static int scig_buzy = 0;

/******************************************
 * delete a graphic window
 ******************************************/

/* add handlers for delete action */

void scig_deletegwin_handler_none (int win)
{
};
void scig_deletegwin_handler_sci (int win)

{
  static char buf[256];
  struct BCG *SciGc;

  SciGc = GetWindowXgcNumber(win);
  if (strlen(SciGc->EventHandler)!=0) {
    sprintf(buf,"%s(%d,0,0,-1000)",SciGc->EventHandler,win);
    StoreCommand1(buf,2);
    }
};
static Scig_deletegwin_handler scig_deletegwin_handler = scig_deletegwin_handler_sci;
/*static Scig_deletegwin_handler scig_deletegwin_handler = scig_deletegwin_handler_none;*/

Scig_deletegwin_handler set_scig_deletegwin_handler (f)
     Scig_deletegwin_handler f;
{
  Scig_deletegwin_handler old = scig_deletegwin_handler;
  scig_deletegwin_handler = f;
  return old;
}

void reset_scig_deletegwin_handler ()
{
  scig_deletegwin_handler = scig_deletegwin_handler_sci;
}

static int sci_graphic_protect = 0;

void   set_delete_win_mode() {  sci_graphic_protect = 0 ;}
extern void   set_no_delete_win_mode()  {  sci_graphic_protect = 1 ;}

int C2F (deletewin) (integer * number)
{
  /* destroying recorded graphic commands */
  scig_erase (*number);
  /* delete the windows and resources */
  if (version_flag()==0) DeleteObjs(*number);
  scig_deletegwin_handler (*number);
  DeleteSGWin (*number);
  return (0);
}

/******************************************
 * Printing and redrawing graphic window 
 ******************************************/

EXPORT void WINAPI
  GraphPrint (struct BCG *ScilabGC)
{
  if (ScilabGC->CWindow && IsWindow (ScilabGC->CWindow))
    SendMessage (ScilabGC->CWindow, WM_COMMAND, M_PRINT, 0L);
}

EXPORT void WINAPI
  GraphRedraw (struct BCG *ScilabGC)
{
  if (ScilabGC->CWindow && IsWindow (ScilabGC->CWindow))
    SendMessage (ScilabGC->CWindow, WM_COMMAND, M_REBUILDTOOLS, 0L);
}

/****************************************
 * copy graph window to clipboard 
 * with the EnHmetafile format (win95/winNT)
 ****************************************/

void NewCopyClip (struct BCG *ScilabGC)
{
  LPGW lpgw;
  RECT rect;
  HANDLE hmf;
  HWND hwnd;
  HDC hdc;
  if (scig_buzy == 1)
    return;
  scig_buzy = 1;
  lpgw = ScilabGC->lpgw;
  hwnd = ScilabGC->CWindow;
  /* view the window */
  if (IsIconic (hwnd))
    ShowWindow (hwnd, SW_SHOWNORMAL);
  BringWindowToTop (hwnd);
  UpdateWindow (hwnd);
  GetClientRect (hwnd, &rect);
  hdc = CreateEnhMetaFile (NULL, NULL, NULL, NULL);
  SetMapMode (hdc, MM_TEXT);
  SetTextAlign (hdc, TA_LEFT | TA_BOTTOM);
  SetWindowExtEx (hdc, rect.right - rect.left,
		  rect.bottom - rect.top, (LPSIZE) NULL);
  /** 
  SetWindowExtEx(hdc, rect.right, rect.bottom, (LPSIZE)NULL); 
  **/
/** fix hdc in the scilab driver **/
  Rectangle (hdc, 0, 0, ScilabGC->CWindowWidthView, ScilabGC->CWindowHeightView);
  scig_replay_hdc ('C', ScilabGC->CurWindow, hdc,
		   ScilabGC->CWindowWidth, ScilabGC->CWindowHeight, 1);
  hmf = CloseEnhMetaFile (hdc);
  OpenClipboard (hwnd);
  EmptyClipboard ();
  SetClipboardData (CF_ENHMETAFILE, hmf);
  CloseClipboard ();
  scig_buzy = 0;
  return;
}

/****************************************
 * copy graph window to clipboard 
 * copy a CF_METAFILEPICT and a CF_BITMAP to the clipboard 
 ****************************************/

void CopyClip (struct BCG *ScilabGC)
{
  LPGW lpgw;
  HDC mem;
  RECT rect;
  HBITMAP bitmap;
  HANDLE hmf;
  HGLOBAL hGMem;
/** XXXX  GLOBALHANDLE hGMem;**/
  LPMETAFILEPICT lpMFP;
  HWND hwnd;
  HDC hdc;
  if (scig_buzy == 1)
    return;
  scig_buzy = 1;
  lpgw = ScilabGC->lpgw;
  hwnd = ScilabGC->CWindow;
  /* view the window */
  if (IsIconic (hwnd))
    ShowWindow (hwnd, SW_SHOWNORMAL);
  BringWindowToTop (hwnd);
  UpdateWindow (hwnd);
  /* get the context */
  hdc = GetDC (hwnd);
  GetClientRect (hwnd, &rect);
  /* make a bitmap and copy it there */
  mem = CreateCompatibleDC (hdc);
  bitmap = CreateCompatibleBitmap (hdc, rect.right - rect.left,
				   rect.bottom - rect.top);
  if (bitmap)
    {
      /* there is enough memory and the bitmaps OK */
      SelectBitmap (mem, bitmap);
      BitBlt (mem, 0, 0, rect.right - rect.left,
	      rect.bottom - rect.top, hdc, rect.left,
	      rect.top, SRCCOPY);
    }
  else
    {
      MessageBeep (MB_ICONHAND);
      MessageBox (hwnd, "Insufficient Memory to Copy Clipboard",
		  lpgw->Title, MB_ICONHAND | MB_OK);
    }
  DeleteDC (mem);
  ReleaseDC (hwnd, hdc);
  hdc = CreateMetaFile ((LPSTR) NULL);

/** SetMapMode(hdc, MM_ANISOTROPIC); **/

  SetMapMode (hdc, MM_TEXT);
  SetTextAlign (hdc, TA_LEFT | TA_BOTTOM);
  SetWindowExtEx (hdc, rect.right - rect.left,
		  rect.bottom - rect.top, (LPSIZE) NULL);
/** fix hdc in the scilab driver **/
  scig_replay_hdc ('C', ScilabGC->CurWindow, hdc,
		   rect.right - rect.left, rect.bottom - rect.top, 1);
  hmf = CloseMetaFile (hdc);
  hGMem = GlobalAlloc (GMEM_MOVEABLE, (DWORD) sizeof (METAFILEPICT));
  lpMFP = (LPMETAFILEPICT) GlobalLock (hGMem);
  /* in MM_ANISOTROPIC, xExt & yExt give suggested size in 0.01mm units */
  hdc = GetDC (hwnd);
  lpMFP->mm = MM_ANISOTROPIC;
  lpMFP->xExt = MulDiv (rect.right - rect.left, 2540, GetDeviceCaps (hdc, LOGPIXELSX));
  lpMFP->yExt = MulDiv (rect.bottom - rect.top, 2540, GetDeviceCaps (hdc, LOGPIXELSX));
  lpMFP->hMF = hmf;
  ReleaseDC (hwnd, hdc);
  GlobalUnlock (hGMem);
  OpenClipboard (hwnd);
  EmptyClipboard ();
  SetClipboardData (CF_METAFILEPICT, hGMem);
  SetClipboardData (CF_BITMAP, bitmap);
  CloseClipboard ();
  scig_buzy = 0;
  return;
}

/****************************************
 * copy graph window to printer         *
 ****************************************/

/* PageGDICalls : only for testing */
#ifdef PageTest
static void PageGDICalls (HDC hdcPrn, int cxPage, int cyPage)
{
  static char szTextStr[] = "Hello, Printer!";
  SetMapMode (hdcPrn, MM_ISOTROPIC);
  SetWindowExtEx (hdcPrn, 1000, 1000, NULL);
  SetViewportExtEx (hdcPrn, cxPage / 2, -cyPage / 2, NULL);
  SetViewportOrgEx (hdcPrn, cxPage / 2, cyPage / 2, NULL);
  Ellipse (hdcPrn, -500, 500, 500, -500);
  SetTextAlign (hdcPrn, TA_LEFT | TA_BOTTOM);
  TextOut (hdcPrn, 0, 0, szTextStr, sizeof (szTextStr) - 1);
}
#endif

int CopyPrint (struct BCG *ScilabGC)
{
  int xPage, yPage;
  static DOCINFO di =
  {sizeof (DOCINFO), "Scilab: Printing", NULL};
  BOOL bError = FALSE;
  HDC printer;
  LPGW lpgw;
  ABORTPROC lpfnAbortProc;
  DLGPROC lpfnPrintDlgProc;
  static PRINTDLG pd;
  HWND hwnd;
  RECT rect;
  PRINT pr;
  if (scig_buzy == 1)
    return TRUE;
  scig_buzy = 1;
  lpgw = ScilabGC->lpgw;
  hwnd = ScilabGC->CWindow;
#ifdef __GNUC__
/** for cygwin and mingwin **/
/** XXXXX : Bug in cygwin : sizeof(PRINTDLG) gives something wrong (68) **/
  memset (&pd, 0, 66);
  pd.lStructSize = 66;
#else
  memset (&pd, 0, sizeof (PRINTDLG));
  pd.lStructSize = sizeof (PRINTDLG);
#endif
  pd.hwndOwner = hwnd;
  pd.hDevMode = NULL;
  pd.hDevNames = NULL;
  pd.hDC = NULL;
  pd.Flags = PD_ALLPAGES | PD_COLLATE | PD_RETURNDC;
  pd.nFromPage = 0;
  pd.nToPage = 0;
  pd.nMinPage = 0;
  pd.nMaxPage = 0;
  pd.nCopies = 1;
  pd.hInstance = NULL;
  pd.lCustData = 0L;
  pd.lpfnPrintHook = NULL;
  pd.lpfnSetupHook = NULL;
  pd.lpPrintTemplateName = NULL;
  pd.lpSetupTemplateName = NULL;
  pd.hPrintTemplate = NULL;
  pd.hSetupTemplate = NULL;

  if (PrintDlg (&pd) == FALSE)
    {
      /** int i;
      i=CommDlgExtendedError();
      sciprint("Printer Menu error code %d\r\n",i);
      **/
      scig_buzy = 0;
      return TRUE;
    }
  printer = pd.hDC;
  if (NULL == printer)
    {
      sciprint ("Can't print \r\n");
      scig_buzy = 0;
      return TRUE;		/* abort */
    }

  pr.hdcPrn = printer;
  xPage = GetDeviceCaps (pr.hdcPrn, HORZRES);
  yPage = GetDeviceCaps (pr.hdcPrn, VERTRES);

  GetClientRect (hwnd, &rect);
  SetLastError (0);
  /** Warning : 
    inside PrintDlgProg we use GetWindowLong(GetParent(hdlg), 4);
    to get the parent window of the print dialog box 
    GetParent(hdlg) returns ScilabGC->hWndParent and not hwnd = ScilabGC->CWindow 
    as we would expect ? 
    So we call SetwindowLong with ScilabGC->hWndParent also 
    **/
  if (SetWindowLong (hwnd, 4, (LONG) ((LPPRINT) & pr)) == 0
      && GetLastError () != 0)
    {
      sciprint ("Can't print : Error in SetWindowLong");
      scig_buzy = 0;
      return TRUE;
    }
  if (SetWindowLong (ScilabGC->hWndParent, 4, (LONG) ((LPPRINT) & pr)) == 0
      && GetLastError () != 0)
    {
      sciprint ("Can't print : Error in SetWindowLong");
      scig_buzy = 0;
      return TRUE;
    }
  PrintRegister ((LPPRINT) & pr);
  {
    /**** Shoud be inserted ? to select the part of the page we want 
    RECT lprect;
    PrintSize( printer, hwnd, &lprect);
    *****/
  }
  EnableWindow (hwnd, FALSE);
  pr.bUserAbort = FALSE;
  lpfnPrintDlgProc = (DLGPROC) MyGetProcAddress ("PrintDlgProc", PrintDlgProc);
  lpfnAbortProc = (ABORTPROC) MyGetProcAddress ("PrintAbortProc", PrintAbortProc);
  pr.hDlgPrint = CreateDialogParam (hdllInstance, "PrintDlgBox", hwnd,
				    lpfnPrintDlgProc, (LPARAM) lpgw->Title);
  SetAbortProc (pr.hdcPrn, lpfnAbortProc);

  if (StartDoc (pr.hdcPrn, &di) > 0)
    {
      if (StartPage (pr.hdcPrn) > 0)
	{
	  int scalef = 1;
/** test : PageGDICalls (pr.hdcPrn, xPage, yPage) ; **/
	  /** 
	    SetMapMode(pr.hdcPrn, MM_ANISOTROPIC);
	    SetWindowExtEx(pr.hdcPrn, rect.right-rect.left, 
	    rect.bottom-rect.right, (LPSIZE)NULL); 
	    **/
	  SetMapMode (pr.hdcPrn, MM_TEXT);
	  SetBkMode (pr.hdcPrn, TRANSPARENT);
	  SetTextAlign (pr.hdcPrn, TA_LEFT | TA_BOTTOM);
	  /** changes the origin 
	    we shoul duse this to get into account the margins 
	    that can be specified with the print dialog 
	    But I don't know how to get back the values ???
	    Rectangle(pr.hdcPrn,0,0,xPage,yPage);
	    SetViewportOrgEx(pr.hdcPrn,xPage/8,yPage/8,NULL);
	    Rectangle(pr.hdcPrn,0,0,xPage -xPage/4,yPage-yPage/4);
	    **/
	  /**
	    put the apropriate scale factor according to printer 
	    resolution and redraw with the printer as hdc 
	  **/
	  scalef = (int) (10.0 * ((double) xPage * yPage) / (6800.0 * 4725.0));
	  scig_replay_hdc ('P', ScilabGC->CurWindow, printer,
			   xPage, yPage, scalef);
	  if (EndPage (pr.hdcPrn) > 0)
	    EndDoc (pr.hdcPrn);
	  else
	    bError = TRUE;
	}
    }
  else
    bError = TRUE;
  if (!pr.bUserAbort)
    {
      EnableWindow (hwnd, TRUE);
      DestroyWindow (pr.hDlgPrint);
    }
  DeleteDC (printer);
  SetWindowLong (hwnd, 4, (LONG) (0L));
  SetWindowLong (ScilabGC->hWndParent, 4, (LONG) (0L));
  PrintUnregister ((LPPRINT) & pr);
  scig_buzy = 0;
  return bError || pr.bUserAbort;
}

/****************************************
 *  INI file stuff 
 *  XXXX : should be upgraded for win95/winnt 
 *  using the registry ? 
 ****************************************/

void WriteGraphIni (struct BCG *ScilabGC)
{
  RECT rect;
  LPSTR file = ScilabGC->lpgw->IniFile;
  LPSTR section = ScilabGC->lpgw->IniSection;
  char profile[80];
  if ((file == (LPSTR) NULL) || (section == (LPSTR) NULL))
    return;
  if (IsIconic (ScilabGC->CWindow))
    ShowWindow (ScilabGC->CWindow, SW_SHOWNORMAL);
  GetWindowRect (ScilabGC->CWindow, &rect);
  wsprintf (profile, "%d %d", rect.left, rect.top);
  WritePrivateProfileString (section, "GraphOrigin", profile, file);
  wsprintf (profile, "%d %d", rect.right - rect.left, rect.bottom - rect.top);
  WritePrivateProfileString (section, "GraphSize", profile, file);
  return;
}

void ReadGraphIni (struct BCG *ScilabGC)
{
  LPSTR file = ScilabGC->lpgw->IniFile;
  LPSTR section = ScilabGC->lpgw->IniSection;
  char profile[81];
  LPSTR p;
  BOOL bOKINI;
  bOKINI = (file != (LPSTR) NULL) && (section != (LPSTR) NULL);
  if (!bOKINI)
    profile[0] = '\0';
  if (bOKINI)
    GetPrivateProfileString (section, "GraphOrigin", "", profile, 80, file);
  if ((p = GetLInt (profile, &ScilabGC->lpgw->Origin.x)) == NULL)
    ScilabGC->lpgw->Origin.x = CW_USEDEFAULT;
  if ((p = GetLInt (p, &ScilabGC->lpgw->Origin.y)) == NULL)
    ScilabGC->lpgw->Origin.y = CW_USEDEFAULT;
  if (bOKINI)
    GetPrivateProfileString (section, "GraphSize", "", profile, 80, file);
  if ((p = GetLInt (profile, &ScilabGC->lpgw->Size.x)) == NULL)
    ScilabGC->lpgw->Size.x = CW_USEDEFAULT;
  if ((p = GetLInt (p, &ScilabGC->lpgw->Size.y)) == NULL)
    ScilabGC->lpgw->Size.y = CW_USEDEFAULT;
}

/****************************************************
 * Debug Function 
 ****************************************************/

#define MAXPRINTF 1024

extern void DebugGW (char *fmt,...)
{
#ifdef DEBUG
  int i, count;
  char buf[MAXPRINTF];
  va_list args;
  va_start (args, fmt);
  count = vsprintf (buf, fmt, args);
  MessageBox (textwin.hWndParent, (LPSTR) buf,
	      textwin.Title, MB_ICONEXCLAMATION);
  va_end (args);
#endif
}

extern void DebugGW1 (char *fmt,...)
{
  int i, count;
  char buf[MAXPRINTF];
  va_list args;
  va_start (args, fmt);
  count = vsprintf (buf, fmt, args);
  MessageBox (textwin.hWndParent, (LPSTR) buf,
	      textwin.Title, MB_ICONEXCLAMATION);
  va_end (args);
}

/**SciViewportMove
 *@description: used to move the panner and the viewport interactively 
 *              through scilab command.
 *
 *@input: struct BCG *ScilabGC : structure associated to a Scilab Graphic window
 *        int x,y : the x,y point of the graphic window to be moved at 
 *        the up-left position of the viewport
 *
 *@output: NONE
 *
 *@author: Matthieu PHILIPPE
 *@date: Dec 1999
 **/
void SciViewportMove (ScilabGC, x, y)
     struct BCG *ScilabGC;
     int x, y;
{
  SCROLLINFO vertsi;
  SCROLLINFO horzsi;

  if (ScilabGC != NULL)
    {
      sciGetScrollInfo (ScilabGC, SB_VERT, &vertsi);
      sciGetScrollInfo (ScilabGC, SB_HORZ, &horzsi);
      if (sciGetwresize () == 0)
	{
	  horzsi.nPos = max (horzsi.nMin, min (horzsi.nMax, x));
	  sciSetScrollInfo (ScilabGC, SB_HORZ, &horzsi, TRUE);
	  vertsi.nPos = min (vertsi.nMax, max (vertsi.nMin, y));
	  sciSetScrollInfo (ScilabGC, SB_VERT, &vertsi, TRUE);
	  InvalidateRect (ScilabGC->CWindow, (LPRECT) NULL, TRUE);
	  UpdateWindow (ScilabGC->CWindow);
	}
    }
}

/**SciViewportGet
 *@description: used to get panner position through scilab command.
 *
 *@input: struct BCG *ScilabGC : structure associated to a Scilab Graphic window
 *        int x,y : the x,y point of the graphic window to be moved at 
 *        the up-left position of the viewport
 *
 *@output: NONE
 *
 *@author: Matthieu PHILIPPE
 *@date: Dec 1999
 **/

void SciViewportGet (ScilabXgc, x, y)
     struct BCG *ScilabXgc;
     int *x, *y;
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

/**GPopupResize
 *@description: a little beat different to windowdim. GPopupResize sets the visible
 * window (parents dimension)
 *
 *@see: setwindowdim
 *
 **/
void GPopupResize (ScilabXgc, width, height)
     struct BCG *ScilabXgc;
     int *width, *height;
{
  RECT rect, rect1;
  int xof, yof;
  SCROLLINFO vertsi;
  SCROLLINFO horzsi;
  if (ScilabXgc->CWindow != (Window) NULL)
    {
      sciGetScrollInfo (ScilabXgc, SB_VERT, &vertsi);
      sciGetScrollInfo (ScilabXgc, SB_HORZ, &horzsi);

      ScilabXgc->CWindowWidthView = *width;
      ScilabXgc->CWindowHeightView = *height;
      /* remise a jour de la fenetre */
      if (sciGetPixmapStatus () == 1)
	{
	  CPixmapResize (ScilabXgc->CWindowWidth, ScilabXgc->CWindowHeight);
	}
      GetWindowRect (ScilabXgc->hWndParent, &rect);
      GetWindowRect (ScilabXgc->CWindow, &rect1);
      xof = (rect.right - rect.left) - (rect1.right - rect1.left);
      yof = (rect.bottom - rect.top) - (rect1.bottom - rect1.top);
      SetWindowPos (ScilabXgc->hWndParent, HWND_TOP, 0, 0,
		    ScilabXgc->CWindowWidthView + xof,
		    ScilabXgc->CWindowHeightView + yof,
		    SWP_NOMOVE | SWP_NOZORDER);

      /* mise a jour de la taille des scroll bars */
      /* changer l'etat de visibilite des scroll bar  */
      /* suivant l etat on ou off et ajouter un offset d'affichage */
      vertsi.nMax = ScilabXgc->CWindowHeight;
      vertsi.nPage = ScilabXgc->CWindowHeightView;
      horzsi.nMax = ScilabXgc->CWindowWidth;
      horzsi.nPage = ScilabXgc->CWindowWidthView;
      sciSetScrollInfo (ScilabXgc, SB_VERT, &vertsi, TRUE);
      sciSetScrollInfo (ScilabXgc, SB_HORZ, &horzsi, TRUE);
    }
}

/***********************************************
 * we keep track of the last MaxCB XXBUTTONDOWN 
 * events while we are in GraphicWindowProc 
 ***********************************************/

#define MaxCB 50
static But ClickBuf[MaxCB];
static int lastc = 0;

/* used by xclick_any and xclick */ 

static int wait_for_click=0;
But SciClickInfo; /* for xclick and xclick_any */

void set_wait_click(val) {  
  wait_for_click=val;
  if ( val == 1 ) 
    SciClickInfo.win=-1; 
}

int scig_click_handler_none (int win,int x,int y,int ibut,int motion,int release)
{
  return 0;
};

int scig_click_handler_sci (int win,int x,int y,int ibut,int motion,int release)

{
  static char buf[256];
  struct BCG *SciGc;

  SciGc = GetWindowXgcNumber(win);
  if (strlen(SciGc->EventHandler)!=0) {
    sprintf(buf,"%s(%d,%d,%d,%d)",SciGc->EventHandler,win,x,y,ibut);
    StoreCommand1(buf,2);
    return 1;}
  else
    return 0;
};

static Scig_click_handler scig_click_handler = scig_click_handler_sci;
/*static Scig_click_handler scig_click_handler = scig_click_handler_none;*/

Scig_click_handler set_scig_click_handler (f)
     Scig_click_handler f;
{
  Scig_click_handler old = scig_click_handler;
  scig_click_handler = f;
  return old;
}

void reset_scig_click_handler ()
{
  scig_click_handler = scig_click_handler_sci;
}

int PushClickQueue (int win,int x,int y,int ibut,int motion,int release)
{
  /* If we are in xclick_any or xclick then send info */
  if ( wait_for_click==1)
    {

      SciClickInfo.win= win;
      SciClickInfo.x= x;
      SciClickInfo.y= y;
      SciClickInfo.ibutton= ibut;
      SciClickInfo.motion= motion;
      SciClickInfo.release= release;
      return 0;
    }
     
  /* first let a click_handler do the job  */
  if ( scig_click_handler(win,x,y,ibut,motion,release)== 1) return 0;
  /* do not record motion events and release button 
   * this is left for a futur release 
   */
  if (motion == 1 || release == 1)
    return 0;
  if (lastc == MaxCB)
    {
      int i;
      for (i = 1; i < MaxCB; i++)
	{
	  ClickBuf[i - 1] = ClickBuf[i];
	}
      ClickBuf[lastc - 1].win = win;
      ClickBuf[lastc - 1].x = x;
      ClickBuf[lastc - 1].y = y;
      ClickBuf[lastc - 1].ibutton = ibut;
      ClickBuf[lastc - 1].motion = motion;
      ClickBuf[lastc - 1].release = release;
    }
  else
    {
      ClickBuf[lastc].win = win;
      ClickBuf[lastc].x = x;
      ClickBuf[lastc].y = y;
      ClickBuf[lastc].ibutton = ibut;
      ClickBuf[lastc].motion = motion;
      ClickBuf[lastc].release = release;
      lastc++;
    }
  return (0);
}

int CheckClickQueue (integer *win,integer *x, integer *y, integer *ibut)
{
  int i;
  for (i = 0; i < lastc; i++)
    {
      int j;
      if (ClickBuf[i].win == *win || *win == -1)
	{
	  *win = ClickBuf[i].win;
	  *x = ClickBuf[i].x;
	  *y = ClickBuf[i].y;
	  *ibut = ClickBuf[i].ibutton;
	  for (j = i + 1; j < lastc; j++)
	    {
	      ClickBuf[j - 1].win = ClickBuf[j].win;
	      ClickBuf[j - 1].x = ClickBuf[j].x;
	      ClickBuf[j - 1].y = ClickBuf[j].y;
	      ClickBuf[j - 1].ibutton = ClickBuf[j].ibutton;
	      ClickBuf[j - 1].motion = ClickBuf[j].motion;
	      ClickBuf[j - 1].release = ClickBuf[j].release;
	    }
	  lastc--;
	  return (1);
	}
    }
  return (0);
}

int ClearClickQueue (int win)
{
  int i;
  if (win == -1)
    {
      lastc = 0;
      return 0;
    }
  for (i = 0; i < lastc; i++)
    {
      int j;
      if (ClickBuf[i].win == win)
	{
	  for (j = i + 1; j < lastc; j++)
	    {
	      ClickBuf[j - 1].win = ClickBuf[j].win;
	      ClickBuf[j - 1].x = ClickBuf[j].x;
	      ClickBuf[j - 1].y = ClickBuf[j].y;
	      ClickBuf[j - 1].ibutton = ClickBuf[j].ibutton;
	      ClickBuf[j - 1].motion = ClickBuf[j].motion;
	      ClickBuf[j - 1].release = ClickBuf[j].release;
	    }
	  lastc--;
	}
    }
  lastc = 0;
  return (0);
}

/*************************************************
 * une petite fonction a la peek moi le message 
 *************************************************/

typedef struct
{
  MSG msg;
  integer flag;
}
SCISEND;

SCISEND sciSend;

void sciSendMessage (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  sciSend.msg.lParam = lParam;
  sciSend.msg.wParam = wParam;
  sciSend.msg.message = message;
  sciSend.msg.hwnd = hwnd;
  sciSend.flag = 1;

}

int sciPeekMessage (MSG * msg)
{
  if (sciSend.flag != 0)
    {
      msg->message = sciSend.msg.message;
      msg->lParam = sciSend.msg.lParam;
      msg->wParam = sciSend.msg.wParam;
      msg->hwnd = sciSend.msg.hwnd;
      sciSend.flag = 0;
      return (1);
    }
  else
    return (0);
}

/****************************************************
 * Drawing graphic window 
 ****************************************************/

static void ScilabPaint (HWND hwnd, struct BCG *ScilabGC)
{
  /* static paint = 0; */
  HDC hdc;
  PAINTSTRUCT ps;
  RECT rect;
  /* paint++; */
  /** wininfo("Painting %d",paint); **/
  /** if we are in pixmap mode ? **/
  if (scig_buzy == 1)
    return;
  scig_buzy = 1;

  hdc = BeginPaint(hwnd, &ps);
  if (ScilabGC->Inside_init != 1)
    {
      SetMapMode (hdc, MM_TEXT);
      SetBkMode (hdc, TRANSPARENT);
      GetClientRect (hwnd, &rect);
      /*SetViewportExtEx(hdc, rect.right, rect.bottom,NULL);*/
      SetViewportExtEx (hdc, ScilabGC->CWindowWidth, ScilabGC->CWindowHeight, NULL);
      DebugGW ("==> paint rect %d %d \n", rect.right, rect.bottom);
      if (sciGetPixmapStatus () == 1)
	{
	  if (sciGetwresize () == 1)
	    {
	      scig_replay_hdc('W',ScilabGC->CurWindow,ScilabGC->hdcCompat,
			      ScilabGC->CWindowWidth, ScilabGC->CWindowHeight,
			      1);
	    }
	  C2F (show) (PI0, PI0, PI0, PI0);
	}
      else
	{
	  if (sciGetwresize () == 0)
	    SetViewportOrgEx (hdc, -ScilabGC->horzsi.nPos, -ScilabGC->vertsi.nPos, NULL);
	  scig_replay_hdc ('U', ScilabGC->CurWindow, hdc, ScilabGC->CWindowWidth,
			   ScilabGC->CWindowHeight, 1);
	}
    }
  EndPaint(hwnd, &ps);

  scig_buzy = 0;
}

/****************************************************
 * Resize 
 ****************************************************/

static void ScilabGResize (HWND hwnd, struct BCG *ScilabGC, WPARAM wParam)
{
  HDC hdc1;
/** We do not paint just check if we must resize the pixmap  **/
  if (scig_buzy == 1)
    return;
  scig_buzy = 1;
  if (ScilabGC->Inside_init != 1
      && ((wParam == SIZE_MAXIMIZED) || (wParam == SIZE_RESTORED)))
    {
      /** a resize can occur while we are executing a routine 
	  inside periWin.c : so we must protect the hdc 
	  XXXX : not useful with scig_resize_pixmap ? **/
      hdc1 = GetDC (ScilabGC->CWindow);
      SetGHdc (hdc1, ScilabGC->CWindowWidthView, ScilabGC->CWindowHeightView);
      scig_resize_pixmap (ScilabGC->CurWindow);
      SetGHdc ((HDC) 0, 0, 0);
    }
  scig_buzy = 0;
}

/****************************************************
 * The Event Handler for the graphic windows 
 ****************************************************/

EXPORT LRESULT CALLBACK
  WndGraphProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  RECT rect;
  struct BCG *ScilabGC;
  int deltax = 0;
  int deltay = 0;
  int x,y,iwin;

  SCROLLINFO vertsi;
  SCROLLINFO horzsi;

  ScilabGC = (struct BCG *) GetWindowLong (hwnd, 0);
  switch (message)
    {
    case WM_SYSCOMMAND:
      switch (LOWORD (wParam))
	{
	case M_GRAPH_TO_TOP:
	case M_COLOR:
	case M_COPY_CLIP:
	case M_PRINT:
	case M_WRITEINI:
	case M_REBUILDTOOLS:
	  SendMessage (hwnd, WM_COMMAND, wParam, lParam);
	  break;
	case M_ABOUT:
	  if (ScilabGC != (struct BCG *) 0 && ScilabGC->lpgw->lptw)
	    AboutBox (hwnd, ScilabGC->lpgw->lptw->AboutText);
	  return 0;
	}
      break;
    case WM_COMMAND:
      if (LOWORD (wParam) < NUMMENU)
	SendGraphMacro (ScilabGC, LOWORD (wParam));
      else
	switch (LOWORD (wParam))
	  {
	  case M_GRAPH_TO_TOP:
	    ScilabGC->lpgw->graphtotop = !ScilabGC->lpgw->graphtotop;
	    SendMessage (hwnd, WM_COMMAND, M_REBUILDTOOLS, 0L);
	    return (0);
	  case M_COPY_CLIP:
	    CopyClip (ScilabGC);
	    return 0;
	  case M_PRINT:
	    CopyPrint (ScilabGC);
	    return 0;
	  case M_WRITEINI:
	    WriteGraphIni (ScilabGC);
	    if (ScilabGC->lpgw->lptw)
	      WriteTextIni (ScilabGC->lpgw->lptw);
	    return 0;
	  case M_REBUILDTOOLS:
	    DebugGW ("rebuild tools \r\n");
/** wininfo("rebuild tools \r\n"); **/
	    GetClientRect (hwnd, &rect);
	    InvalidateRect (hwnd, (LPRECT) & rect, 1);
	    UpdateWindow (hwnd);
	    return 0;
	  }
      return 0;
    case WM_KEYDOWN:
      return (0);
    case WM_CHAR:
      check_pointer_win(&x,&y,&iwin);
      PushClickQueue (ScilabGC->CurWindow, x,y,wParam,0,0);
      return (0);
    case WM_MOUSEMOVE:
      PushClickQueue (ScilabGC->CurWindow, ((int) LOWORD (lParam)) + ScilabGC->horzsi.nPos,
		      HIWORD (lParam) + ScilabGC->vertsi.nPos, -1, 1, 0);
      return 0;
    case WM_LBUTTONDOWN:
      PushClickQueue (ScilabGC->CurWindow, ((int) LOWORD (lParam)) + ScilabGC->horzsi.nPos,
		      HIWORD (lParam) + ScilabGC->vertsi.nPos, 0, 0, 0);
      /*sciSendMessage(hwnd, WM_CHAR, wParam, lParam);*/
      return (0);
    case WM_MBUTTONDOWN:
      PushClickQueue (ScilabGC->CurWindow, ((int) LOWORD (lParam)) + ScilabGC->horzsi.nPos,
		      HIWORD (lParam) + ScilabGC->vertsi.nPos, 1, 0, 0);
      /*sciSendMessage(hwnd, WM_CHAR, wParam, lParam);*/
      return (0);
    case WM_RBUTTONDOWN:
      PushClickQueue (ScilabGC->CurWindow, ((int) LOWORD (lParam)) + ScilabGC->horzsi.nPos,
		      HIWORD (lParam) + ScilabGC->vertsi.nPos, 2, 0, 0);
      /*sciSendMessage(hwnd, WM_CHAR, wParam, lParam);*/
      return (0);
    case WM_LBUTTONUP:
      PushClickQueue (ScilabGC->CurWindow, ((int) LOWORD (lParam)) + ScilabGC->horzsi.nPos
		      ,HIWORD (lParam) + ScilabGC->vertsi.nPos, -5, 0, 1);
      /*sciSendMessage(hwnd, WM_CHAR, wParam, lParam);*/
      return (0);
    case WM_MBUTTONUP:
      PushClickQueue (ScilabGC->CurWindow, ((int) LOWORD (lParam)) + ScilabGC->horzsi.nPos,
		      HIWORD (lParam) + ScilabGC->vertsi.nPos, -4, 0, 1);
      /*sciSendMessage(hwnd, WM_CHAR, wParam, lParam);*/
      return (0);
    case WM_RBUTTONUP:
      PushClickQueue (ScilabGC->CurWindow, ((int) LOWORD (lParam)) + ScilabGC->horzsi.nPos,
		      HIWORD (lParam) + ScilabGC->vertsi.nPos, -3, 0, 1);
      /*sciSendMessage(hwnd, WM_CHAR, wParam, lParam);*/
      return (0);
    case WM_CREATE:
      ScilabGC = ((CREATESTRUCT *) lParam)->lpCreateParams;
      SetWindowLong (hwnd, 0, (LONG) ScilabGC);
      ScilabGC->CWindow = hwnd;
      if (ScilabGC->lpgw->lptw
	  && (ScilabGC->lpgw->lptw->DragPre != (LPSTR) NULL)
	  && (ScilabGC->lpgw->lptw->DragPost != (LPSTR) NULL))
	DragAcceptFiles (hwnd, TRUE);
      return (0);
    case WM_PAINT:
      if (sciGetwresize () == 1)
	{
	  ScilabPaint (hwnd, ScilabGC);
	}
      else
	{
	  ScilabPaint (hwnd, ScilabGC);
	  /*hdc = BeginPaint(ScilabGC->CWindow, &ps);
	     BitBlt(ps.hdc, 0, 0, ScilabGC->CWindowWidthView, ScilabGC->CWindowHeightView,
	     ScilabGC->hdcCompat,ScilabGC->horzsi.nPos,ScilabGC->vertsi.nPos,
	     SRCCOPY);
	     EndPaint(ScilabGC->CWindow, &ps); */
	}
      break;
    case WM_SIZE:
      /* initialisation de SCROLLINFs */
      sciGetScrollInfo (ScilabGC, SB_HORZ, &horzsi);
      sciGetScrollInfo (ScilabGC, SB_VERT, &vertsi);
      if (sciGetwresize () == 0)
	{
	  horzsi.nPage = LOWORD (lParam);
	  vertsi.nPage = HIWORD (lParam);
	  /* on recupere la veritable position des scroll bar
	     on a ainsi la possibilite de tirer le graphe
	     quand on agrandi la fenetre */
	  horzsi.nPos = GetScrollPos (ScilabGC->CWindow, SB_HORZ);
	  vertsi.nPos = GetScrollPos (ScilabGC->CWindow, SB_VERT);
	}
      else
	{
	  /* Pourquoi  ScilabGResize ??? */
	  if (sciGetPixmapStatus () == 1)
	    ScilabGResize (hwnd, ScilabGC, wParam);
	  horzsi.nMax = LOWORD (lParam);
	  vertsi.nMax = HIWORD (lParam);
	  horzsi.nPage = horzsi.nMax;
	  vertsi.nPage = vertsi.nMax;
	}
      sciSetScrollInfo (ScilabGC, SB_HORZ, &(horzsi), TRUE);
      sciSetScrollInfo (ScilabGC, SB_VERT, &(vertsi), TRUE);
      /* on force le reclacule les positions des scrollbars
         et leur validation */
      sciGetScrollInfo (ScilabGC, SB_HORZ, &horzsi);
      sciGetScrollInfo (ScilabGC, SB_VERT, &vertsi);
      break;
    case WM_HSCROLL:
      {
	/* initialisation de SCROLLINFOs */
	sciGetScrollInfo (ScilabGC, SB_HORZ, &horzsi);
	switch (LOWORD (wParam))
	  {
	    /* deltax = 0;*/
	  case SB_PAGEUP:
	    deltax = horzsi.nPos;
	    horzsi.nPos = max (horzsi.nMin, horzsi.nPos - 50);
	    deltax = deltax - horzsi.nPos;
	    break;
	  case SB_PAGEDOWN:
	    deltax = horzsi.nPos;
	    horzsi.nPos = min (horzsi.nMax - (int)horzsi.nPage, horzsi.nPos + 50);
	    deltax = deltax - horzsi.nPos;
	    break;
	  case SB_LINEUP:
	    deltax = horzsi.nPos;
	    horzsi.nPos = max (horzsi.nMin, horzsi.nPos - 5);
	    deltax = deltax - horzsi.nPos;
	    break;
	  case SB_LINEDOWN:
	    deltax = horzsi.nPos;
	    horzsi.nPos = min (horzsi.nMax - (int) horzsi.nPage, horzsi.nPos + 5);
	    deltax = deltax - horzsi.nPos;
	    break;
	  case SB_THUMBTRACK:
	    deltax = horzsi.nPos;
	    horzsi.nPos = max (horzsi.nMin, min (horzsi.nMax, HIWORD (wParam)));
	    deltax = deltax - horzsi.nPos;
	    break;
	  default:
	    deltax=0;
	    break;
	  }
	sciSetScrollInfo (ScilabGC, SB_HORZ, &horzsi, TRUE);
	InvalidateRect (ScilabGC->CWindow, (LPRECT) NULL, TRUE);
	UpdateWindow (ScilabGC->CWindow);
      }
      break;
    case WM_VSCROLL:
      {
	/* initialisation de SCROLLINFs */
	sciGetScrollInfo (ScilabGC, SB_VERT, &vertsi);
	switch (LOWORD (wParam))
	  {
	    /* deltay = 0; */
	  case SB_PAGEUP:
	    deltay = vertsi.nPos;
	    vertsi.nPos = max (vertsi.nMin, vertsi.nPos - 50);
	    deltay = deltay - vertsi.nPos;
	    break;
	  case SB_PAGEDOWN:
	    deltay = vertsi.nPos;
	    vertsi.nPos = min (vertsi.nMax - (int) vertsi.nPage, vertsi.nPos + 50);
	    deltay = deltay - vertsi.nPos;
	    break;
	  case SB_LINEUP:
	    deltay = vertsi.nPos;
	    vertsi.nPos = max (vertsi.nMin, vertsi.nPos - 5);
	    deltay = deltay - vertsi.nPos;
	    break;
	  case SB_LINEDOWN:
	    deltay = vertsi.nPos;
	    vertsi.nPos = min (vertsi.nMax - (int) vertsi.nPage, vertsi.nPos + 5);
	    deltay = deltay - vertsi.nPos;
	    break;
	  case SB_THUMBTRACK:
	    deltay = vertsi.nPos;
	    vertsi.nPos = min (vertsi.nMax, max (vertsi.nMin, HIWORD (wParam)));
	    deltay = deltay - vertsi.nPos;
	    break;
	  default:
	    deltay = 0;
	    break;
	  }
	sciSetScrollInfo (ScilabGC, SB_VERT, &vertsi, TRUE);
	InvalidateRect (ScilabGC->CWindow, (LPRECT) NULL, TRUE);
	UpdateWindow (ScilabGC->CWindow);
      }
      break;
    case WM_DROPFILES:
      if (ScilabGC->lpgw->lptw)
	DragFunc (ScilabGC->lpgw->lptw, (HDROP) wParam);
      break;
    case WM_DESTROY:
      PostQuitMessage (0);
      DebugGW ("Je fais un destroy \n");
      DragAcceptFiles (hwnd, FALSE);
      return 0;
    case WM_CLOSE:
      DebugGW ("Je fais un close \n");
      /*TranslateMessage(&message);*/
      /*DispatchMessage(&message);*/
      PostQuitMessage (0);
      C2F (deletewin) (&(ScilabGC->CurWindow));
      SetWindowLong (hwnd, 0, (LONG) 0L);
      return 0;
    }
  return DefWindowProc (hwnd, message, wParam, lParam);
}

/********************************************************
 * A special Replay for win95 
 * we want to replay with a specific hdc 
 * and reset the hdc to its current value when leaving 
 * we must also protect the alufunction current value 
 *  ( for application like gr_menu or scicos : where 
 *    a redraw can occurs while we are using a alufunction 
 *    in a graphic mode without xtape )
 ********************************************************/

static void scig_replay_hdc (char c, integer win_num, HDC hdc, int width, int height,
			     int scale)
{
  integer verb = 0, cur, na;
  char name[4];
  integer alu, narg, verbose = 0;
  GetDriver1 (name, PI0, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0);
/** Warning : We use a driver which does not touch to hdc **/
  C2F (SetDriver) ("Int", PI0, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0);
  C2F (dr) ("xget", "window", &verb, &cur, &na, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 0L, 0L);
  C2F (dr) ("xset", "window", &win_num, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 0L, 0L);
  C2F (dr) ("xget", "alufunction", &verbose, &alu, &narg, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 0L, 0L);
  SetGHdc (hdc, width, height);
/** new font for printers **/
  if (c == 'P')
    SciG_Font_Printer (scale);
/** the create default font/brush etc... in hdc */
  ResetScilabXgc ();
/** xclear will properly upgrade background if necessary **/
  C2F (dr) ("xclear", "v", PI0, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 0L, 0L);
  if (version_flag() == 0)
    {
      sciRedrawF(&win_num); /* NG */
    }
  else
    C2F (dr) ("xreplay", "v", &win_num, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 0L, 0L);
  C2F (dr1) ("xset", "alufunction", &alu, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 0L, 0L);
  C2F (dr) ("xset", "window", &cur, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0, 0L, 0L);
  C2F (SetDriver) (name, PI0, PI0, PI0, PI0, PI0, PI0, PD0, PD0, PD0, PD0);
  SetGHdc ((HDC) 0, 600, 400);
/** back to usual font size **/
  SciG_Font ();
  SwitchWindow (&cur);
}

/********************************************************
 * parent overlapped window 
 ********************************************************/

EXPORT LRESULT CALLBACK
  WndParentGraphProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  POINT ScreenMinSize =
  {16, 4};
  POINT *MMinfo = (POINT *) lParam;
  TEXTMETRIC tm;
  HDC hdc;
  PAINTSTRUCT ps;
  RECT rect, rect1;
  struct BCG *ScilabGC;
  ScilabGC = (struct BCG *) GetWindowLong (hwnd, 0);
  switch (message)
    {
    case WM_SYSCOMMAND:
      switch (LOWORD (wParam))
	{
	case M_COPY_CLIP:
	case M_PASTE:
	case M_WRITEINI:
	case M_ABOUT:
	  SendMessage (ScilabGC->CWindow, WM_COMMAND, wParam, lParam);
	}
      break;
    case WM_KILLFOCUS:
      SendMessage (textwin.hWndText, message, wParam, lParam);
      return (0);
    case WM_SETFOCUS:
      /** when focus is set in the graphic window we set it to scilab window **/
      /*************** Matthieu PHILIPPE
	je retire cette fonction pour pouvoir recuperer
	les evevenements clavier et les traiter !!
	SendMessage(textwin.hWndText, message, wParam, lParam); 
	********************/
      SetFocus (ScilabGC->CWindow);
      return (0);
    case WM_KEYDOWN:
      SendMessage (ScilabGC->CWindow, message, wParam, lParam);
      return (0);
    case WM_CHAR:
      SendMessage (ScilabGC->CWindow, message, wParam, lParam);
      return (0);
    case WM_GETMINMAXINFO:
      /*** Eventuellement a changer XXXXXXX  **/
      hdc = GetDC (hwnd);
      SelectFont (hdc, GetStockFont (OEM_FIXED_FONT));
      GetTextMetrics (hdc, (LPTEXTMETRIC) & tm);
      ReleaseDC (hwnd, hdc);
      /* minimum size */
      MMinfo[3].x = ScreenMinSize.x * tm.tmAveCharWidth
	+ GetSystemMetrics (SM_CXVSCROLL) + 2 * GetSystemMetrics (SM_CXFRAME);
      MMinfo[3].y = ScreenMinSize.y * tm.tmHeight
	+ GetSystemMetrics (SM_CYHSCROLL) + 2 * GetSystemMetrics (SM_CYFRAME)
	+ GetSystemMetrics (SM_CYCAPTION);
      return (0);
    case WM_SIZE:
      /** sciprint("Resising Parent"); **/
      GetWindowRect (ScilabGC->Statusbar, &rect);
      SetWindowPos (ScilabGC->Statusbar, (HWND) NULL, 0,
		    HIWORD (lParam) - (rect.bottom - rect.top),
		    LOWORD (lParam), (rect.bottom - rect.top),
		    SWP_NOZORDER | SWP_NOACTIVATE);
      SetWindowPos (ScilabGC->CWindow, (HWND) NULL, 0,
		    ScilabGC->lpgw->ButtonHeight,
	     LOWORD (lParam), HIWORD (lParam) - ScilabGC->lpgw->ButtonHeight
		    - (rect.bottom - rect.top),
		    SWP_NOZORDER | SWP_NOACTIVATE);
      GetWindowRect (ScilabGC->Statusbar, &rect1);
      GetClientRect (ScilabGC->hWndParent, &rect);
      ScilabGC->CWindowWidthView = rect.right;
      ScilabGC->CWindowHeightView = rect.bottom - (rect1.bottom - rect1.top);
      if (sciGetwresize () != 0)
	{
	  ScilabGC->CWindowWidth = ScilabGC->CWindowWidthView;
	  ScilabGC->CWindowHeight = ScilabGC->CWindowHeightView;
	}
      return (0);
    case WM_COMMAND:
      /** pass on menu commands */
      if (IsWindow (ScilabGC->CWindow))
	SetFocus (ScilabGC->CWindow);
      SendMessage (ScilabGC->CWindow, message, wParam, lParam);
      return (0);
    case WM_PAINT:
      {				/* a quoi ca sertt Matthieu ?? */
	hdc = BeginPaint (hwnd, &ps);
	if (ScilabGC->lpgw->ButtonHeight)
	  {
	    HBRUSH hbrush;
	    GetClientRect (hwnd, &rect);
	    hbrush = CreateSolidBrush (GetSysColor (COLOR_BTNSHADOW));
	    rect.bottom = ScilabGC->lpgw->ButtonHeight - 1;
	    FillRect (hdc, &rect, hbrush);
	    DeleteBrush (hbrush);
	    SelectPen (hdc, GetStockPen (BLACK_PEN));
	    MoveToEx (hdc, rect.left, ScilabGC->lpgw->ButtonHeight - 1, NULL);
	    LineTo (hdc, rect.right, ScilabGC->lpgw->ButtonHeight - 1);
	  }
	EndPaint (hwnd, &ps);
	break;
      }
    case WM_DROPFILES:
      DragFunc (ScilabGC->lpgw->lptw, (HDROP) wParam);
      break;
    case WM_CREATE:
      {
	ScilabGC = ((CREATESTRUCT FAR *) lParam)->lpCreateParams;
	SetWindowLong (hwnd, 0, (LONG) ScilabGC);
	ScilabGC->hWndParent = hwnd;
	if (ScilabGC->lpgw->lptw
	    && (ScilabGC->lpgw->lptw->DragPre != (LPSTR) NULL)
	    && (ScilabGC->lpgw->lptw->DragPost != (LPSTR) NULL))
	  DragAcceptFiles (hwnd, TRUE);
      }
      break;
    case WM_DESTROY:
      SendMessage (ScilabGC->CWindow, WM_DESTROY, 0, 0);
      DragAcceptFiles (hwnd, FALSE);
      break;
    case WM_CLOSE:
      /** The Graphic window will do the job **/
      SendMessage (ScilabGC->CWindow, WM_CLOSE, 0, 0);
      return 0;
    }
  return DefWindowProc (hwnd, message, wParam, lParam);
}



void C2F(seteventhandler)(int *win_num,char *name,int *ierr)

{  
  struct BCG *SciGc;

  /*ButtonPressMask|PointerMotionMask|ButtonReleaseMask|KeyPressMask */
  *ierr = 0;
  SciGc = GetWindowXgcNumber(*win_num);
  if ( SciGc ==  NULL ) {*ierr=1;return;}
  strncpy(SciGc->EventHandler,name,24);
}

