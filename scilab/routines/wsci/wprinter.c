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
 *   Allan CORNET INRIA 2005
 */
/*-----------------------------------------------------------------------------------*/
#ifndef STRICT
#define STRICT
#endif
/*-----------------------------------------------------------------------------------*/
#include <memory.h>
#include "wresource.h"
#include "wcommon.h"
/*-----------------------------------------------------------------------------------*/
static LP_PRINT prlist = NULL;
/*-----------------------------------------------------------------------------------*/
/******************************
 * Deals with a list of printers 
 ******************************/
void PrintRegister (LP_PRINT lpr)
{
  LP_PRINT next;
  next = prlist;
  prlist = lpr;
  lpr->next = next;
}
/*-----------------------------------------------------------------------------------*/
LP_PRINT PrintFind (HDC hdc)
{
  LP_PRINT this;
  this = prlist;
  while (this && (this->hdcPrn != hdc))
    {
      this = this->next;
    }
  return this;
}
/*-----------------------------------------------------------------------------------*/
void PrintUnregister (LP_PRINT lpr)
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
/*-----------------------------------------------------------------------------------*/
/******************************
 * PrintDialogBox 
 ******************************/
EXPORT BOOL CALLBACK PrintDlgProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
/*-----------------------------------------------------------------------------------*/
EXPORT BOOL CALLBACK PrintAbortProc (HDC hdcPrn, int code)
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
/*-----------------------------------------------------------------------------------*/
