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

#ifndef __RUNNER_HXX__
#define __RUNNER_HXX__

#include <iostream>

extern "C"
{
#include "Thread_Wrapper.h"
}

#include "exp.hxx"
#include "execvisitor.hxx"
#include "threadId.hxx"

class Runner
{
public :
    Runner() { }
    ~Runner() { }

public :
    void execAndWait(ast::Exp* _theProgram, ast::ExecVisitor *_visitor)
    {
        try
        {
            m_theProgram = _theProgram;
            m_visitor = _visitor;
            __threadKey key;

            //init locker
            __InitLock(&m_lock);
            //lock locker
            __Lock(&m_lock);
            //launch thread but is can't really start since locker is locked
            __CreateThreadWithParams(&m_threadId, &key, &Runner::launch, this);
            //register thread
            ConfigVariable::setThread(key, new ThreadId(m_threadId));
            //free locker to release thread
            __UnLock(&m_lock);
            //wait and of thread execution
            __WaitThreadDie(m_threadId);
            //change thread status
            ConfigVariable::getThread(key)->setStatus(ThreadId::Done);
            //unregister thread
            ConfigVariable::deleteThread(key);
        }
        catch(ScilabException se)
        {
            throw se;
        }
    }

    void exec(ast::Exp* _theProgram, ast::ExecVisitor *_visitor)
    {
        m_theProgram = _theProgram;
        m_visitor = _visitor;
        __threadKey key;
        __CreateThreadWithParams(&m_threadId, &key, &Runner::launch, this);
    }

    ast::ExecVisitor *getVisitor()
    {
        return m_visitor;
    }

    ast::Exp* getProgram()
    {
        return m_theProgram;
    }

    __threadId getThreadId(void)
    {
        return m_threadId;
    }

private :
    static void *launch(void *args)
    {
        //try to lock locker ( waiting parent thread register me )
        __Lock(&m_lock);
        //just release locker
        __UnLock(&m_lock);

        //exec !
        Runner *me = (Runner *)args;
        try
        {
            me->getProgram()->accept(*(me->getVisitor()));
            ConfigVariable::clearLastError();
        }
        catch(ScilabException se)
        {
            YaspWriteW(se.GetErrorMessage().c_str());
        }
        return NULL;
    }

private :
    __threadId m_threadId;

public :
    ast::Exp*           m_theProgram;
    ast::ExecVisitor*   m_visitor;
    static __threadLock m_lock;

};
#endif /* !__RUNNER_HXX__ */
