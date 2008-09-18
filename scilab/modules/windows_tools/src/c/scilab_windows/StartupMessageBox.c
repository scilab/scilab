/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#include <windows.h>
#include <windowsx.h>
#include "resource.h"
#include "win_mem_alloc.h"
#include "StartupMessageBox.h"
#include "getScilabDirectory.h"
#include "version.h"
#include "PATH_MAX.h"
/*--------------------------------------------------------------------------*/
static BOOL CALLBACK StartupMessageBoxDlgProc(HWND hwnd,UINT Message, WPARAM wParam, LPARAM lParam);
static BOOL ON_STARTUPMESSAGEBOX_WM_INITDIALOG(HWND hDlg,HWND hwndFocus, LPARAM lParam);
static BOOL ON_STARTUPMESSAGEBOX_WM_COMMAND(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
/*--------------------------------------------------------------------------*/
static int MessageBoxLanguageCode=0;
/*--------------------------------------------------------------------------*/
void StartupMessageBox(void)
{
	HKEY key;
	DWORD result = 0, size = 4;
	char Clef[MAX_PATH];
	int DontShowStartupMessageBox = 0, Ans = 0;

	wsprintf(Clef,"SOFTWARE\\Scilab\\%s\\Settings",SCI_VERSION_STRING);
    result=RegOpenKeyEx(HKEY_CURRENT_USER, Clef, 0, KEY_QUERY_VALUE , &key);

	if ( RegQueryValueEx(key, "StartupMessage", 0, NULL, (LPBYTE)&Ans, &size) !=  ERROR_SUCCESS )
  {
  	DontShowStartupMessageBox = 0;
	}
	else
	{
		DontShowStartupMessageBox = Ans;
	}

	if ( result == ERROR_SUCCESS ) RegCloseKey(key);

	if (DontShowStartupMessageBox == 0)
	{
		HINSTANCE hInstanceThisDll = (HINSTANCE)GetModuleHandle("scilab_windows");
		DialogBox(hInstanceThisDll,MAKEINTRESOURCE(IDD_STARTUPBOX), NULL,(DLGPROC)StartupMessageBoxDlgProc);
	}
}
/*--------------------------------------------------------------------------*/
static BOOL CALLBACK StartupMessageBoxDlgProc(HWND hwnd,UINT Message, WPARAM wParam, LPARAM lParam)
{
   switch(Message)
   {
		case WM_INITDIALOG:
			HANDLE_WM_INITDIALOG(hwnd,wParam,lParam,ON_STARTUPMESSAGEBOX_WM_INITDIALOG);
			return TRUE;
		break;
		case WM_COMMAND:
			HANDLE_WM_COMMAND(hwnd,wParam,lParam,ON_STARTUPMESSAGEBOX_WM_COMMAND);	
			return TRUE;
		break;
    }
   return FALSE;
}
/*--------------------------------------------------------------------------*/
static BOOL ON_STARTUPMESSAGEBOX_WM_INITDIALOG(HWND hDlg,HWND hwndFocus, LPARAM lParam)
{
	extern char ScilexWindowName[MAX_PATH];

	HKEY key;
	DWORD result,size=4;
	int Language;
	char Clef[MAX_PATH];

	wsprintf(Clef,"SOFTWARE\\Scilab\\%s\\Settings",SCI_VERSION_STRING);
	result=RegOpenKeyEx(HKEY_CURRENT_USER, Clef, 0, KEY_QUERY_VALUE , &key);

	if ( RegQueryValueEx(key, "Language", 0, NULL, (LPBYTE)&Language, &size) !=  ERROR_SUCCESS )
	{
		MessageBoxLanguageCode = 83; /* English Default*/
	}
	else
	{
		MessageBoxLanguageCode = Language;
	}

	if ( result == ERROR_SUCCESS ) RegCloseKey(key);

	 CheckDlgButton(hDlg, IDC_CHECKSTARTUP, BST_UNCHECKED);
	 switch (MessageBoxLanguageCode)
	 {
		case 91:
			SetWindowText(hDlg,"Remarque Importante");
			SetDlgItemText(hDlg,IDC_OPENRELEASENOTES,"Ouvrir release_notes.txt");
			SetDlgItemText(hDlg,IDC_STARTUPMESSAGE,"Scilab 5.0 :\n\nVeuillez lire le fichier release_notes pour plus de détails.");
			SetDlgItemText(hDlg,IDC_CHECKSTARTUP,"Ne plus afficher ce message");
		break;

		case 83: default:
			SetWindowText(hDlg,"Warning");
			SetDlgItemText(hDlg,IDC_OPENRELEASENOTES,"Open release_notes.txt");
			SetDlgItemText(hDlg,IDC_STARTUPMESSAGE,"Scilab 5.0 :\n\nPlease read the release_notes file for more details.");
			SetDlgItemText(hDlg,IDC_CHECKSTARTUP,"Don't show this screen at startup");
		break;
	 }
	 return TRUE;
}
/*--------------------------------------------------------------------------*/
static BOOL ON_STARTUPMESSAGEBOX_WM_COMMAND(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch(id)
	{
		case IDOK:
		{
			HKEY key;
			DWORD result,dwsize=4;
			char Clef[MAX_PATH];
			int DontShowStartupMessageBox = 0;
			LONG TstRegCreateKeyEx=0;

			wsprintf(Clef,"SOFTWARE\\Scilab\\%s\\Settings",SCI_VERSION_STRING);  	
			TstRegCreateKeyEx=RegCreateKeyEx(HKEY_CURRENT_USER, Clef, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &key, &result);
			if (TstRegCreateKeyEx == ERROR_SUCCESS)
			{
				if(IsDlgButtonChecked(hwnd, IDC_CHECKSTARTUP) == BST_CHECKED)
				{
					//The Box is CHECKED!
					DontShowStartupMessageBox = 1;
					RegSetValueEx(key, "StartupMessage", 0, REG_DWORD, (LPBYTE)&DontShowStartupMessageBox, dwsize);
				}
				else
				{
					//The Box is NOT checked!
					DontShowStartupMessageBox = 0;
					RegSetValueEx(key, "StartupMessage", 0, REG_DWORD, (LPBYTE)&DontShowStartupMessageBox, dwsize);
				}
				RegCloseKey(key);
			}
			EndDialog(hwnd, IDOK);
		}
		break;

		case IDC_OPENRELEASENOTES:
		{
			char *ScilabDirectory=NULL;
			char Chemin[PATH_MAX];
			int error=0;

			ScilabDirectory = getScilabDirectory(FALSE);

			wsprintf(Chemin,"%s\\release_notes.txt",ScilabDirectory);
			if (ScilabDirectory){FREE(ScilabDirectory);ScilabDirectory=NULL;}		

			#pragma warning(disable:4311)
			/* warning C4311: 'type cast' : pointer truncation from 'HINSTANCE' to 'int' */
			error =(int)ShellExecute(NULL, "open", Chemin, NULL, NULL, SW_SHOWNORMAL);
			#pragma warning(default:4311)

			if (error <= 32)
			{
				switch (MessageBoxLanguageCode)
				{
					case 91:
						MessageBox(hwnd,"Impossible d'ouvrir le fichier release_notes.txt","Attention",MB_ICONWARNING);
					break;

					case 83: default:
						MessageBox(hwnd,"Couldn't Open release_notes.txt","Warning",MB_ICONWARNING);
					break;
				}
			}
		}
		break;
	}
	return TRUE;
}
/*--------------------------------------------------------------------------*/
