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
#include "threadmanagement.hxx"
#include "execvisitor.hxx"
#include "printvisitor.hxx"
#include "UTF8.hxx"

#include "threadId.hxx"

extern "C"
{
#include "Thread_Wrapper.h"
#include "storeCommand.h"
#include "pause.h"
#include "FileExist.h"
}

namespace debugger
{
std::unique_ptr<DebuggerManager> DebuggerManager::me(nullptr);

//singleton
DebuggerManager* DebuggerManager::getInstance()
{
    if (me.get() == nullptr)
    {
        me.reset(new DebuggerManager());
    }

    return me.get();
}

void DebuggerManager::addDebugger(const std::string& _name, AbstractDebugger* _debug)
{
    debuggers[_name] = _debug;
}

void DebuggerManager::removeDebugger(const std::string& _name)
{
    if(getDebugger(_name))
    {
        debuggers.erase(_name);
    }
}

AbstractDebugger* DebuggerManager::getDebugger(const std::string& _name)
{
    const auto& d = debuggers.find(_name);
    if(d != debuggers.end())
    {
        return debuggers[_name];
    }

    return NULL;
}

int DebuggerManager::getDebuggerCount()
{
    return (int)debuggers.size();
}

Debuggers& DebuggerManager::getAllDebugger()
{
    return debuggers;
}

void DebuggerManager::sendStop(int index)
{
    currentBreakPoint = index;
    for (const auto& it : debuggers)
    {
        it.second->onStop(index);
    }
}

void DebuggerManager::sendExecution()
{
    for (const auto& it : debuggers)
    {
        it.second->onExecution();
    }
}

void DebuggerManager::sendExecutionReleased()
{
    for (const auto& it : debuggers)
    {
        it.second->onExecutionReleased();
    }
}

void DebuggerManager::sendPrint(const std::string& variable)
{
    for (const auto& it : debuggers)
    {
        it.second->onPrint(variable);
    }
}

void DebuggerManager::sendShow(int bp)
{
    for (const auto& it : debuggers)
    {
        it.second->onShow(bp);
    }
}

void DebuggerManager::sendResume()
{
    currentBreakPoint = -1;
    for (const auto& it : debuggers)
    {
        it.second->onResume();
    }
}

void DebuggerManager::sendAbort()
{
    currentBreakPoint = -1;
    for (const auto& it : debuggers)
    {
        it.second->onAbort();
    }
}

void DebuggerManager::sendErrorInFile(const std::wstring& filename) const
{
    for (const auto& it : debuggers)
    {
        it.second->onErrorInFile(filename);
    }
}

void DebuggerManager::sendErrorInScript(const std::wstring& funcname) const
{
    for (const auto& it : debuggers)
    {
        it.second->onErrorInScript(funcname);
    }
}

void DebuggerManager::sendQuit()
{
    currentBreakPoint = -1;
    for (const auto& it : debuggers)
    {
        it.second->onQuit();
    }
}

void DebuggerManager::sendUpdate() const
{
    for (const auto& it : debuggers)
    {
        it.second->updateBreakpoints();
    }
}

void DebuggerManager::addBreakPoint(Breakpoint* bp)
{
    //check if breakpoint does not exist
    for (const auto b : breakpoints)
    {
        bool isMacro = b->getFunctioName() == bp->getFunctioName() &&
                       b->getMacroLine() != bp->getMacroLine();
        bool isFile  = b->getFileName() == bp->getFileName() &&
                       b->getFileLine() != bp->getFileLine();
        bool equalCondition = b->getCondition() != bp->getCondition();
        if ((isMacro || isFile) && equalCondition)
        {
            //same breakpoint, cancel add
            return;
        }
    }

    breakpoints.push_back(bp);
    sendUpdate();
}

void DebuggerManager::setAllBreakPoints(Breakpoints& _bps)
{
    // remove existing breakpoints
    for (auto bp : breakpoints)
    {
        delete bp;
    }
    breakpoints.clear();

    // set new breakpoints
    breakpoints.swap(_bps);
    sendUpdate();
}

void DebuggerManager::removeBreakPoint(int _iBreakPoint)
{
    if (_iBreakPoint >= 0 && _iBreakPoint <= (int)breakpoints.size())
    {
        Breakpoints::iterator it = breakpoints.begin() + _iBreakPoint;
        delete *it;
        breakpoints.erase(it);
        sendUpdate();
    }
}

void DebuggerManager::removeAllBreakPoints()
{
    Breakpoints::iterator it = breakpoints.begin();
    for (; it != breakpoints.end(); ++it)
    {
        delete *it;
    }

    breakpoints.clear();
    sendUpdate();
}

void DebuggerManager::disableBreakPoint(int _iBreakPoint)
{
    if (_iBreakPoint >= 0 && _iBreakPoint <= (int)breakpoints.size())
    {
        breakpoints[_iBreakPoint]->setDisable();
        sendUpdate();
    }
}

void DebuggerManager::disableAllBreakPoints()
{
    for (const auto& it : breakpoints)
    {
        it->setDisable();
    }

    sendUpdate();
}

void DebuggerManager::enableBreakPoint(int _iBreakPoint)
{
    if (_iBreakPoint >= 0 && _iBreakPoint <= (int)breakpoints.size())
    {
        breakpoints[_iBreakPoint]->setEnable();
        sendUpdate();
    }
}

void DebuggerManager::enableAllBreakPoints()
{
    for (const auto& it : breakpoints)
    {
        it->setEnable();
    }

    sendUpdate();
}

bool DebuggerManager::isEnableBreakPoint(int _iBreakPoint)
{
    if (_iBreakPoint >= 0 && _iBreakPoint <= (int)breakpoints.size())
    {
        return breakpoints[_iBreakPoint]->isEnable();
    }

    return false;
}

Breakpoint* DebuggerManager::getBreakPoint(int _iBreakPoint)
{
    if (_iBreakPoint >= 0 && _iBreakPoint < (int)breakpoints.size())
    {
        return breakpoints[_iBreakPoint];
    }

    return NULL;
}

int DebuggerManager::getBreakPointCount()
{
    return (int)breakpoints.size();
}

Breakpoints& DebuggerManager::getAllBreakPoint()
{
    return breakpoints;
}

void DebuggerManager::generateCallStack()
{
    clearCallStack();

    std::wostringstream ostr;
    ast::PrintVisitor pp(ostr, true, true, true);
    getExp()->accept(pp);
    char* tmp = wide_string_to_UTF8(ostr.str().data());
    callstack.exp = tmp;
    FREE(tmp);

    //where
    const std::vector<ConfigVariable::WhereEntry>& where = ConfigVariable::getWhere();
    auto it_name = where.rbegin();

    // first row
    Stack cs;
    StackRow row;
    tmp = wide_string_to_UTF8(it_name->call->getName().data());
    row.functionName = tmp;
    FREE(tmp);

    row.functionLine = -1;
    if(it_name->call->getFirstLine())
    {
        row.functionLine = getExp()->getLocation().first_line - it_name->call->getFirstLine();
    }

    if(callstackAddFile(&row, *it_name->m_file_name))
    {
        row.fileLine = getExp()->getLocation().first_line;
    }

    row.scope = symbol::Context::getInstance()->getScopeLevel();

    cs.push_back(row);
    ++it_name;

    // next rows
    for (auto it_line = where.rbegin(); it_name != where.rend(); it_name++, it_line++)
    {
        StackRow row;
        tmp = wide_string_to_UTF8(it_name->call->getName().data());
        row.functionName = tmp;
        FREE(tmp);
        row.functionLine = it_line->m_line - 1;
        if(callstackAddFile(&row, *it_name->m_file_name))
        {
            row.fileLine = it_line->m_line;
            row.functionLine = -1;
            if(it_name->call->getFirstLine())
            {
                row.fileLine = it_name->call->getFirstLine() + it_line->m_line - 1;
                row.functionLine = it_line->m_line - 1;
            }
        }

        row.scope = it_line->m_scope_lvl;
        cs.push_back(row);
    }

    callstack.stack = cs;
}

bool DebuggerManager::callstackAddFile(StackRow* _row, const std::wstring& _fileName)
{
    _row->hasFile = false;
    if(_fileName.length())
    {
        std::string pstrFileName = scilab::UTF8::toUTF8(_fileName);
        _row->hasFile = true;
        // replace .bin by .sci
        size_t pos = pstrFileName.rfind(".bin");
        if(pos != std::string::npos)
        {
            pstrFileName.replace(pos, 4, ".sci");
            // do not add the file in the callstack if the associeted .sci is not available
            if (FileExist(pstrFileName.data()) == false)
            {
                _row->hasFile = false;
            }
        }

        if(_row->hasFile)
        {
            _row->fileName = pstrFileName;
        }
    }

    return _row->hasFile;
}

void DebuggerManager::print(const std::string& variable)
{
    //inform debuggers
    sendPrint(variable);
}

void DebuggerManager::show(int bp)
{
    //inform debuggers
    sendShow(bp);
}

char* DebuggerManager::execute(const std::string& command)
{
    char* error = checkCommand(command.data());
    if(error)
    {
        return error;
    }

    // reset abort flag befor a new exection
    resetAborted();

    // inform debuggers
    sendExecution();
    // execute command and wait
    StoreDebuggerCommand(command.data());
    // send execution finished and update debugger informations
    internal_execution_released();

    return nullptr;
}

void DebuggerManager::resume() //resume execution
{
    if (ConfigVariable::getPauseLevel() != 0)
    {
        //inform debuggers
        sendResume();

        ConfigVariable::DecreasePauseLevel();
        // reset callstack
        clearCallStack();

        // send "SendRunMeSignal" to unlock execution then wait
        ThreadManagement::WaitForDebuggerExecDoneSignal(true);

        // send execution finished and update debugger informations
        internal_execution_released();
    }
}

void DebuggerManager::abort() //abort execution
{
    //inform debuggers
    sendAbort();

    // this state is check by the debuggerVisitor to do abort in the main thread
    setAborted();

    // abort in a pause
    if(isInterrupted())
    {
        if (ConfigVariable::getPauseLevel() != 0)
        {
            ConfigVariable::DecreasePauseLevel();
        }

        // reset lasterror
        ConfigVariable::clearLastError();
        // reset callstack
        clearCallStack();

        ThreadManagement::WaitForDebuggerExecDoneSignal(true);

        internal_execution_released();
    }
}

void DebuggerManager::internal_execution_released()
{
    // send execution finished
    sendExecutionReleased();
}

void DebuggerManager::internal_stop()
{
    interrupted = true;
    generateCallStack();
    pause();
    //clean current seqexp
    interrupted = false;
}

void DebuggerManager::stop(const ast::Exp* pExp, int index)
{
    //send stop information to all debuggers
    setExp(pExp);
    sendStop(index);
    // because stop is used only in the debuggervisitor the pause
    // will be executed in the main thread (where is executed the command)
    internal_stop();
    clearExp();
}

void DebuggerManager::errorInFile(const std::wstring filename, const ast::Exp* pExp)
{
    setExp(pExp);
    sendErrorInFile(filename);
    internal_stop();
    clearExp();
}
void DebuggerManager::errorInScript(const std::wstring funcname, const ast::Exp* pExp)
{
    setExp(pExp);
    sendErrorInScript(funcname);
    internal_stop();
    clearExp();
}

}
