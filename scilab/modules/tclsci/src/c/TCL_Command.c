/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007-2008 - INRIA - Bruno JOFRET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#include <stdlib.h>

#ifdef _MSC_VER
#include <Windows.h>
#define usleep(micro) Sleep(micro/1000)
#else
#include <unistd.h>
#endif

#include "os_string.h"
#include "sci_malloc.h"
#include "TCL_Command.h"
#include "GlobalTclInterp.h"

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
__threadSignalLock	wakeUpLock;

// Work is done
__threadSignal		workIsDone;

// Launch command
__threadSignalLock	launchCommand;

// ***********************
// ** Enable LocalDebug **
// **********************
//#define __LOCAL_DEBUG__


static Tcl_Interp *LocalTCLinterp;

static BOOL FileEvaluationInProgress;
static BOOL CommandEvaluationInProgress;

extern BOOL TK_Started;

/*
** This function is a timer to periodicly wake
** up the loop in startTclLoop.
** Called within a thread.
*/
static void *sleepAndSignal(void* in)
{
    while (TK_Started)
    {
#ifdef __LOCAL_DEBUG__
        //printf(".");
        //fflush(NULL);
#endif
        usleep(TIME_TO_SLEEP);
        __LockSignal(&wakeUpLock);
        __Signal(&wakeUp);
        __UnLockSignal(&wakeUpLock);
    }
    return NULL;
}

/*
** Evaluate the TclCommand inside Global interp
*/
static void evaluateTclCommand()
{
#ifdef __LOCAL_DEBUG__
    printf("[TCL Daemon] Eval : %s\n", TclCommand);
    fflush(NULL);
#endif
    //CommandEvaluationInProgress = TRUE;
    TclInterpReturn = Tcl_Eval(LocalTCLinterp, TclCommand);
    //CommandEvaluationInProgress = FALSE;
    FREE(TclCommand);
    TclCommand = NULL;
}

/*
** Evaluate the TclCommand inside Global interp
*/
static void evaluateTclFile()
{
#ifdef __LOCAL_DEBUG__
    printf("[TCL Daemon] Load : %s\n", TclFile);
    fflush(NULL);
#endif
    FileEvaluationInProgress = TRUE;
    TclInterpReturn = Tcl_EvalFile(LocalTCLinterp, TclFile);
    FileEvaluationInProgress = FALSE;
    FREE(TclFile);
    TclFile = NULL;
}

/*
** This function start an endless Tcl Loop
** in order the Scilab Global Tcl Interpreter
** do some "update" and let the Tcl Applications live.
*/
void startTclLoop()
{
    __threadKey key;
    __threadId sleepThreadId;

    __InitLock(&singleExecutionLock);

    __InitSignal(&wakeUp);
    __InitSignalLock(&wakeUpLock);
    __InitSignal(&workIsDone);
    __InitSignalLock(&launchCommand);

    __CreateThread(&sleepThreadId, &key, sleepAndSignal);

    __LockSignal(&InterpReadyLock);
    __Signal(&InterpReady);
    __UnLockSignal(&InterpReadyLock);
    /*
    ** TCL Event Loop : Threaded
    */
    while (TK_Started)
    {
#ifdef __LOCAL_DEBUG__
        //printf(".");
        //fflush(NULL);
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
            __LockSignal(&launchCommand);

            /* Reinit local interpreter,
               default is the biggest one. */
            LocalTCLinterp = getTclInterp();
            releaseTclInterp();

            /* Check if it's supposed to be run in root or slave */
            if (TclSlave != NULL)
            {
                LocalTCLinterp = Tcl_GetSlave(LocalTCLinterp, TclSlave);
                releaseTclInterp();
                FREE(TclSlave);
                TclSlave = NULL;
            }
            /*
            ** Do the evaluation
            */
            if (TclCommand != NULL)
            {
                evaluateTclCommand();
            }
            /*
            ** Do the evaluation
            */
            else if (TclFile != NULL)
            {
                evaluateTclFile();
            }
            /* Update return value and result */
            if (TclInterpResult)
            {
                FREE(TclInterpResult);
            }
            if (Tcl_GetStringResult(LocalTCLinterp) && strlen(Tcl_GetStringResult(LocalTCLinterp)) != 0)
            {
                TclInterpResult = os_strdup(Tcl_GetStringResult(LocalTCLinterp));
            }
            else
            {
                TclInterpResult = NULL;
            }
#ifdef __LOCAL_DEBUG__
            printf("[SIGNAL] executionDone\n");
            fflush(NULL);
#endif
            releaseTclInterp();
            Tcl_Eval(getTclInterp(), "update");
            releaseTclInterp();
            __Signal(&workIsDone);
            __UnLockSignal(&launchCommand);
        }
        /*
        ** -= ELSE =-
        ** there is no command
        ** do an update and wait to be waked up.
        */
        else
        {
            __LockSignal(&wakeUpLock);
            Tcl_Eval(getTclInterp(), "update");
            releaseTclInterp();
#ifdef __LOCAL_DEBUG__
            //printf("[TCL Daemon] Wait\n");
            //fflush(NULL);
#endif
            __Wait(&wakeUp, &wakeUpLock);
            __UnLockSignal(&wakeUpLock);
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
        __LockSignal(&launchCommand);
        TclFile = os_strdup(file);
        if (slave != NULL)
        {
            TclSlave = os_strdup(slave);
        }
        else
        {
            TclSlave = NULL;
        }
        __LockSignal(&wakeUpLock);
        __Signal(&wakeUp);
        __UnLockSignal(&wakeUpLock);
#ifdef __LOCAL_DEBUG__
        printf("[TCL Main] Wait EXECUTION DONE\n");
        fflush(NULL);
#endif
        __Wait(&workIsDone, &launchCommand);
#ifdef __LOCAL_DEBUG__
        printf("[TCL SendFile] DONE\n");
        fflush(NULL);
#endif
        __UnLockSignal(&launchCommand);
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
    /*
    ** Shortcut for evaluation if there is some file evaluation
    ** in progress.
    */
    if (!FileEvaluationInProgress && !CommandEvaluationInProgress)
    {
#ifdef __LOCAL_DEBUG__
        printf("[TCL %s Send] Eval : %s\n", slave, command);
        fflush(NULL);
#endif

        CommandEvaluationInProgress = TRUE;
        __Lock(&singleExecutionLock);

        __LockSignal(&launchCommand);
        TclCommand = os_strdup(command);
        if (slave != NULL)
        {
            TclSlave = os_strdup(slave);
        }
        else
        {
            TclSlave = NULL;
        }
        __LockSignal(&wakeUpLock);
        __Signal(&wakeUp);
        __UnLockSignal(&wakeUpLock);

#ifdef __LOCAL_DEBUG__
        printf("[TCL Main]Wait EXECUTION DONE\n");
        fflush(NULL);
#endif
        __Wait(&workIsDone, &launchCommand);
#ifdef __LOCAL_DEBUG__
        printf("[TCL Send] DONE\n");
        fflush(NULL);
#endif
        __UnLockSignal(&launchCommand);
        __UnLock(&singleExecutionLock);
        CommandEvaluationInProgress = FALSE;
        return getTclCommandReturn();
    }
    else
    {
#ifdef __LOCAL_DEBUG__
        printf("[TCL %s Send] Eval : %s -- Shortcut\n", slave, command);
        fflush(NULL);
#endif
        /*
        ** File Evaluation in progress
        */
        TclCommand = os_strdup(command);
        if (slave != NULL)
        {
            TclSlave = os_strdup(slave);
        }
        else
        {
            TclSlave = NULL;
        }
        evaluateTclCommand();
    }
    return TCL_OK;
}


/*
** Get Tcl Command Return Value
** Propagate return value of Tcl Interpreter
** caught in thread.
*/
int getTclCommandReturn(void)
{
    int commandResult = TclInterpReturn;

#ifdef __LOCAL_DEBUG__
    printf("[TCL getTclCommandReturn]\n");
    fflush(NULL);
#endif

    TclInterpReturn = 0;
    return commandResult;
}

/*
** Get Tcl Command Result
** Propagate return message of Tcl Interpreter
** caught in thread.
** This can be an error or a string value.
*/
char *getTclCommandResult(void)
{
#ifdef __LOCAL_DEBUG__
    printf("[TCL getTclCommandResult]\n");
    fflush(NULL);
#endif
    if (TclInterpResult != NULL)
    {
        char *result = os_strdup(TclInterpResult);
        TclInterpResult = NULL;
        return result;
    }
    return os_strdup("\0");
}
