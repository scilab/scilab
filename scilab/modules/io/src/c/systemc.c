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
#ifndef _MSC_VER
#include <sys/wait.h>
#endif
#include "MALLOC.h" /* MALLOC */
#include "PATH_MAX.h"
#include "systemc.h"
#include "tmpdir.h"
#include "stack-def.h"
#include "charEncoding.h"
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
		Status=CallWindowsShell(command,FALSE);
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
		status = system(command);
		/* provide exit value of the child */
		*stat = WEXITSTATUS(status);
	}
#endif

	return(0);
}
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
BOOL CallWindowsShell(char *command,BOOL WaitInput)
{
	BOOL bReturn=FALSE;
	wchar_t shellCmd[PATH_MAX];
	wchar_t *CmdLine=NULL;

	PROCESS_INFORMATION piProcInfo; 
	STARTUPINFOW siStartInfo;
	SECURITY_ATTRIBUTES saAttr; 

	DWORD ExitCode;

	wchar_t *TMPDir=NULL;
	wchar_t FileTMPDir[PATH_MAX];

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

	GetEnvironmentVariableW(L"ComSpec", shellCmd, PATH_MAX);
	{
		char *pTemp = getTMPDIR();
		TMPDir = to_wide_string(pTemp);
		swprintf(FileTMPDir, PATH_MAX, L"%s\\DOS.OK",TMPDir);
		FREE(pTemp);
	}
	if (TMPDir) 
	{
		FREE(TMPDir);
		TMPDir=NULL;
	}

	{
		wchar_t * wcommand = to_wide_string(command);
		size_t iCmdSize = (wcslen(shellCmd)+wcslen(wcommand)+wcslen(FileTMPDir)+wcslen(L"%s /a /c \"%s\" && echo DOS>%s")+1);
		CmdLine=(wchar_t*)MALLOC(iCmdSize * sizeof(wchar_t));
		swprintf(CmdLine, iCmdSize, L"%s /a /c \"%s\" && echo DOS>%s", shellCmd, wcommand, FileTMPDir);
	}

	if (CreateProcessW(NULL, CmdLine, NULL, NULL, TRUE,0, NULL, NULL, &siStartInfo, &piProcInfo))
	{
		WaitForSingleObject(piProcInfo.hProcess,INFINITE);

		if ( GetExitCodeProcess(piProcInfo.hProcess,&ExitCode) == STILL_ACTIVE )
		{
			TerminateProcess(piProcInfo.hProcess,0);
		}

		CloseHandle(piProcInfo.hProcess);

		if (CmdLine) {FREE(CmdLine);CmdLine=NULL;}

		{
			FILE *f = _wfopen(FileTMPDir, L"r");
			if(f != NULL)
			{
				fclose(f);
				DeleteFileW(FileTMPDir);
				bReturn=TRUE;
			}
			else
			{
				bReturn=FALSE;
			}
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
