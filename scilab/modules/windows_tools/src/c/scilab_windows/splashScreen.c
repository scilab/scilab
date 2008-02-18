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
#include <Windows.h>
#include "splashScreen.h"
#include "resource.h"
#include "version.h"
#include "WndThread.h"
/*--------------------------------------------------------------------------*/ 
static DWORD WINAPI ThreadSplashScreen( LPVOID lpParam ) ;
/*--------------------------------------------------------------------------*/ 
void splashScreen(void)
{
	DWORD dwThreadId, dwThrdParam = 0; 
	HANDLE hThreadSplashScreen;

	hThreadSplashScreen = CreateThread( 
		NULL,                        // attribut de securité par defaut
		0,                           // taille de la pile par defaut
		ThreadSplashScreen,          // notre function
		&dwThrdParam,                // l'argument pour la fonction
		0,                           // flag de creation par defaut
		&dwThreadId);                // retourne l'id du thread

}
/*--------------------------------------------------------------------------*/ 
static DWORD WINAPI ThreadSplashScreen( LPVOID lpParam ) 
{ 
	int i = 0;
	char titleMainWindow[MAX_PATH];
	HWND hWndMainScilab = NULL;
	HINSTANCE hInstanceThisDll = (HINSTANCE)GetModuleHandle("scilab_windows");
	HWND hdlg = CreateDialog(hInstanceThisDll, MAKEINTRESOURCE(IDD_SPLASHSCREEN), NULL,NULL);

	wsprintf(titleMainWindow,"%s (%d)",SCI_VERSION_STRING,getCurrentScilabId());

	ShowWindow(hdlg, SW_SHOWNORMAL);
	UpdateWindow(hdlg);

	SetWindowPos(hdlg,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
	
	while ( (i< 1500) || (hWndMainScilab == NULL) ) 
	{
		hWndMainScilab = FindWindow(NULL,titleMainWindow);
		if (hWndMainScilab) break;
		else Sleep(1);
		i++;
	}
	DestroyWindow(hdlg);
	return 0; 
} 
/*--------------------------------------------------------------------------*/ 
