#include "wgraph.h"
#include "resource.h"
/*-----------------------------------------------------------------------------------*/
/* add handlers for delete action */
void scig_deletegwin_handler_none (int win)
{
};
/*-----------------------------------------------------------------------------------*/
void scig_deletegwin_handler_sci (int win)
{
  static char buf[256];
  struct BCG *SciGc;

  SciGc = GetWindowXgcNumber(win);
  if (strlen(SciGc->EventHandler)!=0) {
    sprintf(buf,"%s(%d,0,0,-1000)",SciGc->EventHandler,win);
    StoreCommand1(buf,0);
  }
};
/*-----------------------------------------------------------------------------------*/
Scig_deletegwin_handler set_scig_deletegwin_handler (Scig_deletegwin_handler f)
{
  Scig_deletegwin_handler old = scig_deletegwin_handler;
  scig_deletegwin_handler = f;
  return old;
}
/*-----------------------------------------------------------------------------------*/
void reset_scig_deletegwin_handler ()
{
  scig_deletegwin_handler = scig_deletegwin_handler_sci;
}
/*-----------------------------------------------------------------------------------*/
void   set_delete_win_mode()
{ 
	sci_graphic_protect = 0 ;
}
/*-----------------------------------------------------------------------------------*/
extern void   set_no_delete_win_mode()  {  sci_graphic_protect = 1 ;}
/*-----------------------------------------------------------------------------------*/
int C2F (deletewin) (integer * number)
{
  int v_flag = 1;
  double dv=0;
  int v=0;

  /* destroying recorded graphic commands */
  scig_erase (*number);
  /* delete the windows and resources */
  if (version_flag()==0) {DeleteObjs(*number); v_flag = 0;}
  scig_deletegwin_handler (*number);
  DeleteSGWin (*number); /* Here we 1) destroy the ScilabXgc (set to NULL) if it is the last window in the list */
                        /*         2) or reset the ScilabXgc to the next one see DeleteSGWin*/

  /* That's why we can not use version_flag below because this function uses ScilabXgc->graphicsversion 
     that could have been possibly previously deleted !! */
  
  /* So, we use another flag named v_flag :*/
  delete_sgwin_entities(win_num,v_flag);
  return (0);
}
/*-----------------------------------------------------------------------------------*/
/******************************************
 * Printing and redrawing graphic window 
 ******************************************/
EXPORT void WINAPI GraphPrint (struct BCG *ScilabGC)
{
  if (ScilabGC->CWindow && IsWindow (ScilabGC->CWindow))
    SendMessage (ScilabGC->CWindow, WM_COMMAND, M_PRINT, 0L);
}
/*-----------------------------------------------------------------------------------*/
EXPORT void WINAPI GraphRedraw (struct BCG *ScilabGC)
{
  if (ScilabGC->CWindow && IsWindow (ScilabGC->CWindow))
    SendMessage (ScilabGC->CWindow, WM_COMMAND, M_REBUILDTOOLS, 0L);
}
/*-----------------------------------------------------------------------------------*/
/****************************************
 * copy graph window to clipboard 
 * with the EnHmetafile format (win95/winNT)
 ****************************************/
void NewCopyClip (struct BCG *ScilabGC)
{
  
  TCHAR   szDesc[] = "Scilab\0Image\0\0";
  LPGW lpgw;
  
  RECT rect;
  HANDLE hmf;
  
   
  HWND hwnd;
  HDC hdc;


  if (scig_buzy == 1) return;
  scig_buzy = 1;

  lpgw = ScilabGC->lpgw;
  hwnd = ScilabGC->CWindow;

  /* view the window */
  if (IsIconic (hwnd)) ShowWindow (hwnd, SW_SHOWNORMAL);
  BringWindowToTop (hwnd);
  UpdateWindow (hwnd);
  
  hdc = CreateEnhMetaFile (NULL, NULL, NULL, szDesc);
 
  GetClientRect (hwnd, &rect);
  SetMapMode (hdc, MM_TEXT);
  SetTextAlign (hdc, TA_LEFT | TA_BOTTOM);
  SetWindowExtEx (hdc, rect.right - rect.left,rect.bottom - rect.top, (LPSIZE) NULL);
  Rectangle (hdc, 0, 0, ScilabGC->CWindowWidthView, ScilabGC->CWindowHeightView);

  /** fix hdc in the scilab driver **/
  scig_replay_hdc ('C', ScilabGC->CurWindow, GetDC (hwnd),rect.right - rect.left, rect.bottom - rect.top, 1);
  scig_replay_hdc ('C', ScilabGC->CurWindow, hdc,rect.right - rect.left, rect.bottom - rect.top, 1);

  hmf = CloseEnhMetaFile (hdc);  

  OpenClipboard (hwnd);
  EmptyClipboard ();
  SetClipboardData (CF_ENHMETAFILE, hmf);
  CloseClipboard ();

  scig_buzy = 0;
  return;

}
/*-----------------------------------------------------------------------------------*/
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
  LPMETAFILEPICT lpMFP;
  HWND hwnd;
  HDC hdc;


  if (scig_buzy == 1) return;
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
  /* in MM_ANISOTROPIC, xExt & yExt give suggested size in 0.01mm units 
   */
  hdc = GetDC (hwnd);
  lpMFP->mm = MM_ANISOTROPIC;
  lpMFP->xExt = MulDiv (rect.right - rect.left, 2540, GetDeviceCaps 
			(hdc, LOGPIXELSX));
  lpMFP->yExt = MulDiv (rect.bottom - rect.top, 2540, GetDeviceCaps 
			(hdc, LOGPIXELSX));
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
/*-----------------------------------------------------------------------------------*/
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
/*-----------------------------------------------------------------------------------*/
int CopyPrint (struct BCG *ScilabGC)
{
  int xPage, yPage;
  static DOCINFO di = {sizeof (DOCINFO), "Scilab: Printing", NULL};
  BOOL bError = FALSE;
  HDC printer;
  LPGW lpgw;
  ABORTPROC lpfnAbortProc;
  DLGPROC lpfnPrintDlgProc;
  static PRINTDLG pd;
  HWND hwnd;
  RECT rect,RectRestore;
  PRINT pr;

  if (scig_buzy == 1) return TRUE;
  scig_buzy = 1;
  lpgw = ScilabGC->lpgw;
  hwnd = ScilabGC->CWindow;
#ifdef __GNUC__
/** for cygwin and mingwin **/
/** XXXXX : Bug in cygwin : sizeof(PRINTDLG) gives something wrong (68) 
**/
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
  pd.Flags = PD_ALLPAGES | PD_COLLATE | PD_RETURNDC| PD_USEDEVMODECOPIESANDCOLLATE|PD_NOSELECTION|PD_HIDEPRINTTOFILE|PD_NONETWORKBUTTON;
  /* Allan CORNET */
  /* PD_USEDEVMODECOPIESANDCOLLATE supports multiple copies and collation */
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
		/* Redessine si Cancel Impression */
		GetClientRect (hwnd, &RectRestore);
		scig_replay_hdc ('W', ScilabGC->CurWindow, GetDC (hwnd),RectRestore.right - RectRestore.left, RectRestore.bottom - RectRestore.top, 1);
		scig_buzy = 0;
		return TRUE;
    }
  printer = pd.hDC;
  if (NULL == printer)
    {
		/* Redessine si Cancel Impression */
		GetClientRect (hwnd, &RectRestore);
		scig_replay_hdc ('W', ScilabGC->CurWindow, GetDC (hwnd),RectRestore.right - RectRestore.left, RectRestore.bottom - RectRestore.top, 1);
		sciprint ("\r\nCan't print \r\n");
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
    GetParent(hdlg) returns ScilabGC->hWndParent and not hwnd = 
ScilabGC->CWindow 
    as we would expect ? 
    So we call SetwindowLong with ScilabGC->hWndParent also 
    **/
  if (SetWindowLong (hwnd, 4, (LONG) ((LP_PRINT) & pr)) == 0
      && GetLastError () != 0)
    {
      sciprint ("Can't print : Error in SetWindowLong");
      scig_buzy = 0;
      return TRUE;
    }
  if (SetWindowLong(ScilabGC->hWndParent, 4, (LONG) ((LP_PRINT) & pr)) == 0
      && GetLastError () != 0)
    {
      sciprint ("Can't print : Error in SetWindowLong");
      scig_buzy = 0;
      return TRUE;
    }
  PrintRegister ((LP_PRINT) & pr);
  {
    /**** Shoud be inserted ? to select the part of the page we want 
    RECT lprect;
    PrintSize( printer, hwnd, &lprect);
    *****/
  }
  EnableWindow (hwnd, FALSE);
  pr.bUserAbort = FALSE;
  lpfnPrintDlgProc = (DLGPROC) MyGetProcAddress ("PrintDlgProc", 
PrintDlgProc);
  lpfnAbortProc = (ABORTPROC) MyGetProcAddress ("PrintAbortProc", 
PrintAbortProc);
  pr.hDlgPrint = CreateDialogParam (hdllInstance, "PrintDlgBox", hwnd,
				    lpfnPrintDlgProc, (LPARAM) lpgw->Title);
  SetAbortProc (pr.hdcPrn, lpfnAbortProc);

  if (StartDoc (pr.hdcPrn, &di) > 0)
    {
      if (StartPage (pr.hdcPrn) > 0)
	{
	  int scalef = 1;
	  SetMapMode (pr.hdcPrn, MM_TEXT);
	  SetBkMode (pr.hdcPrn, TRANSPARENT);
	  SetTextAlign (pr.hdcPrn, TA_LEFT | TA_BOTTOM);
	  /** changes the origin 
	    we should duse this to get into account the margins 
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
      GetClientRect (hwnd, &RectRestore);
	  /* Evite bug lorsque l'on selectionne la fenetre & que l'on imprime apres */
	  scig_replay_hdc ('P', ScilabGC->CurWindow, GetDC (hwnd),RectRestore.right - RectRestore.left, RectRestore.bottom - RectRestore.top, 1);
	  scig_replay_hdc ('P', ScilabGC->CurWindow, printer,xPage, yPage, scalef);
	  /* Redessine à l'ecran apres l'impression */
	  scig_replay_hdc ('W', ScilabGC->CurWindow, GetDC (hwnd),RectRestore.right - RectRestore.left, RectRestore.bottom - RectRestore.top, 1);
	  if (EndPage (pr.hdcPrn) > 0)  EndDoc (pr.hdcPrn);
	  else bError = TRUE;
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
  PrintUnregister ((LP_PRINT) & pr);
  scig_buzy = 0;
  return bError || pr.bUserAbort;
}
/*-----------------------------------------------------------------------------------*/
void WriteGraphIni (struct BCG *ScilabGC)
{
	/* Modification Allan CORNET Sauvegarde dans la base de registre dans 
			HKEY_CURRENT_USER\\SOFTWARE\\Scilab\\"VERSION"\\Graph Settings
	"Version" correspondant à la version de Scilab
	Sauvegarde dans HKEY_CURRENT_USER car données dépendant de l'utilisateur
	*/

	HKEY key;
	DWORD result,dwsize=4;
	char Clef[MAX_PATH];

	RECT rect;
	long GraphSizeX,GraphSizeY;
	int iconic;

	wsprintf(Clef,"SOFTWARE\\Scilab\\%s\\Graph Settings",VERSION);  	
  	RegCreateKeyEx(HKEY_CURRENT_USER, Clef, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &key, &result);
  	iconic = IsIconic (ScilabGC->hWndParent);
	if (iconic) ShowWindow (ScilabGC->hWndParent, SW_SHOWNORMAL);

	GetWindowRect (ScilabGC->hWndParent, &rect);
	RegSetValueEx(key, "GraphOriginX", 0, REG_DWORD, (LPBYTE)&rect.left, dwsize);
  	RegSetValueEx(key, "GraphOriginY", 0, REG_DWORD, (LPBYTE)&rect.top, dwsize);

	GraphSizeX=rect.right - rect.left;
	GraphSizeY=rect.bottom - rect.top;
	RegSetValueEx(key, "GraphSizeX", 0, REG_DWORD,(LPBYTE)&GraphSizeX, dwsize);
  	RegSetValueEx(key, "GraphSizeY", 0, REG_DWORD,(LPBYTE)&GraphSizeY, dwsize);
	RegSetValueEx(key, "ToolBar", 0, REG_DWORD, (LPBYTE)&ScilabGC->lpmw.ShowToolBar, dwsize);

	RegCloseKey(key);
	if (iconic) ShowWindow (ScilabGC->hWndParent, SW_SHOWMINIMIZED);
  
}
/*-----------------------------------------------------------------------------------*/
void ReadGraphIni (struct BCG *ScilabGC)
{
	/* Modification Allan CORNET Restauration depuis la base de registre dans 
			HKEY_CURRENT_USER\\SOFTWARE\\Scilab\\"VERSION"\\Graph Settings
	"Version" correspondant à la version de Scilab
	*/
	HKEY key;
	DWORD result,size=4;
	long GraphSizeX,GraphSizeY;
	char Clef[MAX_PATH];
	int Toolbar;
	
	RECT rect;

	wsprintf(Clef,"SOFTWARE\\Scilab\\%s\\Graph Settings",VERSION);
  	result=RegOpenKeyEx(HKEY_CURRENT_USER, Clef, 0, KEY_QUERY_VALUE , &key);

	if ( RegQueryValueEx(key, "GraphOriginX", 0, NULL, (LPBYTE)&rect.left, &size) !=  ERROR_SUCCESS )
	{
		ScilabGC->lpgw->Origin.x = CW_USEDEFAULT;

	}
	else
	{
		ScilabGC->lpgw->Origin.x = rect.left;
	}

	if ( RegQueryValueEx(key, "GraphOriginY", 0, NULL, (LPBYTE)&rect.top, &size) !=  ERROR_SUCCESS )
	{
		ScilabGC->lpgw->Origin.y = CW_USEDEFAULT;
	}
	else
	{
		ScilabGC->lpgw->Origin.y = rect.top;
	}

	if ( RegQueryValueEx(key, "GraphSizeX", 0, NULL, (LPBYTE)&GraphSizeX, &size) !=  ERROR_SUCCESS )
	{
		ScilabGC->lpgw->Size.x = CW_USEDEFAULT;
	}
	else
	{
	ScilabGC->lpgw->Size.x = GraphSizeX;
	}

	if ( RegQueryValueEx(key, "GraphSizeY", 0, NULL, (LPBYTE)&GraphSizeY, &size) !=  ERROR_SUCCESS )
	{
		ScilabGC->lpgw->Size.y = CW_USEDEFAULT;
	}
	else
	{
		ScilabGC->lpgw->Size.y = GraphSizeY;
	}

	if ( RegQueryValueEx(key, "ToolBar", 0, NULL, (LPBYTE)&Toolbar, &size) !=  ERROR_SUCCESS )
  	{
		ScilabGC->lpmw.ShowToolBar = TRUE;
	}
	else
	{
		ScilabGC->lpmw.ShowToolBar  = Toolbar;
	}

    DefaultShowToolBar=ScilabGC->lpmw.ShowToolBar;
	ScilabGC->lpmw.LockToolBar=FALSE;

	if ( result == ERROR_SUCCESS ) RegCloseKey(key);
}
/*-----------------------------------------------------------------------------------*/
/****************************************************
 * Debug Function 
 ****************************************************/
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
      /* MAJ D.ABDEMOUCHE*/
      if (ScilabGC->CurResizeStatus == 0)
	{
	  horzsi.nPos = max (horzsi.nMin, min (horzsi.nMax, x));
	  sciSetScrollInfo (ScilabGC, SB_HORZ, &horzsi, TRUE);
	  vertsi.nPos = min (vertsi.nMax, max (vertsi.nMin, y));
	  sciSetScrollInfo (ScilabGC, SB_VERT, &vertsi, TRUE);
	  InvalidateRect (ScilabGC->CWindow, (LPRECT) NULL, FALSE);
	  /* UpdateWindow (ScilabGC->CWindow); jpc  */
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

 if (ScilabXgc->CWindow != (Window) NULL) 
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
  /*
  if (ScilabXgc->CWindow != (Window) NULL)
    {
      sciGetScrollInfo (ScilabXgc, SB_VERT, &vertsi);
      sciGetScrollInfo (ScilabXgc, SB_HORZ, &horzsi);

      ScilabXgc->CWindowWidthView = *width;
      ScilabXgc->CWindowHeightView = *height;
      // remise a jour de la fenetre 
      if ( ScilabXgc->CurPixmapStatus == 1)
	{
//	  ScilabXgc->CWindowWidth = *width;
//      ScilabXgc->CWindowHeight = *height;
	  sci_pixmap_resize(ScilabXgc,
//		  *width, *height);
			    ScilabXgc->CWindowWidth, ScilabXgc->CWindowHeight);
	}
      GetWindowRect (ScilabXgc->hWndParent, &rect);
      GetWindowRect (ScilabXgc->CWindow, &rect1);
      xof = (rect.right - rect.left) - (rect1.right - rect1.left);
      yof = (rect.bottom - rect.top) - (rect1.bottom - rect1.top);
      SetWindowPos (ScilabXgc->hWndParent, HWND_TOP, 0, 0,
		    ScilabXgc->CWindowWidthView + xof,
		    ScilabXgc->CWindowHeightView + yof,
		    SWP_NOMOVE | SWP_NOZORDER);

      // mise a jour de la taille des scroll bars //
      // changer l'etat de visibilite des scroll bar  //
      // suivant l etat on ou off et ajouter un offset d'affichage //
      vertsi.nMax = ScilabXgc->CWindowHeight;
      vertsi.nPage = ScilabXgc->CWindowHeightView;
      horzsi.nMax = ScilabXgc->CWindowWidth;
      horzsi.nPage = ScilabXgc->CWindowWidthView;
      sciSetScrollInfo (ScilabXgc, SB_VERT, &vertsi, TRUE);
      sciSetScrollInfo (ScilabXgc, SB_HORZ, &horzsi, TRUE);
    }
	*/
}
/*-----------------------------------------------------------------------------------*/
void set_wait_click(val)
{  
  wait_for_click=val;
  if ( val == 1 ) 
    SciClickInfo.win=-1; 
}
/*-----------------------------------------------------------------------------------*/
int scig_click_handler_none (int win,int x,int y,int ibut,int motion,int release)
{
  return 0;
};
/*-----------------------------------------------------------------------------------*/
int scig_click_handler_sci (int win,int x,int y,int ibut,int motion,int release)

{
  static char buf[256];
  struct BCG *SciGc;

  SciGc = GetWindowXgcNumber(win);
  if (strlen(SciGc->EventHandler)!=0) {
    sprintf(buf,"%s(%d,%d,%d,%d)",SciGc->EventHandler,win,x,y,ibut);
    StoreCommand1(buf,0);
    return 1;}
  else
    return 0;
};
/*-----------------------------------------------------------------------------------*/
Scig_click_handler set_scig_click_handler (f)
     Scig_click_handler f;
{
  Scig_click_handler old = scig_click_handler;
  scig_click_handler = f;
  return old;
}
/*-----------------------------------------------------------------------------------*/
void reset_scig_click_handler ()
{
  scig_click_handler = scig_click_handler_sci;
}
/*-----------------------------------------------------------------------------------*/
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
/*-----------------------------------------------------------------------------------*/
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
/*-----------------------------------------------------------------------------------*/
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
/*-----------------------------------------------------------------------------------*/
/*************************************************
 * une petite fonction a la peek moi le message 
 *************************************************/
void sciSendMessage (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  sciSend.msg.lParam = lParam;
  sciSend.msg.wParam = wParam;
  sciSend.msg.message = message;
  sciSend.msg.hwnd = hwnd;
  sciSend.flag = 1;

}
/*-----------------------------------------------------------------------------------*/
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
	  sciprint("hdc for backing store failed \r\n");
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
      sciprint("Allocating pixmap for backing store failed \r\n");
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
static void ScilabPaint (HWND hwnd, struct BCG *ScilabGC)
{
  /* static paint = 0; */
  HDC hdc;
  PAINTSTRUCT ps;
  hdc = BeginPaint(hwnd, &ps);  
  if (scig_buzy == 1)  
    {
      EndPaint(hwnd, &ps);
      return; 
    }
  scig_buzy = 1;
  
  if (ScilabGC->Inside_init == 1) goto paint_end;
  /* 
  {
    static int paint=0; 
    paint++; 
    wininfo("Painting %d move=%d",paint,ScilabGC->in_sizemove);
    wininfo("%d erase=%d, [w=%d,h=%d,wv=%d,hv=%d,hn=%d,vn=%d],rcPaint=[%d,%d,%d,%d]",
	    paint,ps.fErase,
	    ScilabGC->CWindowWidth, ScilabGC->CWindowHeight,
	    ScilabGC->CWindowWidthView, ScilabGC->CWindowHeightView,
	    ScilabGC->horzsi.nPos,ScilabGC->vertsi.nPos,
	    ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right,ps.rcPaint.bottom
	    );
  }
  */
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
      scig_buzy = 0;
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
static int ScilabGResize (HWND hwnd, struct BCG *ScilabGC, WPARAM wParam)
{
  /** We do not paint just check if we must resize the pixmap  **/
  if ( ScilabGC->Inside_init != 1
      && ((wParam == SIZE_MAXIMIZED) || (wParam == SIZE_RESTORED)))
    {
      HBITMAP hbmTemp;
      HBITMAP  hbmSave;
      HDC hdc1;
      hdc1 = GetDC (ScilabGC->CWindow);
      hbmTemp = CreateCompatibleBitmap (hdc1,ScilabGC->CWindowWidth,
					ScilabGC->CWindowHeight);
	  ReleaseDC(ScilabGC->CWindow,hdc1);
      if (hbmTemp == NULL )
	{
	  sciprint("Can't resize pixmap\r\n");
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
/****************************************************
 * The Event Handler for the graphic windows 
 ****************************************************/
EXPORT LRESULT CALLBACK WndGraphProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
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
	    InvalidateRect (hwnd, (LPRECT) & rect, FALSE);
	    /* UpdateWindow (hwnd); */
	    return 0;
	  }
      return 0;
      case WM_KEYDOWN:
		{
			switch (wParam)
			{
				case VK_F3:
	      		{
				if (ScilabGC->lpmw.LockToolBar == FALSE)
					{
					if (ScilabGC->lpmw.ShowToolBar)	HideGraphToolBar(ScilabGC);
					else ShowGraphToolBar(ScilabGC);
					}

				}
				break;
				default:
				break;
				
	      		
	      	
			}
		}
	return 0;
    case WM_CHAR:
      check_pointer_win(&x,&y,&iwin);
      PushClickQueue (ScilabGC->CurWindow, x,y,wParam,0,0);
      return (0);
    case WM_MOUSEMOVE:
      PushClickQueue (ScilabGC->CurWindow, ((int) LOWORD (lParam)) + 
		      ScilabGC->horzsi.nPos,
		      HIWORD (lParam) + ScilabGC->vertsi.nPos, -1, 1, 0);
      return 0;
    case WM_LBUTTONDOWN:
      PushClickQueue (ScilabGC->CurWindow, ((int) LOWORD (lParam)) + 
		      ScilabGC->horzsi.nPos,
		      HIWORD (lParam) + ScilabGC->vertsi.nPos, 0, 0, 0);
      /*sciSendMessage(hwnd, WM_CHAR, wParam, lParam);*/
      return (0);
    case WM_MBUTTONDOWN:
      PushClickQueue (ScilabGC->CurWindow, ((int) LOWORD (lParam)) + 
		      ScilabGC->horzsi.nPos,
		      HIWORD (lParam) + ScilabGC->vertsi.nPos, 1, 0, 0);
      /*sciSendMessage(hwnd, WM_CHAR, wParam, lParam);*/
      return (0);
    case WM_RBUTTONDOWN:
      PushClickQueue (ScilabGC->CurWindow, ((int) LOWORD (lParam)) + 
		      ScilabGC->horzsi.nPos,
		      HIWORD (lParam) + ScilabGC->vertsi.nPos, 2, 0, 0);
      /*sciSendMessage(hwnd, WM_CHAR, wParam, lParam);*/
      return (0);
    case WM_LBUTTONUP:
      PushClickQueue (ScilabGC->CurWindow, ((int) LOWORD (lParam)) + 
		      ScilabGC->horzsi.nPos
		      ,HIWORD (lParam) + ScilabGC->vertsi.nPos, -5, 0, 1);
      /*sciSendMessage(hwnd, WM_CHAR, wParam, lParam);*/
      return (0);
    case WM_MBUTTONUP:
      PushClickQueue (ScilabGC->CurWindow, ((int) LOWORD (lParam)) + 
		      ScilabGC->horzsi.nPos,
		      HIWORD (lParam) + ScilabGC->vertsi.nPos, -4, 0, 1);
      /*sciSendMessage(hwnd, WM_CHAR, wParam, lParam);*/
      return (0);
    case WM_RBUTTONUP:
      PushClickQueue (ScilabGC->CurWindow, ((int) LOWORD (lParam)) + 
		      ScilabGC->horzsi.nPos,
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
      /* MAJ D.ABDEMOUCHE*/
      ScilabPaint (hwnd, ScilabGC);
      return 0;
      break;
    case WM_SIZE:
      /* initialisation de SCROLLINFs */
      sciGetScrollInfo (ScilabGC, SB_HORZ, &horzsi);
      sciGetScrollInfo (ScilabGC, SB_VERT, &vertsi);
      /* MAJ D.ABDEMOUCHE*/
      if (ScilabGC->CurResizeStatus == 0)
	{
	  horzsi.nPage = LOWORD (lParam);
	  vertsi.nPage = HIWORD (lParam);
	  /* on recupere la veritable position des scroll bar
	   * on a ainsi la possibilite de tirer le graphe
	   * quand on agrandi la fenetre 
	   */
	  horzsi.nPos = GetScrollPos (ScilabGC->CWindow, SB_HORZ);
	  vertsi.nPos = GetScrollPos (ScilabGC->CWindow, SB_VERT);
	}
      else
	{
	  /* eventually resize the pixmap size */
	  if (  ScilabGC->CurPixmapStatus  == 1 )
	    {
	      ScilabGResize (hwnd, ScilabGC, wParam); 
	    }
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
      InvalidateRect (ScilabGC->CWindow, (LPRECT) NULL,FALSE);
      return 0;
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
	InvalidateRect (ScilabGC->CWindow, (LPRECT) NULL, FALSE);
	/* UpdateWindow (ScilabGC->CWindow); */ 
      }
      return 0;
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
	    vertsi.nPos = min (vertsi.nMax - (int) vertsi.nPage, vertsi.nPos +50);
	    deltay = deltay - vertsi.nPos;
	    break;
	  case SB_LINEUP:
	    deltay = vertsi.nPos;
	    vertsi.nPos = max (vertsi.nMin, vertsi.nPos - 5);
	    deltay = deltay - vertsi.nPos;
	    break;
	  case SB_LINEDOWN:
	    deltay = vertsi.nPos;
	    vertsi.nPos = min (vertsi.nMax - (int) vertsi.nPage, vertsi.nPos +5);
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
	InvalidateRect (ScilabGC->CWindow, (LPRECT) NULL, FALSE);
	/* UpdateWindow (ScilabGC->CWindow); */ 
      }
      return 0;
      break;
    case WM_DROPFILES:
      if (ScilabGC->lpgw->lptw)
	DragFunc (ScilabGC->lpgw->lptw, (HDROP) wParam);
      return 0;
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
static void scig_replay_hdc (char c, integer win_num, HDC hdc, int width,int height,  int scale)
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
  if (version_flag() == 0)
    {
      sciRedrawF(&win_num); /* NG */
	}
  else
    C2F (dr) ("xreplay", "v", &win_num, PI0, PI0, PI0, PI0, PI0, PD0, 
	      PD0, PD0, PD0, 0L, 0L);
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
/********************************************************
 * parent overlapped window 
 ********************************************************/
EXPORT LRESULT CALLBACK WndParentGraphProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  POINT ScreenMinSize =   {16, 4};
  POINT *MMinfo = (POINT *) lParam;
  TEXTMETRIC tm;
  HDC hdc;
  PAINTSTRUCT ps;
  RECT rect, rect1, rrect, rrect1;
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
      /** when focus is set in the graphic window we set it to scilab 
	  window **/
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
    case WM_ENTERSIZEMOVE: 
      /* set  ScilabGC->in_sizemove to zero if you want graphics 
       * redraw during resizing or to one to prevent redraw while sizing.
       */
      ScilabGC->in_sizemove=0;
      return 0;
    case WM_EXITSIZEMOVE: 
      /* wininfo(" SIZE MOVE EXIT");*/
      ScilabGC->in_sizemove=0;
      InvalidateRect (ScilabGC->CWindow, (LPRECT) NULL, FALSE);
      return 0;
    case WM_SIZE:
      /** sciprint("Resising Parent"); **/
      GetWindowRect (ScilabGC->Statusbar, &rect);
      SetWindowPos (ScilabGC->Statusbar, (HWND) NULL, 0,
		    HIWORD (lParam) - (rect.bottom - rect.top),
		    LOWORD (lParam), (rect.bottom - rect.top),
		    SWP_NOZORDER | SWP_NOACTIVATE);
	  
	switch (LOWORD (wParam)) /* Maximized case . F.leray 01.07.04 */
	  {
	case SIZE_MAXIMIZED:
    	GetWindowRect (ScilabGC->Statusbar, &rrect1);
        GetClientRect (ScilabGC->hWndParent, &rrect);
        ScilabGC->CWindowWidthView = rrect.right;
        ScilabGC->CWindowHeightView = rrect.bottom - (rrect1.bottom - rrect1.top);
		if (ScilabGC->CurResizeStatus != 0)
		 {
		  ScilabGC->CWindowWidth = ScilabGC->CWindowWidthView;
		  ScilabGC->CWindowHeight = ScilabGC->CWindowHeightView;
		 }
        break;
	default:
		break;
	  }

      SetWindowPos (ScilabGC->CWindow, (HWND) NULL, 0,
		    0,
		    LOWORD (lParam), HIWORD (lParam) - (rect.bottom - rect.top),
		    SWP_NOZORDER | SWP_NOACTIVATE);
	
      GetWindowRect (ScilabGC->Statusbar, &rect1);
      GetClientRect (ScilabGC->hWndParent, &rect);
      ScilabGC->CWindowWidthView = rect.right;
      ScilabGC->CWindowHeightView = rect.bottom - (rect1.bottom - rect1.top);
      /* MAJ D.ABDEMOUCHE*/
      if (ScilabGC->CurResizeStatus != 0)
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
      hdc = BeginPaint (hwnd, &ps);
      EndPaint (hwnd, &ps);
      return 0;
      break;
    case WM_DROPFILES:
      DragFunc (ScilabGC->lpgw->lptw, (HDROP) wParam);
      return 0;
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
      return 0;
      break;
    case WM_DESTROY:
      SendMessage (ScilabGC->CWindow, WM_DESTROY, 0, 0);
      DragAcceptFiles (hwnd, FALSE);
      return 0;
      break;
    case WM_CLOSE:
      /** The Graphic window will do the job **/
      SendMessage (ScilabGC->CWindow, WM_CLOSE, 0, 0);
      return 0;
    }
  return DefWindowProc (hwnd, message, wParam, lParam);
}
/*-----------------------------------------------------------------------------------*/
void C2F(seteventhandler)(int *win_num,char *name,int *ierr)
{  
  struct BCG *SciGc;

  /*ButtonPressMask|PointerMotionMask|ButtonReleaseMask|KeyPressMask */
  *ierr = 0;
  SciGc = GetWindowXgcNumber(*win_num);
  if ( SciGc ==  NULL ) {*ierr=1;return;}
  strncpy(SciGc->EventHandler,name,24);
}
/*-----------------------------------------------------------------------------------*/
void HideGraphToolBar(struct BCG * ScilabGC)
{
	int i=0;
	ScilabGC->lpmw.ShowToolBar=FALSE;
	for (i=0;i<ScilabGC->lpmw.nButton;i++)
		{
		ShowWindow(ScilabGC->lpmw.hButton[i],SW_HIDE);
		}
}
/*-----------------------------------------------------------------------------------*/
void ShowGraphToolBar(struct BCG * ScilabGC)
{
	int i=0;
	ScilabGC->lpmw.ShowToolBar=TRUE;
	for (i=0;i<ScilabGC->lpmw.nButton;i++)
		{
		ShowWindow(ScilabGC->lpmw.hButton[i],SW_SHOWNORMAL);
		}
}
/*-----------------------------------------------------------------------------------*/
void CreateGraphToolBar(struct BCG * ScilabGC) 
{
#define HMENUIndiceZOOM 11
#define HMENUIndiceUNZOOM 12
#define HMENUIndice3DROT 13

  HICON IconButton;
  ScilabGC->lpmw.nButton=0;

  ScilabGC->lpmw.hButton[0]= CreateWindow("button","Zoom",WS_CHILD|WS_VISIBLE|BS_ICON ,
										  0, 0,
										  ButtonToolBarWeight, ToolBarHeight,
                                          ScilabGC->CWindow,(HMENU)HMENUIndiceZOOM,
                                          graphwin.hInstance, NULL);
  IconButton=(HICON)LoadImage( hdllInstance, (LPCSTR)IDI_ZOOM,IMAGE_ICON,ButtonToolBarWeight,ToolBarHeight,LR_DEFAULTCOLOR);
  SendMessage(ScilabGC->lpmw.hButton[0],BM_SETIMAGE, IMAGE_ICON, (LPARAM)IconButton);
  CreateMyTooltip (ScilabGC->lpmw.hButton[0], "Zoom"); 

  ScilabGC->lpmw.nButton++;
  ScilabGC->lpmw.hButton[1]= CreateWindow("button","UnZoom",WS_CHILD|WS_VISIBLE|BS_ICON ,
										  ButtonToolBarWeight, 0,
										  ButtonToolBarWeight, ToolBarHeight,
                                          ScilabGC->CWindow,(HMENU)HMENUIndiceUNZOOM,
                                          graphwin.hInstance, NULL);
  IconButton=(HICON)LoadImage( hdllInstance, (LPCSTR)IDI_UNZOOM,IMAGE_ICON,ButtonToolBarWeight,ToolBarHeight,LR_DEFAULTCOLOR);
  SendMessage(ScilabGC->lpmw.hButton[1],BM_SETIMAGE, IMAGE_ICON, (LPARAM)IconButton);
  CreateMyTooltip (ScilabGC->lpmw.hButton[1], "UnZoom"); 

  ScilabGC->lpmw.nButton++;
  ScilabGC->lpmw.hButton[2]= CreateWindow("button","2D/3D Rotation",WS_CHILD|WS_VISIBLE|BS_ICON ,
										  ButtonToolBarWeight*2, 0,
										  ButtonToolBarWeight, ToolBarHeight,
                                          ScilabGC->CWindow,(HMENU)HMENUIndice3DROT,
                                          graphwin.hInstance, NULL);
  IconButton=(HICON)LoadImage( hdllInstance, (LPCSTR)IDI_3DROT,IMAGE_ICON,ButtonToolBarWeight,ToolBarHeight,LR_DEFAULTCOLOR);
  SendMessage(ScilabGC->lpmw.hButton[2],BM_SETIMAGE, IMAGE_ICON, (LPARAM)IconButton);
  CreateMyTooltip (ScilabGC->lpmw.hButton[2], "2D/3D Rotation"); 

  ScilabGC->lpmw.nButton++;
  ScilabGC->lpmw.LockToolBar=FALSE;

  if (DefaultShowToolBar == FALSE) HideGraphToolBar(ScilabGC);
  else
  {
	  ScilabGC->lpmw.ShowToolBar=TRUE;
  }
  
}
/*-----------------------------------------------------------------------------------*/
void RefreshGraphToolBar(struct BCG * ScilabGC) 
{
int i=0;

for (i=0;i<ScilabGC->lpmw.nButton;i++)
	{
	InvalidateRect(ScilabGC->lpmw.hButton[i],NULL,TRUE);
	}
}
/*-----------------------------------------------------------------------------------*/
BOOL HdcToBmpFile(HDC hdc, char *pszflname)
{
    HDC memdc;
    HANDLE hfl;
    DWORD dwBytes, dwWidth, dwHeight, dwNumColors, dwBPP, ColorSize;
    void *pBits;
    HBITMAP hbmp;
    BITMAPFILEHEADER fileheader;
    BITMAPINFOHEADER infoheader;
    RGBQUAD colors[256];
    BITMAPINFO bmpinfo;
    HGDIOBJ hret;

    dwWidth = GetDeviceCaps(hdc, HORZRES);
    dwHeight = GetDeviceCaps(hdc, VERTRES);
    dwBPP = GetDeviceCaps(hdc, BITSPIXEL);

    if(dwBPP <= 8) dwNumColors = 256;
    else dwNumColors = 0;

    if(!(memdc = CreateCompatibleDC(hdc))) return 0;

    bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpinfo.bmiHeader.biWidth = dwWidth;
    bmpinfo.bmiHeader.biHeight = dwHeight;
    bmpinfo.bmiHeader.biPlanes = 1;
    bmpinfo.bmiHeader.biBitCount = (WORD) dwBPP;
    bmpinfo.bmiHeader.biCompression = BI_RGB;
    bmpinfo.bmiHeader.biSizeImage = 0;
    bmpinfo.bmiHeader.biXPelsPerMeter = 0;
    bmpinfo.bmiHeader.biYPelsPerMeter = 0;
    bmpinfo.bmiHeader.biClrUsed = dwNumColors;
    bmpinfo.bmiHeader.biClrImportant = dwNumColors;
    hbmp = CreateDIBSection(hdc, &bmpinfo, DIB_PAL_COLORS, &pBits, NULL, 0);
    
	if(!hbmp) goto errato;
    
	hret = SelectObject(memdc, hbmp);
    
	if(!hret || (hret == HGDI_ERROR)) goto errato;
    if(!BitBlt(memdc, 0, 0, dwWidth, dwHeight, hdc, 0, 0, SRCCOPY)) goto errato;
    if(dwNumColors) dwNumColors = GetDIBColorTable(memdc, 0, dwNumColors, colors);
    fileheader.bfType = 0x4D42;
    ColorSize = dwNumColors * sizeof(RGBQUAD);

    fileheader.bfSize = ((dwWidth*dwHeight*dwBPP) >> 3)+ColorSize+sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
    fileheader.bfReserved1 = fileheader.bfReserved2 = 0;
    fileheader.bfOffBits = ColorSize+sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);

    infoheader.biSize = sizeof(BITMAPINFOHEADER);
    infoheader.biWidth = dwWidth;
    infoheader.biHeight = dwHeight;
    infoheader.biPlanes = 1;
    infoheader.biBitCount = (WORD) dwBPP;
    infoheader.biCompression = BI_RGB;
    infoheader.biSizeImage = infoheader.biClrImportant = 0;
    infoheader.biXPelsPerMeter = infoheader.biYPelsPerMeter = 0;
    infoheader.biClrUsed = dwNumColors;

    hfl = CreateFile(pszflname,GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
    if(hfl == INVALID_HANDLE_VALUE) {DeleteObject(hbmp); goto errato;}

    WriteFile(hfl, &fileheader, sizeof(BITMAPFILEHEADER), &dwBytes, 0);
    WriteFile(hfl, &infoheader, sizeof(BITMAPINFOHEADER), &dwBytes, 0);

    if(!dwNumColors) WriteFile(hfl, colors, ColorSize, &dwBytes, 0);
    ColorSize = (dwWidth*dwHeight*dwBPP) >> 3;
    WriteFile(hfl, pBits, ColorSize, &dwBytes, 0);

    CloseHandle(hfl);

    DeleteObject(hbmp);
    DeleteDC(memdc);

    return 1;
errato:
    DeleteDC(memdc); return 0;
}
/*-----------------------------------------------------------------------------------*/
BOOL HwndToBmpFile(HWND hwnd, char *pszflname)
{
    HDC memdc, hdc;
    HANDLE hfl;
    DWORD dwBytes, dwNumColors;
    void *pBits;
    HBITMAP hbmp;
    BITMAPFILEHEADER fileheader;
    RGBQUAD colors[256];
    BITMAPINFO bmpinfo;
    HGDIOBJ hret;
    RECT rct;

    hdc = GetWindowDC(hwnd);

    if(!hdc) return 0;

    GetWindowRect(hwnd, &rct);
    rct.bottom -= rct.top;
    rct.right -= rct.left;
    rct.top = GetDeviceCaps(hdc, BITSPIXEL);
    if(rct.top <= 8) dwNumColors = 256;
    else dwNumColors = 0;

    if(!(memdc = CreateCompatibleDC(hdc))) goto relHwndDc;

    bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpinfo.bmiHeader.biWidth = rct.right;
    bmpinfo.bmiHeader.biHeight = rct.bottom;
    bmpinfo.bmiHeader.biPlanes = 1;
    bmpinfo.bmiHeader.biBitCount = (WORD) rct.top;
    bmpinfo.bmiHeader.biCompression = BI_RGB;
    bmpinfo.bmiHeader.biSizeImage = 0;
    bmpinfo.bmiHeader.biXPelsPerMeter = 0;
    bmpinfo.bmiHeader.biYPelsPerMeter = 0;
    bmpinfo.bmiHeader.biClrUsed = dwNumColors;
    bmpinfo.bmiHeader.biClrImportant = dwNumColors;

    hbmp = CreateDIBSection(hdc, &bmpinfo, DIB_PAL_COLORS, &pBits, NULL, 0);
    if(!hbmp) goto errato;

    hret = SelectObject(memdc, hbmp);
    if(!hret || (hret == HGDI_ERROR)) goto errato;
    if(!BitBlt(memdc, 0, 0, rct.right, rct.bottom, hdc, 0, 0, SRCCOPY)) goto errato;
    if(dwNumColors) dwNumColors = GetDIBColorTable(memdc, 0, dwNumColors, colors);
    fileheader.bfType = 0x4D42;
    rct.left = dwNumColors * sizeof(RGBQUAD);

    fileheader.bfSize = ((rct.right * rct.bottom * rct.top) >> 3) + rct.left + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    fileheader.bfReserved1 = fileheader.bfReserved2 = 0;
    fileheader.bfOffBits = rct.left + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    bmpinfo.bmiHeader.biClrImportant = 0;
    bmpinfo.bmiHeader.biClrUsed = dwNumColors;
    hfl = CreateFile(pszflname,GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
    if(hfl == INVALID_HANDLE_VALUE) {DeleteObject(hbmp); goto errato;}
    WriteFile(hfl, &fileheader, sizeof(BITMAPFILEHEADER), &dwBytes, 0);
    WriteFile(hfl, &bmpinfo.bmiHeader, sizeof(BITMAPINFOHEADER), &dwBytes, 0);
    if(!dwNumColors) WriteFile(hfl, colors, rct.left, &dwBytes, 0);
    WriteFile(hfl, pBits, (rct.right * rct.bottom * rct.top) >> 3, &dwBytes, 0);
    CloseHandle(hfl);
    DeleteObject(hbmp);
    DeleteDC(memdc);
    return 1;
errato:
    DeleteDC(memdc);
relHwndDc:
    ReleaseDC(hwnd, hdc); return 0;
}

/*-----------------------------------------------------------------------------------*/
void ExportBMP(struct BCG *ScilabGC,char *pszflname)
{
  RECT rect;
   
  HWND hwnd;
 
  if (scig_buzy == 1) return;
  scig_buzy = 1;

  hwnd = ScilabGC->CWindow;
  
  /* view the window */
  if (IsIconic (hwnd)) ShowWindow (hwnd, SW_SHOWNORMAL);
  BringWindowToTop (hwnd);
  UpdateWindow (hwnd);
  

  GetClientRect (hwnd, &rect);
  scig_replay_hdc ('C', ScilabGC->CurWindow, GetDC (hwnd),rect.right - rect.left, rect.bottom - rect.top, 1);
  HwndToBmpFile(hwnd,pszflname);
  scig_buzy = 0;
  
}
/*-----------------------------------------------------------------------------------*/
void ExportEMF(struct BCG *ScilabGC,char *pszflname)
{    
  RECT rect;
  HWND hwnd;
  HDC hdc;


  if (scig_buzy == 1) return;
  scig_buzy = 1;

  hwnd = ScilabGC->CWindow;

  /* view the window */
  if (IsIconic (hwnd)) ShowWindow (hwnd, SW_SHOWNORMAL);
  BringWindowToTop (hwnd);
  UpdateWindow (hwnd);
  
  hdc = CreateEnhMetaFile (GetWindowDC(hwnd), pszflname, NULL, "Scilab Graphics");
 
  GetClientRect (hwnd, &rect);
  SetMapMode (hdc, MM_TEXT);
  SetTextAlign (hdc, TA_LEFT | TA_BOTTOM);
  SetWindowExtEx (hdc, rect.right - rect.left,rect.bottom - rect.top, (LPSIZE) NULL);
  Rectangle (hdc, 0, 0, ScilabGC->CWindowWidthView, ScilabGC->CWindowHeightView);

  /** fix hdc in the scilab driver **/
  scig_replay_hdc ('C', ScilabGC->CurWindow, GetDC (hwnd),rect.right - rect.left, rect.bottom - rect.top, 1);
  scig_replay_hdc ('C', ScilabGC->CurWindow, hdc,rect.right - rect.left, rect.bottom - rect.top, 1);

  CloseEnhMetaFile (hdc);  

 

  scig_buzy = 0;
  return;
}
/*-----------------------------------------------------------------------------------*/
