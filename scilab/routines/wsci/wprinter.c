/*******************************************
 * Original source : GNUPLOT - win/wprinter.c 
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
 * AUTHORS
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
/* #include <windows.h>
   #include <windowsx.h>*/


#ifndef __ABSC__
#include <memory.h>
#endif

/*#include "wgnuplib.h"*/
#include "wresource.h"
#include "wcommon.h"
#include <commdlg.h>

LP_PRINT prlist = NULL;
/** list of selected printers **/

extern HDC TryToGetDC(HWND hWnd);

/********************************************
 * A Dialog Box for choosing the size for graphics 
 * on the printer page 
 ********************************************/

EXPORT BOOL CALLBACK
PrintSizeDlgProc (HWND hdlg, UINT wmsg, WPARAM wparam, LPARAM lparam)
{

  char buf[8];
  LP_PRINT lpr;
  HWND parent;
  parent = GetParent (hdlg);
  lpr = (LP_PRINT) GetWindowLong (parent, 4);
  switch (wmsg)
    {
    case WM_INITDIALOG:
      wsprintf (buf, "%d", lpr->pdef.x);
      SetDlgItemText (hdlg, PSIZE_DEFX, buf);
      wsprintf (buf, "%d", lpr->pdef.y);
      SetDlgItemText (hdlg, PSIZE_DEFY, buf);
      wsprintf (buf, "%d", lpr->poff.x);
      SetDlgItemText (hdlg, PSIZE_OFFX, buf);
      wsprintf (buf, "%d", lpr->poff.y);
      SetDlgItemText (hdlg, PSIZE_OFFY, buf);
      wsprintf (buf, "%d", lpr->psize.x);
      SetDlgItemText (hdlg, PSIZE_X, buf);
      wsprintf (buf, "%d", lpr->psize.y);
      SetDlgItemText (hdlg, PSIZE_Y, buf);
      CheckDlgButton (hdlg, PSIZE_DEF, TRUE);
      EnableWindow (GetDlgItem (hdlg, PSIZE_X), FALSE);
      EnableWindow (GetDlgItem (hdlg, PSIZE_Y), FALSE);
      return TRUE;
    case WM_COMMAND:
      switch (wparam)
	{
	case PSIZE_DEF:
	  EnableWindow (GetDlgItem (hdlg, PSIZE_X), FALSE);
	  EnableWindow (GetDlgItem (hdlg, PSIZE_Y), FALSE);
	  return FALSE;
	case PSIZE_OTHER:
	  EnableWindow (GetDlgItem (hdlg, PSIZE_X), TRUE);
	  EnableWindow (GetDlgItem (hdlg, PSIZE_Y), TRUE);
	  return FALSE;
	case IDOK:
	  if (SendDlgItemMessage (hdlg, PSIZE_OTHER, BM_GETCHECK, 0, 0L))
	    {
	      SendDlgItemMessage (hdlg, PSIZE_X, WM_GETTEXT, 7, (LPARAM) ((LPSTR) buf));
	      GetLInt (buf, &lpr->psize.x);
	      SendDlgItemMessage (hdlg, PSIZE_Y, WM_GETTEXT, 7, (LPARAM) ((LPSTR) buf));
	      GetLInt (buf, &lpr->psize.y);
	    }
	  else
	    {
	      lpr->psize.x = lpr->pdef.x;
	      lpr->psize.y = lpr->pdef.y;
	    }
	  SendDlgItemMessage (hdlg, PSIZE_OFFX, WM_GETTEXT, 7, (LPARAM) ((LPSTR) buf));
	  GetLInt (buf, &lpr->poff.x);
	  SendDlgItemMessage (hdlg, PSIZE_OFFY, WM_GETTEXT, 7, (LPARAM) ((LPSTR) buf));
	  GetLInt (buf, &lpr->poff.y);

	  if (lpr->psize.x <= 0)
	    lpr->psize.x = lpr->pdef.x;
	  if (lpr->psize.y <= 0)
	    lpr->psize.y = lpr->pdef.y;

	  EndDialog (hdlg, IDOK);
	  return TRUE;
	case IDCANCEL:
	  EndDialog (hdlg, IDCANCEL);
	  return TRUE;
	}
      break;
    }
  return FALSE;
}


/* GetWindowLong(hwnd, 4) must be available for use */

BOOL 
PrintSize (HDC printer, HWND hwnd, LPRECT lprect)
{
  HDC hdc;
  DLGPROC lpfnPrintSizeDlgProc;
  BOOL status = FALSE;
  PRINT pr;
  SetWindowLong (hwnd, 4, (LONG) ((LP_PRINT) & pr));
  pr.poff.x = 0;
  pr.poff.y = 0;
  pr.psize.x = GetDeviceCaps (printer, HORZSIZE);
  pr.psize.y = GetDeviceCaps (printer, VERTSIZE);
  hdc = (HDC)TryToGetDC (hwnd);
  GetClientRect (hwnd, lprect);
  pr.pdef.x = MulDiv (lprect->right - lprect->left, 254, 10 * GetDeviceCaps (hdc, LOGPIXELSX));
  pr.pdef.y = MulDiv (lprect->bottom - lprect->top, 254, 10 * GetDeviceCaps (hdc, LOGPIXELSX));
  ReleaseDC (hwnd, hdc);
  lpfnPrintSizeDlgProc = (DLGPROC) MyGetProcAddress ("PrintSizeDlgProc",
						     PrintSizeDlgProc);
  if (DialogBox (hdllInstance, "PrintSizeDlgBox", hwnd, lpfnPrintSizeDlgProc)
      == IDOK)
    {
      lprect->left = MulDiv (pr.poff.x * 10, GetDeviceCaps (printer, LOGPIXELSX), 254);
      lprect->top = MulDiv (pr.poff.y * 10, GetDeviceCaps (printer, LOGPIXELSY), 254);
      lprect->right = lprect->left + MulDiv (pr.psize.x * 10, GetDeviceCaps (printer, LOGPIXELSX), 254);
      lprect->bottom = lprect->top + MulDiv (pr.psize.y * 10, GetDeviceCaps (printer, LOGPIXELSY), 254);
      status = TRUE;
    }
  SetWindowLong (hwnd, 4, (LONG) (0L));
  return status;
}

/******************************
 * Deals with a list of printers 
 ******************************/

void
PrintRegister (LP_PRINT lpr)
{
  LP_PRINT next;
  next = prlist;
  prlist = lpr;
  lpr->next = next;
}

LP_PRINT
PrintFind (HDC hdc)
{
  LP_PRINT this;
  this = prlist;
  while (this && (this->hdcPrn != hdc))
    {
      this = this->next;
    }
  return this;
}

void
PrintUnregister (LP_PRINT lpr)
{
  LP_PRINT this, prev;
  prev = (LP_PRINT) NULL;
  this = prlist;
  while (this && (this != lpr))
    {
      prev = this;
      this = this->next;
    }
  if (this && (this == lpr))
    {
      /* unhook it */
      if (prev)
	prev->next = this->next;
      else
	prlist = this->next;
    }
}

/******************************
 * PrintDialogBox 
 ******************************/

/* GetWindowLong(GetParent(hDlg), 4) must be available for use */

EXPORT BOOL CALLBACK
PrintDlgProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
  LP_PRINT lpr;
  HWND parent;
  parent = GetParent (hDlg);
  lpr = (LP_PRINT) GetWindowLong (parent, 4);
  switch (message)
    {
    case WM_INITDIALOG:
      lpr->hDlgPrint = hDlg;
      SetWindowText (hDlg, (LPSTR) lParam);
      EnableMenuItem (GetSystemMenu (hDlg, FALSE), SC_CLOSE, MF_GRAYED);
      return TRUE;
    case WM_COMMAND:
      lpr->bUserAbort = TRUE;
      lpr->hDlgPrint = 0;
      EnableWindow (GetParent (hDlg), TRUE);
      EndDialog (hDlg, FALSE);
      return TRUE;
    }
  return FALSE;
}


EXPORT BOOL CALLBACK
PrintAbortProc (HDC hdcPrn, int code)
{
  MSG msg;
  LP_PRINT lpr;
  lpr = PrintFind (hdcPrn);
  while (!lpr->bUserAbort && PeekMessage (&msg, 0, 0, 0, PM_REMOVE))
    {
      if (!lpr->hDlgPrint || !IsDialogMessage (lpr->hDlgPrint, &msg))
	{
	  TranslateMessage (&msg);
	  DispatchMessage (&msg);
	}
    }
  return (!lpr->bUserAbort);
}
