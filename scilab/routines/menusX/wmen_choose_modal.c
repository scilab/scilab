/* Copyright ENPC */
/* wchoose.c 
 * Scilab [ gcwin32 and WIN32 ] 
 *   Jean-Philipe Chancelier 
 *   Bugs and mail : Scilab@inria.fr 
 * 
 */

#include "wmen_scilab.h"

/****************************************************
 * Scilab Choose Menu (Windows part )
 ****************************************************/

EXPORT BOOL CALLBACK 
ChooseDlgProc(HWND hdlg, UINT wmsg, WPARAM wparam, LPARAM lparam)
{
 
  int i;
  ChooseMenu *Ch;  
  Ch = (ChooseMenu * )GetWindowLong(GetParent(hdlg), 4);
  switch (wmsg) {
  case WM_INITDIALOG:
    for (i=0; i < Ch->nstrings  ; i++) {
      SendDlgItemMessage(hdlg, CHOOSE_LINENUM, LB_ADDSTRING, 0, 
			 (LPARAM)((LPSTR) Ch->strings[i]));
    }
    SendDlgItemMessage(hdlg, CHOOSE_LINENUM, LB_SETCURSEL,Ch->choice, 0L);
    SetDlgItemText(hdlg, CHOOSE_TEXT, Ch->description);
    return TRUE;
  case WM_COMMAND:
    Ch->choice = (UINT)SendDlgItemMessage(hdlg,CHOOSE_LINENUM,LB_GETCURSEL,0,0L);
    switch (LOWORD(wparam)) 
      {
      case CHOOSE_LINENUM:
	return FALSE;
      case IDOK:
	EndDialog(hdlg, IDOK);
	return TRUE;
      case IDCANCEL:
	EndDialog(hdlg, IDCANCEL);
	return TRUE;
      }
    break;
  case WM_DRAWITEM:
    {
      LPDRAWITEMSTRUCT lpdis = (LPDRAWITEMSTRUCT)lparam;
      DrawIcon(lpdis->hDC, 0, 0, 
	       (HICON)GetClassLong(GetParent(hdlg), GCL_HICON));
      Ch->choice = (UINT)SendDlgItemMessage(hdlg, CHOOSE_LINENUM, LB_GETCURSEL,
					    (WPARAM)0, (LPARAM)0);
    }
  return FALSE;
  }
  return FALSE;
}

/****************************************************
 * Activate the Menu 
 ****************************************************/

int ExposeChooseWindow( ChooseMenu *PCh)
{
  DLGPROC   MyChooseDlgProc ;
  int status = FALSE;
  SetWindowLong(textwin.hWndParent, 4, (LONG) PCh);
  MyChooseDlgProc = (DLGPROC) MyGetProcAddress("ChooseDlgProc",
						    ChooseDlgProc);
  if (DialogBox(hdllInstance, "SciChooseDlgBox", 
		textwin.hWndParent, MyChooseDlgProc)  == IDOK) 
    {
      status = TRUE;
    }
  SetWindowLong(textwin.hWndParent, 4, (LONG)(0L));
  return status;
}

