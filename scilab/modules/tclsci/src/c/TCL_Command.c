/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <stdlib.h>

#ifdef _MSC_VER
#include <Windows.h>
#include "strdup_windows.h"
#define usleep(micro) Sleep(micro/1000)
#else
#include <unistd.h>
#endif

#include "MALLOC.h"
#include "TCL_Command.h"
#include "GlobalTclInterp.h"

// TCL Interpreter creation and loop ID
__threadId		TclThread;
// Globla Tcl Slave Name
char *			TclSlave;
// Global Tcl Command Buffer
char *			TclCommand;
// Global Tcl Script to Evaluate
char *			TclFile;
// Global Tcl Return Code.
int			TclInterpReturn;
// Global Tcl Return Result.
char *			TclInterpResult;

// Single execution
__threadLock		singleExecutionLock;

// Wake Up
__threadSignal		wakeUp;
__threadLock		wakeUpLock;

// Work is done
__threadSignal		workIsDone;

// Launch command
__threadLock		launchCommand;

// ***********************
// ** Enable LocalDebug **
// **********************
//#define __LOCAL_DEBUG__


extern BOOL TK_Started;

/*
** This function is a timer to periodicly wake
** up the loop in startTclLoop.
** Called within a thread.
*/
static void *sleepAndSignal(void* in) {
  while(TK_Started) {
#ifdef __LOCAL_DEBUG__
    printf(".");
    fflush(NULL);
#endif
    usleep(TIME_TO_SLEEP);
    __Lock(&wakeUpLock);
    __Signal(&wakeUp);
    __UnLock(&wakeUpLock);
  }
  return NULL;
}

/*
** This function start an endless Tcl Loop
** in order the Scilab Global Tcl Interpreter
** do some "update" and let the Tcl Applications live.
*/
void startTclLoop()
{
  __threadId sleepThreadId;

  Tcl_Interp *LocalTCLinterp;
  __InitLock(&singleExecutionLock);

  __InitSignal(&wakeUp);
  __InitLock(&wakeUpLock);
  __InitSignal(&workIsDone);
  __InitLock(&launchCommand);

  __CreateThread(&sleepThreadId, sleepAndSignal);

  /*
  ** TCL Event Loop : Threaded
  */
  while(TK_Started) {
#ifdef __LOCAL_DEBUG__
    printf(".");
    fflush(NULL);
#endif
    /*
    ** -= IF =-
    ** there is a command to run
    ** RUN IT.
    **
    ** -= or =-
    ** there is a script to load
    ** LOAD IT.
    */
    if (TclCommand != NULL || TclFile != NULL)
      {
#ifdef __LOCAL_DEBUG__
	printf("[LOCK] launch Command\n");
	fflush(NULL);
#endif
	__Lock(&launchCommand);

	/* Reinit local interpreter,
	   default is the biggest one. */
	LocalTCLinterp = requestTclInterp();

	/* Check if it's supposed to be run in root or slave */
	if(TclSlave != NULL)
	  {
	    LocalTCLinterp = Tcl_GetSlave(LocalTCLinterp, TclSlave);
	    FREE(TclSlave);
	    TclSlave = NULL;
	  }
	/*
	** Do the evaluation
	*/
	if (TclCommand != NULL)
	  {
#ifdef __LOCAL_DEBUG__
	    printf("[TCL Daemon] Eval : %s\n", TclCommand);
	    fflush(NULL);
#endif
	    TclInterpReturn = Tcl_Eval(LocalTCLinterp, TclCommand);
	    FREE(TclCommand);
	    TclCommand = NULL;
	  }
	/*
	** Do the evaluation
	*/
	else if (TclFile != NULL)
	  {
#ifdef __LOCAL_DEBUG__
	    printf("[TCL Daemon] Load : %s\n", TclFile);
	    fflush(NULL);
#endif
	    TclInterpReturn = Tcl_EvalFile(LocalTCLinterp, TclFile);
	    FREE(TclFile);
	    TclFile = NULL;
	  }
	/* Update return value and result */
	if (LocalTCLinterp->result && strlen(LocalTCLinterp->result) != 0)
	  {
	    TclInterpResult = strdup(LocalTCLinterp->result);
	  }
	else
	  {
	    TclInterpResult = NULL;
	  }
#ifdef __LOCAL_DEBUG__
	printf("[SIGNAL] executionDone\n");
	fflush(NULL);
#endif
	__Signal(&workIsDone);
	__UnLock(&launchCommand);
      }
    /*
    ** -= ELSE =-
    ** there is no command
    ** do an update and wait to be waked up.
    */
    else
      {
	__Lock(&wakeUpLock);
	Tcl_Eval(requestTclInterp(), "update");
#ifdef __LOCAL_DEBUG__
	printf("[TCL Daemon] Wait\n");
	fflush(NULL);
#endif
	__Wait(&wakeUp, &wakeUpLock);
	__UnLock(&wakeUpLock);
      }
  }
  /* TK is stopped... Kill interpreter. */
  deleteTclInterp();

}

/*
** Send Tcl_EvalFile
** Ask the interpreter to execute some command
** by send a SIGNAL to the other thread.
*/
int sendTclFileToSlave(char* file, char* slave)
{
#ifdef __LOCAL_DEBUG__
  printf("[TCL %s SendFile] File : %s\n", slave, file);
  fflush(NULL);
#endif
  __Lock(&singleExecutionLock);
  {
    __Lock(&launchCommand);
    TclFile = strdup(file);
    if (slave != NULL)
      {
	TclSlave = strdup(slave);
      }
    else
      {
	TclSlave = NULL;
      }
    __Lock(&wakeUpLock);
    __Signal(&wakeUp);
    __UnLock(&wakeUpLock);
#ifdef __LOCAL_DEBUG__
    printf("[TCL Main] Wait EXECUTION DONE\n");
    fflush(NULL);
#endif
    __Wait(&workIsDone, &launchCommand);
#ifdef __LOCAL_DEBUG__
    printf("[TCL SendFile] DONE\n");
    fflush(NULL);
#endif
    __UnLock(&launchCommand);
  }
  __UnLock(&singleExecutionLock);
  return getTclCommandReturn();
}

/*
** Send Tcl Command To Slave
** Ask the interpreter to execute some command
** by send a SIGNAL to the other thread.
*/
int sendTclCommandToSlave(char* command, char* slave)
{
#ifdef __LOCAL_DEBUG__
  printf("[TCL %s Send] Eval : %s\n", slave, command);
  fflush(NULL);
#endif
  __Lock(&singleExecutionLock);
  {
    __Lock(&launchCommand);
    TclCommand = strdup(command);
    if (slave != NULL)
      {
	TclSlave = strdup(slave);
      }
    else
      {
	TclSlave = NULL;
      }
    __Lock(&wakeUpLock);
    __Signal(&wakeUp);
    __UnLock(&wakeUpLock);
#ifdef __LOCAL_DEBUG__
    printf("[TCL Main]Wait EXECUTION DONE\n");
    fflush(NULL);
#endif
    __Wait(&workIsDone, &launchCommand);
#ifdef __LOCAL_DEBUG__
    printf("[TCL Send] DONE\n");
    fflush(NULL);
#endif
    __UnLock(&launchCommand);
  }
  __UnLock(&singleExecutionLock);
  return getTclCommandReturn();
}


/*
** Get Tcl Command Return Value
** Porpagate return value of Tcl Interpreter
** caught in thread.
*/
int getTclCommandReturn(void)
{
#ifdef __LOCAL_DEBUG__
  printf("[TCL getTclCommandReturn]\n");
  fflush(NULL);
#endif
  int commandResult = TclInterpReturn;
  TclInterpReturn = 0;
  return commandResult;
}

/*
** Get Tcl Command Result
** Porpagate return message of Tcl Interpreter
** caught in thread.
** This can be an error or a string value.
*/
char *getTclCommandResult(void)
{
#ifdef __LOCAL_DEBUG__
  printf("[TCL getTclCommandResult]\n");
  fflush(NULL);
#endif
  if (TclInterpResult != NULL) {
    char *result = strdup(TclInterpResult);
    TclInterpResult = NULL;
    return result;
  }
  return strdup("\0");
}
