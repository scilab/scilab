/* Copyright ENPC */
/* wmchoice1.c
 * Scilab 
 *   Jean-Philippe Chancelier 
 *   Bugs and mail : Scilab@ENPC.fr 
 */

#include "wmen_scilab.h"

/* Function prototypes */
static void FillInPropertyPage( PROPSHEETPAGE* , int, LPSTR, DLGPROC);
static int  GetVal(HWND hDlg,int i,int j);


/* WARNING: it's not enough to change the following
 * define in order to increase the number of possible items 
 */

#define NPAGESMAX 10
#define NITEMMAXPAGE 3 

typedef struct {
  int  NItPg[NPAGESMAX];  /* Number of Items in page i; */
  int  NPages;            /* Number of Pages  */
  int  CPage;             /* The number of current page **/
  int  nv;                /** number of items **/
  int  ierr;
  char *label;
}  MChoice ;

static MChoice SciMChoice;        /** used to stored the MChoice data **/
#define MAX_STR MAX_PATH

int SciChoiceI(label,defval,nitems)
     char *label;
     int defval[], nitems;
{
  int rep,i;
  SciMChoice.nv = nitems;
  SciMChoice.ierr=0;
  SciMChoice.label = label;
  rep=mChoiceWindow();
  if (  rep == TRUE ) 
    {
      for ( i=0 ; i < nitems ; i++) 
	{
	  /*sciprint("Item %d choix %d\r\n",i+1,  
	    Everything[i]->choice.default_toggle +1); */
	  defval[i]= Everything[i]->choice.default_toggle +1;
	}
      return(TRUE);
    }
  else
    return(FALSE);
}

/****************************************************
 *   SciChoiceCreate(items,defval,nitems) 
 *   This fuction is used to create the required SciStuff  
 *   Object in order to call create_choices  
 *   from a simpler data structure in order to be able  
 *   to communicate with Scilab  
 *   char*  items[] = {     "Label1",      "choice1",      "choice2",...,NULL, 
 *                          "Label2",      "choice1",      "choice2",...,NULL} 
 *   les valeurs par defaut sont numerotes a partir de 1  
 *   int  defval[]={1,2,3,....} 
 *   nitems : number of labels 	 
 *   En sortie defval contient ce qu'on a choisit  
 ****************************************************/

int 
SciChoiceCreate(items,defval,nitems)
     char **items;
     int defval[];
     int nitems;
{
  int i,j;
  if ( Everything != (SciStuff **) NULL) 
    {
      /** someone is using toggles at the same time */
      return(-1);
    }
  Everything= (SciStuff **) MALLOC( (nitems+1)*sizeof(SciStuff *));
  if ( Everything == (SciStuff **) NULL) return(0);
  Everything[nitems]= (SciStuff *) NULL;
  for ( i=0 ; i < nitems ; i++) 
    {
      char **loc= items ;
      int numch=0;
      while ( *loc != (char *) NULL) { loc++;numch++; };
      numch--;
      if ( numch == 0) 
	{
	  sciprint("x_choices : There's no choice to the %d item\r\n",i);
	  return(0);
	};
      Everything[i]= (SciStuff *) MALLOC( sizeof(SciStuff));
      if ( Everything[i] == (SciStuff *) NULL) 
	{
	  return(0);
	}
      if ( AllocAndCopy(&(Everything[i]->choice.name),items[0]) == 0) 
	{
	  return(0);
	}
      if ( AllocAndCopy(&(Everything[i]->choice.text),items[0]) == 0) 
	{
	  return(0);
	}
      Everything[i]->choice.num_toggles= numch;
      Everything[i]->choice.default_toggle = Min(Max(0,defval[i]-1),numch-1);
      Everything[i]->data = (SciData *) MALLOC( numch*sizeof(SciData));
      if ( Everything[i]->data == (SciData *) NULL) 
	{
	  return(0);
	}
      for ( j = 0 ; j < numch ; j++) 
	{
	  char loc[8];
	  SciData *dataloc = Everything[i]->data ;
	  if ( AllocAndCopy(&(dataloc[j].name),items[j+1]) == 0) 
	    {
	      return(0);
	    }
	  sprintf(loc,"%d %d",i,j);
	  if ( AllocAndCopy(&(dataloc[j].cbinfo),loc) == 0) 
	    {
	      return(0);
	    }
	}
      items = items + numch+2;
    }
  return(1);
}



int AllocAndCopy(strh1,str2)
     char **strh1, *str2;
{
  *strh1= (char *) MALLOC((strlen(str2)+1)*sizeof(char));
  if ( *strh1 == (char *) NULL) return(0);
  strcpy(*strh1,str2);
  return(1);
}


int SciChoiceFree(int nitems) 
{
  int i,j;
  for ( i=0 ; i < nitems ; i++) 
    {
      for (j = 0 ; j < Everything[i]->choice.num_toggles ; j++) 
	{
	  SciData *dataloc = Everything[i]->data ;
	  FREE(dataloc[j].name);
	  FREE(dataloc[j].cbinfo);
	}
      FREE(Everything[i]->data) ;
      FREE(Everything[i]->choice.name);
      FREE(Everything[i]->choice.text);
    }
  FREE(Everything);
  Everything = NULL;
  return(0);
}


/**************************************
 *  FUNCTION: MChoicePage(HWND, UINT, UINT, LONG)
 *  PURPOSE:  Processes messages for "Your Information" page 
 *  MESSAGES:
 *	
 *	WM_INITDIALOG - intializes the page
 *	WM_NOTIFY - processes the notifications sent to the page
 **************************************/

static BOOL APIENTRY MChoicePage(HWND hDlg,UINT message,UINT wParam,LONG lParam)
{
  int i,j;
  switch (message)
    {
    case WM_INITDIALOG:
      for ( i=0 ; i < SciMChoice.NItPg[SciMChoice.CPage] ; i++ )
	{
	  SetDlgItemText(hDlg, IDE_MDTEXT1+i, 
			 Everything[SciMChoice.CPage*NITEMMAXPAGE+i]->choice.name);
	  for ( j=0 ; j < Everything[SciMChoice.CPage*NITEMMAXPAGE+i]->choice.num_toggles; j++ ) 
	    SendDlgItemMessage(hDlg,IDE_MD1+i , CB_ADDSTRING, 0, 
			       (LPARAM)((LPSTR) Everything[SciMChoice.CPage*NITEMMAXPAGE+i]->data[j].name));
	  SendDlgItemMessage(hDlg,IDE_MD1+i , CB_SETCURSEL, 
			     (LPARAM)((LPSTR) Everything[SciMChoice.CPage*NITEMMAXPAGE+i]->choice.default_toggle) ,
			     0L);
	}
      SetDlgItemText(hDlg, IDE_MDTIT, SciMChoice.label);
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
	  if ( SciMChoice.CPage == 0 ) 
	    PropSheet_SetWizButtons(GetParent(hDlg), PSWIZB_NEXT);
	  if ( SciMChoice.CPage > 0 && SciMChoice.CPage < SciMChoice.NPages -1 ) 
	    PropSheet_SetWizButtons(GetParent(hDlg), PSWIZB_BACK | PSWIZB_NEXT);
	  if ( SciMChoice.CPage == SciMChoice.NPages -1 && SciMChoice.CPage != 0 )
	    PropSheet_SetWizButtons(GetParent(hDlg), PSWIZB_BACK | PSWIZB_FINISH);
	  if ( SciMChoice.CPage == SciMChoice.NPages -1 && SciMChoice.CPage == 0 )
	    PropSheet_SetWizButtons(GetParent(hDlg), PSWIZB_FINISH);
	  SendMessage(GetDlgItem(hDlg,0x3024 ), BM_SETSTYLE, (WPARAM)BS_PUSHBUTTON, MAKELONG(FALSE, 0));

	  /** Je ne sais pas pourquoi mais ce qui suit fait que le dernier menu est mal mis a jour 
	    avec gcwin32 avec VC++ ca marche : J'ai donc comment'e ce qui suit 
	    et mis le meme code plus haut au initdialog 
	    j'espere que ca ne fout pas la merde SciMChoice.CPageXXXXXX
	  for ( i =0 ; i < SciMChoice.NItPg[SciMChoice.CPage] ; i++)
	    SendMessage(GetDlgItem(hDlg, (IDE_MD1+i)), WM_SETTEXT, 0, 
			(LPARAM)SciMChoice.pszName[SciMChoice.CPage*NITEMMAXPAGE+i]);

	    **/
	  break;
	case PSN_WIZBACK:
	  for ( i =0 ; i < SciMChoice.NItPg[SciMChoice.CPage] ; i++) 
	    {
	      if ( GetVal(hDlg,i, SciMChoice.CPage*NITEMMAXPAGE+i) == 0) 
		SetWindowLong(hDlg,	DWL_MSGRESULT, FALSE);
	    }
	  SciMChoice.CPage--;
	  break;
	case PSN_WIZNEXT:
	  for ( i =0 ; i < SciMChoice.NItPg[SciMChoice.CPage] ; i++) 
	    {
	      if ( GetVal(hDlg,i, SciMChoice.CPage*NITEMMAXPAGE+i) == 0) 
		SetWindowLong(hDlg,	DWL_MSGRESULT, FALSE);
	    }
	  SciMChoice.CPage++;
	  break;
	case PSN_WIZFINISH:
	  for ( i =0 ; i < SciMChoice.NItPg[SciMChoice.CPage] ; i++) 
	    {
	      if ( GetVal(hDlg,i, SciMChoice.CPage*NITEMMAXPAGE+i) == 0) 
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
  Everything[j]->choice.default_toggle = (UINT)SendDlgItemMessage(hDlg, (IDE_MD1+i), CB_GETCURSEL, 0, 0L);
  return(1);
}


/**************************************
 *  FUNCTION: FillInPropertyPage(PROPSHEETPAGE *, int, LPSTR, LPFN) 
 *  PURPOSE: Fills in the given PROPSHEETPAGE structure 
 *  COMMENTS:
 *      This function fills in a PROPSHEETPAGE structure with the
 *      information the system needs to create the page.
 **************************************/

static void FillInPropertyPage( PROPSHEETPAGE* psp, int idDlg, LPSTR pszProc, DLGPROC pfnDlgProc)
{
  psp->dwSize = sizeof(PROPSHEETPAGE);
  psp->dwFlags = 0;
  psp->hInstance =  hdllInstance;
#ifdef __GNUC__
  psp->u1.pszTemplate = MAKEINTRESOURCE(idDlg);
  psp->u2.pszIcon = NULL;
#else 
  psp->pszTemplate = MAKEINTRESOURCE(idDlg);
  psp->pszIcon = NULL;
#endif
  psp->pfnDlgProc = pfnDlgProc;
  psp->pszTitle = pszProc;
  psp->lParam = 0;
}

/**************************************
 *    FUNCTION:   mChoiceWindow();
 * WARNING Th calling function must check 
 * the maximum number of pages 
 **************************************/

int mChoiceWindow(void)
{
  int i,lastitems;
  PROPSHEETPAGE psp[NPAGESMAX];
  PROPSHEETHEADER psh;
  HWND hwndOwner;
  if ( (hwndOwner = GetActiveWindow()) == NULL) 
    hwndOwner =  textwin.hWndParent;
  SciMChoice.NPages = SciMChoice.nv /  NITEMMAXPAGE;
  for ( i = 0 ; i < SciMChoice.NPages ; i++) 
    {
      SciMChoice.NItPg[i]= NITEMMAXPAGE;
      FillInPropertyPage( &psp[i], IDD_MCHOICE3, TEXT("Scilab Dialog"),
			  (DLGPROC) MChoicePage );
    }
  /** Items in the last page **/
  lastitems =  SciMChoice.nv % NITEMMAXPAGE;
  if ( lastitems != 0 ) 
    {
      SciMChoice.NPages++;
      SciMChoice.NItPg[SciMChoice.NPages-1]= lastitems ;
      FillInPropertyPage( &psp[SciMChoice.NPages-1], IDD_MCHOICE1+lastitems-1, 
			  TEXT("Scilab Dialog"), 
			  (DLGPROC)  MChoicePage);
    }
  SciMChoice.CPage = 0; 
  psh.dwSize = sizeof(PROPSHEETHEADER);
  psh.dwFlags = PSH_PROPSHEETPAGE | PSH_WIZARD | PSH_NOAPPLYNOW;
  psh.hwndParent = hwndOwner;
  psh.pszCaption = (LPSTR) TEXT("Review Wizard");
  psh.nPages = SciMChoice.NPages;
#ifdef __GNUC__
  psh.u2.nStartPage = SciMChoice.CPage;
  psh.u3.ppsp = (LPCPROPSHEETPAGE) &psp;
#else 
  psh.nStartPage = SciMChoice.CPage;
  psh.ppsp = (LPCPROPSHEETPAGE) &psp;
#endif
  return (PropertySheet(&psh));
}



