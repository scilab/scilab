/* Copyright ENPC */
/* wmdialog.c
 * Scilab 
 *   Jean-Philipe Chancelier 
 *   Bugs and mail : Scilab@inria.fr 
 * scilab command  x_mdialog ( Windows part )
 * see also men_mdialog.c 
 */

#include "wmen_scilab.h"

/* Function prototypes */
static void FillInPropertyPage( PROPSHEETPAGE* , int, LPSTR, DLGPROC);
static int  GetVal(HWND hDlg,int i,int j);

extern MDialog SciMDialog;        /** used to stored the mdialog data **/

#define MAX_STR MAX_PATH
static char buf[MAX_STR]; /** maximum size of answer **/

/**************************************
 *  FUNCTION: MDialogPage(HWND, UINT, UINT, LONG)
 *  PURPOSE:  Processes messages for "Your Information" page 
 *  MESSAGES:
 *	
 *	WM_INITDIALOG - intializes the page
 *	WM_NOTIFY - processes the notifications sent to the page
 **************************************/

BOOL APIENTRY MDialogPage(HWND hDlg,UINT message,UINT wParam,LONG lParam)
{
  int i;
  switch (message)
    {
    case WM_INITDIALOG:
      for ( i=0 ; i < SciMDialog.NItPg[SciMDialog.CPage] ; i++ )
	{
	  SetDlgItemText(hDlg, IDE_MDTEXT1+i,
			 SciMDialog.pszTitle[SciMDialog.CPage*NITEMMAXPAGE+i]);
	  SendMessage(GetDlgItem(hDlg, (IDE_MD1+i)), WM_SETTEXT, 0, 
		      (LPARAM)SciMDialog.pszName[SciMDialog.CPage*NITEMMAXPAGE+i]);
	}
      SetDlgItemText(hDlg, IDE_MDTIT, SciMDialog.labels);
      break;
      /** sans effets pour l''instant 
    case WM_DRAWITEM:
      {
	  LPDRAWITEMSTRUCT lpdis = (LPDRAWITEMSTRUCT)lParam;
	  DrawIcon(lpdis->hDC, 0, 0, 
	  (HICON)GetClassLong(GetParent(hDlg), GCL_HICON));
      }
      break;
      **/
    case WM_NOTIFY:
      switch (((NMHDR FAR *) lParam)->code) 
	{
	case PSN_KILLACTIVE:
	  SetWindowLong(hDlg,	DWL_MSGRESULT, FALSE);
	  return 1;
	  break;
	case PSN_RESET:
	  /** cancel **/
	  SetWindowLong(hDlg,	DWL_MSGRESULT, FALSE);
	  break;
	case PSN_SETACTIVE:
	  if ( SciMDialog.CPage == 0 ) 
	    PropSheet_SetWizButtons(GetParent(hDlg), PSWIZB_NEXT);
	  if ( SciMDialog.CPage > 0 && SciMDialog.CPage < SciMDialog.NPages -1 ) 
	    PropSheet_SetWizButtons(GetParent(hDlg), PSWIZB_BACK | PSWIZB_NEXT);
	  if ( SciMDialog.CPage == SciMDialog.NPages -1 && SciMDialog.CPage != 0 )
	    PropSheet_SetWizButtons(GetParent(hDlg), PSWIZB_BACK | PSWIZB_FINISH);
	  if ( SciMDialog.CPage == SciMDialog.NPages -1 && SciMDialog.CPage == 0 )
	    PropSheet_SetWizButtons(GetParent(hDlg), PSWIZB_FINISH);
	  SendMessage(GetDlgItem(hDlg,0x3024 ), BM_SETSTYLE, (WPARAM)BS_PUSHBUTTON, MAKELONG(FALSE, 0));

	  /** Je ne sais pas pourquoi mais ce qui suit fait que le dernier menu est mal mis a jour 
	    avec gcwin32 avec VC++ ca marche : J'ai donc comment'e ce qui suit 
	    et mis le meme code plus haut au initdialog 
	    j'espere que ca ne fout pas la merde SciMDialog.CPageXXXXXX
	  for ( i =0 ; i < SciMDialog.NItPg[SciMDialog.CPage] ; i++)
	    SendMessage(GetDlgItem(hDlg, (IDE_MD1+i)), WM_SETTEXT, 0, 
			(LPARAM)SciMDialog.pszName[SciMDialog.CPage*NITEMMAXPAGE+i]);

	    **/
	  break;
	case PSN_WIZBACK:
	  for ( i =0 ; i < SciMDialog.NItPg[SciMDialog.CPage] ; i++) 
	    {
	      if ( GetVal(hDlg,i, SciMDialog.CPage*NITEMMAXPAGE+i) == 0) 
		SetWindowLong(hDlg,	DWL_MSGRESULT, FALSE);
	    }
	  SciMDialog.CPage--;
	  break;
	case PSN_WIZNEXT:
	  for ( i =0 ; i < SciMDialog.NItPg[SciMDialog.CPage] ; i++) 
	    {
	      if ( GetVal(hDlg,i, SciMDialog.CPage*NITEMMAXPAGE+i) == 0) 
		SetWindowLong(hDlg,	DWL_MSGRESULT, FALSE);
	    }
	  SciMDialog.CPage++;
	  break;
	case PSN_WIZFINISH:
	  for ( i =0 ; i < SciMDialog.NItPg[SciMDialog.CPage] ; i++) 
	    {
	      if ( GetVal(hDlg,i, SciMDialog.CPage*NITEMMAXPAGE+i) == 0) 
		SetWindowLong(hDlg,	DWL_MSGRESULT, FALSE);
	    }
	  break;
	default:
	  return FALSE;
    	}
      break;
    default:
      return FALSE;
    }
  return TRUE;   
}

/***/

static int GetVal(HWND hDlg,int i,int j)
{
  int n;
  SendDlgItemMessage(hDlg, (IDE_MD1+i),WM_GETTEXT,(WPARAM)MAX_STR,(LPARAM) buf);
  n=strlen(buf);
  if ( n != (int) strlen( SciMDialog.pszName[j]))
    {
      char *p;
      p = (char *) realloc((char*) SciMDialog.pszName[j], (n+1)*sizeof(char));
      if ( p == (char *) 0)
	{
	  SciMDialog.ierr=1;
	  return(0);
	}
      else 
	{
	  SciMDialog.pszName[j] = p ;
	}
    }
  strcpy(SciMDialog.pszName[j],buf);
  return(1);
}


/**************************************
 *  FUNCTION: FillInPropertyPage(PROPSHEETPAGE *, int, LPSTR, LPFN) 
 *  PURPOSE: Fills in the given PROPSHEETPAGE structure 
 *  COMMENTS:
 *      This function fills in a PROPSHEETPAGE structure with the
 *      information the system needs to create the page.
 **************************************/


void FillInPropertyPage( PROPSHEETPAGE* psp, int idDlg, LPSTR pszProc, DLGPROC pfnDlgProc)
{
  psp->dwSize = sizeof(PROPSHEETPAGE);
  psp->dwFlags = 0;
  psp->hInstance =  hdllInstance;
#ifdef __GNUC__
  psp->u1.pszTemplate = MAKEINTRESOURCE(idDlg);
  psp->u2.pszIcon = "texticon";
#else 
  psp->pszTemplate = MAKEINTRESOURCE(idDlg);
  psp->pszIcon = "texticon";
#endif
  psp->pfnDlgProc = pfnDlgProc;
  psp->pszTitle = pszProc;
  psp->lParam = 0;
}

/**************************************
 *    FUNCTION:   mDialogWindow();
 * WARNING Th calling function must check 
 * the maximum number of pages 
 **************************************/

int mDialogWindow(void)
{
  int i,lastitems;
  PROPSHEETPAGE psp[NPAGESMAX];
  PROPSHEETHEADER psh;
  HWND hwndOwner;
  if ( (hwndOwner = GetActiveWindow()) == NULL) 
    hwndOwner =  textwin.hWndParent;
  SciMDialog.NPages = SciMDialog.nv /  NITEMMAXPAGE;
  for ( i = 0 ; i < SciMDialog.NPages ; i++) 
    {
      SciMDialog.NItPg[i]= NITEMMAXPAGE;
      FillInPropertyPage( &psp[i], IDD_INFO3, TEXT("Scilab Dialog"),
			  (DLGPROC) MDialogPage );
    }
  /** Items in the last page **/
  lastitems =  SciMDialog.nv % NITEMMAXPAGE;
  if ( lastitems != 0 ) 
    {
      SciMDialog.NPages++;
      SciMDialog.NItPg[SciMDialog.NPages-1]= lastitems ;
      FillInPropertyPage( &psp[SciMDialog.NPages-1], IDD_INFO1+lastitems-1, 
			  TEXT("Scilab Dialog"), 
			  (DLGPROC)  MDialogPage);
    }
  SciMDialog.CPage = 0; 

  psh.dwSize = sizeof(PROPSHEETHEADER);
  psh.dwFlags = PSH_PROPSHEETPAGE | PSH_WIZARD | PSH_NOAPPLYNOW | PSP_USEHICON;
  psh.hwndParent = hwndOwner;
  psh.pszCaption = (LPSTR) TEXT("Scilab mdialog Wizard");
  psh.nPages = SciMDialog.NPages;
#ifdef  __GNUC__
  psh.u2.nStartPage = SciMDialog.CPage;
  psh.u3.ppsp = (LPCPROPSHEETPAGE) &psp;
#else 
  psh.nStartPage = SciMDialog.CPage;
  psh.ppsp = (LPCPROPSHEETPAGE) &psp;
#endif
  return (PropertySheet(&psh));
}






