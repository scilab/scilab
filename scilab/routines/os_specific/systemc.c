/*-----------------------------------------------------------------------------------*/
/* Interface with system C function */
/* Copyright (Allan CORNET) INRIA 2005 */
/*-----------------------------------------------------------------------------------*/
#include<stdio.h>
#ifdef WIN32
#include <windows.h>
#include "../wsci/GetOS.h"
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif
#include "../machine.h"
/*-----------------------------------------------------------------------------------*/
#if WIN32
extern BOOL IsAFile(char *chainefichier);
BOOL CallWindowsShell(char *command,BOOL WaitInput);
#endif
/*-----------------------------------------------------------------------------------*/
#ifdef __STDC__
#include <stdlib.h>
#else
int system();
#endif
/*-----------------------------------------------------------------------------------*/
int C2F(systemc)(char *command, integer *stat)
{
#if WIN32
	{
		int OS=GetOSVersion();
		if ( (OS==OS_WIN32_WINDOWS_NT_3_51) ||
			(OS==OS_WIN32_WINDOWS_NT_4_0) ||
			(OS==OS_WIN32_WINDOWS_95) ||
			(OS==OS_WIN32_WINDOWS_98) ||
			(OS==OS_WIN32_WINDOWS_Me) )
		{
				int status;
				status=system(command);
				*stat=(integer)status;
		}
		else
		{	
			BOOL Status=FALSE;
			Status=CallWindowsShell(command,FALSE);
			if (Status)
			{
				*stat=(integer)0;
			}
			else
			{
				*stat=(integer)1;
			}
		}
	}
#else
	{
		int status;
		status=system(command);
		*stat=(integer)status;
	}
#endif

	return(0);
}
/*-----------------------------------------------------------------------------------*/
#if WIN32
BOOL CallWindowsShell(char *command,BOOL WaitInput)
{
	BOOL bReturn=FALSE;
	char shellCmd[_MAX_PATH];
	char *CmdLine=NULL;

	PROCESS_INFORMATION piProcInfo; 
	STARTUPINFO siStartInfo;
	SECURITY_ATTRIBUTES saAttr; 

	DWORD ExitCode;

	char *TMPDir=NULL;
	char FileTMPDir[MAX_PATH];

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

	GetEnvironmentVariable("ComSpec", shellCmd, _MAX_PATH);
	TMPDir=getenv("TMPDIR");
	sprintf(FileTMPDir,"%s\\DOS.OK",TMPDir);

	CmdLine=(char*)MALLOC( (strlen(shellCmd)+strlen(command)+strlen(FileTMPDir)+strlen("%s /a /c %s && echo DOS>%s")+1)*sizeof(char) );
	sprintf(CmdLine,"%s /a /c %s && echo DOS>%s",shellCmd,command,FileTMPDir);
	if (IsAFile(FileTMPDir)) DeleteFile(FileTMPDir);

	if (CreateProcess(NULL, CmdLine, NULL, NULL, TRUE,0, NULL, NULL, &siStartInfo, &piProcInfo))
	{
		WaitForSingleObject(piProcInfo.hProcess,INFINITE);

		if ( GetExitCodeProcess(piProcInfo.hProcess,&ExitCode) == STILL_ACTIVE )
		{
			TerminateProcess(piProcInfo.hProcess,0);
		}

		CloseHandle(piProcInfo.hProcess);

		if (CmdLine) {FREE(CmdLine);CmdLine=NULL;}

		if (IsAFile(FileTMPDir))
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
/*-----------------------------------------------------------------------------------*/
