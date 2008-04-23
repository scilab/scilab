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
#include "machine.h"
#include "SetConsolePrompt.h"
#include "scilabmode.h"
#include "sci_mem_alloc.h"
#include "prompt.h"
#include "HistoryManager.h"
#include "dynamic_menus.h" /* for ismenu() */



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

static char * __CommandLine;

/*--------------------------------------------------------------------------*/

IMPORT_SIGNAL __threadSignal	LaunchScilab;
IMPORT_SIGNAL __threadLock	LaunchScilabLock;

static __threadSignal	TimeToWork;

static __threadLock ReadyForLaunch;

static BOOL WSCThreadAlive = FALSE;
static __threadId WSCThread;

static BOOL WGCLThreadAlive = FALSE;
static __threadId WGCLThread;

static BOOL initialized = FALSE;

char *TermReadAndProcess(void);

/***********************************************************************
 * line editor
 **********************************************************************/
static void getCommandLine(void)
{
  GetCurrentPrompt(Sci_Prompt);

  if (getScilabMode() == SCILAB_STD)
    {
      /* Send new prompt to Java Console, do not display it */
      SetConsolePrompt(Sci_Prompt);
      setSearchedTokenInScilabHistory(NULL);
      /* Call Java Console to get a string */
      __CommandLine = ConsoleRead();
    }
  else
    {
      /* Call Term Management for NW and NWNI to get a string */
      __CommandLine = TermReadAndProcess();
    }
}

/***********************************************************************/
/*
** This function is threaded and watch for a signal.
** sent when StoreCommand is performed.
*/
static void initAll(void) {
  initialized = TRUE;
  __InitSignal(&TimeToWork);
  __InitLock(&ReadyForLaunch);
}


/***********************************************************************/
/*
** This function is threaded and watch for a signal.
** sent when StoreCommand is performed.
*/
static void *watchStoreCommand(void *in) {
  __UnLock(&LaunchScilabLock);
  __Wait(&LaunchScilab, &LaunchScilabLock);


  __Lock(&ReadyForLaunch);
  WSCThreadAlive=FALSE;
  __Signal(&TimeToWork);
  __UnLock(&ReadyForLaunch);

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
  __Lock(&ReadyForLaunch);
  WGCLThreadAlive = FALSE;
  __Signal(&TimeToWork);
  __UnLock(&ReadyForLaunch);

   return NULL;

}

/***********************************************************************/
/*
** Old zzledt... Called by Fortran...
** @TODO rename that function !!!
*/
void C2F(zzledt)(char *buffer,int *buf_size,int *len_line,int * eof,
		 int *menusflag,int * modex,long int dummy1)
{
  if(!initialized)
    {
      initAll();
    }

  __Lock(&ReadyForLaunch);
  __CommandLine = strdup("");

  if (ismenu() == 0) {
    if (!WGCLThreadAlive)
      {
	if (WGCLThread) {
	  __WaitThreadDie(WGCLThread);
	}
	__CreateThread(&WGCLThread, &watchGetCommandLine);
	WGCLThreadAlive = TRUE;
      }
    if (!WSCThreadAlive)
      {
	if (WSCThread) {
	  __WaitThreadDie(WSCThread);
	}
	__CreateThread(&WSCThread, &watchStoreCommand);
	WSCThreadAlive = TRUE;
      }

    __Wait(&TimeToWork, &ReadyForLaunch);

  }
  __UnLock(&ReadyForLaunch);

  /*
  ** WARNING : Old crappy f.... code
  ** do not change reference to buffer
  ** or fortran will be lost !!!!
  */
  strcpy(buffer, __CommandLine);
  *len_line = (int)strlen(buffer);
  *eof = FALSE;
}

