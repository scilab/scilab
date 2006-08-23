/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "Splashscreen.h"
extern HINSTANCE hdllInstance;
/*-----------------------------------------------------------------------------------*/
void CreateSplashscreen(void)
{
	HWND hdlg;
	hdlg = CreateDialog(hdllInstance, "IDD_SPLASH", NULL,NULL);

	ShowWindow(hdlg, SW_SHOWNORMAL);
	UpdateWindow(hdlg);
	Sleep(1500);

	DestroyWindow(hdlg);
}
/*-----------------------------------------------------------------------------------*/