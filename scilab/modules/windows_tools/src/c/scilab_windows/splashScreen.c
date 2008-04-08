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
static BOOL stopSplashScreen(void);
static int timeSplashScreen = 0;
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
	HINSTANCE hInstanceThisDll = (HINSTANCE)GetModuleHandle("scilab_windows");
	HWND hdlg = CreateDialog(hInstanceThisDll, MAKEINTRESOURCE(IDD_SPLASHSCREEN), NULL,NULL);

	ShowWindow(hdlg, SW_SHOWNORMAL);
	UpdateWindow(hdlg);

	SetWindowPos(hdlg,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);

	while ( !stopSplashScreen() )
	{
		Sleep(20);
	}

	DestroyWindow(hdlg);
	return 0; 
} 
/*--------------------------------------------------------------------------*/ 
static BOOL stopSplashScreen(void)
{
	HWND hWndMainScilab = NULL;
	char titleMainWindow[MAX_PATH];

	wsprintf(titleMainWindow,"%s (%d)",SCI_VERSION_STRING,getCurrentScilabId());
	hWndMainScilab = FindWindow(NULL,titleMainWindow);

	if ( hWndMainScilab && (timeSplashScreen > 1000) )
	{
		return TRUE;
	}
	else
	{
		wsprintf(titleMainWindow,"Console");
		hWndMainScilab = FindWindow(NULL,titleMainWindow);

		if ( hWndMainScilab && (timeSplashScreen > 1000) )
		{
			return TRUE;
		}
		else
		{
			timeSplashScreen = timeSplashScreen + 50;
		}
	}
	return FALSE;
}
/*--------------------------------------------------------------------------*/ 
