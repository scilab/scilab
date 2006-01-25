/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "ExportBox.h"
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
/*-----------------------------------------------------------------------------------*/
char *Print_Formats[] =
	{
		"Postscript",
		"Postscript No Preamble",
		"Postscript-Latex",
		"Xfig",
		"GIF",
		"PPM",
		"BMP",
		"Enhanced Meta File (.emf)"
	};

LS ls =	{1, 0, 0, 0};
/*-----------------------------------------------------------------------------------*/
extern HINSTANCE hdllInstance;
/*-----------------------------------------------------------------------------------*/
/****************************************************
 * Event handler function for the line style window 
 * uses GetWindowLong(hwnd, 4) and SetWindowLong
 ****************************************************/

EXPORT BOOL CALLBACK
ExportStyleDlgProc (HWND hdlg, UINT wmsg, WPARAM wparam, LPARAM lparam)
{
  int i;
  switch (wmsg)
    {
    case WM_INITDIALOG:
      SendDlgItemMessage (hdlg, PS_COLOR, CB_ADDSTRING, 0,
			  (LPARAM) ((LPSTR) MSG_SCIMSG92));
      SendDlgItemMessage (hdlg, PS_COLOR, CB_ADDSTRING, 0,
			  (LPARAM) ((LPSTR) MSG_SCIMSG93));
      SendDlgItemMessage (hdlg, PS_COLOR, CB_SETCURSEL,
			  ls.colored, 0L);
      SendDlgItemMessage (hdlg, PS_LAND, CB_ADDSTRING, 0,
			  (LPARAM) ((LPSTR) MSG_SCIMSG94));
      SendDlgItemMessage (hdlg, PS_LAND, CB_ADDSTRING, 0,
			  (LPARAM) ((LPSTR) MSG_SCIMSG95));
      SendDlgItemMessage (hdlg, PS_LAND, CB_SETCURSEL,
			  ls.land, 0L);
      if (ls.use_printer == 0)
	{
	  for (i = 0; i < 8; i++)
	    SendDlgItemMessage (hdlg, PS_TYPE, CB_ADDSTRING, 0,
				(LPARAM) ((LPSTR) Print_Formats[i]));
	  SendDlgItemMessage (hdlg, PS_TYPE, CB_SETCURSEL,
			      ls.ps_type, 0L);
	}
      return TRUE;
    case WM_COMMAND:
      switch (LOWORD (wparam))
	{
	case PS_COLOR:
	  ls.colored =
	    (UINT) SendDlgItemMessage (hdlg, PS_COLOR, CB_GETCURSEL, 0, 0L);
	  return FALSE;
	case PS_TYPE:
	  ls.ps_type =
	    (UINT) SendDlgItemMessage (hdlg, PS_TYPE, CB_GETCURSEL, 0, 0L);
	  return FALSE;
	case PS_LAND:
	  ls.land =
	    (UINT) SendDlgItemMessage (hdlg, PS_LAND, CB_GETCURSEL, 0, 0L);
	  return FALSE;
	case IDOK:
	  EndDialog (hdlg, IDOK);
	  return TRUE;
	case IDCANCEL:
	  EndDialog (hdlg, IDCANCEL);
	  return TRUE;
	}
      break;
      return FALSE;
    }
  return FALSE;
}
/*-----------------------------------------------------------------------------------*/
/****************************************************
 * Export style dialog box 
 ****************************************************/
BOOL ExportStyle (struct BCG * ScilabGC)
{
  DLGPROC lpfnExportStyleDlgProc;
  BOOL status = FALSE;
  lpfnExportStyleDlgProc = (DLGPROC) MyGetProcAddress ("ExportStyleDlgProc",ExportStyleDlgProc);
  if (DialogBox (hdllInstance,"ExportStyleDlgBox",
		 ScilabGC->hWndParent, lpfnExportStyleDlgProc) == IDOK)
    {
      status = TRUE;
    }
  return status;
}
/*-----------------------------------------------------------------------------------*/
