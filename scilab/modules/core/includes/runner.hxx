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
#include "exp.hxx"
#include "execvisitor.hxx"

extern "C"
{
#include "Thread_Wrapper.h"
#include "dynlib_core.h"
}

#include "threadId.hxx"

class CORE_IMPEXP Runner
{
private :
    Runner(ast::Exp* _theProgram, ast::ExecVisitor *_visitor)
    {
        m_theProgram = _theProgram;
        m_visitor = _visitor;
    }
    ~Runner()
    {
        delete m_theProgram;
        delete m_visitor;
    }

public :

    static void init();

    static void execAndWait(ast::Exp* _theProgram, ast::ExecVisitor *_visitor);

    void exec(ast::Exp* _theProgram, ast::ExecVisitor *_visitor);

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

    void setThreadId(__threadId _threadId)
    {
        m_threadId = _threadId;
    }

    __threadKey getThreadKey(void)
    {
        return m_threadKey;
    }

    void setThreadKey(__threadKey _threadId)
    {
        m_threadKey = _threadId;
    }

    static void UnlockPrompt();

    static void LockPrompt();

private :
    static void *launch(void *args);

private :
    __threadKey m_threadKey;
    __threadId m_threadId;
    ast::Exp*           m_theProgram;
    ast::ExecVisitor*   m_visitor;

private :
    static __threadSignal m_awakeScilab;
    static __threadSignalLock m_awakeScilabLock;
    static __threadLock m_lock;
};
#endif /* !__RUNNER_HXX__ */
