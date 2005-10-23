/*-----------------------------------------------------------------------------------*/
/* Interface with system C function */
/* Copyright (Allan CORNET) INRIA 2005 */
/*-----------------------------------------------------------------------------------*/
#ifdef WIN32
  #include <Windows.h>
  #include "../wsci/GetOs.h"
#endif

#include<stdio.h>
#include "../machine.h"

#ifdef WIN32
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif

/*-----------------------------------------------------------------------------------*/
#ifdef __STDC__
	#include <stdlib.h>
#else
	int  system();
#endif

#ifdef WIN32
int Windows_system(char *command);
extern BOOL IsAFile(char *chainefichier);
#endif
/*-----------------------------------------------------------------------------------*/
int C2F(systemc)(char *command, integer *stat)
{
	/*#ifndef WIN32
		int status;
		status=system(command);
		*stat=(integer)status;
	#else
		*stat=(integer)Windows_system(command);
	#endif*/
	int status;
		status=system(command);
		*stat=(integer)status;

	return(0);
}
/*-----------------------------------------------------------------------------------*/
#ifdef WIN32
int Windows_system(char *command)
{
	int CurrentOS=0;
	int bReturn=FALSE;
	
	CurrentOS=GetOSVersion();

	/* Old mode for old Windows ... */
	if ( (CurrentOS==OS_WIN32_WINDOWS_NT_3_51) ||
		 (CurrentOS==OS_WIN32_WINDOWS_NT_4_0) ||
		 (CurrentOS==OS_WIN32_WINDOWS_95) ||
		 (CurrentOS==OS_WIN32_WINDOWS_98) ||
		 (CurrentOS==OS_WIN32_WINDOWS_Me) )
	{
		bReturn=system(command);
	}
	else
	{
		BOOL bOK=FALSE;
		SHELLEXECUTEINFO ShExecInfo;
		char DefaultShell[MAX_PATH];
		char *CommandLine=NULL;
		char HostFilename[MAX_PATH];
		char *TMPDir=NULL;
		DWORD _Result=0;

		GetEnvironmentVariable("COMSPEC",DefaultShell,MAX_PATH-1);
		TMPDir=getenv("TMPDIR");

		wsprintf(HostFilename,"%s\\host.txt",TMPDir);
		if (IsAFile(HostFilename))
		{
			DeleteFile(HostFilename);
		}
		CommandLine=(char*) MALLOC (sizeof(char)*(strlen(command)+strlen(HostFilename)+strlen("/C \"%s &&echo hostOK>%s\"")+1));
		wsprintf(CommandLine,"/C \"%s &&echo hostOK>%s\"",command,HostFilename);

		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		ShExecInfo.hwnd = NULL;
		ShExecInfo.lpVerb = "open";
		ShExecInfo.lpFile = DefaultShell;
		ShExecInfo.lpParameters = CommandLine;
		ShExecInfo.lpDirectory = NULL;
		ShExecInfo.nShow = SW_HIDE;
		ShExecInfo.hInstApp = NULL;
		bOK=ShellExecuteEx(&ShExecInfo);

		_Result = WaitForSingleObject(ShExecInfo.hProcess,INFINITE);

		if (IsAFile(HostFilename))
		{
			CloseHandle(ShExecInfo.hProcess);
			DeleteFile(HostFilename);
			bReturn=FALSE;
		}
		else
		{
			TerminateProcess(ShExecInfo.hProcess,0);
			CloseHandle(ShExecInfo.hProcess);
			bReturn=TRUE;
		}

		if (CommandLine)
		{
			FREE(CommandLine);
			CommandLine=NULL;
		}
	}
	return bReturn;
}
#endif
/*-----------------------------------------------------------------------------------*/
