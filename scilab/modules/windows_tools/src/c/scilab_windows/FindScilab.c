/*--------------------------------------------------------------------------*/
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Allan CORNET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
/*--------------------------------------------------------------------------*/ 
#include <string.h>
#include "FindScilab.h"
#include "version.h"
#include "win_mem_alloc.h" /* MALLOC */
#include "WndThread.h"
#include "resource.h"
/*--------------------------------------------------------------------------*/
#define LineMax 255
#define NumberScilabMax 10
/*--------------------------------------------------------------------------*/
static char BeginningHiddenScilabWindow[LineMax];
static char ListScilabName[NumberScilabMax][LineMax];
static char ListHiddenScilabName[NumberScilabMax][LineMax];
static int NumberScilab=0;
static BOOL MoreMaxNumberScilabMax=FALSE;
static int ItemChooseScilab;
/*--------------------------------------------------------------------------*/
static BOOL ExposeDialogBox=FALSE;
static char * ChooseScilabBox(void);
static BOOL CALLBACK ChooseScilabDlgProc(HWND hdlg, UINT wmsg, WPARAM wparam, LPARAM lparam);
/*--------------------------------------------------------------------------*/
BOOL HaveAnotherWindowScilab(void)
{
	BOOL Retour=FALSE;
	HWND CurrenthWnd=NULL;
	
	wsprintf(BeginningHiddenScilabWindow,FORMAT_TITLE_HIDDEN_WINDOWS,SCI_VERSION_STRING,0);
	/* scilab-5.0 hidden window */
	BeginningHiddenScilabWindow[strlen(BeginningHiddenScilabWindow)-4] = '\0';
	
	CurrenthWnd = GetWindow(GetDesktopWindow(),GW_CHILD);
	CurrenthWnd = GetWindow(CurrenthWnd,GW_HWNDFIRST);

	while ( CurrenthWnd != NULL )
	{
		char Title[LineMax];

		GetWindowText(CurrenthWnd,Title,(int)strlen(BeginningHiddenScilabWindow)+1);
		if (strcmp(Title,BeginningHiddenScilabWindow) == 0)
		{
			GetWindowText(CurrenthWnd,Title,LineMax);
			if (NumberScilab<NumberScilabMax)
			{
				wsprintf(ListHiddenScilabName[NumberScilab],"%s",Title);
			}
			else MoreMaxNumberScilabMax = TRUE;
			NumberScilab++;
			Retour = TRUE;
		}
		CurrenthWnd = GetWindow(CurrenthWnd,GW_HWNDNEXT);
	}
	return Retour;
}
/*--------------------------------------------------------------------------*/
char * ChooseAnotherWindowScilab(void)
{
	char *TitleScilabChoose = NULL;

	if (NumberScilab == 1)
	{
		TitleScilabChoose = (char*)MALLOC( (strlen(ListScilabName[NumberScilab-1])+1) * sizeof(char) );
		wsprintf(TitleScilabChoose,"%s",ListScilabName[NumberScilab-1]);
	}
	else
	{
		HWND hWndScilab = NULL;
		TitleScilabChoose = ChooseScilabBox();
		hWndScilab = FindWindow(NULL,TitleScilabChoose);
		if (hWndScilab == NULL) /* La fenetre n'existe plus */
		{
			FREE(TitleScilabChoose);
			TitleScilabChoose = NULL;
		}
	}
	return TitleScilabChoose;
}
/*--------------------------------------------------------------------------*/
static char * ChooseScilabBox(void)
{
	HWND hWndChooseScilabBox=NULL;
	char *TitleScilabChoose=NULL;
	HINSTANCE hInstanceThisDll = (HINSTANCE)GetModuleHandle("scilab_windows");
	
	hWndChooseScilabBox= CreateDialog(hInstanceThisDll,(LPCSTR)IDD_CHOOSEASCILAB,NULL,(DLGPROC)ChooseScilabDlgProc);

	if (hWndChooseScilabBox)
	{
		MSG msg;
		ExposeDialogBox=TRUE;
		ShowWindow(hWndChooseScilabBox, SW_SHOW);
		UpdateWindow(hWndChooseScilabBox);

		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE) || ExposeDialogBox )
		{
          TranslateMessage(&msg);
	      DispatchMessage(&msg);
		}
	}

	if (ItemChooseScilab == 0) // Launch A new Scilab
	{
		TitleScilabChoose = NULL;
	}
	else
	{
		TitleScilabChoose = MALLOC( (strlen(ListScilabName[ItemChooseScilab-1])+1) * sizeof(char) );
		wsprintf(TitleScilabChoose,"%s",ListScilabName[ItemChooseScilab-1]);
	}

	return TitleScilabChoose;
}
/*--------------------------------------------------------------------------*/
static BOOL CALLBACK ChooseScilabDlgProc(HWND hdlg, UINT wmsg, WPARAM wparam, LPARAM lparam)
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
					ItemChooseScilab = (UINT)SendDlgItemMessage(hdlg,IDC_LISTCHOOSEASCILAB,LB_GETCURSEL,0,0L);
					ExposeDialogBox = FALSE;
					DestroyWindow(hdlg);
					return TRUE;
				}
			return FALSE;

			case IDC_OK:
				{
					ItemChooseScilab = (UINT)SendDlgItemMessage(hdlg,IDC_LISTCHOOSEASCILAB,LB_GETCURSEL,0,0L);
					ExposeDialogBox = FALSE;
					DestroyWindow(hdlg);
				}
			return TRUE;
		}
	break;
	case WM_CLOSE:
		ExposeDialogBox = FALSE;
		DestroyWindow(hdlg);
		exit(0);
	break ;
  }
  return FALSE;
}
/*--------------------------------------------------------------------------*/
