/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <string.h>
#include "Thread_Wrapper.h" /* Thread should be first for Windows */
#include "BOOL.h"
#include "ConsoleRead.h"
#include "SetConsolePrompt.h"
#include "scilabmode.h"
#include "MALLOC.h"
#include "prompt.h"
#include "HistoryManager.h"
#include "dynamic_menus.h" /* for ismenu() */
#include "charEncoding.h"
#include "zzledt.h"
#include "GetCommandLine.h"
#if _MSC_VER
#include "TermReadAndProcess.h"
#endif
#include "stack-def.h"

#ifdef _MSC_VER
#define IMPORT_SIGNAL __declspec(dllimport)
#define strdup _strdup
#else
#define IMPORT_SIGNAL extern
#endif
#define WK_BUF_SIZE 520

#define NUL '\0'

/*--------------------------------------------------------------------------*/
static char Sci_Prompt[10];
static char* tmpPrompt = NULL;

static char * __CommandLine;

/*--------------------------------------------------------------------------*/

IMPORT_SIGNAL __threadSignal		LaunchScilab;
IMPORT_SIGNAL __threadSignalLock	LaunchScilabLock;

static __threadSignal	TimeToWork;

static __threadSignalLock ReadyForLaunch;

static BOOL WatchStoreCmdThreadAlive = FALSE;
static __threadId WatchStoreCmdThread;

static BOOL WatchGetCmdLineThreadAlive = FALSE;
static __threadId WatchGetCmdLineThread;

static BOOL initialized = FALSE;

char *TermReadAndProcess(void);

/***********************************************************************
 * line editor
 **********************************************************************/
static void getCommandLine(void)
{
  tmpPrompt = GetTemporaryPrompt();
  GetCurrentPrompt(Sci_Prompt);

  if (getScilabMode() == SCILAB_STD)
    {
      /* Send new prompt to Java Console, do not display it */
      if (tmpPrompt != NULL)
        {
          SetConsolePrompt(tmpPrompt);
          ClearTemporaryPrompt();
        }
      else
        {
          SetConsolePrompt(Sci_Prompt);
        }
      setSearchedTokenInScilabHistory(NULL);
      /* Call Java Console to get a string */
      __CommandLine = ConsoleRead();
    }
  else
    {
      /* Call Term Management for NW and NWNI to get a string */
		char szTempUTF[bsiz];
      __CommandLine = localeToUTF(TermReadAndProcess(), szTempUTF);
    }
}

/***********************************************************************/
/*
** used by mscanf to get a line from the Scilab console
*/
char *getConsoleInputLine(void)
{
  getCommandLine();
  return strdup(__CommandLine);
}

/***********************************************************************/
/*
** This function is threaded and watch for a signal.
** sent when StoreCommand is performed.
*/
static void initAll(void) {
  initialized = TRUE;
  __InitSignal(&TimeToWork);
  __InitSignalLock(&ReadyForLaunch);
}


/***********************************************************************/
/*
** This function is threaded and watch for a signal.
** sent when StoreCommand is performed.
*/
static void *watchStoreCommand(void *in) {
  __LockSignal(&LaunchScilabLock);
  __Wait(&LaunchScilab, &LaunchScilabLock);
  __UnLockSignal(&LaunchScilabLock);

  __LockSignal(&ReadyForLaunch);
  WatchStoreCmdThreadAlive=FALSE;
  __Signal(&TimeToWork);
  __UnLockSignal(&ReadyForLaunch);

  return NULL;
}

/***********************************************************************/
/*
** This function is threaded and wait until
** some command has been input by user using
** the shell.
*/
static void *watchGetCommandLine(void *in) {
  getCommandLine();

  __LockSignal(&ReadyForLaunch);
  WatchGetCmdLineThreadAlive = FALSE;
  __Signal(&TimeToWork);
  __UnLockSignal(&ReadyForLaunch);

  return NULL;

}

/***********************************************************************/
/*
 * Old zzledt... Called by Fortran...
 * @TODO rename that function !!!
 * @TODO remove unused arg buf_size, menusflag, modex & dummy1
*/
void C2F(zzledt)(char *buffer,int *buf_size,int *len_line,int * eof,
		 int *menusflag,int * modex,long int dummy1)
{
  if(!initialized)
    {
      initAll();
    }

  __LockSignal(&ReadyForLaunch);
  __CommandLine = strdup("");

  if (ismenu() == 0)
    {
      if (!WatchGetCmdLineThreadAlive)
	{
	  if (WatchGetCmdLineThread) {
	    __WaitThreadDie(WatchGetCmdLineThread);
	  }
	  __CreateThread(&WatchGetCmdLineThread, &watchGetCommandLine);
	  WatchGetCmdLineThreadAlive = TRUE;
	}
      if (!WatchStoreCmdThreadAlive)
	{
	  if (WatchStoreCmdThread) {
	    __WaitThreadDie(WatchStoreCmdThread);
	  }
	  __CreateThread(&WatchStoreCmdThread, &watchStoreCommand);
	  WatchStoreCmdThreadAlive = TRUE;
	}

      __Wait(&TimeToWork, &ReadyForLaunch);
    }
  __UnLockSignal(&ReadyForLaunch);

  /*
  ** WARNING : Old crappy f.... code
  ** do not change reference to buffer
  ** or fortran will be lost !!!!
  */
  if (__CommandLine)
  {
	strcpy(buffer, __CommandLine);
  }
  else
  {
	  strcpy(buffer,"");
  }
  *len_line = (int)strlen(buffer);

  *eof = FALSE;
}

