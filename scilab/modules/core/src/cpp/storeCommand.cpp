/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
extern "C"
{
#include "storeCommand.h"
}

#include "parser.hxx"
#include "execvisitor.hxx"
#include "tasks.hxx"
#include "scilabWrite.hxx"
#include "scilabexception.hxx"
#include "localization.hxx"
#include "runner.hxx"
#include "threadmanagement.hxx"

using namespace ast;
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
    int     m_isConsole;            /* 1 if the command come from console */
    CommandRec(char* command, int isInterruptible, int isPrioritary, int isConsole) : m_command(command), m_isInterruptible(isInterruptible), m_isPrioritary(isPrioritary), m_isConsole(isConsole) {}
};
/*--------------------------------------------------------------------------*/
static std::list<CommandRec> commandQueue;
static std::list<CommandRec> commandQueuePrioritary;
/*--------------------------------------------------------------------------*/
int StoreCommand(char *command)
{
    ThreadManagement::LockStoreCommand();
    commandQueue.emplace_back(os_strdup(command),
                              /*is prioritary*/ 0,
                              /* is interruptible*/ 1,
                              /* from console */ 0);

    ThreadManagement::UnlockStoreCommand();
    // Awake Scilab to execute a new command
    ThreadManagement::SendCommandStoredSignal();

    return 0;
}

int StoreConsoleCommand(char *command)
{
    ThreadManagement::LockStoreCommand();
    commandQueuePrioritary.emplace_back(os_strdup(command),
                                        /*is prioritary*/ 1,
                                        /* is interruptible*/ 1,
                                        /* from console */ 1);

    // Awake Scilab to execute a new command
    ThreadManagement::SendCommandStoredSignal();
    // Awake Runner to execute this prioritary command
    ThreadManagement::SendAwakeRunnerSignal();

    ThreadManagement::UnlockStoreCommand();

    return 0;
}

int StorePrioritaryCommand(char *command)
{
    ThreadManagement::LockStoreCommand();
    commandQueuePrioritary.emplace_back(os_strdup(command),
                                        /*is prioritary*/ 1,
                                        /* is interruptible*/ 0,
                                        /* from console */ 0);

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
int GetCommand (char** cmd, int* piInterruptible, int* piPrioritary, int* piConsole)
{
    int iCommandReturned = 0;

    ThreadManagement::LockStoreCommand();
    if (commandQueuePrioritary.empty() == false)
    {
        *cmd = os_strdup(commandQueuePrioritary.front().m_command);
        *piInterruptible = commandQueuePrioritary.front().m_isInterruptible;
        *piPrioritary = commandQueuePrioritary.front().m_isPrioritary;
        *piConsole = commandQueuePrioritary.front().m_isConsole;

        FREE (commandQueuePrioritary.front().m_command);
        commandQueuePrioritary.pop_front();

        iCommandReturned = 1;
    }
    else if (commandQueue.empty() == false)
    {
        *cmd = os_strdup(commandQueue.front().m_command);
        *piInterruptible = commandQueue.front().m_isInterruptible;
        *piPrioritary = commandQueue.front().m_isPrioritary;
        *piConsole = commandQueue.front().m_isConsole;

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
/* menu/button info for Scilab */
int C2F(getmen)(char * btn_cmd, int * lb, int * entry)
{
    //#pragma message("WARNING : C2F(getmen) is deprecated. It will be removed _BEFORE_ Scilab 6.0.")
    // FIXME : Do not forget to remove me.
    return 0;
}
/*--------------------------------------------------------------------------*/
