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
#include "win_mem_alloc.h" /* MALLOC */
/*--------------------------------------------------------------------------*/
#define MSG_DETECT_2K_OR_MORE "Scilab requires Windows 2000 or more."
#define MSG_WARNING "Warning"
#define MSG_LOAD_LIBRARIES "scilex.exe failed with error %d: %s"
#define MAIN_FUNCTION "Console_Main"
#define SCILAB_LIBRARY  "scilab_windows"
#define ARG_NW "-nw"
#define ARG_NWNI "-nwni"
#define ARG_NOGUI "-nogui"
#define LENGTH_BUFFER_SECURITY 64
/*--------------------------------------------------------------------------*/
typedef int (*MYPROC1) (int , char **);
/*--------------------------------------------------------------------------*/
int main (int argc, char **argv)
{
	#define MAXCMDTOKENS 128
	UINT LastErrorMode = 0;
	HINSTANCE hinstLib = NULL; 


	BOOL fFreeResult = FALSE, fRunTimeLinkSuccess = FALSE; 
	
	int argcbis = -1;
	LPSTR argvbis[MAXCMDTOKENS];
	int i = 0;
	int FindNW = 0;

	if (GetWindowsVersion()<OS_WIN32_WINDOWS_2000)
	{
		MessageBox(NULL,TEXT(MSG_DETECT_2K_OR_MORE),TEXT(MSG_WARNING),MB_ICONWARNING);
		return -1;
	}

	for (i=0;i<argc;i++)
	{
		if ( _stricmp(argv[i],ARG_NW) == 0 ) FindNW=1;
		if ( _stricmp(argv[i],ARG_NWNI) == 0 ) FindNW=1;
		if ( _stricmp(argv[i],ARG_NOGUI) == 0 ) FindNW=1;
	}

	if ( FindNW == 0 )
	{
		char *nwparam=NULL;
		nwparam=(char*)MALLOC((strlen(ARG_NW)+1)*sizeof(char));
		strcpy_s(nwparam,(strlen(ARG_NW)+1),ARG_NW);
		for (i=0;i<argc;i++)
		{
			argvbis[i]=argv[i];
		}
		argvbis[argc]=nwparam;
		argcbis=argc+1;
	}
	else
	{
		for (i=0;i<argc;i++)
		{
			argvbis[i]=argv[i];
		}
		argcbis=argc;
	}

	/* Disable system errors msgbox */
	LastErrorMode = SetErrorMode( SEM_FAILCRITICALERRORS );

	hinstLib = LoadLibrary(TEXT(SCILAB_LIBRARY )); 	

	/* re enable system errors msgbox */
	SetErrorMode(LastErrorMode);

	if (hinstLib != NULL) 
	{ 
		MYPROC1 Console_Main = NULL; 

		/* launch main */
		Console_Main = (MYPROC1) GetProcAddress(hinstLib,MAIN_FUNCTION); 

		if (NULL != Console_Main) 
		{
			fRunTimeLinkSuccess = TRUE;

			#ifndef _DEBUG
			/* catch system errors msgbox (release mode only) */
			/* http://msdn.microsoft.com/en-us/library/ms680621(VS.85).aspx */
			LastErrorMode = SetErrorMode( SEM_FAILCRITICALERRORS|SEM_NOALIGNMENTFAULTEXCEPT|SEM_NOGPFAULTERRORBOX );
			_try
			{
			#endif

			(Console_Main)(argcbis,argvbis);

			#ifndef _DEBUG
			}
			_except (EXCEPTION_EXECUTE_HANDLER)
			{	
			}
			#endif
		}
		fFreeResult = FreeLibrary(hinstLib); 
	} 

	if (! fRunTimeLinkSuccess) 
	{
		#define BUFFER_SIZE 512
		char buffer[BUFFER_SIZE];
		char *OutputMsg = NULL;
		DWORD dw = GetLastError(); 

		if (FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, 
			              dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			              buffer, BUFFER_SIZE, NULL) == 0) 
		{
			StringCchPrintf(buffer,strlen("Unknown Error")+1,"Unknown Error"); 
		}
				
		fprintf(stderr,"scilex can't launch scilab.\nError code : %lu\n",dw);
		OutputMsg = (char*)MALLOC((strlen(buffer)+1)*sizeof(char));
		if (OutputMsg)
		{
			CharToOem(buffer,OutputMsg);
			fprintf(stderr,"%s\n",OutputMsg);
			FREE(OutputMsg);
		}
		exit(1);
	}
	else exit(0);

    return 0;

}
/*--------------------------------------------------------------------------*/
