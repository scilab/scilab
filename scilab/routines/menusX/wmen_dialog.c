/* Copyright ENPC */
/***********************************************
 * wmdialog.c / Scilab
 *   Jean-Philippe Chancelier 
 *   Bugs and mail : Scilab@inria.fr 
 ***********************************************/

#include "wmen_scilab.h"

extern SciDialog ScilabDialog;

/****************************************************
 * Event handler function for the line style window 
 * uses GetWindowLong(hwnd, 4) and SetWindowLong
 ****************************************************/

EXPORT int CALLBACK 
SciDialogDlgProc(HWND hdlg, UINT wmsg, WPARAM wparam, LPARAM lparam)
{
  HWND dlgw;
  switch (wmsg) {
  case WM_INITDIALOG:
    if ( SciMenusRect.left != -1) 
      SetWindowPos(hdlg,HWND_TOP,SciMenusRect.left,SciMenusRect.top,0,0,
		   SWP_NOSIZE | SWP_NOZORDER );
    SetDlgItemText(hdlg, DI_TIT, ScilabDialog.description);
    SetDlgItemText(hdlg, DI_TEXT,ScilabDialog.init);
    SetDlgItemText(hdlg, IDOK,ScilabDialog.pButName[0]);
    SetDlgItemText(hdlg, IDCANCEL,ScilabDialog.pButName[1]);
    return TRUE;
  case WM_COMMAND:
    switch (LOWORD(wparam)) {
    case IDOK:
      dlgw = GetDlgItem(hdlg, DI_TEXT);
      if ( dlgw != NULL) 
	{
	  int l;
	  l= GetWindowTextLength(dlgw);
	  /* sciprint("string of size %d \n",l); */
	  if ( (dialog_str =  MALLOC( (l+2)*(sizeof(char)))) == NULL) 
	    {
	      Scistring("Malloc : No more place");
	      EndDialog(hdlg, IDCANCEL);
	      return TRUE;
	    }
	  GetDlgItemText(hdlg, DI_TEXT, dialog_str, l+1);
	  /* sciprint("str [%s]\n",dialog_str); */
	  if (dialog_str[strlen(dialog_str)-1] == '\n') 
	    dialog_str[strlen(dialog_str)-1] = '\0' ;
	  GetWindowRect(hdlg,&SciMenusRect);
	  EndDialog(hdlg, IDOK);
	  return TRUE;
	}
      else 
	{
	  EndDialog(hdlg, IDCANCEL);
	  return TRUE;
	}
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
 * Activate the Line Style Dialog box window 
 * GetWindowLong(hwnd, 4) must be available for use 
 ****************************************************/

int  DialogWindow()
{
  char *c;
  int i=0;
  HWND hwndOwner ;
  DLGPROC lpfnSciDialogDlgProc ;
  lpfnSciDialogDlgProc = (DLGPROC) MyGetProcAddress("SciDialogDlgProc",
						    SciDialogDlgProc);
  c=ScilabDialog.description;
  while ( *c != '\0') { if ( *c == '\n' ) i++; c++;};
  if ( i >= 3 ) 
    c="SciBigDialogDlgBox";
  else
    c="SciDialogDlgBox";
  if ( (hwndOwner = GetActiveWindow()) == NULL) 
    hwndOwner =  textwin.hWndParent;
  if (DialogBox(hdllInstance,c,hwndOwner,
		lpfnSciDialogDlgProc)  == IDOK) 
    return(TRUE);
  else 
    return(FALSE);
}


