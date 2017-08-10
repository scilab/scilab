/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises - Cedric DELAMARRE
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

#include "threadmanagement.hxx"
#include "runner.hxx"

#ifdef DEBUG_THREAD
#include <iostream>
#include <iomanip>

#define PRINT_COL_SIZE 32

__threadKey ThreadManagement::m_tkMain;
__threadKey ThreadManagement::m_tkReadAndExec;
__threadKey ThreadManagement::m_tkConsole;
#endif // DEBUG_THREAD

__threadLock ThreadManagement::m_RunnerLock;
__threadLock ThreadManagement::m_ParseLock;
__threadLock ThreadManagement::m_StoreCommandLock;
__threadLock ThreadManagement::m_ScilabReadLock;

__threadSignal ThreadManagement::m_ConsoleExecDone;
__threadSignalLock ThreadManagement::m_ConsoleExecDoneLock;

__threadSignal ThreadManagement::m_AwakeRunner;
__threadSignalLock ThreadManagement::m_AwakeRunnerLock;

__threadSignal ThreadManagement::m_AvailableRunner;
__threadSignalLock ThreadManagement::m_AvailableRunnerLock;

__threadSignal ThreadManagement::m_StartPending;
__threadSignalLock ThreadManagement::m_StartPendingLock;

__threadSignal ThreadManagement::m_CommandStored;
__threadSignalLock ThreadManagement::m_CommandStoredLock;

__threadSignal ThreadManagement::m_RunMe;
__threadSignalLock ThreadManagement::m_RunMeLock;

bool ThreadManagement::m_AvailableRunnerWasSignalled    = false;
bool ThreadManagement::m_ConsoleExecDoneWasSignalled    = false;
bool ThreadManagement::m_AwakeRunnerWasSignalled        = false;
bool ThreadManagement::m_StartPendingWasSignalled       = false;
bool ThreadManagement::m_CommandStoredWasSignalled      = false;
bool ThreadManagement::m_RunMeWasSignalled              = false;

void ThreadManagement::initialize()
{
    __InitLock(&m_RunnerLock);
    __InitLock(&m_ParseLock);
    __InitLock(&m_StoreCommandLock);
    __InitLock(&m_ScilabReadLock);

    __InitSignal(&m_AwakeRunner);
    __InitSignalLock(&m_AwakeRunnerLock);

    __InitSignal(&m_ConsoleExecDone);
    __InitSignalLock(&m_ConsoleExecDoneLock);

    __InitSignal(&m_AvailableRunner);
    __InitSignalLock(&m_AvailableRunnerLock);

    __InitSignal(&m_StartPending);
    __InitSignalLock(&m_StartPendingLock);

    __InitSignal(&m_CommandStored);
    __InitSignalLock(&m_CommandStoredLock);

    __InitSignal(&m_RunMe);
    __InitSignalLock(&m_RunMeLock);
}

/***
    [Runner Lock]
    Used when we want to access to the Parser.
***/
void ThreadManagement::LockParser(void)
{
#ifdef DEBUG_THREAD
    PrintDebug("LockParser");
#endif // DEBUG_THREAD
    __Lock(&m_ParseLock);
}

void ThreadManagement::UnlockParser(void)
{
#ifdef DEBUG_THREAD
    PrintDebug("UnlockParser");
#endif // DEBUG_THREAD
    __UnLock(&m_ParseLock);
}

/***
    [Runner Lock]
    Used when we want to access to the Store Command.
***/
void ThreadManagement::LockStoreCommand(void)
{
#ifdef DEBUG_THREAD
    PrintDebug("LockStoreCommand");
#endif // DEBUG_THREAD
    __Lock(&m_StoreCommandLock);
}

void ThreadManagement::UnlockStoreCommand(void)
{
#ifdef DEBUG_THREAD
    PrintDebug("UnlockStoreCommand");
#endif // DEBUG_THREAD
    __UnLock(&m_StoreCommandLock);
}

/***
    [Runner Lock]
    Used when we want to access to the global Runner.
***/
void ThreadManagement::LockRunner(void)
{
#ifdef DEBUG_THREAD
    PrintDebug("LockRunner");
#endif // DEBUG_THREAD
    __Lock(&m_RunnerLock);
}

void ThreadManagement::UnlockRunner(void)
{
#ifdef DEBUG_THREAD
    PrintDebug("UnlockRunner");
#endif // DEBUG_THREAD
    __UnLock(&m_RunnerLock);
}

/***
    [ScilabRead Lock]
    Used to manage scilabRead output wich can be used by Console thread or
    main thread through mscanf function.
***/
void ThreadManagement::LockScilabRead(void)
{
#ifdef DEBUG_THREAD
    PrintDebug("LockScilabRead");
#endif // DEBUG_THREAD
    __Lock(&m_ScilabReadLock);
}

void ThreadManagement::UnlockScilabRead(void)
{
#ifdef DEBUG_THREAD
    PrintDebug("UnlockScilabRead");
#endif // DEBUG_THREAD
    __UnLock(&m_ScilabReadLock);
}

/***
    [AvailableRunner Signal]

    Send : The global Runner is available to store a new one.
    Wait : This happens when the last Runner is not yet in execution.

    This signal can be sent without any threads are waiting for,
    so we have to perform the Wait for each call to WaitForConsoleExecDoneSignal.

    The loop while is used to avoid spurious wakeup of __Wait.
***/
void ThreadManagement::SendAvailableRunnerSignal(void)
{
    __LockSignal(&m_AvailableRunnerLock);
    m_AvailableRunnerWasSignalled = true;
#ifdef DEBUG_THREAD
    PrintDebug("SendAvailableRunnerSignal");
#endif // DEBUG_THREAD
    __Signal(&m_AvailableRunner);
    __UnLockSignal(&m_AvailableRunnerLock);
}

void ThreadManagement::WaitForAvailableRunnerSignal(void)
{
    __LockSignal(&m_AvailableRunnerLock);
    m_AvailableRunnerWasSignalled = false;
    while (m_AvailableRunnerWasSignalled == false)
    {
#ifdef DEBUG_THREAD
        PrintDebug("WaitForAvailableRunnerSignal");
#endif // DEBUG_THREAD
        __Wait(&m_AvailableRunner, &m_AvailableRunnerLock);
    }
    __UnLockSignal(&m_AvailableRunnerLock);
}

/***
    [ConsoleExecDone Signal]

    Send : A console command is excuted.
    Wait : Wait for the last console command ends.

    This signal can be sent without any threads are waiting for,
    so we have to perform the Wait for each call to WaitForConsoleExecDoneSignal.
    (in case of "pause", we send this signal in sci_pause and in Runner::launch)

    The loop while is used to avoid spurious wakeup of __Wait.
***/
void ThreadManagement::SendConsoleExecDoneSignal(void)
{
#ifdef DEBUG_THREAD
    PrintDebug("SendConsoleExecDoneSignal");
#endif // DEBUG_THREAD
    __LockSignal(&m_ConsoleExecDoneLock);
    m_ConsoleExecDoneWasSignalled = true;
    __Signal(&m_ConsoleExecDone);
    __UnLockSignal(&m_ConsoleExecDoneLock);
}

void ThreadManagement::WaitForConsoleExecDoneSignal(void)
{
# ifdef __DEBUG_SIGNAL
    std::cout << "WaitForConsoleExecDoneSignal" << std::endl;
# endif // __DEBUG_SIGNAL
    __LockSignal(&m_ConsoleExecDoneLock);
    ThreadManagement::UnlockStoreCommand();
    m_ConsoleExecDoneWasSignalled = false;
    while (m_ConsoleExecDoneWasSignalled == false)
    {
#ifdef DEBUG_THREAD
        PrintDebug("WaitForConsoleExecDoneSignal");
#endif // DEBUG_THREAD
        __Wait(&m_ConsoleExecDone, &m_ConsoleExecDoneLock);
    }
    __UnLockSignal(&m_ConsoleExecDoneLock);
}

/***
    [AwakeRunner Signal]

    Send : Wakeup the runner when:
    Wait : Runner is waiting for:
            - a new prioritary command have to be execute.
            - a pause is executed, to allow a new console command.
            - the last execution is made.

    This signal can be sent without any threads are waiting for,
    so we have to perform the Wait for each call to WaitForAwakeRunnerSignal.

    The loop while is used to avoid spurious wakeup of __Wait.
***/

void ThreadManagement::SendAwakeRunnerSignal(void)
{
# ifdef __DEBUG_SIGNAL
    std::cout << "SendAwakeRunnerSignal" << std::endl;
# endif // __DEBUG_SIGNAL
    __LockSignal(&m_AwakeRunnerLock);
    m_AwakeRunnerWasSignalled = true;
#ifdef DEBUG_THREAD
    PrintDebug("SendAwakeRunnerSignal");
#endif // DEBUG_THREAD
    __Signal(&m_AwakeRunner);
    __UnLockSignal(&m_AwakeRunnerLock);
}

void ThreadManagement::WaitForAwakeRunnerSignal(void)
{
# ifdef __DEBUG_SIGNAL
    std::cout << "WaitForAwakeRunnerSignal" << std::endl;
# endif // __DEBUG_SIGNAL
    __LockSignal(&m_AwakeRunnerLock);
    ThreadManagement::UnlockRunner();
    m_AwakeRunnerWasSignalled = false;
    while (m_AwakeRunnerWasSignalled == false)
    {
#ifdef DEBUG_THREAD
        PrintDebug("WaitForAwakeRunnerSignal");
#endif // DEBUG_THREAD
        __Wait(&m_AwakeRunner, &m_AwakeRunnerLock);
    }
    __UnLockSignal(&m_AwakeRunnerLock);
}

/***
    [StartPending Signal]

    This signal is used in case where we have a console thread and a command to execute passed by -f argument.
    We have to waiting for the "-f" execution before lets users to enter a new command through the console.

    Send : The console thread (scilabReadAndStore) is ready.
    Wait : The main thread can create the read and exec command thread (scilabReadAndExecCommand).

    To avoid non-expected lost signal, we have to check if the signal was
    already sent to know if we have to waiting for or not.

    The loop while is used to avoid spurious wakeup of __Wait.
***/
void ThreadManagement::SendStartPendingSignal(void)
{
# ifdef __DEBUG_SIGNAL
    std::cout << "SendStartPendingSignal" << std::endl;
# endif // __DEBUG_SIGNAL
    __LockSignal(&m_StartPendingLock);
    m_StartPendingWasSignalled = true;
#ifdef DEBUG_THREAD
    PrintDebug("SendStartPendingSignal");
#endif // DEBUG_THREAD
    __Signal(&m_StartPending);
    __UnLockSignal(&m_StartPendingLock);
}

void ThreadManagement::WaitForStartPendingSignal(void)
{
# ifdef __DEBUG_SIGNAL
    std::cout << "WaitForStartPendingSignal" << std::endl;
# endif // __DEBUG_SIGNAL
    __LockSignal(&m_StartPendingLock);
    while (m_StartPendingWasSignalled == false)
    {
#ifdef DEBUG_THREAD
        PrintDebug("WaitForStartPendingSignal");
#endif // DEBUG_THREAD
        __Wait(&m_StartPending, &m_StartPendingLock);
    }
    m_StartPendingWasSignalled = false;
    __UnLockSignal(&m_StartPendingLock);
}

/***
    [CommandStored Signal]

    Send : A new command is available in the store command.
    Wait : Wait for a new command.

    To avoid non-expected lost signal, we have to check if the signal was
    already sent to know if we have to waiting for or not.

    The loop while is used to avoid spurious wakeup of __Wait.
***/
void ThreadManagement::SendCommandStoredSignal(void)
{
    __LockSignal(&m_CommandStoredLock);
    m_CommandStoredWasSignalled = true;
#ifdef DEBUG_THREAD
    PrintDebug("SendCommandStoredSignal");
#endif // DEBUG_THREAD
    __Signal(&m_CommandStored);
    __UnLockSignal(&m_CommandStoredLock);
}

void ThreadManagement::WaitForCommandStoredSignal(void)
{
    __LockSignal(&m_CommandStoredLock);
    while (m_CommandStoredWasSignalled == false)
    {
#ifdef DEBUG_THREAD
        PrintDebug("WaitForCommandStoredSignal");
#endif // DEBUG_THREAD
        __Wait(&m_CommandStored, &m_CommandStoredLock);
    }
    m_CommandStoredWasSignalled = false;
    __UnLockSignal(&m_CommandStoredLock);
}

/***
    [RunMe Signal]

    Send : A new runner is available for execution.
    Wait : Wait for an available Runner.

    This signal can be sent without any threads are waiting for,
    so we have to perform the Wait for each call to WaitForRunMeSignal.
    (This can happends when an execution is interrupted by an other one.
     This signal is sent but the main thread is not waiting for.)

    The loop while is used to avoid spurious wakeup of __Wait.
***/
void ThreadManagement::SendRunMeSignal(void)
{
    __LockSignal(&m_RunMeLock);
    m_RunMeWasSignalled = true;
#ifdef DEBUG_THREAD
    PrintDebug("SendRunMeSignal");
#endif // DEBUG_THREAD
    __Signal(&m_RunMe);
    __UnLockSignal(&m_RunMeLock);
}

void ThreadManagement::WaitForRunMeSignal(void)
{
    __LockSignal(&m_RunMeLock);
    m_RunMeWasSignalled = false;
    // Some times, the signal "SendRunMeSignal" can be sent before the main thread is waiting for.
    // If a Runner is available do not perform this wait.
    bool bWait = StaticRunner_isRunnerAvailable() == false;
    while (m_RunMeWasSignalled == false && bWait)
    {
#ifdef DEBUG_THREAD
        PrintDebug("WaitForRunMeSignal");
#endif // DEBUG_THREAD
        __Wait(&m_RunMe, &m_RunMeLock);
    }
    __UnLockSignal(&m_RunMeLock);
}

#ifdef DEBUG_THREAD
void ThreadManagement::SetThreadKey(__threadKey tkMain, __threadKey tkReadAndExec, __threadKey tkConsole)
{
    m_tkMain = tkMain;
    m_tkReadAndExec = tkReadAndExec;
    m_tkConsole = tkConsole;
}

void ThreadManagement::PrintDebug(const char* pcfunName)
{
    if (__GetCurrentThreadKey() == m_tkConsole)
    {
        std::cout.width(2 * PRINT_COL_SIZE);
        std::cout << " ";
    }

    if (__GetCurrentThreadKey() == m_tkReadAndExec)
    {
        std::cout.width(PRINT_COL_SIZE);
        std::cout << " ";
    }

    std::cout << pcfunName << std::endl;
}

void ThreadManagement::PrintDebugHead()
{
    std::cout << std::endl;
    std::cout.fill('-');
    std::cout.width(3 * PRINT_COL_SIZE);
    std::cout << "-";

    std::cout.fill(' ');
    std::cout << std::endl;
    std::cout << std::left;
    std::cout.width(PRINT_COL_SIZE);
    std::cout << "Main Thread";
    std::cout.width(PRINT_COL_SIZE);
    std::cout << "ReadAndExec Thread";
    std::cout.width(PRINT_COL_SIZE);
    std::cout << "Console Thread";
    std::cout << std::endl << std::endl;
}
#endif // DEBUG_THREAD
