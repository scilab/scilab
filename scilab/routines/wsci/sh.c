

/* Copyright (C) 1998 Chancelier Jean-Philippe */
#include <stdio.h>
#include <windows.h>
#include <string.h>

HANDLE hChildStdinRd, hChildStdinWr, hChildStdinWrDup, hChildStdoutRd,
  hChildStdoutWr, hInputFile, hSaveStdin, hSaveStdout;

BOOL CreateChildProcess (VOID);
VOID WriteToPipe (VOID);
VOID UsePipe (VOID);
VOID ReadFromPipe (VOID);
VOID ErrMsg (LPTSTR, BOOL);

int 
InitSh ()
{
  SECURITY_ATTRIBUTES saAttr;
  BOOL fSuccess;

  /* Set the bInheritHandle flag so pipe handles are inherited. */

  saAttr.nLength = sizeof (SECURITY_ATTRIBUTES);
  saAttr.bInheritHandle = TRUE;
  saAttr.lpSecurityDescriptor = NULL;

  /* 
   * The steps for redirecting child's STDOUT: 
   *     1.  Save current STDOUT, to be restored later. 
   *     2.  Create anonymous pipe to be STDOUT for child. 
   *     3.  Set STDOUT of parent to be write handle of pipe, so 
   *         it is inherited by child. 
   */

  /* Save the handle to the current STDOUT. */

  hSaveStdout = GetStdHandle (STD_OUTPUT_HANDLE);

  /* Create a pipe for the child's STDOUT. */

  if (!CreatePipe (&hChildStdoutRd, &hChildStdoutWr, &saAttr, 0))
    {
      sciprint ("Stdout pipe creation failed\n");
      return (1);
    }

  /* Set a write handle to the pipe to be STDOUT. */

  if (!SetStdHandle (STD_OUTPUT_HANDLE, hChildStdoutWr))
    {
      sciprint ("Redirecting STDOUT failed\r\n");
      return (1);
    }

  /* 
   * The steps for redirecting child's STDIN: 
   *     1.  Save current STDIN, to be restored later. 
   *     2.  Create anonymous pipe to be STDIN for child. 
   *     3.  Set STDIN of parent to be read handle of pipe, so 
   *         it is inherited by child. 
   *     4.  Create a noninheritable duplicate of write handle, 
   *         and close the inheritable write handle. 
   */

  /* Save the handle to the current STDIN. */

  hSaveStdin = GetStdHandle (STD_INPUT_HANDLE);

  /* Create a pipe for the child's STDIN. */

  if (!CreatePipe (&hChildStdinRd, &hChildStdinWr, &saAttr, 0))
    {
      sciprint ("Stdin pipe creation failed\r\n");;
      return (1);
    }

  /* Set a read handle to the pipe to be STDIN. */

  if (!SetStdHandle (STD_INPUT_HANDLE, hChildStdinRd))
    {
      sciprint ("Redirecting Stdin failed\r\n");
      return (1);
    }
  /* Duplicate the write handle to the pipe so it is not inherited. */

  fSuccess = DuplicateHandle (GetCurrentProcess (), hChildStdinWr,
			      GetCurrentProcess (), &hChildStdinWrDup, 0,
			      FALSE,	/* not inherited */
			      DUPLICATE_SAME_ACCESS);
  if (!fSuccess)
    {
      sciprint ("DuplicateHandle failed\r\n");
      return (1);
    }

  CloseHandle (hChildStdinWr);

  /* Now create the child process. */

  if (!CreateChildProcess ())
    {
      sciprint ("Create process failed\r\n");
    }

  /* After process creation, restore the saved STDIN and STDOUT. */

  if (!SetStdHandle (STD_INPUT_HANDLE, hSaveStdin))
    {
      sciprint ("Re-redirecting Stdin failed\r\n");
      return (1);
    }

  if (!SetStdHandle (STD_OUTPUT_HANDLE, hSaveStdout))
    {
      sciprint ("Re-redirecting Stdout failed\r\n");
      return (1);
    }
  return (0);
}

BOOL 
CreateChildProcess ()
{
  PROCESS_INFORMATION piProcInfo;
  STARTUPINFO siStartInfo;

  /* Set up members of STARTUPINFO structure. */

  siStartInfo.cb = sizeof (STARTUPINFO);
  siStartInfo.lpReserved = NULL;
  siStartInfo.lpReserved2 = NULL;
  siStartInfo.cbReserved2 = 0;
  siStartInfo.lpDesktop = NULL;
  siStartInfo.dwFlags = 0;

  /* Create the child process. */

  return CreateProcess (NULL,
			"child",	/* command line                       */
			NULL,	/* process security attributes        */
			NULL,	/* primary thread security attributes */
			TRUE,	/* handles are inherited              */
			0,	/* creation flags                     */
			NULL,	/* use parent's environment           */
			NULL,	/* use parent's current directory     */
			&siStartInfo,	/* STARTUPINFO pointer                */
			&piProcInfo);	/* receives PROCESS_INFORMATION       */
}

void 
SendToSh (char *str)
{
  DWORD dwWritten;
  WriteFile (hChildStdinWrDup, str, strlen (str), &dwWritten, NULL);
}

#define BUFSIZE 256

void 
ReadAnswer (void)
{
  int count = 0;
  DWORD dwRead;
  CHAR chBuf[BUFSIZE], c[1];
  for (;;)
    {
      ReadFile (hChildStdoutRd, c, 1, &dwRead, NULL);
      if (dwRead == 0)
	return;
      chBuf[count] = c[0];
      count++;
      if (strncmp (chBuf, "bash", 4) == 0)
	return;
      if (c[0] == '\n' || count == BUFSIZE - 1)
	{
	  chBuf[count] = '\0';
	  sciprint (chBuf);
	  count = 0;
	}
    }
}
