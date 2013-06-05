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

#ifndef __JITTER_HXX__
#define __JITTER_HXX__

#include <iostream>
#include "exp.hxx"
#include "execvisitor.hxx"
#include "JITvisitor.hxx"

extern "C"
{
#include "Thread_Wrapper.h"
#include "dynlib_core_gw.h"
}

#include "threadId.hxx"

class CORE_GW_IMPEXP Jitter
{
private :
    Jitter(ast::Exp* _theProgram, ast::JITVisitor *_visitor)
    {
        m_theProgram = _theProgram;
        m_visitor = _visitor;
    }
    ~Jitter()
    {
        delete m_theProgram;
        delete m_visitor;
    }

public :

    static void init();

    static void execAndWait(ast::Exp* _theProgram, ast::JITVisitor *_visitor);

    void exec(ast::Exp* _theProgram, ast::JITVisitor *_visitor);

    ast::JITVisitor *getVisitor()
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
    ast::JITVisitor*   m_visitor;

private :
    static __threadSignal m_awakeScilab;
    static __threadSignalLock m_awakeScilabLock;
    static __threadLock m_lock;
};
#endif /* !__RUNNER_HXX__ */
