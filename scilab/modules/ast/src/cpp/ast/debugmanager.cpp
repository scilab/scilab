/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
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

#include <memory>

#include "debugmanager.hxx"
#include "configvariable.hxx"
#include "threadmanagement.hxx"
#include "runner.hxx"

#include "threadId.hxx"

extern "C"
{
#include "Thread_Wrapper.h"
}

namespace debugger
{
std::unique_ptr<DebuggerMagager> DebuggerMagager::me(nullptr);

//singleton
DebuggerMagager* DebuggerMagager::getInstance()
{
    if (me.get() == nullptr)
    {
        me.reset(new DebuggerMagager());
    }

    return me.get();
}

void DebuggerMagager::addDebugger(AbstractDebugger* _debug)
{
    debuggers.push_back(_debug);
}

void DebuggerMagager::removeDebugger(int _iDebugger)
{
    if (_iDebugger >= 0 && _iDebugger <= debuggers.size())
    {
        debuggers.erase(debuggers.begin() + _iDebugger);
    }
}

AbstractDebugger* DebuggerMagager::getDebugger(int _iDebugger)
{
    if (_iDebugger >= 0 && _iDebugger <= debuggers.size())
    {
        return debuggers[_iDebugger];
    }

    return NULL;
}

int DebuggerMagager::getDebuggerCount()
{
    return (int)debuggers.size();
}

Debuggers& DebuggerMagager::getAllDebugger()
{
    return debuggers;
}

void DebuggerMagager::sendStop(int index)
{
    currentBreakPoint = index;
    for (const auto& it : debuggers)
    {
        it->onStop(index);
    }
}

void DebuggerMagager::sendResume()
{
    currentBreakPoint = -1;
    for (const auto& it : debuggers)
    {
        it->onResume();
    }
}

void DebuggerMagager::sendAbort()
{
    currentBreakPoint = -1;
    for (const auto& it : debuggers)
    {
        it->onAbort();
    }
}

void DebuggerMagager::sendErrorInFile(const std::wstring& filename) const
{
    for (const auto& it : debuggers)
    {
        it->onErrorInFile(filename);
    }
}

void DebuggerMagager::sendErrorInScript(const std::wstring& funcname) const
{
    for (const auto& it : debuggers)
    {
        it->onErrorInScript(funcname);
    }
}

void DebuggerMagager::sendQuit()
{
    currentBreakPoint = -1;
    for (const auto& it : debuggers)
    {
        it->onQuit();
    }
}

void DebuggerMagager::sendUpdate() const
{
    for (const auto& it : debuggers)
    {
        it->updateBreakpoints();
    }
}

void DebuggerMagager::addBreakPoint(Breakpoint* bp)
{
    //check if breakpoint does not exist

    bool add = true;
    for (const auto b : breakpoints)
    {
        if (b->getFunctioName() != bp->getFunctioName())
        {
            continue;
        }

        if (b->getMacroLine() != bp->getMacroLine())
        {
            continue;
        }

        if (b->getCondition() != bp->getCondition())
        {
            continue;
        }

        //same breakpoint, cancel add
        add = false;
    }

    if (add)
    {
        breakpoints.push_back(bp);
        sendUpdate();
    }
}

void DebuggerMagager::removeBreakPoint(int _iBreakPoint)
{
    if (_iBreakPoint >= 0 && _iBreakPoint <= breakpoints.size())
    {
        Breakpoints::iterator it = breakpoints.begin() + _iBreakPoint;
        delete *it;
        breakpoints.erase(it);
        sendUpdate();
    }
}

void DebuggerMagager::removeAllBreakPoints()
{
    Breakpoints::iterator it = breakpoints.begin();
    for (; it != breakpoints.end(); ++it)
    {
        delete *it;
    }

    breakpoints.clear();
    sendUpdate();
}

void DebuggerMagager::disableBreakPoint(int _iBreakPoint)
{
    if (_iBreakPoint >= 0 && _iBreakPoint <= breakpoints.size())
    {
        breakpoints[_iBreakPoint]->setDisable();
        sendUpdate();
    }
}

void DebuggerMagager::disableAllBreakPoints()
{
    for (const auto& it : breakpoints)
    {
        it->setDisable();
    }

    sendUpdate();
}

void DebuggerMagager::enableBreakPoint(int _iBreakPoint)
{
    if (_iBreakPoint >= 0 && _iBreakPoint <= breakpoints.size())
    {
        breakpoints[_iBreakPoint]->setEnable();
    }
}

void DebuggerMagager::enableAllBreakPoints()
{
    for (const auto& it : breakpoints)
    {
        it->setEnable();
    }

    sendUpdate();
}

bool DebuggerMagager::isEnableBreakPoint(int _iBreakPoint)
{
    if (_iBreakPoint >= 0 && _iBreakPoint <= breakpoints.size())
    {
        return breakpoints[_iBreakPoint]->isEnable();
    }

    return false;
}

Breakpoint* DebuggerMagager::getBreakPoint(int _iBreakPoint)
{
    if (_iBreakPoint >= 0 && _iBreakPoint < breakpoints.size())
    {
        return breakpoints[_iBreakPoint];
    }

    return NULL;
}

int DebuggerMagager::getBreakPointCount()
{
    return (int)breakpoints.size();
}

Breakpoints& DebuggerMagager::getAllBreakPoint()
{
    return breakpoints;
}

void DebuggerMagager::resume() //resume execution
{
    if (ConfigVariable::getPauseLevel() != 0)
    {
        ConfigVariable::DecreasePauseLevel();
        //inform debuggers
        sendResume();
    }

}

void DebuggerMagager::abort() //abort execution
{
    if (ConfigVariable::getPauseLevel() != 0)
    {
        //inform debuggers
        sendAbort();

        throw ast::InternalAbort();
    }
}

void DebuggerMagager::internal_stop()
{
    interrupted = true;

    ConfigVariable::IncreasePauseLevel();

    // unlock console thread to display prompt again
    ThreadManagement::SendConsoleExecDoneSignal();

    //return to console so change mode to 2
    int iOldMode = ConfigVariable::getPromptMode();
    ConfigVariable::setPromptMode(2);

    int iPauseLevel = ConfigVariable::getPauseLevel();
    while (ConfigVariable::getPauseLevel() == iPauseLevel)
    {
        ThreadManagement::SendAwakeRunnerSignal();
        ThreadManagement::WaitForRunMeSignal();
        try
        {
            StaticRunner_launch();
        }
        catch (const ast::InternalError& ie)
        {
            //return from console so change mode to initial
            ConfigVariable::setPromptMode(iOldMode);
            //clean current seqexp
            clearExp();
            interrupted = false;

            throw ie;
        }
        catch (const ast::InternalAbort& ia)
        {
            //return from console so change mode to initial
            ConfigVariable::setPromptMode(iOldMode);
            //clean current seqexp
            clearExp();
            interrupted = false;

            throw ia;
        }
    }

    //return from console so change mode to initial
    ConfigVariable::setPromptMode(iOldMode);
    //clean current seqexp
    interrupted = false;
}

void DebuggerMagager::stop(const ast::Exp* pExp, int index)
{
    //send stop information to all debuggers
    setExp(pExp);
    sendStop(index);
    internal_stop();
    clearExp();
}
void DebuggerMagager::errorInFile(const std::wstring filename, const ast::Exp* pExp)
{
    setExp(pExp);
    sendErrorInFile(filename);
    internal_stop();
    clearExp();
}
void DebuggerMagager::errorInScript(const std::wstring funcname, const ast::Exp* pExp)
{
    setExp(pExp);
    sendErrorInScript(funcname);
    internal_stop();
    clearExp();
}

}
