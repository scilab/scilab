/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *  Copyright (C) 2014-2015 - Scilab Enterprises - Cedric Delamarre
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

std::atomic<Runner*> StaticRunner::m_RunMe(nullptr);
std::atomic<bool> StaticRunner::m_bInterruptibleCommand(true);

void StaticRunner::launch()
{
    // get the runner to execute
    Runner* runMe = getRunner();
    // set if the current comment is interruptible
    setInterruptibleCommand(runMe->isInterruptible());

    try
    {
        runMe->getProgram()->accept(*(runMe->getVisitor()));
    }
    catch (const ast::InternalError& se)
    {
        scilabErrorW(se.GetErrorMessage().c_str());
        scilabErrorW(L"\n");
        std::wostringstream ostr;
        ConfigVariable::whereErrorToString(ostr);
        scilabErrorW(ostr.str().c_str());
        ConfigVariable::resetWhereError();
    }
    catch (const ast::InternalAbort& ia)
    {
        // management of pause
        if (ConfigVariable::getPauseLevel())
        {
            ConfigVariable::DecreasePauseLevel();
            delete runMe;
            throw ia;
        }

        // close all scope before return to console scope
        symbol::Context* pCtx = symbol::Context::getInstance();
        while (pCtx->getScopeLevel() != SCOPE_CONSOLE)
        {
            pCtx->scope_end();
        }

        if (runMe->isConsoleCommand())
        {
            ThreadManagement::SendConsoleExecDoneSignal();
        }

        delete runMe;
        throw ia;
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

    if (runMe->isConsoleCommand())
    {
        ThreadManagement::SendConsoleExecDoneSignal();
    }

    delete runMe;
}

void StaticRunner::setRunner(Runner* _RunMe)
{
    m_RunMe = _RunMe;
}

Runner* StaticRunner::getRunner(void)
{
    Runner* tmp = m_RunMe.exchange(nullptr);
    ThreadManagement::SendAvailableRunnerSignal();
    return tmp;
}

// return true if a Runner is already set in m_RunMe.
bool StaticRunner::isRunnerAvailable(void)
{
    return m_RunMe.load() != nullptr;
}

void StaticRunner::setInterruptibleCommand(bool _bInterruptibleCommand)
{
    m_bInterruptibleCommand = _bInterruptibleCommand;
}

bool StaticRunner::isInterruptibleCommand()
{
    return m_bInterruptibleCommand;
}

void StaticRunner::execAndWait(ast::Exp* _theProgram, ast::ExecVisitor *_visitor,
                               bool _isPrioritaryThread, bool _isInterruptible, bool _isConsoleCommand)
{
    if (isRunnerAvailable())
    {
        // wait for managenement of last Runner
        ThreadManagement::WaitForAvailableRunnerSignal();
    }

    // lock runner to be sure we are waiting for
    // "AwakeRunner" signal before start execution
    ThreadManagement::LockRunner();
    Runner *runMe = new Runner(_theProgram, _visitor, _isConsoleCommand, _isInterruptible);
    setRunner(runMe);

    ThreadManagement::SendRunMeSignal();
    ThreadManagement::WaitForAwakeRunnerSignal();
}

void StaticRunner::exec(ast::Exp* _theProgram, ast::ExecVisitor *_visitor)
{
    Runner *runMe = new Runner(_theProgram, _visitor);
    setRunner(runMe);
    launch();
}

void StaticRunner_launch(void)
{
    StaticRunner::launch();
}

int StaticRunner_isRunnerAvailable(void)
{
    return StaticRunner::isRunnerAvailable() ? 1 : 0;
}

int StaticRunner_isInterruptibleCommand(void)
{
    return StaticRunner::isInterruptibleCommand() ? 1 : 0;
}

void StaticRunner_setInterruptibleCommand(int val)
{
    StaticRunner::setInterruptibleCommand(val == 1);
}
