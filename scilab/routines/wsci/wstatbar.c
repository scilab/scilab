
/* Copyright (C) 1998-2002 Chancelier Jean-Philippe */
/*********************************************
 *   STATBAR.C -- Status bar helper functions.
 *              (c) Paul Yao, 1996
 * Modifications for Scilab 
 *   Jean-Philipe Chancelier (1997)
 *   Bugs and mail : Scilab@inria.fr 
 *********************************************/


#ifndef STRICT
#define STRICT
#endif
#include <windows.h>
#include <windowsx.h>
#include <commdlg.h>
#include <commctrl.h>

#include "Messages.h"
#include "Warnings.h"
#include "Errors.h"


typedef struct tagPOPUPSTRING
  {
    HMENU hMenu;
    UINT uiString;
  }
POPUPSTRING;

#define MAX_MENUS 5

static POPUPSTRING popstr[MAX_MENUS];

DWORD dwStatusBarStyles = WS_CHILD | WS_VISIBLE |
WS_CLIPSIBLINGS | CCS_BOTTOM |
SBARS_SIZEGRIP;


/*-------------------------------------------------------------------*/

HWND 
InitStatusBar (HWND hwndParent)
{
  HWND hwndSB;
  /* Initialize values for WM_MENUSELECT message handling */
  hwndSB = CreateStatusWindow (dwStatusBarStyles,
			       MSG_SCIMSG55,
			       hwndParent,
			       2);
  return hwndSB;
}

/**********************************
 * Not used XXXXX to be finished 
 * GADGET in PETZOLD 
 ********************************/

LRESULT
Statusbar_MenuSelect (HWND hwnd, WPARAM wParam, LPARAM lParam)
{
  UINT fuFlags = (UINT) HIWORD (wParam);
  HMENU hMainMenu = NULL;
  int iMenu = 0;
  /* Handle non-system popup menu descriptions. */
  if ((fuFlags & MF_POPUP) &&
      (!(fuFlags & MF_SYSMENU)))
    {
      for (iMenu = 1; iMenu < MAX_MENUS; iMenu++)
	{
	  if ((HMENU) lParam == popstr[iMenu].hMenu)
	    {
	      hMainMenu = (HMENU) lParam;
	      break;
	    }
	}
    }

  /* Display helpful text in status bar
     MenuHelp (WM_MENUSELECT, wParam, lParam, hMainMenu, hInst, 
     hwndStatusBar, (UINT *) &popstr[iMenu]) ;
   */
  return 0;
}
