/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include <Windows.h>
#include "splashScreen.h"
#include "resource.h"
/*-----------------------------------------------------------------------------------*/ 
static DWORD WINAPI ThreadSplashScreen( LPVOID lpParam ) ;
/*-----------------------------------------------------------------------------------*/ 
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
/*-----------------------------------------------------------------------------------*/ 
static DWORD WINAPI ThreadSplashScreen( LPVOID lpParam ) 
{ 
	int i = 0;
	DWORD CurrentProcessId = GetCurrentProcessId();
	HINSTANCE hInstanceThisDll = (HINSTANCE)GetModuleHandle("scilab_windows");
	HWND hdlg = CreateDialog(hInstanceThisDll, MAKEINTRESOURCE(IDD_SPLASHSCREEN), NULL,NULL);


	ShowWindow(hdlg, SW_SHOWNORMAL);
	UpdateWindow(hdlg);
		
	while (i< 1500)
	{
		if  (GetForegroundWindow() != hdlg)
		{
			SetForegroundWindow(hdlg);
			SetActiveWindow(hdlg);
		}
		else
		{
			Sleep(1);
		}
		i++;
		
	}
	DestroyWindow(hdlg);
	return 0; 
} 
/*-----------------------------------------------------------------------------------*/ 
