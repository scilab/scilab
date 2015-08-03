/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises - Cedric DELAMARRE
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "threadmanagement.hxx"

//#define DEBUG_THREAD
#ifdef DEBUG_THREAD
#include <iostream>
#endif // DEBUG_THREAD

__threadLock ThreadManagement::m_StartLock;
__threadLock ThreadManagement::m_RunnerLock;
__threadLock ThreadManagement::m_ParseLock;
__threadLock ThreadManagement::m_StoreCommandLock;
__threadLock ThreadManagement::m_AstLock;

__threadSignal ThreadManagement::m_ConsoleExecDone;
__threadSignalLock ThreadManagement::m_ConsoleExecDoneLock;

__threadSignal ThreadManagement::m_AwakeRunner;
__threadSignalLock ThreadManagement::m_AwakeRunnerLock;

__threadSignal ThreadManagement::m_AstPending;
__threadSignalLock ThreadManagement::m_AstPendingLock;

__threadSignal ThreadManagement::m_StartPending;
__threadSignalLock ThreadManagement::m_StartPendingLock;

__threadSignal ThreadManagement::m_CommandStored;
__threadSignalLock ThreadManagement::m_CommandStoredLock;

bool ThreadManagement::m_AstPendingWasSignalled         = false;
bool ThreadManagement::m_ConsoleExecDoneWasSignalled    = false;
bool ThreadManagement::m_AwakeRunnerWasSignalled        = false;
bool ThreadManagement::m_StartPendingWasSignalled       = false;
bool ThreadManagement::m_CommandStoredWasSignalled      = false;

void ThreadManagement::initialize()
{
    __InitLock(&m_RunnerLock);
    __InitLock(&m_StartLock);
    __InitLock(&m_ParseLock);
    __InitLock(&m_StoreCommandLock);
    __InitLock(&m_AstLock);

    __InitSignal(&m_AwakeRunner);
    __InitSignalLock(&m_AwakeRunnerLock);

    __InitSignal(&m_ConsoleExecDone);
    __InitSignalLock(&m_ConsoleExecDoneLock);

    __InitSignal(&m_AstPending);
    __InitSignalLock(&m_AstPendingLock);

    __InitSignal(&m_StartPending);
    __InitSignalLock(&m_StartPendingLock);

    __InitSignal(&m_CommandStored);
    __InitSignalLock(&m_CommandStoredLock);
}

void ThreadManagement::LockStart(void)
{
#ifdef DEBUG_THREAD
    std::cout << "[" << __GetCurrentThreadKey() << "] " << "LockStart" << std::endl;
#endif // DEBUG_THREAD
    __Lock(&m_StartLock);
}

void ThreadManagement::UnlockStart(void)
{
#ifdef DEBUG_THREAD
    std::cout << "[" << __GetCurrentThreadKey() << "] " << "UnlockStart" << std::endl;
#endif // DEBUG_THREAD
    __UnLock(&m_StartLock);
}

void ThreadManagement::LockParser(void)
{
#ifdef DEBUG_THREAD
    std::cout << "[" << __GetCurrentThreadKey() << "] " << "LockParser" << std::endl;
#endif // DEBUG_THREAD
    __Lock(&m_ParseLock);
}

void ThreadManagement::UnlockParser(void)
{
#ifdef DEBUG_THREAD
    std::cout << "[" << __GetCurrentThreadKey() << "] " << "UnlockParser" << std::endl;
#endif // DEBUG_THREAD
    __UnLock(&m_ParseLock);
}

void ThreadManagement::LockStoreCommand(void)
{
#ifdef DEBUG_THREAD
    std::cout << "[" << __GetCurrentThreadKey() << "] " << "LockStoreCommand" << std::endl;
#endif // DEBUG_THREAD
    __Lock(&m_StoreCommandLock);
}

void ThreadManagement::UnlockStoreCommand(void)
{
#ifdef DEBUG_THREAD
    std::cout << "[" << __GetCurrentThreadKey() << "] " << "UnlockStoreCommand" << std::endl;
#endif // DEBUG_THREAD
    __UnLock(&m_StoreCommandLock);
}

void ThreadManagement::LockRunner(void)
{
#ifdef DEBUG_THREAD
    std::cout << "[" << __GetCurrentThreadKey() << "] " << "LockRunner" << std::endl;
#endif // DEBUG_THREAD
    __Lock(&m_RunnerLock);
}

void ThreadManagement::UnlockRunner(void)
{
#ifdef DEBUG_THREAD
    std::cout << "[" << __GetCurrentThreadKey() << "] " << "UnlockRunner" << std::endl;
#endif // DEBUG_THREAD
    __UnLock(&m_RunnerLock);
}

void ThreadManagement::LockAst(void)
{
#ifdef DEBUG_THREAD
    std::cout << "[" << __GetCurrentThreadKey() << "] " << "LockAst" << std::endl;
#endif // DEBUG_THREAD
    __Lock(&m_AstLock);
}

void ThreadManagement::UnlockAst(void)
{
#ifdef DEBUG_THREAD
    std::cout << "[" << __GetCurrentThreadKey() << "] " << "UnlockAst" << std::endl;
#endif // DEBUG_THREAD
    __UnLock(&m_AstLock);
}

void ThreadManagement::SendAstPendingSignal(void)
{
#ifdef DEBUG_THREAD
    std::cout << "[" << __GetCurrentThreadKey() << "] " << "SendAstPendingSignal" << std::endl;
#endif // DEBUG_THREAD
    __LockSignal(&m_AstPendingLock);
    m_AstPendingWasSignalled = true;
    __Signal(&m_AstPending);
    __UnLockSignal(&m_AstPendingLock);
}

void ThreadManagement::WaitForAstPendingSignal(void)
{
#ifdef DEBUG_THREAD
    std::cout << "[" << __GetCurrentThreadKey() << "] " << "WaitForAstPendingSignal" << std::endl;
#endif // DEBUG_THREAD
    __LockSignal(&m_AstPendingLock);
    ThreadManagement::UnlockAst();
    m_AstPendingWasSignalled = false;
    while (m_AstPendingWasSignalled == false)
    {
        __Wait(&m_AstPending, &m_AstPendingLock);
    }
    __UnLockSignal(&m_AstPendingLock);

}

void ThreadManagement::SendConsoleExecDoneSignal(void)
{
#ifdef DEBUG_THREAD
    std::cout << "[" << __GetCurrentThreadKey() << "] " << "SendConsoleExecDoneSignal" << std::endl;
#endif // DEBUG_THREAD
    __LockSignal(&m_ConsoleExecDoneLock);
    m_ConsoleExecDoneWasSignalled = true;
    __Signal(&m_ConsoleExecDone);
    __UnLockSignal(&m_ConsoleExecDoneLock);
}

void ThreadManagement::WaitForConsoleExecDoneSignal(void)
{
#ifdef DEBUG_THREAD
    std::cout << "[" << __GetCurrentThreadKey() << "] " << "WaitForConsoleExecDoneSignal" << std::endl;
#endif // DEBUG_THREAD
    __LockSignal(&m_ConsoleExecDoneLock);
    m_ConsoleExecDoneWasSignalled = false;
    while (m_ConsoleExecDoneWasSignalled == false)
    {
        __Wait(&m_ConsoleExecDone, &m_ConsoleExecDoneLock);
    }
    __UnLockSignal(&m_ConsoleExecDoneLock);
}

void ThreadManagement::SendAwakeRunnerSignal(void)
{
#ifdef DEBUG_THREAD
    std::cout << "[" << __GetCurrentThreadKey() << "] " << "SendAwakeRunnerSignal" << std::endl;
#endif // DEBUG_THREAD
    __LockSignal(&m_AwakeRunnerLock);
    m_AwakeRunnerWasSignalled = true;
    __Signal(&m_AwakeRunner);
    __UnLockSignal(&m_AwakeRunnerLock);
}

void ThreadManagement::WaitForAwakeRunnerSignal(void)
{
#ifdef DEBUG_THREAD
    std::cout << "[" << __GetCurrentThreadKey() << "] " << "WaitForAwakeRunnerSignal" << std::endl;
#endif // DEBUG_THREAD
    __LockSignal(&m_AwakeRunnerLock);
    ThreadManagement::UnlockRunner();
    m_AwakeRunnerWasSignalled = false;
    while (m_AwakeRunnerWasSignalled == false)
    {
        __Wait(&m_AwakeRunner, &m_AwakeRunnerLock);
    }
    __UnLockSignal(&m_AwakeRunnerLock);
}

void ThreadManagement::SendStartPendingSignal(void)
{
#ifdef DEBUG_THREAD
    std::cout << "[" << __GetCurrentThreadKey() << "] " << "SendStartPendingSignal" << std::endl;
#endif // DEBUG_THREAD
    __LockSignal(&m_StartPendingLock);
    m_StartPendingWasSignalled = true;
    __Signal(&m_StartPending);
    __UnLockSignal(&m_StartPendingLock);
}

void ThreadManagement::WaitForStartPendingSignal(void)
{
#ifdef DEBUG_THREAD
    std::cout << "[" << __GetCurrentThreadKey() << "] " << "WaitForStartPendingSignal" << std::endl;
#endif // DEBUG_THREAD
    __LockSignal(&m_StartPendingLock);
    ThreadManagement::UnlockStart();
    m_StartPendingWasSignalled = false;
    while (m_StartPendingWasSignalled == false)
    {
        __Wait(&m_StartPending, &m_StartPendingLock);
    }
    __UnLockSignal(&m_StartPendingLock);
}

void ThreadManagement::SendCommandStoredSignal(void)
{
#ifdef DEBUG_THREAD
    std::cout << "[" << __GetCurrentThreadKey() << "] " << "SendCommandStoredSignal" << std::endl;
#endif // DEBUG_THREAD
    __LockSignal(&m_CommandStoredLock);
    m_CommandStoredWasSignalled = true;
    __Signal(&m_CommandStored);
    __UnLockSignal(&m_CommandStoredLock);
}

void ThreadManagement::WaitForCommandStoredSignal(void)
{
#ifdef DEBUG_THREAD
    std::cout << "[" << __GetCurrentThreadKey() << "] " << "WaitForCommandStoredSignal" << std::endl;
#endif // DEBUG_THREAD
    __LockSignal(&m_CommandStoredLock);
    m_CommandStoredWasSignalled = false;
    while (m_CommandStoredWasSignalled == false)
    {
        __Wait(&m_CommandStored, &m_CommandStoredLock);
    }
    __UnLockSignal(&m_CommandStoredLock);
}
