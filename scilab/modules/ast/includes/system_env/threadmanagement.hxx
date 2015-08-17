/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2015 - Scilab-Enterprises - Cedric Delamarre
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __THREADMANAGEMENT_HXX__
#define __THREADMANAGEMENT_HXX__

extern "C"
{
#include "Thread_Wrapper.h"
#include "dynamiclibrary.h"
#include "dynlib_ast.h"
}

//#define DEBUG_THREAD

class EXTERN_AST ThreadManagement
{
private :
    static __threadLock m_RunnerLock;
    static __threadLock m_ParseLock;
    static __threadLock m_StoreCommandLock;
    static __threadLock m_ScilabReadLock;

    static __threadSignal m_ConsoleExecDone;
    static __threadSignalLock m_ConsoleExecDoneLock;

    static __threadSignal m_AwakeRunner;
    static __threadSignalLock m_AwakeRunnerLock;

    static __threadSignal m_AvailableRunner;
    static __threadSignalLock m_AvailableRunnerLock;

    static __threadSignal m_StartPending;
    static __threadSignalLock m_StartPendingLock;

    static __threadSignal m_CommandStored;
    static __threadSignalLock m_CommandStoredLock;

    static __threadSignal m_RunMe;
    static __threadSignalLock m_RunMeLock;

    // used to avoid "Spurious Wakeups"
    static bool m_AvailableRunnerWasSignalled;
    static bool m_ConsoleExecDoneWasSignalled;
    static bool m_AwakeRunnerWasSignalled;
    static bool m_StartPendingWasSignalled;
    static bool m_CommandStoredWasSignalled;
    static bool m_RunMeWasSignalled;

#ifdef DEBUG_THREAD
    static __threadKey m_tkMain;
    static __threadKey m_tkReadAndExec;
    static __threadKey m_tkConsole;
#endif // DEBUG_THREAD

public :
    static void initialize(void);

    static void LockParser(void);
    static void UnlockParser(void);
    static void LockStoreCommand(void);
    static void UnlockStoreCommand(void);
    static void LockRunner(void);
    static void UnlockRunner(void);
    static void LockScilabRead(void);
    static void UnlockScilabRead(void);

    static void SendAvailableRunnerSignal(void);
    static void WaitForAvailableRunnerSignal(void);
    static void SendConsoleExecDoneSignal(void);
    static void WaitForConsoleExecDoneSignal(void);
    static void SendAwakeRunnerSignal(void);
    static void WaitForAwakeRunnerSignal(void);
    static void SendStartPendingSignal(void);
    static void WaitForStartPendingSignal(void);
    static void SendCommandStoredSignal(void);
    static void WaitForCommandStoredSignal(void);
    static void SendRunMeSignal(void);
    static void WaitForRunMeSignal(void);

#ifdef DEBUG_THREAD
    static void SetThreadKey(__threadKey tkMain, __threadKey tkReadAndExec, __threadKey tkConsole);
    static void PrintDebugHead();
private :
    static void PrintDebug(const char* pcfunName);
#endif // DEBUG_THREAD


};

#endif /* !__THREADMANAGEMENT_HXX__ */
