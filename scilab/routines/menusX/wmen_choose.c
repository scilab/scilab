/* Copyright ENPC */
/* wchoose.c 
 * Scilab [ gcwin32 and WIN32 ] 
 *   Jean-Philipe Chancelier 
 *   Bugs and mail : Scilab@inria.fr 
 * modeless version 
 */

#include "wmen_scilab.h"

static HWND ChooseModeless = (HWND) 0 ; /* the modeless Choose Window */
static ChooseMenu *Ch ;

/****************************************************
 * Scilab Choose Menu (Windows part )
 ****************************************************/

EXPORT BOOL CALLBACK 
ChooseDlgProc(HWND hdlg, UINT wmsg, WPARAM wparam, LPARAM lparam)
{
  int i;
  /** Ch = (ChooseMenu * )GetWindowLong(GetParent(hdlg), 4); **/
  switch (wmsg) 
  {
  case WM_INITDIALOG:
		if ( SciMenusRect.left != -1) 
			SetWindowPos(hdlg,HWND_TOP,SciMenusRect.left,SciMenusRect.top,0,0,
				         SWP_NOSIZE | SWP_NOZORDER );
		for (i=0; i < Ch->nstrings  ; i++)
		{
			SendDlgItemMessage(hdlg, CHOOSE_LINENUM, LB_ADDSTRING, 0, 
								(LPARAM)((LPSTR) Ch->strings[i]));
		}
		SendDlgItemMessage(hdlg, CHOOSE_LINENUM, LB_SETCURSEL,Ch->choice, 0L);
		SetDlgItemText(hdlg, CHOOSE_TEXT, Ch->description);
		/* Change le nom du bouton */
		SetDlgItemText(hdlg, IDCANCEL,Ch->buttonname[0]);
  return TRUE;
  case WM_COMMAND:
		Ch->choice = (UINT)SendDlgItemMessage(hdlg,CHOOSE_LINENUM,LB_GETCURSEL,0,0L);
		switch (LOWORD(wparam)) 
		{

			case CHOOSE_LINENUM:
				/* Ajout Gestion double-click */
				if ( HIWORD( wparam ) == LBN_DBLCLK )
				{
					GetWindowRect(hdlg,&SciMenusRect);
					DestroyWindow(hdlg);
					ChooseModeless = (HWND)0;
					Ch->status = TRUE;
					return TRUE;
				}
			return FALSE;
			case IDOK:
				GetWindowRect(hdlg,&SciMenusRect);
				DestroyWindow(hdlg);
				ChooseModeless = (HWND)0;
				Ch->status = TRUE;
			return TRUE;
			case IDCANCEL:
				GetWindowRect(hdlg,&SciMenusRect);
				DestroyWindow(hdlg);
				ChooseModeless = (HWND)0;
				Ch->status = FALSE;
			return TRUE;
		}
  break;
  case WM_CLOSE:
		/** sciprint("OK fini sur close : \r\n");**/
		GetWindowRect(hdlg,&SciMenusRect);
		DestroyWindow(hdlg);
		ChooseModeless = (HWND)0;
		Ch->status = FALSE;
		return TRUE;
  break ;
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
  MSG msg;
  DLGPROC   MyChooseDlgProc ;
  PCh->status = -1;
  /** SetWindowLong(textwin.hWndParent, 4, (LONG) PCh); **/
  Ch = PCh;
  MyChooseDlgProc = (DLGPROC) MyGetProcAddress("ChooseDlgProc",
					       ChooseDlgProc);

  if (ChooseModeless  == (HWND)0 )
    ChooseModeless= CreateDialog(hdllInstance,"SciChooseDlgBox",
				 NULL,
				 /** NULL or textwin.hWndParent, **/
				 MyChooseDlgProc) ;
  else
    {
      sciprint("You cannot open more than one choose dialog at a time \r\n");
      return  FALSE;
    }
  while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE) || PCh->status == -1 )
    {
	  Sleep(1);
      if ( CtrlCHit(&textwin) == 1) 
		{
			if ( ChooseModeless != (HWND) 0) DestroyWindow( ChooseModeless) ;
			ChooseModeless = (HWND)0;
			PCh->status = FALSE;
			break;
		}
      if (  ChooseModeless ==0 || ! IsDialogMessage(ChooseModeless,&msg)) 
		{
	      TranslateMessage(&msg);
	      DispatchMessage(&msg);
		}
    }
  SetWindowLong(textwin.hWndParent, 4, (LONG)(0L));
  return PCh->status;
}

