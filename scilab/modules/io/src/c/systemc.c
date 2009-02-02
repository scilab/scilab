/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
/* Interface with system C function */
/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "MALLOC.h" /* MALLOC */
#include "PATH_MAX.h"
#include "systemc.h"
#include "tmpdir.h"
#include "charEncoding.h"
#include "stack-def.h"
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
#include "FileExist.h"
BOOL CallWindowsShell(char *command,BOOL WaitInput);
#endif
/*--------------------------------------------------------------------------*/
int C2F(systemc)(char *command, int *stat)
{
#ifdef _MSC_VER
	{
		BOOL Status=FALSE;
		char szLocale[bsiz];
		Status=CallWindowsShell(UTFToLocale(command, szLocale),FALSE);
		if (Status)
		{
			*stat=(int) 0;
		}
		else
		{
			*stat=(int) 1;
		}
	}
#else
	{
		int status;
		char szLocale[bsiz];
		status=system(UTFToLocale(command, szLocale));
		*stat=(int) status;
	}
#endif

	return(0);
}
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
BOOL CallWindowsShell(char *command,BOOL WaitInput)
{
	BOOL bReturn=FALSE;
	char shellCmd[PATH_MAX];
	char *CmdLine=NULL;

	PROCESS_INFORMATION piProcInfo; 
	STARTUPINFO siStartInfo;
	SECURITY_ATTRIBUTES saAttr; 

	DWORD ExitCode;

	char *TMPDir=NULL;
	char FileTMPDir[PATH_MAX];

	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
	saAttr.bInheritHandle = TRUE; 
	saAttr.lpSecurityDescriptor = NULL; 

	ZeroMemory( &piProcInfo, sizeof(PROCESS_INFORMATION) );

	ZeroMemory( &siStartInfo, sizeof(STARTUPINFO) );
	siStartInfo.cb = sizeof(STARTUPINFO); 
	siStartInfo.dwFlags      = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	siStartInfo.wShowWindow  = SW_HIDE;
	
	if (WaitInput)
	{
		siStartInfo.hStdInput=GetStdHandle(STD_INPUT_HANDLE);
	}
	else
	{
		siStartInfo.hStdInput=NULL;
	}

	siStartInfo.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	siStartInfo.hStdError = GetStdHandle(STD_ERROR_HANDLE);

	GetEnvironmentVariable("ComSpec", shellCmd, PATH_MAX);
	TMPDir=getTMPDIR();
	sprintf(FileTMPDir,"%s\\DOS.OK",TMPDir);
	if (TMPDir) 
	{
		FREE(TMPDir);
		TMPDir=NULL;
	}

	CmdLine=(char*)MALLOC( (strlen(shellCmd)+strlen(command)+strlen(FileTMPDir)+strlen("%s /a /c \"%s\" && echo DOS>%s")+1)*sizeof(char) );
	sprintf(CmdLine,"%s /a /c \"%s\" && echo DOS>%s",shellCmd,command,FileTMPDir);
	if (FileExist(FileTMPDir)) DeleteFile(FileTMPDir);

	if (CreateProcess(NULL, CmdLine, NULL, NULL, TRUE,0, NULL, NULL, &siStartInfo, &piProcInfo))
	{
		WaitForSingleObject(piProcInfo.hProcess,INFINITE);

		if ( GetExitCodeProcess(piProcInfo.hProcess,&ExitCode) == STILL_ACTIVE )
		{
			TerminateProcess(piProcInfo.hProcess,0);
		}

		CloseHandle(piProcInfo.hProcess);

		if (CmdLine) {FREE(CmdLine);CmdLine=NULL;}

		if (FileExist(FileTMPDir))
		{
			DeleteFile(FileTMPDir);
			bReturn=TRUE;
		}
		else
		{
			bReturn=FALSE;
		}
	}
	else
	{
		CloseHandle(piProcInfo.hProcess);
		if (CmdLine) {FREE(CmdLine);CmdLine=NULL;}
		bReturn=FALSE;
	}
	return bReturn;
}
#endif
/*--------------------------------------------------------------------------*/
