/***********************************************************************/
/* Copyright (C) 2005 INRIA Allan CORNET */
/***********************************************************************/
#include <Windows.h>
/***********************************************************************/
typedef int (*MYPROC) (HINSTANCE, HINSTANCE ,LPSTR szCmdLine, int iCmdShow);
/***********************************************************************/
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR szCmdLine, int iCmdShow)
{
	HINSTANCE hinstLib; 
	MYPROC Windows_Main; 
	BOOL fFreeResult, fRunTimeLinkSuccess = FALSE; 
	
	hinstLib = LoadLibrary(TEXT("Libscilab")); 	
	if (hinstLib != NULL) 
	{ 
		Windows_Main = (MYPROC) GetProcAddress(hinstLib, TEXT("Windows_Main")); 

		if (NULL != Windows_Main) 
		{
			fRunTimeLinkSuccess = TRUE;
			(Windows_Main)(hInstance,hPrevInstance,szCmdLine, iCmdShow);
		}
		fFreeResult = FreeLibrary(hinstLib); 
	} 

	if (! fRunTimeLinkSuccess) 
	{
		MessageBox(NULL,"Wscilex.exe : Libscilab.dll not found !","Warning",MB_ICONERROR); 
		exit(1);
	}
	else exit(0);

    return 0;
}
/***********************************************************************/
