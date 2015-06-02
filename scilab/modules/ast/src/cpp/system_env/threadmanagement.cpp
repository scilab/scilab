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

__threadLock ThreadManagement::m_StartLock;
__threadLock ThreadManagement::m_RunnerLock;
__threadLock ThreadManagement::m_ParseLock;
__threadLock ThreadManagement::m_StoreCommandLock;

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


void ThreadManagement::initialize()
{
    __InitLock(&m_RunnerLock);
    __InitLock(&m_StartLock);
    __InitLock(&m_ParseLock);
    __InitLock(&m_StoreCommandLock);

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
    __Lock(&m_StartLock);
}

void ThreadManagement::UnlockStart(void)
{
    __UnLock(&m_StartLock);
}

void ThreadManagement::LockParser(void)
{
    __Lock(&m_ParseLock);
}

void ThreadManagement::UnlockParser(void)
{
    __UnLock(&m_ParseLock);
}

void ThreadManagement::LockStoreCommand(void)
{
    __Lock(&m_StoreCommandLock);
}

void ThreadManagement::UnlockStoreCommand(void)
{
    __UnLock(&m_StoreCommandLock);
}

void ThreadManagement::LockRunner(void)
{
    __Lock(&m_RunnerLock);
}

void ThreadManagement::UnlockRunner(void)
{
    __UnLock(&m_RunnerLock);
}

void ThreadManagement::SendAstPendingSignal(void)
{
    __LockSignal(&m_AstPendingLock);
    __Signal(&m_AstPending);
    __UnLockSignal(&m_AstPendingLock);
}

void ThreadManagement::WaitForAstPendingSignal(void)
{
    __LockSignal(&m_AstPendingLock);
    __Wait(&m_AstPending, &m_AstPendingLock);
    __UnLockSignal(&m_AstPendingLock);
}

void ThreadManagement::SendConsoleExecDoneSignal(void)
{
    __LockSignal(&m_ConsoleExecDoneLock);
    __Signal(&m_ConsoleExecDone);
    __UnLockSignal(&m_ConsoleExecDoneLock);
}

void ThreadManagement::WaitForConsoleExecDoneSignal(void)
{
    __LockSignal(&m_ConsoleExecDoneLock);
    __Wait(&m_ConsoleExecDone, &m_ConsoleExecDoneLock);
    __UnLockSignal(&m_ConsoleExecDoneLock);
}

void ThreadManagement::SendAwakeRunnerSignal(void)
{
    __LockSignal(&m_AwakeRunnerLock);
    __Signal(&m_AwakeRunner);
    __UnLockSignal(&m_AwakeRunnerLock);
}

void ThreadManagement::WaitForAwakeRunnerSignal(void)
{
    __LockSignal(&m_AwakeRunnerLock);
    ThreadManagement::UnlockRunner();
    __Wait(&m_AwakeRunner, &m_AwakeRunnerLock);
    __UnLockSignal(&m_AwakeRunnerLock);
}

void ThreadManagement::SendStartPendingSignal(void)
{
    __LockSignal(&m_StartPendingLock);
    __Signal(&m_StartPending);
    __UnLockSignal(&m_StartPendingLock);
}

void ThreadManagement::WaitForStartPendingSignal(void)
{
    __LockSignal(&m_StartPendingLock);
    ThreadManagement::UnlockStart();
    __Wait(&m_StartPending, &m_StartPendingLock);
    __UnLockSignal(&m_StartPendingLock);
}

void ThreadManagement::SendCommandStoredSignal(void)
{
    __LockSignal(&m_CommandStoredLock);
    __Signal(&m_CommandStored);
    __UnLockSignal(&m_CommandStoredLock);
}

void ThreadManagement::WaitForCommandStoredSignal(void)
{
    __LockSignal(&m_CommandStoredLock);
    __Wait(&m_CommandStored, &m_CommandStoredLock);
    __UnLockSignal(&m_CommandStoredLock);
}
