/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <Windows.h>
#include "../../../../../libs/DetectFramework2/DetectFramework.h"
#include "../../../../../libs/GetWindowsVersion/GetWindowsVersion.h"
/*-----------------------------------------------------------------------------------*/
typedef int (*MYPROC) (HINSTANCE, HINSTANCE ,LPSTR szCmdLine, int iCmdShow);
/*-----------------------------------------------------------------------------------*/
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR szCmdLine, int iCmdShow)
{
	HINSTANCE hinstLib; 
	MYPROC Windows_Main; 
	BOOL fFreeResult, fRunTimeLinkSuccess = FALSE; 

	if (!DetectFrameWorkNET2())
	{
		MessageBox(NULL,"The .NET Framework 2.0 is not installed","Warning",MB_ICONWARNING);
		return -1;
	}

	if (GetWindowsVersion()<OS_WIN32_WINDOWS_2000)
	{
		MessageBox(NULL,"Scilab requires Windows 2000 or more.","Warning",MB_ICONWARNING);
		return -1;
	}

	hinstLib = LoadLibrary(TEXT("scilab_windows")); 	
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
		MessageBox(NULL,"Wscilex.exe : scilab_windows.dll not found !","Warning",MB_ICONERROR); 
		exit(1);
	}
	else exit(0);

    return 0;
}
/*-----------------------------------------------------------------------------------*/
