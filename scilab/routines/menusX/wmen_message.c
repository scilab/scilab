/* Copyright ENPC */

/* wmessage.c
 * Scilab 
 *   Jean-Philipe Chancelier 
 *   Bugs and mail : Scilab@inria.fr 
 */

#include "wmen_scilab.h"

extern SciMess ScilabMessage;

RECT SciMenusRect = {-1,-1,0,0};

/****************************************************
 * Event handler function for the line style window 
 * uses GetWindowLong(hwnd, 4) and SetWindowLong
 ****************************************************/

EXPORT BOOL CALLBACK 
SciMessageDlgProc(HWND hdlg, UINT wmsg, WPARAM wparam, LPARAM lparam)
{
  switch (wmsg) {
  case WM_INITDIALOG:
    if ( SciMenusRect.left != -1) 
      SetWindowPos(hdlg,HWND_TOP,SciMenusRect.left,SciMenusRect.top,0,0,
		   SWP_NOSIZE | SWP_NOZORDER );
    SetDlgItemText(hdlg, DI_TEXT,ScilabMessage.string);
    SetDlgItemText(hdlg, IDOK,ScilabMessage.pButName[0]);
    if ( ScilabMessage.nb >= 2) 
      SetDlgItemText(hdlg, IDCANCEL,ScilabMessage.pButName[1]);
    return TRUE;
  case WM_COMMAND:
    switch (LOWORD(wparam)) {
    case IDOK:
      /** GetDlgItemText(hdlg, DI_TEXT, str, MAXSTR-1);
      if (str[strlen(str)-1] == '\n') str[strlen(str)-1] = '\0' ;
      **/
      GetWindowRect(hdlg,&SciMenusRect);
      EndDialog(hdlg, IDOK);
      return TRUE;
    case IDCANCEL:
      GetWindowRect(hdlg,&SciMenusRect);
      EndDialog(hdlg, IDCANCEL);
      return TRUE;
    }
    break;
  }
  return FALSE;
}


/****************************************************
 * Activate the Message Dialog box window 
 ****************************************************/

static void MessageWinSize(char *str,int *w,int *h)
{
  int wl=0;
  *w=0,*h=0;
  while ( *str != '\0') 
    {
      if ( *str == '\n' ) 
	{
	  (*h)++; *w=Max(*w,wl);wl=0;
	}
      else 
	wl++;
      str++;
    }
}


int  ExposeMessageWindow()
{
  int w,h;
  char *buf;
  DLGPROC lpfnSciMessageDlgProc ;
  HWND hwndOwner ;
  lpfnSciMessageDlgProc = (DLGPROC) MyGetProcAddress("SciMessageDlgProc",
						    SciMessageDlgProc);
  MessageWinSize(ScilabMessage.string,&w,&h);
  if ( ScilabMessage.nb == 2) 
    {
      if ( h <= 8 && w <= 45)
	buf = "SciMessageDlgBox2";
      else 
	buf = "SciBigMessageDlgBox2";
    }
  else 
    {
      if ( h <= 8 && w <= 45)
	buf = "SciMessageDlgBox1";
      else 
	buf = "SciBigMessageDlgBox1";
    }
  if ( (hwndOwner = GetActiveWindow()) == NULL) 
    hwndOwner =  textwin.hWndParent;
  if (DialogBox(hdllInstance, buf,hwndOwner,
		lpfnSciMessageDlgProc)  == IDOK) 
    return(1);
  else 
    return(2);
}

int  ExposeMessageWindow1()
{
  int w,h;
  char *buf;
  DLGPROC lpfnSciMessageDlgProc ;
  HWND hwndOwner ;
  lpfnSciMessageDlgProc = (DLGPROC) MyGetProcAddress("SciMessageDlgProc",
						    SciMessageDlgProc);
  MessageWinSize(ScilabMessage.string,&w,&h);
  if ( ScilabMessage.nb == 2) 
    {
      if ( h <= 8 && w <= 45)
	buf = "SciMessageDlgBox2";
      else 
	buf = "SciBigMessageDlgBox2";
    }
  else 
    {
      if ( h <= 8 && w <= 45)
	buf = "SciMessageDlgBox1";
      else 
	buf = "SciBigMessageDlgBox1";
    }
  if ( (hwndOwner = GetActiveWindow()) == NULL) 
    hwndOwner =  textwin.hWndParent;
  CreateDialog(hdllInstance,buf,
	       hwndOwner, 
	       lpfnSciMessageDlgProc) ;
  return(0);
}
