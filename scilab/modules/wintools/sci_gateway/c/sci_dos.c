/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "gw_wintools.h"
#include "MALLOC.h" /* MALLOC */
#include "sciprint.h"
#include "tmpdir.h"
/*-----------------------------------------------------------------------------------*/
#define BUFSIZE 4096
/*-----------------------------------------------------------------------------------*/
#ifdef _MSC_VER
/* globals */
typedef struct pipeinfo
{
	HANDLE pipe;
	char *OutputBuffer;
	int NumberOfLines;
} pipeinfo;
#endif
/*-----------------------------------------------------------------------------------*/
#ifdef _MSC_VER
static DWORD WINAPI ReadFromPipe (LPVOID args);
static int ClosePipeInfo (pipeinfo pipe);
static int spawncommand(char *command,BOOL DetachProcess);
static int GetNumberOfLines(char *lines);
static char **CreateOuput(pipeinfo *pipe,BOOL DetachProcess);
static int PrintOuput(char **ouput,int nbrlines);
static BOOL DetectDetachProcessInCommandLine(char *command);
extern BOOL IsWindowInterface(void);
extern BOOL IsAFile(char *chainefichier);
#endif

/*-----------------------------------------------------------------------------------*/
#ifdef _MSC_VER
static pipeinfo pipeOut= {INVALID_HANDLE_VALUE, NULL,0};
static pipeinfo pipeErr= {INVALID_HANDLE_VALUE, NULL,0};
#endif
/*-----------------------------------------------------------------------------------*/
#ifdef _MSC_VER
int C2F(sci_dos) _PARAMS((char *fname,unsigned long l))
{
	int m1,n1,l1;
	int *Status=NULL;
	char *Param1String=NULL;
	char *Param2String=NULL;
	BOOL ECHOMODE=FALSE;
	BOOL DetachProcessOption=FALSE;

	char **Output=NULL;
	int numberoflines=0;

	CheckRhs(1,2);
	CheckLhs(1,2);

	if (GetType(1)!=sci_strings) 
	{
		Scierror(999,"first parameter must be a string.\r\n");
		return 0;
	}

	GetRhsVar(1,"c",&m1,&n1,&l1);
	Param1String=cstk(l1);
	DetachProcessOption=DetectDetachProcessInCommandLine(Param1String);

	if (Rhs == 2)
	{
		if (GetType(2)!=sci_strings) 
		{
			Scierror(999,"second parameter must be a string.\r\n");
			return 0;
		}
		GetRhsVar(2,"c",&m1,&n1,&l1);
		Param2String=cstk(l1);

		if (strcmp("-echo",Param2String))
		{
			Scierror(999,"Unrecognized option: %s.\r\n",Param2String);
			return 0;
		}
		else
		{
			ECHOMODE=TRUE;
		}
	}

	spawncommand(Param1String,DetachProcessOption);

	Status=(int*)MALLOC(sizeof(int));

	if (DetachProcessOption)
	{
		if ( strlen(pipeErr.OutputBuffer) )
		{
			/* StdErr will be "Output" */
			*Status=FALSE;
			Output=CreateOuput(&pipeErr,DetachProcessOption);
			numberoflines=pipeErr.NumberOfLines;
		}
		else
		{
			/* StdOut will be "Output" */
			*Status=TRUE;
			Output=CreateOuput(&pipeOut,DetachProcessOption);
			numberoflines=pipeOut.NumberOfLines;
		}
	}
	else
	{
		char *TMPDir=NULL;
		char FileTMPDir[PATH_MAX];

		TMPDir=getTMPDIR();
		sprintf(FileTMPDir,"%s\\DOS.OK",TMPDir);
		if (TMPDir)
		{
			FREE(TMPDir);
			TMPDir=NULL;
		}

		if (IsAFile(FileTMPDir))
		{
			DeleteFile(FileTMPDir);
			/* StdOut will be "Output" */
			*Status=TRUE;
			Output=CreateOuput(&pipeOut,DetachProcessOption);
			numberoflines=pipeOut.NumberOfLines;
		}
		else
		{
			/* StdErr will be "Output" */
			*Status=FALSE;
			Output=CreateOuput(&pipeErr,DetachProcessOption);
			numberoflines=pipeErr.NumberOfLines;
		}
	}

	if (ECHOMODE) PrintOuput(Output,numberoflines);

	if (Lhs == 1)
	{
		m1=1;n1=1;
		CreateVarFromPtr(Rhs+1, "b", &n1, &n1, &Status);
		LhsVar(1)=Rhs+1;
	}
	else /* Lhs == 2 */
	{
		if (Output[0])
		{
			m1=numberoflines;
			n1=1;
			CreateVarFromPtr(Rhs+1, "S",&m1, &n1, Output);
			
		}
		else
		{
			m1=0;
			n1=0;
			l1=0;
			CreateVar(Rhs+1,"d",  &m1, &n1, &l1);
		}
		
		LhsVar(1)=Rhs+1;

		m1=1;n1=1;
		CreateVarFromPtr(Rhs+2, "b", &n1, &n1, &Status);
		LhsVar(2)=Rhs+2;
	}

	C2F(putlhsvar)();

	if (Status) {FREE(Status);Status=NULL;}
	if (Output)
	{
		int i=0;
		for(i=0;i<numberoflines;i++) if (Output[i]) {FREE(Output[i]);Output[i]=NULL;}
		FREE(Output);
		Output=NULL;
	}

	ClosePipeInfo (pipeOut);
	ClosePipeInfo (pipeErr);

	return 0;
}
#else
/*-----------------------------------------------------------------------------------*/
int C2F(sci_dos) _PARAMS((char *fname,unsigned long l))
{
	int m1,n1,l1;
	int *Status=NULL;
	char *Param1String=NULL;
	char *Param2String=NULL;

	char **Output=NULL;
	int numberoflines=0;

	CheckRhs(1,2);
	CheckLhs(1,2);

	if (GetType(1)!=sci_strings) 
	{
		Scierror(999,"first parameter must be a string.\r\n");
		return 0;
	}

	GetRhsVar(1,"c",&m1,&n1,&l1);
	Param1String=cstk(l1);

	if (Rhs == 2)
	{
		if (GetType(2)!=sci_strings) 
		{
			Scierror(999,"second parameter must be a string.\r\n");
			return 0;
		}
		GetRhsVar(2,"c",&m1,&n1,&l1);
		Param2String=cstk(l1);

		if (strcmp("-echo",Param2String))
		{
			Scierror(999,"Unrecognized option: %s.\r\n",Param2String);
			return 0;
		}
	}

	Status=(int*)MALLOC(sizeof(int));
	*Status=FALSE;
	numberoflines=1;

	if (Lhs == 1)
	{
		m1=1;n1=1;
		CreateVarFromPtr(Rhs+1, "b", &n1, &n1, &Status);
		LhsVar(1)=Rhs+1;
	}
	else /* Lhs == 2 */
	{
		char *StringTmp=NULL;
		m1=numberoflines;
		n1=1;
		Output=(char**)MALLOC((numberoflines)*sizeof(char**));
		StringTmp=(char*)MALLOC((512)*sizeof(char*));
		strcpy(StringTmp,"only for Windows.");
		Output[0]=StringTmp;
		CreateVarFromPtr(Rhs+1, "S",&m1, &n1, Output);
		LhsVar(1)=Rhs+1;

		m1=1;n1=1;
		CreateVarFromPtr(Rhs+2, "b", &n1, &n1, &Status);
		LhsVar(2)=Rhs+2;
	}

	C2F(putlhsvar)();

	if (Status) {FREE(Status);Status=NULL;}
	if (Output)
	{
		int i=0;
		for(i=0;i<numberoflines;i++) if (Output[i]) {FREE(Output[i]);Output[i]=NULL;}
		FREE(Output);
		Output=NULL;
	}
	return 0;
}
#endif
/*-----------------------------------------------------------------------------------*/
#ifdef _MSC_VER
static int spawncommand(char *command,BOOL DetachProcess)
{
	char shellCmd[_PATH_MAX];
	char *CmdLine=NULL;

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	SECURITY_ATTRIBUTES sa;
	DWORD threadID;
	DWORD dwCreationFlags;
	BOOL ok;
	HANDLE hProcess, h, pipeThreads[2];
	DWORD ExitCode;

	hProcess = GetCurrentProcess();

	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags   = STARTF_USESTDHANDLES;
	si.hStdInput = INVALID_HANDLE_VALUE;

	ZeroMemory(&sa, sizeof(SECURITY_ATTRIBUTES));
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	/* create a non-inheritible pipe. */
	CreatePipe(&pipeOut.pipe, &h, &sa, 0);

	/* dupe the write side, make it inheritible, and close the original. */
	DuplicateHandle(hProcess, h, hProcess, &si.hStdOutput, 
		0, TRUE, DUPLICATE_SAME_ACCESS | DUPLICATE_CLOSE_SOURCE);

	/* Same as above, but for the error side. */
	CreatePipe(&pipeErr.pipe, &h, &sa, 0);
	DuplicateHandle(hProcess, h, hProcess, &si.hStdError, 
		0, TRUE, DUPLICATE_SAME_ACCESS | DUPLICATE_CLOSE_SOURCE);

	/* base command line */
	GetEnvironmentVariable("ComSpec", shellCmd, _PATH_MAX);

	if (DetachProcess)
	{
		CmdLine=(char*)MALLOC( (strlen(shellCmd)+strlen(command)+strlen("%s /A /C %s")+1)*sizeof(char) );
		sprintf(CmdLine,"%s /A /C %s",shellCmd,command);

		dwCreationFlags=DETACHED_PROCESS;
	}
	else
	{
		char *TMPDir=NULL;
		char FileTMPDir[PATH_MAX];

		TMPDir=getTMPDIR();
		sprintf(FileTMPDir,"%s\\DOS.OK",TMPDir);
		if (TMPDir)
		{
			FREE(TMPDir);
			TMPDir=NULL;
		}
		if (IsAFile(FileTMPDir)) DeleteFile(FileTMPDir);

		CmdLine=(char*)MALLOC( (strlen(shellCmd)+strlen(command)+strlen("%s /A /C %s && echo DOS>%s")+strlen(FileTMPDir)+1)*sizeof(char) );
		sprintf(CmdLine,"%s /A /C %s && echo DOS>%s",shellCmd,command,FileTMPDir);

		dwCreationFlags=0;
	}

	ok = CreateProcess(
		NULL,	    /* Module name. */
		CmdLine,	    /* Command line. */
		NULL,	    /* Process handle not inheritable. */
		NULL,	    /* Thread handle not inheritable. */
		TRUE,	    /* yes, inherit handles. */
		dwCreationFlags, /* No console for you. */
		NULL,	    /* Use parent's environment block. */
		NULL,	    /* Use parent's starting directory. */
		&si,	    /* Pointer to STARTUPINFO structure. */
		&pi);	    /* Pointer to PROCESS_INFORMATION structure. */

	if (!ok) return 2;

	if (CmdLine) {FREE(CmdLine);CmdLine=NULL;}

	/* close our references to the write handles that have now been inherited. */
	CloseHandle(si.hStdOutput);
	CloseHandle(si.hStdError);

	WaitForInputIdle(pi.hProcess, 5000);
	CloseHandle(pi.hThread);

	/* start the pipe reader threads. */
	pipeThreads[0] = CreateThread(NULL, 0, ReadFromPipe, &pipeOut, 0, &threadID);
	pipeThreads[1] = CreateThread(NULL, 0, ReadFromPipe, &pipeErr, 0, &threadID);

	/* block waiting for the process to end. */
	WaitForSingleObject(pi.hProcess, INFINITE);

	if ( GetExitCodeProcess(pi.hProcess,&ExitCode) == STILL_ACTIVE )
	{
		TerminateProcess(pi.hProcess,0);
	}

	CloseHandle(pi.hProcess);

	/* wait for our pipe to get done reading */
	WaitForMultipleObjects(2, pipeThreads, TRUE, 500);
	CloseHandle(pipeThreads[0]);
	CloseHandle(pipeThreads[1]);

	return 0;
}
#endif
/*-----------------------------------------------------------------------------------*/
#ifdef _MSC_VER
static int ClosePipeInfo (pipeinfo pipe)
{
	CloseHandle(pipe.pipe);
	if (pipe.OutputBuffer)
	{
		FREE(pipe.OutputBuffer);
		pipe.OutputBuffer=NULL;
		pipe.NumberOfLines=0;
	}
	return 0;
}
#endif
/*-----------------------------------------------------------------------------------*/
#ifdef _MSC_VER
static DWORD WINAPI ReadFromPipe (LPVOID args)
{
	pipeinfo *pi = (pipeinfo *) args;
	int readSoFar = 0;
	DWORD dwRead;
	BOOL moreOutput = TRUE;
	char *op=NULL;

	pi->OutputBuffer = (char*) MALLOC(BUFSIZE);
	op = pi->OutputBuffer;

	while (moreOutput) 
	{
		BOOL bres = ReadFile( pi->pipe, op, BUFSIZE-1, &dwRead, NULL);

		moreOutput = bres || (dwRead != 0);

		if (moreOutput) 
		{
			readSoFar += dwRead;
			pi->OutputBuffer  = (char*) REALLOC(pi->OutputBuffer , readSoFar+BUFSIZE);
			op = pi->OutputBuffer + readSoFar;
		}
	} 
	*op = '\0';
	return 0;
}
#endif
/*-----------------------------------------------------------------------------------*/
#ifdef _MSC_VER
static int GetNumberOfLines(char *lines)
{
	int NumberOfLines=0;
	if (lines)
	{
		int i=0;
		while(lines[i]!='\0')
		{
			if (lines[i]=='\n') NumberOfLines++;
			i++;
		}
		if (NumberOfLines==0) NumberOfLines=1;
	}
	return NumberOfLines;
}
#endif
/*-----------------------------------------------------------------------------------*/
#ifdef _MSC_VER
static char **CreateOuput(pipeinfo *pipe,BOOL DetachProcess)
{
	char **OuputStrings=NULL;

	pipe->NumberOfLines=GetNumberOfLines(pipe->OutputBuffer);

	if(pipe->NumberOfLines)
	{
		char *line=NULL;
		int i=0;

		OuputStrings=(char**)MALLOC((pipe->NumberOfLines)*sizeof(char**));
		line=strtok(pipe->OutputBuffer,"\n");

		while(line)
		{
			OuputStrings[i]=MALLOC((strlen(line)+1)*sizeof(char));

			if (IsWindowInterface())
			{
				if (DetachProcess)
				{
					sprintf(OuputStrings[i],"%s",line);
				}
				else
				{
					OemToChar(line,OuputStrings[i]);
				}
			}
			else 
			{
				if (DetachProcess)
				{
					CharToOem(line,OuputStrings[i]);
				}
				else
				{
					sprintf(OuputStrings[i],"%s",line);
				}
			}

			if (OuputStrings[i][strlen(OuputStrings[i])-1] == '\r') OuputStrings[i][strlen(OuputStrings[i])-1] = 0;
			line=strtok(NULL,"\n");
			i++;
		}
	}
	return OuputStrings;
}
#endif
/*-----------------------------------------------------------------------------------*/
#ifdef _MSC_VER
static int PrintOuput(char **ouput,int nbrlines)
{
	BOOL bOK=FALSE;
	if (ouput)
	{
		int i=0;
		for(i=0;i<nbrlines;i++) 
		{
			if (ouput[i]) sciprint("%s\n",ouput[i]);
		}
		bOK=TRUE;
	}
	return bOK;
}
#endif
/*-----------------------------------------------------------------------------------*/
#ifdef _MSC_VER
static BOOL DetectDetachProcessInCommandLine(char *command)
{
	BOOL bOK=FALSE;
	if (command)
	{
		int i=(int)strlen(command);
		for (i=(int)strlen(command)-1;i>=0;i--)
		{
			if (command[i]==' ') command[i]='\0';
			else break;
		}
		i=(int)strlen(command);
		if ( (i>0) &&(command[i-1]=='&') ) bOK=TRUE;
	}
	return bOK;
}
#endif
/*-----------------------------------------------------------------------------------*/
