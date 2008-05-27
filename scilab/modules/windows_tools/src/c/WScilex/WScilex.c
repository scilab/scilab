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
#include <windows.h>
#include <strsafe.h>
#include <string.h>
#include <stdio.h>
#include "GetWindowsVersion.h"
#include "scilabDefaults.h"
#include "win_mem_alloc.h" /* MALLOC */
/*--------------------------------------------------------------------------*/
#define MSG_DETECT_2K_OR_MORE "Scilab requires Windows 2000 or more."
#define MSG_WARNING "Warning"
#define MSG_LOAD_LIBRARIES "Wscilex.exe failed with error %d: %s"
#define MAIN_FUNCTION "Windows_Main"
#define SCILAB_LIBRARY "scilab_windows"
#define ARG_NW "-nw"
#define ARG_NWNI "-nwni"
#define ARG_NOGUI "-nogui"
#define LENGTH_BUFFER_SECURITY 64
/*--------------------------------------------------------------------------*/
typedef int (*MYPROC2) (HINSTANCE, HINSTANCE ,LPSTR szCmdLine, int iCmdShow);
static void setLC_MESSAGES(void);
/*--------------------------------------------------------------------------*/
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR szCmdLine, int iCmdShow)
{
	HINSTANCE hinstLib = NULL; 
	BOOL fFreeResult = FALSE, fRunTimeLinkSuccess = FALSE; 

	if (GetWindowsVersion() < OS_WIN32_WINDOWS_2000)
	{
		MessageBox(NULL,TEXT(MSG_DETECT_2K_OR_MORE),TEXT(MSG_WARNING),MB_ICONWARNING);
		return -1;
	}

	setLC_MESSAGES();

	hinstLib = LoadLibrary(TEXT(SCILAB_LIBRARY)); 	
	if (hinstLib != NULL) 
	{ 
		MYPROC2 Windows_Main = NULL; 

		/* launch main */
		Windows_Main = (MYPROC2) GetProcAddress(hinstLib,MAIN_FUNCTION); 
		if (NULL != Windows_Main) 
		{
			fRunTimeLinkSuccess = TRUE;
			(Windows_Main)(hInstance,hPrevInstance,szCmdLine, iCmdShow);
		}
		fFreeResult = FreeLibrary(hinstLib); 
	} 

	if (! fRunTimeLinkSuccess) 
	{
		LPVOID lpMsgBuf;
		LPVOID lpDisplayBuf;

		DWORD dw = GetLastError(); 

		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dw,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR) &lpMsgBuf,
			0, NULL );

		lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,(lstrlen((LPCTSTR)lpMsgBuf)+LENGTH_BUFFER_SECURITY)*sizeof(TCHAR)); 
		StringCchPrintf((LPTSTR)lpDisplayBuf,LocalSize(lpDisplayBuf) / sizeof(TCHAR),TEXT(MSG_LOAD_LIBRARIES), dw, lpMsgBuf); 

		MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT(MSG_WARNING), MB_ICONERROR); 

		LocalFree(lpMsgBuf);
		LocalFree(lpDisplayBuf);
		exit(1);
	}
	else exit(0);

    return 0;
}
/*--------------------------------------------------------------------------*/
/* patch to initialize LC_MESSAGES */
/* bug on Windows multi language */
/* thanks to J-B & Simoné to detect this probleme*/
/*--------------------------------------------------------------------------*/
static void setLC_MESSAGES(void)
{
	#define LENGTH_BUFFER 1024
	char buffer_LOCALE_SISO639LANGNAME[LENGTH_BUFFER];
	char buffer_LOCALE_SISO3166CTRYNAME[LENGTH_BUFFER];
	char *localeStr = NULL;
	int ret = 0;
	ret = GetLocaleInfoA(LOCALE_USER_DEFAULT,
						LOCALE_SISO639LANGNAME,
						&buffer_LOCALE_SISO639LANGNAME[0],
						LENGTH_BUFFER);
	if (ret > 0)
	{
		ret = GetLocaleInfoA(LOCALE_USER_DEFAULT,
							LOCALE_SISO3166CTRYNAME,
							&buffer_LOCALE_SISO3166CTRYNAME[0],
							LENGTH_BUFFER);
		if (ret >0)
		{
			int length_localeStr = (int)(strlen(buffer_LOCALE_SISO639LANGNAME)+
										 strlen(buffer_LOCALE_SISO3166CTRYNAME)+
										 strlen("_"));
			localeStr = (char*)MALLOC(sizeof(char)*(length_localeStr)+1);
			if (localeStr)
			{
				#define FORMAT_LOCALE "%s_%s"
				StringCchPrintfA(localeStr,length_localeStr,"%s_%s",buffer_LOCALE_SISO639LANGNAME,buffer_LOCALE_SISO3166CTRYNAME);
				SetEnvironmentVariableA(EXPORTENVLOCALESTR,localeStr);
				FREE(localeStr);
			}
		}
	}
}
/*--------------------------------------------------------------------------*/
