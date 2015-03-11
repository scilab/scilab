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

__threadLock Runner::m_lock;

__threadSignal Runner::m_consoleExecDone;

__threadSignal Runner::m_awakeScilab;
__threadSignalLock Runner::m_awakeScilabLock;

__threadSignal Runner::m_AstPending;
__threadSignalLock Runner::m_AstPendingLock;


using namespace ast;

__threadSignal* getAstPendingSignal(void)
{
    return Runner::getAstPendingSignal();
}

__threadSignal* getConsoleExecDone(void)
{
    return Runner::getConsoleExecDone();
}


void Runner::init()
{
    __InitSignal(&m_awakeScilab);
    __InitSignalLock(&m_awakeScilabLock);

    __InitSignal(&m_consoleExecDone);

    __InitSignal(&m_AstPending);
    __InitSignalLock(&m_AstPendingLock);
}

void *Runner::launch(void *args)
{
    bool bdoUnlock = false;
    //try to lock locker ( waiting parent thread register me )
    __Lock(&m_lock);
    //just release locker
    __UnLock(&m_lock);

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
        __Signal(&Runner::m_AstPending);
    }

    if (pThread->isConsoleCommand())
    {
        __Signal(&Runner::m_consoleExecDone);
    }

    //unregister thread
    ConfigVariable::deleteThread(currentThreadKey);

    delete me;

    if (bdoUnlock)
    {
        UnlockPrompt();
    }

    return NULL;
}

void Runner::LockPrompt()
{
    __LockSignal(&m_awakeScilabLock);
    //free locker to release thread
    __UnLock(&m_lock);
    __Wait(&m_awakeScilab, &m_awakeScilabLock);
    __UnLockSignal(&m_awakeScilabLock);
}

void Runner::UnlockPrompt()
{
    __LockSignal(&m_awakeScilabLock);
    __Signal(&m_awakeScilab);
    __UnLockSignal(&m_awakeScilabLock);
}


void Runner::execAndWait(ast::Exp* _theProgram, ast::ExecVisitor *_visitor,
                         bool _isPrioritaryThread, bool _isInterruptibleThread, bool _isConsoleCommand)
{
    try
    {
        Runner *runMe = new Runner(_theProgram, _visitor);
        __threadKey threadKey;
        __threadId threadId;

        //init locker
        __InitLock(&m_lock);
        //lock locker
        __Lock(&m_lock);

        types::ThreadId* pInterruptibleThread = ConfigVariable::getLastRunningThread();
        if (_isPrioritaryThread)
        {
            if (pInterruptibleThread)
            {
                if (pInterruptibleThread->isInterruptible())
                {
                    pInterruptibleThread->setInterrupt(true);
                    __LockSignal(&m_AstPendingLock);
                    __Wait(&m_AstPending, &m_AstPendingLock);
                    __UnLockSignal(&m_AstPendingLock);
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
        LockPrompt();

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
