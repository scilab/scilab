/*
**  -*- C -*-
**
** TCL_Command.c
** Made by  Bruno JOFRET <bruno.jofret@inria.fr>
**
** Started on  Wed Jan  9 09:15:30 2008 bruno
** Last update Wed Jan 16 08:37:32 2008 bruno
**
** Copyright INRIA 2008
*/
#include <stdlib.h>
#ifndef _MSC_VER
#include <unistd.h>
#endif

#ifdef _MSC_VER
#include <Windows.h>
#include "strdup_windows.h"
#define usleep(micro) Sleep(micro/1000)

#endif
#include "MALLOC.h"
#include "TCL_Command.h"


// TCL Interpreter creation and loop ID
__threadId		TclThread;
// Globla Tcl Slave Name
char *			TclSlave;
// Global Tcl Command Buffer
char *			TclCommand;
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




extern BOOL TK_Started;

/*
** This function is a timer to periodicly wake
** up the loop in startTclLoop.
** Called within a thread.
*/
static void *sleepAndSignal(void* in) {
  while(TK_Started) {
    //printf(".");
    //fflush(NULL);
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
void startTclLoop(Tcl_Interp *TCLinterp)
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
    //printf(".");
    //fflush(NULL);
    /*
    ** -= IF =-
    ** there is a command to run
    ** RUN IT.
    */
    if (TclCommand != NULL)
      {
	//printf("[LOCK] launch Command\n");
	//fflush(NULL);
	__Lock(&launchCommand);

	/* Reinit local interpreter,
	   default is the biggest one. */
	LocalTCLinterp = TCLinterp;

	/* Check if it's supposed to be run in root or slave */
	if(TclSlave != NULL)
	  {
	    LocalTCLinterp = Tcl_GetSlave(TCLinterp, TclSlave);
	    FREE(TclSlave);
	    TclSlave = NULL;
	  }
	/* Do the evaluation */
	//printf("[TCL Daemon] Eval : %s\n", TclCommand);
	//fflush(NULL);
	TclInterpReturn = Tcl_Eval(LocalTCLinterp, TclCommand);
	/* Update return value and result */
	if (LocalTCLinterp->result && strlen(LocalTCLinterp->result) != 0)
	  {
	    TclInterpResult = strdup(LocalTCLinterp->result);
	  }
	else
	  {
	    TclInterpResult = NULL;
	  }
	FREE(TclCommand);
	TclCommand = NULL;
	//printf("[SIGNAL] executionDone\n");
	//fflush(NULL);
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
	Tcl_Eval(TCLinterp, "update");
	//printf("[TCL Daemon] Wait\n");
	//fflush(NULL);
	__Wait(&wakeUp, &wakeUpLock);
	__UnLock(&wakeUpLock);
      }
  }
  /* TK is stopped... Kill interpreter. */
  Tcl_DeleteInterp(TCLinterp);

}

/*
** Send Tcl Command
** Ask the interpreter to execute some command
** by send a SIGNAL to the other thread.
*/
void sendTclCommand(char* command)
{
  //printf("[TCL Send] Eval : %s\n", command);
  //fflush(NULL);
  __Lock(&singleExecutionLock);
  {
    __Lock(&launchCommand);
    TclCommand = strdup(command);
    __Lock(&wakeUpLock);
    __Signal(&wakeUp);
    __UnLock(&wakeUpLock);
    //printf("[TCL Main]Wait EXECUTION DONE\n");
    //fflush(NULL);
    __Wait(&workIsDone, &launchCommand);
    //printf("[TCL Send] DONE\n");
    //fflush(NULL);
    __UnLock(&launchCommand);
  }
  __UnLock(&singleExecutionLock);
}

/*
** Send Tcl Command To Slave
** Ask the interpreter to execute some command
** by send a SIGNAL to the other thread.
*/
void sendTclCommandToSlave(char* command, char* slave)
{

  //printf("[TCL %s Send] Eval : %s\n", slave, command);
  //fflush(NULL);
  __Lock(&singleExecutionLock);
  {
    __Lock(&launchCommand);
    TclCommand = strdup(command);
    TclSlave = strdup(slave);
    __Lock(&wakeUpLock);
    __Signal(&wakeUp);
    __UnLock(&wakeUpLock);
    //printf("[TCL Main]Wait EXECUTION DONE\n");
    //fflush(NULL);
    __Wait(&workIsDone, &launchCommand);
    //printf("[TCL Send] DONE\n");
    //fflush(NULL);
    __UnLock(&launchCommand);
  }
  __UnLock(&singleExecutionLock);
}


/*
** Get Tcl Command Return Value
** Porpagate return value of Tcl Interpreter
** caught in thread.
*/
int getTclCommandReturn(void)
{
  //printf("[TCL getTclCommandReturn]\n");
  //fflush(NULL);
  return TclInterpReturn;
}

/*
** Get Tcl Command Result
** Porpagate return message of Tcl Interpreter
** caught in thread.
** This can be an error or a string value.
*/
char *getTclCommandResult(void)
{
  //printf("[TCL getTclCommandResult]\n");
  //fflush(NULL);
  if (TclInterpResult != NULL) {
    char *result = strdup(TclInterpResult);
    TclInterpResult = NULL;
    return result;
  }
  return strdup("\0");
}
