/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <Windows.h>
#include <string.h>
#include <stdio.h>
#include "../../../../../libs/DetectFramework2/DetectFramework.h"
#include "../../../../../libs/GetWindowsVersion/GetWindowsVersion.h"
/*-----------------------------------------------------------------------------------*/
#define MSG_DETECT_2K_OR_MORE "Scilab requires Windows 2000 or more."
#define MSG_DETECT_FRAMEWORK "The .NET Framework 2.0 is not installed"
#define MSG_WARNING "Warning"
#define MSG_LOAD_LIBRARIES "Wscilex.exe : Impossible to load Scilab libraries."
#define MAIN_FUNCTION "Windows_Main"
#define LIBRARY_TO_LOAD "scilab_windows"
#define ARG_NW "-nw"
#define ARG_NWNI "-nwni"
#define ARG_NOGUI "-nogui"
/*-----------------------------------------------------------------------------------*/
typedef int (*MYPROC) (HINSTANCE, HINSTANCE ,LPSTR szCmdLine, int iCmdShow);
/*-----------------------------------------------------------------------------------*/
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR szCmdLine, int iCmdShow)
{
	HINSTANCE hinstLib = NULL; 
	MYPROC Windows_Main = NULL; 
	BOOL fFreeResult = FALSE, fRunTimeLinkSuccess = FALSE; 

	if (!DetectFrameWorkNET2())
	{
		MessageBox(NULL,TEXT(MSG_DETECT_FRAMEWORK),TEXT(MSG_WARNING),MB_ICONWARNING);
		return -1;
	}

	if (GetWindowsVersion() < OS_WIN32_WINDOWS_2000)
	{
		MessageBox(NULL,TEXT(MSG_DETECT_2K_OR_MORE),TEXT(MSG_WARNING),MB_ICONWARNING);
		return -1;
	}

	hinstLib = LoadLibrary(TEXT(LIBRARY_TO_LOAD)); 	
	if (hinstLib != NULL) 
	{ 
		Windows_Main = (MYPROC) GetProcAddress(hinstLib,MAIN_FUNCTION); 

		if (NULL != Windows_Main) 
		{
			fRunTimeLinkSuccess = TRUE;
			(Windows_Main)(hInstance,hPrevInstance,szCmdLine, iCmdShow);
		}
		fFreeResult = FreeLibrary(hinstLib); 
	} 

	if (! fRunTimeLinkSuccess) 
	{
		MessageBox(NULL,TEXT(MSG_LOAD_LIBRARIES),TEXT(MSG_WARNING),MB_ICONERROR); 
		exit(1);
	}
	else exit(0);

    return 0;
}
/*-----------------------------------------------------------------------------------*/
