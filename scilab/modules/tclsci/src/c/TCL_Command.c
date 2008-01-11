/*
**  -*- C -*-
**
** TCL_Command.c
** Made by  Bruno JOFRET <bruno.jofret@inria.fr>
**
** Started on  Wed Jan  9 09:15:30 2008 bruno
** Last update Thu Jan 10 15:40:31 2008 bruno
**
** Copyright INRIA 2008
*/

#ifdef _MSC_VER
	#include <Windows.h>
	#define usleep(micro) Sleep(micro/1000)
	#define strdup _strdup
#endif
#include "TCL_Command.h"

// TCL Interpreter creation and loop ID
__threadId		TclThread;
// Globla Tcl Slave Name
char *			TclSlave;
// Global Tcl Command Buffer
char *			TclCommand;
// Command Lock
__threadLock		CommandLock;
// Global Tcl Return Code.
int			TclInterpReturn;
// Return Mutex
__threadLock		ReturnLock;
// Global Tcl Return Result.
char *			TclInterpResult;
// Result Mutex
__threadLock		ResultLock;
// Condition on TclCommand
__threadLock		wakeUpLock;
__threadSignal		wakeUp;

extern BOOL TK_Started;

/*
** This function is a timer to periodicly wake
** up the loop in startTclLoop.
** Called within a thread.
*/
static void *sleepAndSignal(void* in) {
  while(TK_Started) {
    usleep(TIME_TO_SLEEP);
    __Lock(&wakeUpLock);
    __Signal(&wakeUp);
    __UnLock(&wakeUpLock);
  }
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
  __Lock(&ReturnLock);
  __Lock(&ResultLock);
  __InitSignal(&wakeUp);
  __CreateThread(&sleepThreadId, sleepAndSignal);

  /*
  ** TCL Event Loop : Threaded
  */
  while(TK_Started) {
    __Lock(&wakeUpLock);
    /*
    ** -= IF =-
    ** there is a command to run
    ** RUN IT.
    */
    if (TclCommand != NULL)
      {
	/* Reinit local interpreter,
	   default is the biggest one. */
	LocalTCLinterp = TCLinterp;

	/* Check if it's supposed to be run in root or slave */
	if(TclSlave != NULL)
	  {
	    LocalTCLinterp = Tcl_GetSlave(TCLinterp, TclSlave);
	    free(TclSlave);
	    TclSlave = NULL;
	  }
	/* Do the evaluation */
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
	__UnLock(&ResultLock);
	__UnLock(&ReturnLock);
	free(TclCommand);
	TclCommand = NULL;
	__UnLock(&CommandLock);
      }
    /*
    ** -= ELSE =-
    ** there is no command
    ** do an update and wait to be waked up.
    */
    else
      {
	Tcl_Eval(TCLinterp, "update");
	__Wait(&wakeUp, &wakeUpLock);
      }
    __UnLock(&wakeUpLock);
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
  __Lock(&CommandLock);
  TclCommand = strdup(command);
  __Lock(&wakeUpLock);
  __Signal(&wakeUp);
  __UnLock(&wakeUpLock);
}

/*
** Send Tcl Command To Slave
** Ask the interpreter to execute some command
** by send a SIGNAL to the other thread.
*/
void sendTclCommandToSlave(char* command, char* slave)
{
  __Lock(&CommandLock);
  TclCommand = strdup(command);
  TclSlave = strdup(slave);
  __Lock(&wakeUpLock);
  __Signal(&wakeUp);
  __UnLock(&wakeUpLock);
}


/*
** Get Tcl Command Return Value
** Porpagate return value of Tcl Interpreter
** caught in thread.
*/
int getTclCommandReturn(void)
{
  __Lock(&ReturnLock);
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
  __Lock(&ResultLock);
  if (TclInterpResult != NULL) {
    char *result = strdup(TclInterpResult);
    TclInterpResult = NULL;
    return result;
  }
  return strdup("\0");
}
