/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "intdos.h"
#ifdef WIN32
#include "../wsci/GetOS.h"
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif
/*-----------------------------------------------------------------------------------*/
#ifdef WIN32
  char ** CallWindowsShell(char *command,int *Status,int *NumberOfLines);
  extern BOOL IsAFile(char *chainefichier);
#endif
/*-----------------------------------------------------------------------------------*/
#define ErrMsgDos "Only for Windows 2k and more."
/*-----------------------------------------------------------------------------------*/
int C2F(intdos) _PARAMS((char *fname))
{
	int i=0;
	static int l1,n1,m1;
	char **Result=NULL;
	int *Status=NULL;
	int NumbersOfLines=1;

	CheckRhs(1,1);
	CheckLhs(1,2);

	Status=(int*)MALLOC(sizeof(int));

	if (GetType(1)==sci_strings)
	{
		int OS=0;
		char *CommandLine=NULL;
		GetRhsVar(1,"c",&m1,&n1,&l1);
		CommandLine=cstk(l1);
		#ifdef WIN32
			OS=GetOSVersion();
			if ( (OS==OS_WIN32_WINDOWS_NT_3_51) ||
				 (OS==OS_WIN32_WINDOWS_NT_4_0) ||
				 (OS==OS_WIN32_WINDOWS_95) ||
				 (OS==OS_WIN32_WINDOWS_98) ||
				 (OS==OS_WIN32_WINDOWS_Me) )
			{
				Result=(char**)MALLOC(sizeof(char**));
				Result[0]=MALLOC((strlen(ErrMsgDos)+1)*sizeof(char));
				sprintf(Result[0],"%s",ErrMsgDos);
				NumbersOfLines=1;
				m1=NumbersOfLines;
				n1=1;
				*Status=FALSE;
			}
			else Result=CallWindowsShell(CommandLine,Status,&NumbersOfLines);
		#endif
	}
	else
	{
		Scierror(999,"Invalid parameter must a string.\r\n");
	}
	
	if (Lhs == 1)
	{
		#ifndef WIN32
			*Status=FALSE;
		#else
		{
			int OS=GetOSVersion();
			if ( (OS==OS_WIN32_WINDOWS_NT_3_51) ||
				(OS==OS_WIN32_WINDOWS_NT_4_0) ||
				(OS==OS_WIN32_WINDOWS_95) ||
				(OS==OS_WIN32_WINDOWS_98) ||
				(OS==OS_WIN32_WINDOWS_Me) )
			{
				sciprint("%s\n",ErrMsgDos);
			}
		}
		#endif
        n1=1;

		CreateVarFromPtr(Rhs+1, "b", &n1, &n1, &Status);
		LhsVar(1)=Rhs+1;
	}
	else /* Lhs == 2 */
	{
		#ifndef WIN32
			Result=(char**)MALLOC(sizeof(char**));
			Result[0]=MALLOC((strlen(ErrMsgDos)+1)*sizeof(char));
			sprintf(Result[0],"%s",ErrMsgDos);
			m1=1;
			n1=1;
			*Status=FALSE;

			CreateVarFromPtr(Rhs+1, "b", &n1, &n1, &Status);
			LhsVar(1)=Rhs+1;

			CreateVarFromPtr(Rhs+2, "S",&m1, &n1, Result);
			LhsVar(2)=Rhs+2;
		
		#else
		{
			int OS=GetOSVersion();
			if ( !((OS==OS_WIN32_WINDOWS_NT_3_51) ||
				(OS==OS_WIN32_WINDOWS_NT_4_0) ||
				(OS==OS_WIN32_WINDOWS_95) ||
				(OS==OS_WIN32_WINDOWS_98) ||
				(OS==OS_WIN32_WINDOWS_Me)) )
			{
				m1=NumbersOfLines;
				n1=1;
			}

			CreateVarFromPtr(Rhs+1, "b", &n1, &n1, &Status);
			LhsVar(1)=Rhs+1;

			if (m1)
			{
				CreateVarFromPtr(Rhs+2, "S",&m1, &n1, Result);
			}
			else
			{
				m1=0;
				n1=0;
				l1=0;
				CreateVar(Rhs+2,"d",  &m1, &n1, &l1);
			}
			LhsVar(2)=Rhs+2;

		}	
		#endif
	}

	C2F(putlhsvar)();

	if (Status) {FREE(Status);Status=NULL;}
	if (Result)
	{
		for(i=0;i<m1;i++)
		{
			if (Result[i]) {FREE(Result[i]);Result[i]=NULL;}
		}
		FREE(Result);
		Result=NULL;
	}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
#ifdef WIN32
char ** CallWindowsShell(char *command,int *Status,int *NumberOfLines)
{
	#define BUFSIZE 4096
	char shellCmd[_MAX_PATH];
	char *CmdLine=NULL;
	HANDLE hChildStdinRd= INVALID_HANDLE_VALUE;
	HANDLE hChildStdinWr= INVALID_HANDLE_VALUE;
	HANDLE hChildStdinWrDup= INVALID_HANDLE_VALUE;
    HANDLE hChildStdoutRd= INVALID_HANDLE_VALUE;
	HANDLE hChildStdoutWr= INVALID_HANDLE_VALUE;
	HANDLE hChildStdoutRdDup= INVALID_HANDLE_VALUE;
    HANDLE hInputFile= INVALID_HANDLE_VALUE;
	HANDLE hSaveStdin= INVALID_HANDLE_VALUE;
	HANDLE hSaveStdout= INVALID_HANDLE_VALUE; 

	PROCESS_INFORMATION piProcInfo; 
	STARTUPINFO siStartInfo;
	BOOL bFuncRetn = FALSE; 
	SECURITY_ATTRIBUTES saAttr; 
	BOOL fSuccess; 

	DWORD dwRead; 
	BOOL moreOutput = TRUE;
	int readSoFar = 0;
	char *Output=NULL;
	char *OEMstring=NULL;
	char *op=NULL;
	
	char *line=NULL;
	char *CopyOutput=NULL;

	char **OuputStrings=NULL;

	DWORD ExitCode;
	int i=0;

	char *TMPDir=NULL;
	char FileTMPDir[MAX_PATH];

	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
	saAttr.bInheritHandle = TRUE; 
	saAttr.lpSecurityDescriptor = NULL; 
	hSaveStdout = GetStdHandle(STD_OUTPUT_HANDLE); 

	CreatePipe(&hChildStdoutRd, &hChildStdoutWr, &saAttr, 0);
	SetStdHandle(STD_OUTPUT_HANDLE, hChildStdoutWr);

	fSuccess = DuplicateHandle(GetCurrentProcess(), hChildStdoutRd,
		GetCurrentProcess(), &hChildStdoutRdDup , 0,
		FALSE,
		DUPLICATE_SAME_ACCESS);


	CloseHandle(hChildStdoutRd);
	
	ZeroMemory( &piProcInfo, sizeof(PROCESS_INFORMATION) );
	
	ZeroMemory( &siStartInfo, sizeof(STARTUPINFO) );
	siStartInfo.cb = sizeof(STARTUPINFO); 
	siStartInfo.dwFlags      = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	siStartInfo.wShowWindow  = SW_HIDE;
	siStartInfo.hStdInput= NULL;
	//siStartInfo.hStdInput=INVALID_HANDLE_VALUE;
	//siStartInfo.hStdInput=GetStdHandle(STD_INPUT_HANDLE);
	siStartInfo.hStdOutput = hChildStdoutWr;
	siStartInfo.hStdError = hChildStdoutWr;

	GetEnvironmentVariable("ComSpec", shellCmd, _MAX_PATH);
	TMPDir=getenv("TMPDIR");
	sprintf(FileTMPDir,"%s\\DOS.OK",TMPDir);

	CmdLine=(char*)MALLOC( (strlen(shellCmd)+strlen(command)+strlen(FileTMPDir)+strlen("%s /a /c %s && echo DOS>%s")+1)*sizeof(char) );
	sprintf(CmdLine,"%s /a /c %s && echo DOS>%s",shellCmd,command,FileTMPDir);
	if (IsAFile(FileTMPDir)) DeleteFile(FileTMPDir);

	if (!CreateProcess(NULL, CmdLine, NULL, NULL, TRUE,0, NULL, NULL, &siStartInfo, &piProcInfo))
	{
		*NumberOfLines=0;
		if (CmdLine) {FREE(CmdLine);CmdLine=NULL;}
		return OuputStrings;
	}
	if (CmdLine) {FREE(CmdLine);CmdLine=NULL;}

	SetStdHandle(STD_OUTPUT_HANDLE, hSaveStdout);
	CloseHandle(hChildStdoutWr); 

	Output = (char*) MALLOC(BUFSIZE);
	op = Output;
	while (moreOutput) 
	{
		BOOL bres = ReadFile( hChildStdoutRdDup, op, BUFSIZE-1, &dwRead, NULL);
		moreOutput = bres || (dwRead != 0);
		if (moreOutput) 
		{
			readSoFar += dwRead;
			Output = (char*) REALLOC(Output, readSoFar+BUFSIZE);
			op = Output + readSoFar;
		}
	} 
	*op = '\0';

	OEMstring=Output;
	Output=NULL;
	Output=(char*)MALLOC((strlen(OEMstring)+1)*sizeof(char));
	OemToChar(OEMstring,Output);

	CopyOutput=(char*)MALLOC((strlen(OEMstring)+1)*sizeof(char));
	strcpy(CopyOutput,Output);

	i=0;
	for (line=strtok(CopyOutput,"\n");line;line=strtok(NULL,"\n"))
	{
		i++;
	}
	*NumberOfLines=i;

	if (*NumberOfLines)
	{
		OuputStrings=(char**)MALLOC((*NumberOfLines)*sizeof(char**));

		i=0;
		line=NULL;
		for (line=strtok(Output,"\n");line;line=strtok(NULL,"\n"))
		{
			if (line[strlen(line)-1] == '\r') line[strlen(line)-1] = 0;
			OuputStrings[i]=MALLOC((strlen(line)+1)*sizeof(char));
			sprintf(OuputStrings[i],"%s",line);
			i++;
		}
	}

	if (GetExitCodeProcess(piProcInfo.hProcess,&ExitCode))
	{
		if (ExitCode==0)
		{
			*Status=TRUE;
		}
		else
		{
			*Status=FALSE;
		}
		CloseHandle(piProcInfo.hProcess);
	}
	else
	{
		TerminateProcess(piProcInfo.hProcess,0);
		CloseHandle(piProcInfo.hProcess);

		if (IsAFile(FileTMPDir))
		{
			DeleteFile(FileTMPDir);
			*Status=TRUE;
		}
		else
		{
			*Status=FALSE;
		}
	}

	return OuputStrings;
}
#endif
/*-----------------------------------------------------------------------------------*/
