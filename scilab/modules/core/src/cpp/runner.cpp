/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "runner.hxx"
#include "threadmanagement.hxx"
#include "configvariable.hxx"

extern "C"
{
#include "BrowseVarManager.h"
#include "FileBrowserChDir.h"
#include "scicurdir.h"
}

using namespace ast;

void *Runner::launch(void *args)
{
    bool bdoUnlock = false;
    //try to lock locker ( waiting parent thread register me )
    ThreadManagement::LockRunner();
    //just release locker
    ThreadManagement::UnlockRunner();

    __threadKey currentThreadKey = __GetCurrentThreadKey();
    ThreadId* pThread = ConfigVariable::getThread(currentThreadKey);

    //exec !
    Runner *me = (Runner *)args;

    try
    {
        me->getProgram()->accept(*(me->getVisitor()));
        //ConfigVariable::clearLastError();
    }
    catch (const ast::ScilabException& se)
    {
        scilabErrorW(se.GetErrorMessage().c_str());
        scilabErrorW(L"\n");
        std::wostringstream ostr;
        ConfigVariable::whereErrorToString(ostr);
        scilabErrorW(ostr.str().c_str());
        ConfigVariable::resetWhereError();
    }

    if (getScilabMode() != SCILAB_NWNI && getScilabMode() != SCILAB_API)
    {
        char *cwd = NULL;
        int err = 0;

        UpdateBrowseVar();
        cwd = scigetcwd(&err);
        if (cwd)
        {
            FileBrowserChDir(cwd);
            FREE(cwd);
        }
    }

    // reset error state when new prompt occurs
    ConfigVariable::resetError();

    //change thread status
    if (pThread->getStatus() != ThreadId::Aborted)
    {
        pThread->setStatus(ThreadId::Done);
        bdoUnlock = true;
    }

    if (pThread->getInterrupt()) // non-prioritary
    {
        // Unlock prioritary thread waiting for
        // non-prioritary thread end this "SeqExp" execution.
        // This case appear when error is throw or when
        // non-prioritary execute this last SeqExp.
        pThread->setInterrupt(false);
        ThreadManagement::SendAstPendingSignal();
    }

    if (pThread->isConsoleCommand())
    {
        ThreadManagement::SendConsoleExecDoneSignal();
    }

    //unregister thread
    ConfigVariable::deleteThread(currentThreadKey);

    delete me;

    if (bdoUnlock)
    {
        ThreadManagement::SendAwakeRunnerSignal();
    }

    return NULL;
}

void Runner::execAndWait(ast::Exp* _theProgram, ast::ExecVisitor *_visitor,
                         bool _isPrioritaryThread, bool _isInterruptibleThread, bool _isConsoleCommand)
{
    try
    {
        Runner *runMe = new Runner(_theProgram, _visitor);
        __threadKey threadKey;
        __threadId threadId;

        types::ThreadId* pInterruptibleThread = ConfigVariable::getLastRunningThread();
        if (_isPrioritaryThread)
        {
            if (pInterruptibleThread)
            {
                if (pInterruptibleThread->isInterruptible())
                {
                    pInterruptibleThread->setInterrupt(true);
                    ThreadManagement::WaitForAstPendingSignal();
                }
                else
                {
                    __WaitThreadDie(pInterruptibleThread->getThreadId());
                    pInterruptibleThread = NULL;
                }
            }
        }
        else if (pInterruptibleThread)
        {
            __WaitThreadDie(pInterruptibleThread->getThreadId());
            pInterruptibleThread = NULL;
        }

        // Lock the thread "Runner::launch" to be sure that all
        // stuff performed before the "WaitForAwakeRunnerSignal"
        // are done.
        ThreadManagement::LockRunner();

        //launch thread but is can't really start since locker is locked
        __CreateThreadWithParams(&threadId, &threadKey, &Runner::launch, runMe);
        runMe->setThreadId(threadId);
        runMe->setThreadKey(threadKey);

        //register thread
        types::ThreadId* pThread = new ThreadId(threadId, threadKey);
        ConfigVariable::addThread(pThread);
        pThread->setConsoleCommandFlag(_isConsoleCommand);
        pThread->setInterruptible(_isInterruptibleThread);

        //free locker to release thread && wait and of thread execution
        ThreadManagement::WaitForAwakeRunnerSignal();

        if (pInterruptibleThread && pInterruptibleThread->getInterrupt())
        {
            pInterruptibleThread->setInterrupt(false);
            pInterruptibleThread->resume();
        }

        types::ThreadId* pExecThread = ConfigVariable::getThread(threadKey);
        if (pExecThread == NULL)
        {
            //call pthread_join to clean stack allocation
            __WaitThreadDie(threadId);
        }
    }
    catch (const ast::ScilabException& se)
    {
        throw se;
    }
}

void Runner::exec(ast::Exp* _theProgram, ast::ExecVisitor *_visitor)
{
    m_theProgram = _theProgram;
    m_visitor = _visitor;
    __CreateThreadWithParams(&m_threadId, &m_threadKey, &Runner::launch, this);
}
