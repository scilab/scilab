/*
 *  Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
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

#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif

#include "runner.hxx"
#include "runtimeaccess.hxx"
#include "timer.hxx"
#include "threadmanagement.hxx"
#include "configvariable.hxx"
#include "debugmanager.hxx"
#include "printvisitor.hxx"
#include "execvisitor.hxx"
#include "prettyprintvisitor.hxx"
#include "debuggervisitor.hxx"
#include "visitor_common.hxx"

extern "C"
{
#include "HistoryManager.h"
#include "BrowseVarManager.h"
#include "FileBrowserChDir.h"
#include "scicurdir.h"
#include "Scierror.h"
#include "Sciwarning.h"
#include "InitializeJVM.h"
}

Runner* StaticRunner::m_CurrentRunner = nullptr;
bool StaticRunner::m_bDumpStack = false;
bool StaticRunner::m_bExecAst = true;
bool StaticRunner::m_bDumpAst = false;
bool StaticRunner::m_bPrintAst = false;

static Timer _timer;

int StaticRunner::launch()
{
    int iRet = 0;

    // Wait for a new command only if the command queue is empty
    ThreadManagement::WaitForCommandStoredSignal();

    Runner* runMe = (Runner*)GetCommand();
    RuntimeExecutionGuard runtimeGuard;
    processRunner(runMe);

    // save current runner
    Runner* pRunSave = m_CurrentRunner;
    m_CurrentRunner = runMe;

    debugger::DebuggerManager* manager = debugger::DebuggerManager::getInstance();
    manager->resetAborted();

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

    int iPauseLvl = ConfigVariable::getPauseLevel();

    // Debugged when stored as such, or as what it interrupts: an execution,
    // else Scilab itself. Never in a stop of the debugger or a normal pause.
    const bool inDebugRun = manager->isDebugging();
    const bool inUserPause = inDebugRun == false && iPauseLvl > 0;
    const bool surroundingMode = pRunSave ? manager->isDebugExecution() : manager->isDebugMode();
    bool debugged = false;
    if (manager->isInterrupted() == false && inUserPause == false
        && runMe->getCommandOrigin() != INTERNAL && runMe->getCommandMode() != MODE_NORMAL)
    {
        debugged = runMe->getCommandMode() == MODE_DEBUG || surroundingMode;
    }

    runMe->setPauseLevel(iPauseLvl);
    runMe->setCallDepth((int)ConfigVariable::getWhere().size());

    // Debuggers are told of an execution from a prompt to the next one, not
    // of a command run inside another; a debug run tells them itself.
    const bool fromPrompt = pRunSave == nullptr || iPauseLvl > pRunSave->getPauseLevel();
    const bool reported = fromPrompt && inDebugRun == false;
    if (reported)
    {
        manager->sendExecution();
    }

    debugger::ExecutionModeGuard modeGuard(debugged);

    try
    {
        int level = ConfigVariable::getRecursionLevel();
        std::unique_ptr<ast::ConstVisitor> exec(ConfigVariable::getDefaultVisitor());
        try
        {
            runMe->getProgram()->accept(*exec);
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
            // Release the console to display the prompt after aborting a callback execution
            sendExecDoneSignal();
            delete m_CurrentRunner;
            // set back the runner
            m_CurrentRunner = pRunSave;

            // dumping stack after execution
            if (m_bDumpStack)
            {
                dumpStackTask();
            }

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

        if (reported)
        {
            // a pause asked too late must not stop the next run
            manager->resetPauseRequest();

            // send information about execution done to debuggers
            manager->sendExecutionReleased();
        }

        delete m_CurrentRunner;

        // set back the runner
        m_CurrentRunner = pRunSave;

        // dumping stack after execution
        if (m_bDumpStack)
        {
            dumpStackTask();
        }
        throw ia;
    }

    if (runMe->getCommandOrigin() == TCLSCI)
    {
        ConfigVariable::setPromptMode(iOldPromptMode);
    }

    if (getScilabMode() != SCILAB_NWNI)
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

    // resume will make the execution continue
    // even if resume is a console command, it must not release the prompt
    // because the prompt will be released at the end of the original console command
    // but it must be released if the original command is a callback.
    if(iPauseLvl == ConfigVariable::getPauseLevel() || (pRunSave && pRunSave->getCommandOrigin() != CONSOLE))
    {
        // send the good signal about the end of execution
        sendExecDoneSignal();
    }

    if (reported)
    {
        // The prompt is not back yet when a command is waiting behind this
        // one, nor when this one resumed a paused execution.
        if (iPauseLvl == ConfigVariable::getPauseLevel() && isEmptyCommandQueue())
        {
            // send information about execution done to debuggers
            manager->sendExecutionReleased();
        }

        //clean debugger step flag if debugger is not interrupted ( end of debug )
        manager->resetStep();

        // a pause asked too late must not stop the next run
        manager->resetPauseRequest();
    }

    delete m_CurrentRunner;

    // set back the runner
    m_CurrentRunner = pRunSave;

    // dumping stack after execution
    if (m_bDumpStack)
    {
        dumpStackTask();
    }

    return iRet;
}

void StaticRunner::sendExecDoneSignal()
{
    switch (m_CurrentRunner->getCommandOrigin())
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

// return true if a command is running or paused.
bool StaticRunner::isRunning(void)
{
    return m_CurrentRunner != nullptr;
}

bool StaticRunner::isInterruptibleCommand()
{
    return m_CurrentRunner->isInterruptible();
}

bool StaticRunner::isCalledFromConsoleLine()
{
    // the call stack holds this call only, above what the line interrupted
    return m_CurrentRunner && m_CurrentRunner->getCommandOrigin() == CONSOLE
        && (int)ConfigVariable::getWhere().size() <= m_CurrentRunner->getCallDepth() + 1;
}

void StaticRunner::setDumpStack(bool _bValue)
{
    m_bDumpStack = _bValue;
}

void StaticRunner::setExecAst(bool _bValue)
{
    m_bExecAst = _bValue;
}

void StaticRunner::setDumpAst(bool _bValue)
{
    m_bDumpAst = _bValue;
}

bool StaticRunner::getDumpAst()
{
    return m_bDumpAst;
}

void StaticRunner::setPrintAst(bool _bValue)
{
    m_bPrintAst = _bValue;
}

bool StaticRunner::getPrintAst()
{
    return m_bPrintAst;
}

bool StaticRunner::execCommand(const std::string& _stCMD)
{
    if(StoreCommand(_stCMD.data()))
    {
        return false;
    }

    try
    {
        launch();
    }
    catch (const ast::InternalAbort& /*ia*/)
    {
        // catch exit command in .start or .quit
        return true;
    }
    catch (const ast::RecursionException& /*re*/)
    {
        return true;
    }

    return false;
}

void StaticRunner::processRunner(Runner* _runner)
{
    ast::Exp* tree = _runner->getProgram();

    // dumping tree
    if (m_bDumpAst)
    {
        dumpAstTask(tree);
    }

    // pretty print tree
    if (m_bPrintAst)
    {
        printAstTask(tree);
    }

    // executing tree
    if (m_bExecAst)
    {
        if (ConfigVariable::getSerialize())
        {
            ast::Exp* newTree = NULL;
            if (ConfigVariable::getTimed())
            {
                newTree = callTyper(tree, L"tasks");
            }
            else
            {
                newTree = callTyper(tree);
            }

            delete tree;
            tree = newTree;
            _runner->setProgram(tree);
        }
    }
}

void StaticRunner::dumpStackTask()
{
    bool timed = ConfigVariable::getTimed();
    if (timed)
    {
        _timer.start();
    }

    symbol::Context::getInstance()->print(std::wcout);

    if (timed)
    {
        _timer.check(L"Dumping Stack");
    }
}

void StaticRunner::printAstTask(ast::Exp *tree)
{
    if(ConfigVariable::getStartProcessing() || ConfigVariable::getEndProcessing())
    {
        return;
    }

    bool timed = ConfigVariable::getTimed();
    if (timed)
    {
        _timer.start();
    }

    if (tree)
    {
#ifdef _WIN32
        int iOldMode = _setmode(_fileno(stdout), _O_U8TEXT);
#endif
        ast::PrintVisitor printMe (std::wcout);
        tree->accept(printMe);
#ifdef _WIN32
        _setmode(_fileno(stdout), iOldMode);
#endif
    }

    if (timed)
    {
        _timer.check(L"Pretty Print");
    }
}

void StaticRunner::dumpAstTask(ast::Exp *tree)
{
    if(ConfigVariable::getStartProcessing() || ConfigVariable::getEndProcessing())
    {
        return;
    }

    bool timed = ConfigVariable::getTimed();
    if (timed)
    {
        _timer.start();
    }

    ast::PrettyPrintVisitor debugMe;
    if (tree)
    {
        tree->accept(debugMe);
    }

    if (timed)
    {
        _timer.check(L"AST Dump");
    }
}

void StaticRunner_launch(void)
{
    StaticRunner::launch();
}

int StaticRunner_isRunning(void)
{
    return StaticRunner::isRunning() ? 1 : 0;
}

int StaticRunner_isInterruptibleCommand(void)
{
    return StaticRunner::isInterruptibleCommand() ? 1 : 0;
}

int StaticRunner_execCommand(const char* cmd)
{
    return StaticRunner::execCommand(cmd) ? 1 : 0;
}
