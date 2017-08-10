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

extern "C"
{
#include "sciprint.h"
#include "prompt.h"
}

namespace debugger
{
void ConsoleDebugger::onStop(int index)
{
    if (index >= 0)
    {
        debugger::DebuggerMagager* manager = debugger::DebuggerMagager::getInstance();
        debugger::Breakpoint* bp = manager->getBreakPoint(index);
        if (bp)
        {
            sciprint(_("debugger stop on breakpoint(%d) in function %ls line %d\n"), index, bp->getFunctioName().data(), bp->getMacroLine());
        }
    }

    printExp();
}

void ConsoleDebugger::onResume()
{
    //sciprint("ConsoleDebugger::onResume.\n");
}

void ConsoleDebugger::onAbort()
{
    sciprint(_("Execution aborted.\n"));
}

void ConsoleDebugger::onErrorInFile(const std::wstring& filename)
{
    debugger::DebuggerMagager* manager = debugger::DebuggerMagager::getInstance();
    ast::Exp* exp = manager->getExp();
    sciprint(_("debugger stop on error in file %ls line %d\n"), filename.data(), exp->getLocation().first_line);
    printExp();
}

void ConsoleDebugger::onErrorInScript(const std::wstring& funcname)
{
    debugger::DebuggerMagager* manager = debugger::DebuggerMagager::getInstance();
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
    debugger::DebuggerMagager* manager = debugger::DebuggerMagager::getInstance();
    debugger::Breakpoints& brks = manager->getAllBreakPoint();

    if (brks.size() == 0)
    {
        sciprint("No breakpoint\n");
        return;
    }

    sciprint("% 3ls % 7ls %24ls % 5ls %ls\n\n", L"num", L"enable", L"function", L"line", L"condition");
    int i = 0;
    for (const auto& b : brks)
    {
        if (b->isMacro())
        {
            std::wstring condition = b->getCondition();
            sciprint("% 3d % 7s %24ls % 5d %ls\n", i, b->isEnable() ? "true" : "false", b->getFunctioName().c_str(), b->getMacroLine(),
                     condition.size() < 30 ? condition.c_str() :
                     (std::wstring(condition.begin(), condition.begin() + 27) + L"...").c_str());
        }

        ++i;
    }
}

void ConsoleDebugger::printExp()
{
    debugger::DebuggerMagager* manager = debugger::DebuggerMagager::getInstance();
    std::wostringstream ostr;
    ast::PrintVisitor pp(ostr, true, true, true);
    manager->getExp()->accept(pp);
    sciprint("%s%ls\n", SCIPROMPT_PAUSE, ostr.str().data());
}
}
