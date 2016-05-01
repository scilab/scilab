/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
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
/*--------------------------------------------------------------------------*/
extern "C"
{
#include "storeCommand.h"
}

#include "parser.hxx"
#include "tasks.hxx"
#include "scilabWrite.hxx"
#include "scilabexception.hxx"
#include "localization.hxx"
#include "runner.hxx"
#include "threadmanagement.hxx"

/*--------------------------------------------------------------------------*/
/*
 *  Command queue functions
 *  This function is used to store Scilab command in a queue
 *
 *  PUBLIC : int StoreCommand( char *command)
 *           int StoreConsoleCommand(char *command)
 *           int StorePrioritaryCommand(char *command)
 *           int C2F(ismenu)()
 *           int C2F(getmen)(char * btn_cmd,int * lb, int * entry)
 */
/*--------------------------------------------------------------------------*/
struct CommandRec
{
    char*   m_command;              /* command info one string two integers */
    int     m_isInterruptible;      /* 1 if the command execution can be interrupted */
    int     m_isPrioritary;         /* 1 if the command is prioritary */
    command_origin_t     m_iCommandOrigin;       /* Indicate who have stored the command (ie: console, tcl) */
    CommandRec(char* command, int isInterruptible, int isPrioritary, command_origin_t iCmdOrigin) :
        m_command(command), m_isInterruptible(isInterruptible), m_isPrioritary(isPrioritary), m_iCommandOrigin(iCmdOrigin) {}
};
/*--------------------------------------------------------------------------*/
static std::list<CommandRec> commandQueue;
static std::list<CommandRec> commandQueuePrioritary;
/*--------------------------------------------------------------------------*/
int StoreCommandWithFlags(const char* command, int iPrioritary, int iInterruptible, command_origin_t iCmdOrigin)
{
    ThreadManagement::LockStoreCommand();
    if (iPrioritary)
    {
        commandQueuePrioritary.emplace_back(os_strdup(command), iPrioritary, iInterruptible, iCmdOrigin);

        // Awake Runner to execute this prioritary command
        ThreadManagement::SendAwakeRunnerSignal();
    }
    else
    {
        commandQueue.emplace_back(os_strdup(command), iPrioritary, iInterruptible, iCmdOrigin);
    }

    ThreadManagement::UnlockStoreCommand();
    // Awake Scilab to execute a new command
    ThreadManagement::SendCommandStoredSignal();

    return 0;
}

int StoreCommand(const char *command)
{
    ThreadManagement::LockStoreCommand();
    commandQueue.emplace_back(os_strdup(command),
                              /*is prioritary*/ 0,
                              /* is interruptible*/ 1,
                              /* cmd origin */ NONE);

    ThreadManagement::UnlockStoreCommand();
    // Awake Scilab to execute a new command
    ThreadManagement::SendCommandStoredSignal();

    return 0;
}

int StoreConsoleCommand(const char *command, int iWaitFor)
{
    ThreadManagement::LockStoreCommand();
    commandQueuePrioritary.emplace_back(os_strdup(command),
                                        /*is prioritary*/ 1,
                                        /* is interruptible*/ 1,
                                        /* cmd origin */ CONSOLE);

    // Awake Scilab to execute a new command
    ThreadManagement::SendCommandStoredSignal();
    // Awake Runner to execute this prioritary command
    ThreadManagement::SendAwakeRunnerSignal();

    if (iWaitFor)
    {
        // make this wait before unlock the Store Command will prevent
        // dead lock in case where an other thread get this command
        // and execute it before this thread is waiting for.
        ThreadManagement::WaitForConsoleExecDoneSignal();
    }
    else
    {
        ThreadManagement::UnlockStoreCommand();
    }

    return 0;
}

int StorePrioritaryCommand(const char *command)
{
    ThreadManagement::LockStoreCommand();
    commandQueuePrioritary.emplace_back(os_strdup(command),
                                        /*is prioritary*/ 1,
                                        /* is interruptible*/ 0,
                                        /* cmd origin */ NONE);

    // Awake Scilab to execute a new command
    ThreadManagement::SendCommandStoredSignal();
    // Awake Runner to execute this prioritary command
    ThreadManagement::SendAwakeRunnerSignal();

    ThreadManagement::UnlockStoreCommand();

    return 0;
}

int isEmptyCommandQueue(void)
{
    return (commandQueuePrioritary.empty() && commandQueue.empty());
}

/*
 * Gets the next command to execute
 * and remove it from the queue
 */
int GetCommand(char** cmd, int* piPrioritary, int* piInterruptible, command_origin_t* piCmdOrigin)
{
    int iCommandReturned = 0;

    ThreadManagement::LockStoreCommand();
    if (commandQueuePrioritary.empty() == false)
    {
        *cmd = os_strdup(commandQueuePrioritary.front().m_command);
        *piInterruptible = commandQueuePrioritary.front().m_isInterruptible;
        *piPrioritary = commandQueuePrioritary.front().m_isPrioritary;
        *piCmdOrigin = commandQueuePrioritary.front().m_iCommandOrigin;

        FREE (commandQueuePrioritary.front().m_command);
        commandQueuePrioritary.pop_front();

        iCommandReturned = 1;
    }
    else if (commandQueue.empty() == false)
    {
        *cmd = os_strdup(commandQueue.front().m_command);
        *piInterruptible = commandQueue.front().m_isInterruptible;
        *piPrioritary = commandQueue.front().m_isPrioritary;
        *piCmdOrigin = commandQueue.front().m_iCommandOrigin;

        FREE (commandQueue.front().m_command);
        commandQueue.pop_front();

        iCommandReturned = 1;
    }
    ThreadManagement::UnlockStoreCommand();

    return iCommandReturned;
}

/*--------------------------------------------------------------------------*/
int ismenu(void)
{
    //#pragma message("WARNING : ismenu is deprecated. It will be removed _BEFORE_ Scilab 6.0.")
    // FIXME : Do not forget to remove me.
    return 0;
}
/*--------------------------------------------------------------------------*/
