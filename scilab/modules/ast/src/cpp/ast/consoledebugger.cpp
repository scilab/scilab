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

#include "consoledebugger.hxx"
#include "debugmanager.hxx"
#include "printvisitor.hxx"
#include "configvariable.hxx"

extern "C"
{
#include "sciprint.h"
#include "prompt.h"
#include "storeCommand.h"
}

namespace debugger
{
void ConsoleDebugger::onStop(int index)
{
    if (index >= 0)
    {
        debugger::DebuggerManager* manager = debugger::DebuggerManager::getInstance();
        debugger::Breakpoint* bp = manager->getBreakPoint(index);
        if (bp)
        {
            if(bp->hasMacro())
            {
                sciprint(_("debugger stop on breakpoint(%d) in function %s at line %d\n"), index, bp->getFunctioName().data(), bp->getMacroLine());
            }
            else
            {
                sciprint(_("debugger stop on breakpoint(%d) in file %s at line %d\n"), index, bp->getFileName().data(), bp->getFileLine());
            }
        }
    }
    printExp();
}

void ConsoleDebugger::onExecution()
{
    // sciprint("ConsoleDebugger::onExecution.\n");
}

void ConsoleDebugger::onExecutionReleased()
{
    // sciprint("ConsoleDebugger::onExecutionReleased.\n");
    // debugger::DebuggerManager* manager = debugger::DebuggerManager::getInstance();
    // if(manager->getCurrentBreakPoint())
    // {
    //     sciprint("execution stopped on breakpoint.\n");
    // }
    // else
    // {
    //     sciprint("execution finished.\n");
    // }
}

void ConsoleDebugger::onPrint(const std::string& variable)
{
    // sciprint("ConsoleDebugger::onPrint.\n");
    StoreDebuggerCommand(std::string("disp("+variable+")").data());
}

void ConsoleDebugger::onShow(int bp)
{
    // sciprint("ConsoleDebugger::onShow.\n");
    debugger::DebuggerManager* manager = debugger::DebuggerManager::getInstance();
    if(bp == -1)
    {
        debugger::Breakpoints& bps = manager->getAllBreakPoint();
        printBreakPoints(bps);
    }
    else
    {
        debugger::Breakpoints bps;
        bps.push_back(manager->getBreakPoint(bp));
        printBreakPoints(bps);
    }
}

void ConsoleDebugger::onResume()
{
    // sciprint("ConsoleDebugger::onResume.\n");
}

void ConsoleDebugger::onAbort()
{
    sciprint(_("Execution aborted.\n"));
}

void ConsoleDebugger::onErrorInFile(const std::wstring& filename)
{
    debugger::DebuggerManager* manager = debugger::DebuggerManager::getInstance();
    ast::Exp* exp = manager->getExp();
    sciprint(_("debugger stop on error in file %ls line %d\n"), filename.data(), exp->getLocation().first_line);
    printExp();
}

void ConsoleDebugger::onErrorInScript(const std::wstring& funcname)
{
    debugger::DebuggerManager* manager = debugger::DebuggerManager::getInstance();
    ast::Exp* exp = manager->getExp();
    sciprint(_("debugger stop on error in function %ls line %d\n"), funcname.data(), exp->getLocation().first_line);
    printExp();
}

void ConsoleDebugger::onQuit()
{
    sciprint(_("Leave debugger.\n"));
}

void ConsoleDebugger::updateBreakpoints()
{
    debugger::DebuggerManager* manager = debugger::DebuggerManager::getInstance();
    debugger::Breakpoints& bps = manager->getAllBreakPoint();
    printBreakPoints(bps);
}

void ConsoleDebugger::printExp()
{
    debugger::DebuggerManager* manager = debugger::DebuggerManager::getInstance();
    std::wostringstream ostr;
    ast::PrintVisitor pp(ostr, true, true, true);
    manager->getExp()->accept(pp);
    sciprint("%s%ls\n", SCIPROMPT_PAUSE, ostr.str().data());
}

void ConsoleDebugger::printBreakPoints(debugger::Breakpoints& bps)
{
    if (bps.size() == 0)
    {
        sciprint("No breakpoint\n");
        return;
    }

    sciprint("% 3s % 7s % 24s % 24s  %s\n\n", "num", "enable", "function", "file", "condition");
    int i = 0;
    for (const auto& b : bps)
    {
        std::string condition = b->getCondition();
        std::string macro = b->hasMacro() ? b->getFunctioName() + ":" + std::to_string(b->getMacroLine()) : "";
        std::string file  = b->hasFile()  ? b->getFileName() +    ":" + std::to_string(b->getFileLine())  : "";

        sciprint("% 3d % 7s % 24s % 24s  %s\n",
                i, b->isEnable() ? "true" : "false", macro.data(), file.data(),
                condition.size() < 30 ? condition.c_str() :
                    (std::string(condition.begin(), condition.begin() + 27) + "...").c_str());
        ++i;
    }
}
}
