/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *  Copyright (C) 2014-2015 - Scilab Enterprises - Cedric Delamarre
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

#include <atomic>
#include <memory>

#include "exp.hxx"
#include "runvisitor.hxx"

extern "C"
{
#include "dynlib_core.h"
}

class CORE_IMPEXP Runner
{
public :
    Runner(ast::Exp* _theProgram, ast::RunVisitor *_visitor) : m_theProgram(_theProgram), m_visitor(_visitor), m_isConsoleCommand(false), m_isInterruptible(true)
    { }

    Runner(ast::Exp* _theProgram, ast::RunVisitor *_visitor, bool _isConsoleCommand, bool _isInterruptible) : m_theProgram(_theProgram), m_visitor(_visitor), m_isConsoleCommand(_isConsoleCommand), m_isInterruptible(_isInterruptible)
    { }

    ~Runner()
    {
        delete m_theProgram;
    }

    ast::RunVisitor *getVisitor()
    {
        return m_visitor.get();
    }

    ast::Exp* getProgram()
    {
        return m_theProgram;
    }

    bool isConsoleCommand()
    {
        return m_isConsoleCommand;
    }

    bool isInterruptible()
    {
        return m_isInterruptible;
    }

private :
    ast::Exp* m_theProgram;
    std::unique_ptr<ast::RunVisitor> m_visitor;
    bool m_isConsoleCommand;
    bool m_isInterruptible;

    // static members to manage execution
};

class StaticRunner
{
public:
    static void launch(void);
    static void setRunner(Runner* _RunMe);
    static Runner* getRunner(void);
    static bool isRunnerAvailable(void);
    static bool isInterruptibleCommand(void);
    static void setInterruptibleCommand(bool _isInterruptible);
    static void execAndWait(ast::Exp* _theProgram, ast::RunVisitor *_visitor,
                            bool _isInterruptible, bool _isPrioritary, bool _isConsoleCommand);
    static void exec(ast::Exp* _theProgram, ast::RunVisitor *_visitor);

private:
    static std::atomic<Runner*> m_RunMe;
    static std::atomic<bool> m_bInterruptibleCommand;
};

extern "C"
{
    void StaticRunner_launch(void);
    int StaticRunner_isRunnerAvailable(void);
    int StaticRunner_isInterruptibleCommand(void);
    void StaticRunner_setInterruptibleCommand(int val);
}

#endif /* !__RUNNER_HXX__ */
