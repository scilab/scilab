#include "FindScilab.h"
#include "../version.h"
#include "resource.h"
/* Allan CORNET INRIA 2004 */
/*-----------------------------------------------------------------------------------*/
#define LineMax 255
#define NumberScilabMax 10
/*-----------------------------------------------------------------------------------*/
static char BeginningWindowScilabName[LineMax];

static char ListScilabName[NumberScilabMax][LineMax];
static int NumberScilab=0;
static BOOL MoreMaxNumberScilabMax=FALSE;
/*-----------------------------------------------------------------------------------*/
static BOOL ExposeDialogBox=FALSE;
char * ChooseScilabBox(void);
void ExposeChooseScilabBox(void);
BOOL CALLBACK ChooseDlgProc(HWND hdlg, UINT wmsg, WPARAM wparam, LPARAM lparam);
static int ItemChooseScilab;
/*-----------------------------------------------------------------------------------*/
BOOL HaveAnotherWindowScilab(void)
{
	BOOL Retour=FALSE;
	HWND CurrenthWnd=NULL;
	
	int a=0;

	wsprintf(BeginningWindowScilabName,"%s (",VERSION);
	
	CurrenthWnd=GetWindow(GetDesktopWindow(),GW_CHILD);
	CurrenthWnd=GetWindow(CurrenthWnd,GW_HWNDFIRST);

	while ( CurrenthWnd!= NULL )
	{
		char Title[LineMax];

		GetWindowText(CurrenthWnd,Title,strlen(BeginningWindowScilabName)+1);
		if (strcmp(Title,BeginningWindowScilabName) == 0)
		{
			GetWindowText(CurrenthWnd,Title,LineMax);
			if (NumberScilab<NumberScilabMax)
			{
				wsprintf(ListScilabName[NumberScilab],"%s",Title);
			}
			else MoreMaxNumberScilabMax=TRUE;
			NumberScilab++;
			Retour=TRUE;
		}
	
		CurrenthWnd=GetWindow(CurrenthWnd,GW_HWNDNEXT);
	}

	return Retour;
}
/*-----------------------------------------------------------------------------------*/
char * ChooseAnotherWindowScilab(void)
{
	char *TitleScilabChoose=NULL;
	if (NumberScilab == 1)
	{
		TitleScilabChoose=malloc( (strlen(ListScilabName[NumberScilab-1])+1) * sizeof(char) );
		wsprintf(TitleScilabChoose,"%s",ListScilabName[NumberScilab-1]);
	}
	else
	{
		HWND hWndScilab=NULL;
		TitleScilabChoose=ChooseScilabBox();
		hWndScilab=FindWindow(NULL,TitleScilabChoose);
		if (hWndScilab==NULL) /* La fenetre n'existe plus */
		{
			free(TitleScilabChoose);
			TitleScilabChoose=NULL;
		}
	}

	return TitleScilabChoose;


}
/*-----------------------------------------------------------------------------------*/
char * ChooseScilabBox(void)
{
	extern HINSTANCE hdllInstance;
	MSG msg;
	HWND hWndChooseScilabBox=NULL;
	char *TitleScilabChoose=NULL;
	DLGPROC   MyChooseScilabDlgProc ;

	
	MyChooseScilabDlgProc = (DLGPROC) GetProcAddress((HINSTANCE)GetModuleHandle("LibScilab"),"ChooseScilabDlgProc");
	
	hWndChooseScilabBox= CreateDialog((HINSTANCE)GetModuleHandle(NULL),(LPCSTR)IDD_CHOOSEASCILAB,NULL,MyChooseScilabDlgProc) ;

	if (hWndChooseScilabBox)
	{
		ExposeDialogBox=TRUE;
		ShowWindow(hWndChooseScilabBox, SW_SHOW);
		UpdateWindow(hWndChooseScilabBox);

		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE) || ExposeDialogBox )
		{
          TranslateMessage(&msg);
	      DispatchMessage(&msg);
		}
    
	}

	if (ItemChooseScilab==0) // Launch A new Scilab
	{
		TitleScilabChoose=NULL;
	}
	else
	{
		TitleScilabChoose=malloc( (strlen(ListScilabName[ItemChooseScilab-1])+1) * sizeof(char) );
		wsprintf(TitleScilabChoose,"%s",ListScilabName[ItemChooseScilab-1]);
	}

	return TitleScilabChoose;
}
/*-----------------------------------------------------------------------------------*/

BOOL CALLBACK ChooseScilabDlgProc(HWND hdlg, UINT wmsg, WPARAM wparam, LPARAM lparam)
{
  switch (wmsg) 
  {
	case WM_INITDIALOG:
		{
			int i=0;

			SendDlgItemMessage(hdlg, IDC_LISTCHOOSEASCILAB, LB_ADDSTRING, 0, (LPARAM)((LPSTR)"New Scilab"));
			for (i=0; i < NumberScilab  ; i++)
				{
				 if (i < NumberScilabMax) SendDlgItemMessage(hdlg, IDC_LISTCHOOSEASCILAB, LB_ADDSTRING, 0, (LPARAM)((LPSTR)ListScilabName[i] ));
				}
			/* Par défaut , le premier Scilab Trouvé est selectionné */
			SendDlgItemMessage(hdlg, IDC_LISTCHOOSEASCILAB, LB_SETCURSEL,1, 0L);
		}
	return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wparam)) 
		{
			case IDC_LISTCHOOSEASCILAB:
				if ( HIWORD( wparam ) == LBN_DBLCLK )
				{
					ItemChooseScilab=(UINT)SendDlgItemMessage(hdlg,IDC_LISTCHOOSEASCILAB,LB_GETCURSEL,0,0L);
					ExposeDialogBox=FALSE;
					DestroyWindow(hdlg);
					return TRUE;
				}
			return FALSE;

			case IDC_OK:
				{
					ItemChooseScilab=(UINT)SendDlgItemMessage(hdlg,IDC_LISTCHOOSEASCILAB,LB_GETCURSEL,0,0L);
					ExposeDialogBox=FALSE;
					DestroyWindow(hdlg);
				}
			return TRUE;

		}
	break;
	case WM_CLOSE:
		ExposeDialogBox=FALSE;
		DestroyWindow(hdlg);
		exit(0);
		return TRUE;
	break ;

  }
  return FALSE;
}
/*-----------------------------------------------------------------------------------*/
