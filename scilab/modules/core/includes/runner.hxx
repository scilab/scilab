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

#include "Thread_Wrapper.h"

#include "exp.hxx"
#include "execvisitor.hxx"

class Runner
{
public :
    Runner() { }
    ~Runner() { }

public :
    void execAndWait(ast::Exp* _theProgram, ast::ExecVisitor *_visitor)
    {
        m_theProgram = _theProgram;
        m_visitor = _visitor;

#ifdef _MSC_VER
        //It seems libxml crash with multithread under Windows
        Runner::launch(this);
#else
        __CreateThreadWithParams(&m_threadId, &Runner::launch, this);
        __WaitThreadDie(m_threadId);
#endif
    }

    void exec(ast::Exp* _theProgram, ast::ExecVisitor *_visitor)
    {
        m_theProgram = _theProgram;
        m_visitor = _visitor;
        __CreateThreadWithParams(&m_threadId, &Runner::launch, this);
    }

    ast::ExecVisitor *getVisitor()
    {
        return m_visitor;
    }

    ast::Exp* getProgram()
    {
        return m_theProgram;
    }

private :
    static void *launch(void *args)
    {
        try
        {
            //std::cerr << "-*- static void * Runner::launch() THREAD START -*-" << std::endl;
            Runner *me = (Runner *)args;
            me->getProgram()->accept(*(me->getVisitor()));
            //std::cerr << "-*- static void * Runner::launch() THREAD END -*-" << std::endl;
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

};
#endif /* !__RUNNER_HXX__ */
