

/* Copyright (C) 1998 Chancelier Jean-Philippe */
/************************************************************************
 * wmhelp 
 * help menu for scilab 
 * Author Jean-Philippe Chancelier ( from the CDTEST demo of VC++) 
 ************************************************************************/

#ifndef STRICT
#define STRICT
#endif
#include <windows.h>
#ifndef __GNUC__
#include <commdlg.h>
#include <winnls.h>
#endif

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "wgnuplib.h"
#include "wresource.h"
#include "wcommon.h"
#include "../sun/h_help.h"

extern TW textwin;
HWND HelpModeless = (HWND) 0;	/* the modeless Help Window */

/* function prototypes and general variables */

BOOL APIENTRY HelpProc (HWND hwnd, UINT msg, UINT wParam, LONG lParam);
static void FillHelpsBox (HWND, DWORD);
static void FillHelpList (HWND hwnd);
static void SciApropos (HWND hwnd, char *str);

/************************************************************************
 * Function: DoHelpDialog(HWND)
 * Purpose: Creates the scilab help dialog 
 * Returns: Nothing.
 * Comments:
 ************************************************************************/

static int using_menu_help = 0;

void 
DoHelpDialog ()
{
  if (Help_Init () == 1)
    {
      sciprint ("Sorry, Scilab man will not work\r\n");
      return;
    }
  if (HelpModeless == (HWND) 0)
    {
      HelpModeless = CreateDialog (hdllInstance, MAKEINTRESOURCE (ID_HELPDIALOG),
				   NULL,
/** NULL or textwin.hWndParent, **/
				   HelpProc);
      using_menu_help = 1;
    }
  else
    sciprint ("You cannot open more than one help dialog at a time \r\n");
}

int 
help_popped_status ()
{
  return (using_menu_help);
}

/************************************************************************
 * Function: HelpProc(HWND, UINT, UINT, LONG)
 * Purpose:
 * Callback function for Scilab Help dialog.
 * Returns: TRUE or FALSE depending on the situation.
 ************************************************************************/
#define BUFSIZE 32

BOOL APIENTRY 
HelpProc (HWND hwnd, UINT msg, UINT wParam, LONG lParam)
{
  char buf[BUFSIZE];
  static int ihelp, ihelp1, ihelp2;
  switch (msg)
    {
    case WM_INITDIALOG:
      FillHelpsBox (hwnd, 1);
      FillHelpList (hwnd);
      ihelp1 = ihelp2 = -1;
      return TRUE;
      break;
    case WM_COMMAND:
      switch (LOWORD (wParam))
	{
	case ID_HelpLIST:
	  /** ihelp2=
	    (UINT)SendDlgItemMessage(hwnd,ID_HelpLIST,LB_GETCURSEL,0,0L); **/
/** sciprint("OK selection %d \r\n",ihelp2); **/
	  break;
	case ID_HELPSHOW:
	  ihelp2 = (UINT) SendDlgItemMessage (hwnd, ID_HelpLIST, LB_GETCURSEL, 0, 0L);
	  if (ihelp2 >= 0)
	    HelpActivate (ihelp2);
	  break;
	case ID_HelpLIST1:
	  ihelp = (UINT) SendDlgItemMessage (hwnd, ID_HelpLIST1, LB_GETCURSEL, 0, 0L);
	  FillHelpsBox (hwnd, ihelp + 1);
	  break;
	case IDCANCEL:
/** sciprint("OK fini sur cancel : \r\n");**/
	  DestroyWindow (hwnd);
	  HelpModeless = (HWND) 0;
	  return TRUE;
	  break;
	case ID_RESETHelp:
	  FillHelpsBox (hwnd, 2);
	  break;
	case ID_HELPEDIT:
	  GetDlgItemText (hwnd, ID_HELPEDIT, buf, BUFSIZE - 1);
/** sciprint(" Apropos : %s\n",buf);**/
	  if (strlen (buf) > 2)
	    SciApropos (hwnd, buf);
	  break;
	default:
	  break;
	}
      break;
    case WM_CLOSE:
/** sciprint("OK fini sur close : \r\n");**/
      DestroyWindow (hwnd);
      HelpModeless = (HWND) 0;
      return TRUE;
      break;
    default:
      break;
    }
  return FALSE;
}



/************************************************************************
 *  Function: FillHelpsBox(HWND, DWORD)
 * Changes the help list to help chapter iList 
 * if Ilist == 0 the list is supposed to come from SciApropos
 ************************************************************************/

#define TOPICSIZE 256

static void 
FillHelpsBox (HWND hwnd, DWORD iList)
{
  int i, k;
  HWND hwndControl;
  char topic[TOPICSIZE];
  SendDlgItemMessage (hwnd, ID_HelpLIST, LB_RESETCONTENT, (WPARAM) 0, (LPARAM) 0);
  hwndControl = GetDlgItem (hwnd, ID_HelpLIST);
  SendMessage (hwndControl, WM_SETREDRAW, FALSE, 0L);
  if (iList == 0)
    {
      for (i = 0; i < AP.nTopic; i++)
	{
	  strncpy (topic, AP.HelpTopic[i], TOPICSIZE);
	  for (k = strlen (topic); k >= 0; k--)
	    {
	      if (topic[k] == '@')
		{
		  topic[k] = '\0';
		  break;
		}
	    }
	  SendDlgItemMessage (hwnd, ID_HelpLIST, LB_ADDSTRING,
			      (WPARAM) 0, (LONG) (LPTSTR) topic);
	}
    }
  else
    {
      setHelpTopicInfo (iList);
      if (nTopicInfo > 0)
	{
	  for (i = 0; i < nTopicInfo; i++)
	    {
	      strncpy (topic, helpTopicInfo[i], TOPICSIZE);
	      for (k = strlen (topic); k >= 0; k--)
		{
		  if (topic[k] == '@')
		    {
		      topic[k] = '\0';
		      break;
		    }
		}
	      SendDlgItemMessage (hwnd, ID_HelpLIST, LB_ADDSTRING,
				  (WPARAM) 0, (LONG) (LPTSTR) topic);
	    }
	}
    }
  /* Now redraw the Help list */
  SendDlgItemMessage (hwnd, ID_HelpLIST, LB_SETCURSEL, (WPARAM) 0, 0L);
  SendMessage (hwndControl, WM_SETREDRAW, TRUE, 0L);
  InvalidateRect (hwndControl, NULL, FALSE);
  return;
}

static void 
FillHelpList (HWND hwnd)
{
  int i;
  HWND hwndControl;
  SendDlgItemMessage (hwnd, ID_HelpLIST1, LB_RESETCONTENT, (WPARAM) 0, (LPARAM) 0);
  hwndControl = GetDlgItem (hwnd, ID_HelpLIST1);
  SendMessage (hwndControl, WM_SETREDRAW, FALSE, 0L);
  for (i = 0; i < nInfo; i++)
    SendDlgItemMessage (hwnd, ID_HelpLIST1, LB_ADDSTRING,
			(WPARAM) 0, (LONG) (LPTSTR) helpInfo[i]);
  SendDlgItemMessage (hwnd, ID_HelpLIST1, LB_SETCURSEL, (WPARAM) 0, 0L);
  SendMessage (hwndControl, WM_SETREDRAW, TRUE, 0L);
  InvalidateRect (hwndControl, NULL, FALSE);
  return;
}


/************************************
 * Scilab apropos function 
 * changes the current list to fit the apropos list for 
 * str 
 ************************************/

static void 
SciApropos (HWND hwnd, char *str)
{
  char buf[256];
  int status = AP.nTopic;
  if (SetAproposTopics (str) == 1)
    return;
/** memory allocation **/
  if (AP.nTopic == 0)
    {
      if (status != 0)
	{
	  FillHelpsBox (hwnd, (DWORD) 1);
	  sprintf (buf, "no topic %s, back to chap. one", str);
	  SetDlgItemText (hwnd, ID_HELPINFO, buf);
	}
      else
	{
	  sprintf (buf, "No Info on topic %s", str);
	  SetDlgItemText (hwnd, ID_HELPINFO, buf);
	}
      return;
    }
  sprintf (buf, " ");
  SetDlgItemText (hwnd, ID_HELPINFO, buf);
  FillHelpsBox (hwnd, (DWORD) 0);
}
