/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include "PATH_MAX.h"
#include "MALLOC.h"
#include "tmpdir.h" /* getTMPDIR */
#include "FileExist.h"
#include "scilabmode.h"
#include "spawncommand.h"
#include "strdup_windows.h"
#include "charEncoding.h"
#include "getshortpathname.h"
/*--------------------------------------------------------------------------*/
#define BUFSIZE 4096
/*--------------------------------------------------------------------------*/
extern pipeinfo pipeOut;
extern pipeinfo pipeErr;
/*--------------------------------------------------------------------------*/
int spawncommand(char *command,BOOL DetachProcess)
{
	char shellCmd[PATH_MAX];
	char *CmdLine = NULL;

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	SECURITY_ATTRIBUTES sa;
	DWORD threadID;
	DWORD dwCreationFlags;
	BOOL ok = FALSE;
	HANDLE hProcess = NULL, h = NULL, pipeThreads[2];
	DWORD ExitCode = 0;

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
	GetEnvironmentVariable("ComSpec", shellCmd, PATH_MAX);

	if (DetachProcess)
	{
		CmdLine = (char*)MALLOC( (strlen(shellCmd)+strlen(command)+strlen("%s /A /C %s")+1)*sizeof(char) );
		sprintf(CmdLine, "%s /A /C %s",shellCmd,command);

		dwCreationFlags = DETACHED_PROCESS;
	}
	else
	{
		char FileTMPDir[PATH_MAX + 16];
		BOOL bConvert = FALSE;

		char *TMPDirLong = getTMPDIR();
		char *TMPDirShort = getshortpathname(TMPDirLong, &bConvert);
		
		sprintf(FileTMPDir,"%s\\DOS.OK",TMPDirLong);
		FREE(TMPDirLong); TMPDirLong = NULL;
		FREE(TMPDirShort); TMPDirShort = NULL;

		if (FileExist(FileTMPDir)) DeleteFile(FileTMPDir);

		CmdLine = (char*)MALLOC( (strlen(shellCmd)+strlen(command)+strlen("%s /A /C %s && echo DOS>%s")+strlen(FileTMPDir)+1)*sizeof(char) );
		sprintf(CmdLine, "%s /A /C %s && echo DOS>%s",shellCmd,command,FileTMPDir);

		dwCreationFlags = 0;
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

	if (CmdLine) {FREE(CmdLine); CmdLine = NULL;}

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

	return ExitCode;
}
/*--------------------------------------------------------------------------*/
int ClosePipeInfo (pipeinfo pipe)
{
	CloseHandle(pipe.pipe);
	if (pipe.OutputBuffer)
	{
		FREE(pipe.OutputBuffer);
		pipe.OutputBuffer = NULL;
		pipe.NumberOfLines = 0;
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
DWORD WINAPI ReadFromPipe (LPVOID args)
{
	pipeinfo *pi = (pipeinfo *) args;
	int readSoFar = 0;
	DWORD dwRead;
	BOOL moreOutput = TRUE;
	unsigned char *op = NULL;

	pi->OutputBuffer = (unsigned char*) MALLOC(BUFSIZE);
	op = pi->OutputBuffer;

	while (moreOutput) 
	{
		BOOL bres = ReadFile( pi->pipe, op, BUFSIZE-1, &dwRead, NULL);

		moreOutput = bres || (dwRead != 0);

		if (moreOutput) 
		{
			readSoFar += dwRead;
			pi->OutputBuffer  = (unsigned char*) REALLOC(pi->OutputBuffer , readSoFar+BUFSIZE);
			op = pi->OutputBuffer + readSoFar;
		}
	} 
	*op = '\0';
	return 0;
}
/*--------------------------------------------------------------------------*/
static int GetNumberOfLines(char *lines)
{
	int NumberOfLines = 0;
	if (lines)
	{
		int i = 0;
		while(lines[i] != '\0')
		{
			if (lines[i] == '\n') NumberOfLines++;
			i++;
		}
		if (NumberOfLines == 0) NumberOfLines = 1;
	}
	return NumberOfLines;
}
/*--------------------------------------------------------------------------*/
char **CreateOuput(pipeinfo *pipe,BOOL DetachProcess)
{
	char **OuputStrings = NULL;

	if (pipe)
	{
		if (pipe->OutputBuffer)
		{
			int lenbuf = ((int)strlen(pipe->OutputBuffer) + 1) * 2;
			char *buffer = MALLOC(sizeof(char)*lenbuf);
			strcpy(buffer, pipe->OutputBuffer);

			if (getScilabMode() == SCILAB_STD)
			{
				if (DetachProcess)
				{
					strcpy(buffer, pipe->OutputBuffer);
				}
				else
				{
					OemToChar(pipe->OutputBuffer, buffer);
				}
			}
			else 
			{
				if (DetachProcess)
				{
					CharToOem(pipe->OutputBuffer, buffer);
				}
				else
				{
					strcpy(buffer, pipe->OutputBuffer);
				}
			}

			pipe->NumberOfLines = GetNumberOfLines(buffer);

			if(pipe->NumberOfLines)
			{
				char *line = NULL;
				int i = 0;

				OuputStrings = (char**)MALLOC((pipe->NumberOfLines)*sizeof(char*));
				line = strtok(buffer, "\n");

				while(line)
				{
					int j = 0;
					int len = 0;
					OuputStrings[i] = (char*)MALLOC((strlen(line)+1)*sizeof(char));
					if (OuputStrings[i])
					{
						strcpy(OuputStrings[i], line);

						len = (int)strlen(OuputStrings[i]);

						if ( (OuputStrings[i][len - 1] == 10) || (OuputStrings[i][len - 1] == 13) )
						{
							OuputStrings[i][len - 1] = 0;
						}

						len = (int)strlen(OuputStrings[i]);
						for(j = 0; j < len; j++)
						{
							/* remove some no printable characters */
							if (OuputStrings[i][j] == -96)
							{
								OuputStrings[i][j] = ' ';
							}
						}

						line = strtok(NULL, "\n");
						i++;
						if (i > pipe->NumberOfLines) break;
					}
				}
			}

			if (buffer) {FREE(buffer); buffer = NULL;}
		}
	}
	return OuputStrings;
}
/*--------------------------------------------------------------------------*/
BOOL DetectDetachProcessInCommandLine(char *command)
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
/*--------------------------------------------------------------------------*/

