/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *  Copyright (C) 2014-2015 - Scilab Enterprises - Cedric Delamarre
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#include "runner.hxx"
#include "threadmanagement.hxx"
#include "configvariable.hxx"
#include "debugmanager.hxx"

extern "C"
{
#include "HistoryManager.h"
#include "BrowseVarManager.h"
#include "FileBrowserChDir.h"
#include "scicurdir.h"
#include "Scierror.h"
#include "InitializeJVM.h"
}

std::atomic<Runner*> StaticRunner::m_RunMe(nullptr);
std::atomic<Runner*> StaticRunner::m_CurrentRunner(nullptr);

static bool initialJavaHooks = false;

void StaticRunner::sendExecDoneSignal()
{
    switch (m_CurrentRunner.load()->getCommandOrigin())
    {
        case DEBUGGER :
        {
            ThreadManagement::SendDebuggerExecDoneSignal();
            break;
        }
        case CONSOLE :
        {
            ThreadManagement::SendConsoleExecDoneSignal();
            break;
        }
        case TCLSCI :
        case NONE :
        default : {}
    }
}

int StaticRunner::launch()
{
    //set execution thread in java
    if (!initialJavaHooks && getScilabMode() != SCILAB_NWNI)
    {
        initialJavaHooks = true;
        // Execute the initial hooks registered in Scilab.java
        ExecuteInitialHooks();
    }

    int iRet = 0;

    // save current runner
    Runner* pRunSave = m_CurrentRunner.load();

    // get the runner to execute
    std::unique_ptr<Runner> runMe(getRunner());

    debugger::DebuggerManager* manager = debugger::DebuggerManager::getInstance();

    ConfigVariable::resetExecutionBreak();

    int oldMode = ConfigVariable::getPromptMode();
    symbol::Context* pCtx = symbol::Context::getInstance();
    int scope = pCtx->getScopeLevel();

    // a TCL command display nothing
    int iOldPromptMode = 0;
    if (runMe->getCommandOrigin() == TCLSCI)
    {
        iOldPromptMode = ConfigVariable::getPromptMode();
        ConfigVariable::setPromptMode(-1);
    }

    try
    {
        int level = ConfigVariable::getRecursionLevel();
        try
        {
            runMe->getProgram()->accept(*(runMe->getVisitor()));
        }
        catch (const ast::RecursionException& re)
        {
            // management of pause
            if (ConfigVariable::getPauseLevel())
            {
                ConfigVariable::DecreasePauseLevel();
                throw re;
            }

            //close opened scope during try
            while (pCtx->getScopeLevel() > scope)
            {
                pCtx->scope_end();
            }

            //decrease recursion to init value and close where
            while (ConfigVariable::getRecursionLevel() > level)
            {
                ConfigVariable::where_end();
                ConfigVariable::decreaseRecursion();
            }

            ConfigVariable::resetWhereError();
            ConfigVariable::setPromptMode(oldMode);

            //print msg about recursion limit and trigger an error
            wchar_t sz[1024];
            os_swprintf(sz, 1024, _W("Recursion limit reached (%d).\n").data(), ConfigVariable::getRecursionLimit());
            throw ast::InternalError(sz);
        }
    }
    catch (const ast::InternalError& se)
    {
        if (runMe->getCommandOrigin() == TCLSCI)
        {
            ConfigVariable::setPromptMode(iOldPromptMode);
        }

        std::wostringstream ostr;
        ConfigVariable::whereErrorToString(ostr);
        scilabErrorW(ostr.str().c_str());
        scilabErrorW(se.GetErrorMessage().c_str());
        ConfigVariable::resetWhereError();
        iRet = 1;
    }
    catch (const ast::InternalAbort& ia)
    {
        if (runMe->getCommandOrigin() == TCLSCI)
        {
            ConfigVariable::setPromptMode(iOldPromptMode);
        }

        // management of pause
        if (ConfigVariable::getPauseLevel())
        {
            ConfigVariable::DecreasePauseLevel();
            // set back the runner wich have been overwritten in StaticRunner::getRunner
            m_CurrentRunner.store(pRunSave);
            throw ia;
        }

        // close all scope before return to console scope
        symbol::Context* pCtx = symbol::Context::getInstance();
        while (pCtx->getScopeLevel() > scope)
        {
            pCtx->scope_end();
        }

        // debugger leave with abort state
        manager->setAborted();

        // send the good signal about the end of execution
        sendExecDoneSignal();

        // set back the runner wich have been overwritten in StaticRunner::getRunner
        m_CurrentRunner.store(pRunSave);
        throw ia;
    }

    if (runMe->getCommandOrigin() == TCLSCI)
    {
        ConfigVariable::setPromptMode(iOldPromptMode);
    }

    if (getScilabMode() != SCILAB_NWNI && getScilabMode() != SCILAB_API)
    {
        char *cwd = NULL;
        int err = 0;

        UpdateBrowseVar();
        saveScilabHistoryToFile();
        cwd = scigetcwd(&err);
        if (cwd)
        {
            FileBrowserChDir(cwd);
            FREE(cwd);
        }
    }

    // reset error state when new prompt occurs
    ConfigVariable::resetError();

    // send the good signal about the end of execution
    sendExecDoneSignal();

    //clean debugger step flag if debugger is not interrupted ( end of debug )
    manager->resetStep();

    // set back the runner wich have been overwritten in StaticRunner::getRunner
    m_CurrentRunner.store(pRunSave);

    return iRet;
}

void StaticRunner::setRunner(Runner* _RunMe)
{
    m_RunMe = _RunMe;
}

Runner* StaticRunner::getRunner(void)
{
    m_CurrentRunner.store(m_RunMe.exchange(nullptr));
    ThreadManagement::SendAvailableRunnerSignal();
    return m_CurrentRunner.load();
}

// return true if a Runner is already set in m_RunMe.
bool StaticRunner::isRunnerAvailable(void)
{
    return m_RunMe.load() != nullptr;
}

bool StaticRunner::isInterruptibleCommand()
{
    return m_CurrentRunner.load()->isInterruptible();
}

command_origin_t StaticRunner::getCommandOrigin()
{
    return m_RunMe.load()->getCommandOrigin();
}

void StaticRunner::setCommandOrigin(command_origin_t _origin)
{
    m_CurrentRunner.load()->setCommandOrigin(_origin);
}

void StaticRunner::execAndWait(ast::Exp* _theProgram, ast::RunVisitor *_visitor,
                               bool /*_isPrioritaryThread*/, bool _isInterruptible, command_origin_t _iCommandOrigin)
{
    if (isRunnerAvailable())
    {
        // wait for managenement of last Runner
        ThreadManagement::WaitForAvailableRunnerSignal();
    }

    // lock runner to be sure we are waiting for
    // "AwakeRunner" signal before start execution
    ThreadManagement::LockRunner();
    Runner *runMe = new Runner(_theProgram, _visitor, _iCommandOrigin, _isInterruptible);
    setRunner(runMe);

    ThreadManagement::SendRunMeSignal();
    ThreadManagement::WaitForAwakeRunnerSignal();
}

bool StaticRunner::exec(ast::Exp* _theProgram, ast::RunVisitor *_visitor)
{
    Runner *runMe = new Runner(_theProgram, _visitor);
    setRunner(runMe);

    try
    {
        launch();
    }
    catch (const ast::InternalAbort& /*ia*/)
    {
        //catch exit command in .start or .quit
        return false;
    }

    return true;
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

command_origin_t StaticRunner_getCommandOrigin(void)
{
    return StaticRunner::getCommandOrigin();
}

void StaticRunner_setCommandOrigin(command_origin_t _origin)
{
    StaticRunner::setCommandOrigin(_origin);
}
