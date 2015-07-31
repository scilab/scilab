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

class EXTERN_AST ThreadManagement
{
private :
    static __threadLock m_RunnerLock;
    static __threadLock m_StartLock;
    static __threadLock m_ParseLock;
    static __threadLock m_StoreCommandLock;
    static __threadLock m_AstLock;

    static __threadSignal m_ConsoleExecDone;
    static __threadSignalLock m_ConsoleExecDoneLock;

    static __threadSignal m_AwakeRunner;
    static __threadSignalLock m_AwakeRunnerLock;

    static __threadSignal m_AstPending;
    static __threadSignalLock m_AstPendingLock;

    static __threadSignal m_StartPending;
    static __threadSignalLock m_StartPendingLock;

    static __threadSignal m_CommandStored;
    static __threadSignalLock m_CommandStoredLock;

    // used to avoid "Spurious Wakeups"
    static bool m_AstPendingWasSignalled;
    static bool m_ConsoleExecDoneWasSignalled;
    static bool m_AwakeRunnerWasSignalled;
    static bool m_StartPendingWasSignalled;
    static bool m_CommandStoredWasSignalled;

public :
    static void initialize(void);

    static void LockStart(void);
    static void UnlockStart(void);
    static void LockParser(void);
    static void UnlockParser(void);
    static void LockStoreCommand(void);
    static void UnlockStoreCommand(void);
    static void LockRunner(void);
    static void UnlockRunner(void);
    static void LockAst(void);
    static void UnlockAst(void);

    static void SendAstPendingSignal(void);
    static void WaitForAstPendingSignal(void);
    static void SendConsoleExecDoneSignal(void);
    static void WaitForConsoleExecDoneSignal(void);
    static void SendAwakeRunnerSignal(void);
    static void WaitForAwakeRunnerSignal(void);
    static void SendStartPendingSignal(void);
    static void WaitForStartPendingSignal(void);
    static void SendCommandStoredSignal(void);
    static void WaitForCommandStoredSignal(void);

};

#endif /* !__THREADMANAGEMENT_HXX__ */
