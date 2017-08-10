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

#include "core_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "printvisitor.hxx"
#include "execvisitor.hxx"
#include "debuggervisitor.hxx"
#include "configvariable.hxx"
#include "debugmanager.hxx"
#include "threadId.hxx"

extern "C"
{
#include "Scierror.h"
#include "sciprint.h"
#include "Thread_Wrapper.h"
}

enum EnumCommand
{
    AbortCommand,
    BreakCommand,
    ContinueCommand,
    DisableCommand,
    DeleteCommand,
    EnableCommand,
    HelpCommand,
    HelpShortCommand,
    ListCommand,
    NextCommand,
    QuitCommand,
    ShowCommand,
    StepInCommand,
    StepOutCommand,
    UnknowCommand,
    WhereCommand
};

const char fname[] = "debug";
const std::string HELP_DEBUG_COMMAND("help debug");

void print_help();
EnumCommand getCommand(const std::wstring& command);

types::Function::ReturnValue sci_debug(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() == 0)
    {
        //debugger already active
        if (ConfigVariable::getEnableDebug())
        {
            return types::Function::OK;
        }

        //debugger can be lauch only on console scope level
        if (symbol::Context::getInstance()->getScopeLevel() != SCOPE_CONSOLE)
        {
            Scierror(999, _("%s: Debugger can be activated only at console scope level\n"), fname);
            return types::Function::Error;
        }

        ConfigVariable::setEnableDebug(true);
        ConfigVariable::setDefaultVisitor(new ast::DebuggerVisitor());
        return types::Function::OK;
    }

    if (ConfigVariable::getEnableDebug() == false)
    {
        Scierror(999, _("%s: Debugger is not running. Call \"%s\" without parameter first.\n"), fname, fname);
        return types::Function::Error;
    }

    int iRhs = (int)in.size();
    for (int i = 0; i < iRhs; i++)
    {
        if (in[i]->isString() == false || ((types::String*)in[i])->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, i + 1);
            return types::Function::Error;
        }
    }

    debugger::DebuggerMagager* manager = debugger::DebuggerMagager::getInstance();

    std::wstring command(((types::String*)in[0])->get(0));
    switch (getCommand(command))
    {
        case AbortCommand:
        {
            if (iRhs > 1)
            {
                Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), "abort", 0);
                return types::Function::Error;
            }

            //abort
            if (manager->isInterrupted())
            {
                manager->abort();
            }
            else
            {
                sciprint("debugger is not paused\n");
            }

            return types::Function::OK;
        }
        case BreakCommand:
        {
            if (iRhs < 2 || iRhs > 4)
            {
                Scierror(999, _("%s: Wrong number of input arguments: %d to %d expected.\n"), "breakpoint", 1, 3);
                return types::Function::Error;
            }

            //breakpoint
            wchar_t* pwstFunctionName = NULL;
            int iLine = -1;
            wchar_t* pwstCondition = NULL;
            debugger::Breakpoint* bp = NULL;
            //function name
            if (iRhs > 1)
            {
                //do not check name
                //we can set breakpoint before function declaration
                //for embedded function for example
                pwstFunctionName = ((types::String*)in[1])->get(0);
            }

            if (iRhs > 2)
            {
                wchar_t* pwstLineNumber = ((types::String*)in[2])->get(0);
                wchar_t* pwstEnd = NULL;
                iLine = wcstol(pwstLineNumber, &pwstEnd, 10);
                if (pwstEnd == NULL || *pwstEnd != 0)
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d: Scalar positive integer expected.\n"), "breakpoint", 2);
                    return types::Function::Error;
                }
            }

            if (iRhs > 3)
            {
                pwstCondition = ((types::String*)in[3])->get(0);
                bp = new debugger::Breakpoint(pwstFunctionName, iLine, pwstCondition);
            }
            else
            {
                bp = new debugger::Breakpoint(pwstFunctionName, iLine);
            }

            manager->addBreakPoint(bp);
            return types::Function::OK;
        }
        case ContinueCommand:
        {
            if (iRhs > 1)
            {
                Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), "continue", 0);
                return types::Function::Error;
            }

            //continue
            if (manager->isInterrupted())
            {
                manager->resume();
            }
            else
            {
                sciprint("debugger is not paused\n");
            }

            return types::Function::OK;
        }
        case DisableCommand:
        {
            if (iRhs > 2)
            {
                Scierror(999, _("%s: Wrong number of input arguments: %d to %d expected.\n"), "disable", 0, 1);
                return types::Function::Error;
            }

            //disable
            if (iRhs == 2)
            {
                wchar_t* pEnd = NULL;
                int iBp = (int)wcstol(((types::String*)in[1])->get(0), &pEnd, 10);
                if (pEnd == NULL || *pEnd != 0)
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d: Scalar positive integer expected.\n"), "disable", 1);
                    return types::Function::Error;
                }

                if (manager->getBreakPoint(iBp) == NULL)
                {
                    Scierror(999, _("%s: Unable to retrieve information about breakpoint %d.\n"), "disable", iBp);
                    return types::Function::Error;
                }

                manager->disableBreakPoint(iBp);
            }
            else
            {
                manager->disableAllBreakPoints();
            }

            return types::Function::OK;
        }
        case DeleteCommand :
        {
            if (iRhs > 2)
            {
                Scierror(999, _("%s: Wrong number of input arguments: %d to %d expected.\n"), "delete", 0, 1);
                return types::Function::Error;
            }

            //delete
            if (iRhs == 2)
            {
                wchar_t* pEnd = NULL;
                int iBp = (int)wcstol(((types::String*)in[1])->get(0), &pEnd, 10);
                if (pEnd == NULL || *pEnd != 0)
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d: Scalar positive integer expected.\n"), "disable", 1);
                    return types::Function::Error;
                }

                if (manager->getBreakPoint(iBp) == NULL)
                {
                    Scierror(999, _("%s: Unable to retrieve information about breakpoint %d.\n"), "delete", iBp);
                    return types::Function::Error;
                }

                manager->removeBreakPoint(iBp);
            }
            else
            {
                manager->removeAllBreakPoints();
            }

            return types::Function::OK;
        }
        case EnableCommand:
        {
            if (iRhs > 2)
            {
                Scierror(999, _("%s: Wrong number of input arguments: %d to %d expected.\n"), "enable", 0, 1);
                return types::Function::Error;
            }

            //enable
            if (iRhs == 2)
            {
                wchar_t* pEnd = NULL;
                int iBp = (int)wcstol(((types::String*)in[1])->get(0), &pEnd, 10);
                if (pEnd == NULL || *pEnd != 0)
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d: Scalar positive integer expected.\n"), "enable", 1);
                    return types::Function::Error;
                }

                if (manager->getBreakPoint(iBp) == NULL)
                {
                    Scierror(999, _("%s: Unable to retrieve information about breakpoint %d.\n"), "enable", iBp);
                    return types::Function::Error;
                }

                manager->enableBreakPoint(iBp);
            }
            else
            {
                manager->enableAllBreakPoints();
            }

            return types::Function::OK;
        }
        case HelpCommand:
        {
            if (iRhs > 1)
            {
                Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), "help", 0);
                return types::Function::Error;
            }

            if (ConfigVariable::getScilabMode() == SCILAB_NW || ConfigVariable::getScilabMode() == SCILAB_STD)
            {
                StorePrioritaryCommand(HELP_DEBUG_COMMAND.data());
                return types::Function::OK;
            }

            //continue tp HelpShortCommand
        }
        case HelpShortCommand:
        {
            if (iRhs > 1)
            {
                Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), "help", 0);
                return types::Function::Error;
            }

            //help
            print_help();
            return types::Function::OK;
        }
        case ListCommand:
        {
            if (iRhs > 1)
            {
                Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), "list", 0);
                return types::Function::Error;
            }

            if (manager->isInterrupted())
            {
                std::wostringstream ostr;
                ast::PrintVisitor pp(ostr, true, true, true);
                manager->getExp()->accept(pp);
                sciprint(_("%ls"), ostr.str().data());
            }
            else
            {
                sciprint("debugger is not paused\n");
            }
            return types::Function::OK;
        }
        case StepInCommand:
        {
            if (iRhs > 1)
            {
                Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), "stepin", 0);
                return types::Function::Error;
            }

            if (manager->isInterrupted())
            {
                manager->setStepIn();
                manager->resume();
            }
            return types::Function::OK;
        }
        case NextCommand:
        {
            if (iRhs > 1)
            {
                Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), "next", 0);
                return types::Function::Error;
            }

            if (manager->isInterrupted())
            {
                manager->setStepNext();
                manager->resume();
            }
            else
            {
                sciprint("debugger is not paused\n");
            }
            return types::Function::OK;
        }
        case StepOutCommand:
        {
            if (iRhs > 1)
            {
                Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), "stepout", 0);
                return types::Function::Error;
            }

            //need to level to go stepout
            if (manager->isInterrupted())
            {
                manager->setStepOut();
                manager->resume();
            }
            else
            {
                sciprint("debugger is not paused\n");
            }
            return types::Function::OK;
        }
        case QuitCommand:
        {
            if (iRhs > 1)
            {
                Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), "quit", 0);
                return types::Function::Error;
            }

            //quit : disable debugger + abort + quit
            ConfigVariable::setEnableDebug(false);
            ConfigVariable::setDefaultVisitor(new ast::ExecVisitor());
            manager->abort();
            manager->sendQuit();
            return types::Function::OK;
        }
        case ShowCommand:
        {
            if (iRhs > 2)
            {
                Scierror(999, _("%s: Wrong number of input arguments: %d to %d expected.\n"), "show", 0, 1);
                return types::Function::Error;
            }

            sciprint("% 3ls % 7ls %24ls % 5ls %ls\n\n", L"num", L"enable", L"function name", L"line", L"condition");

            if (iRhs > 1)
            {
                wchar_t* pEnd = NULL;
                int iBp = (int)wcstol(((types::String*)in[1])->get(0), &pEnd, 10);
                if (pEnd == NULL || *pEnd != 0)
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d: Scalar positive integer expected.\n"), "disable", 1);
                    return types::Function::Error;
                }

                debugger::Breakpoint* bp = manager->getBreakPoint(iBp);
                if (bp == NULL)
                {
                    Scierror(999, _("%s: Unable to retrieve information about breakpoint %d.\n"), "showbreakpoint", iBp);
                    return types::Function::Error;
                }

                if (bp->isMacro())
                {
                    std::wstring condition = bp->getCondition();
                    sciprint("% 3d % 7s %24ls % 5d %ls\n", iBp, bp->isEnable() ? "true" : "false", bp->getFunctioName().c_str(), bp->getMacroLine(),
                             condition.size() < 30 ? condition.c_str() :
                             (std::wstring(condition.begin(), condition.begin() + 27) + L"...").c_str());
                }
            }
            else
            {
                debugger::Breakpoints bps = manager->getAllBreakPoint();
                debugger::Breakpoints::iterator it = bps.begin();
                for (int i = 0; it != bps.end(); ++it, ++i)
                {
                    debugger::Breakpoint* bp = *it;
                    if (bp->isMacro())
                    {
                        std::wstring condition = bp->getCondition();
                        sciprint("% 3d % 7s %24ls % 5d %ls\n", i, bp->isEnable() ? "true" : "false", bp->getFunctioName().c_str(), bp->getMacroLine(),
                                 condition.size() < 30 ? condition.c_str() :
                                 (std::wstring(condition.begin(), condition.begin() + 27) + L"...").c_str());
                    }
                }
            }

            return types::Function::OK;
        }
        case WhereCommand:
        {
            if (iRhs > 1)
            {
                Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), "where", 0);
                return types::Function::Error;
            }

            if (manager->isInterrupted() == false)
            {
                sciprint("debugger is not paused\n");
                return types::Function::OK;
            }

            int curLine = (manager->getExp()->getLocation().first_line - ConfigVariable::getMacroFirstLines()) + 1;

            //where
            int i = 0;
            sciprint("%s\n", _("callstack:"));

            std::wostringstream ostr;
            ast::PrintVisitor pp(ostr, true, true, true);
            manager->getExp()->accept(pp);
#define BT_PRINT "#%-5d%ls (line %d)\n"
            sciprint("#%-5d%ls\n", 0, ostr.str().data());

            ConfigVariable::WhereVector where = ConfigVariable::getWhere();
            auto it1 = where.rbegin();
            auto it2 = ++where.rbegin();

            sciprint(_(BT_PRINT), 1, it2->m_name.data(), curLine);
            ++it1;
            ++it2;
            for (int j = 2; it2 != where.rend(); it1++, it2++, j++)
            {
                sciprint(_(BT_PRINT), j, it2->m_name.data(), it1->m_line);
            }

            return types::Function::OK;
        }

        case UnknowCommand:
        default :
            break;
    }

    sciprint("Unknown command \"%ls\".\n\n", command.c_str());
    sciprint("use 'h' for more information\n\n");
    return types::Function::OK;
}

void print_help()
{
    //a,b,c,d,h,i,n,o,q,s,w

    sciprint(_("debug commands : \n"));
    sciprint("  h                            : %s.\n", _("show this help"));
    sciprint("  help                         : %s.\n", _("open debug documentation page"));
    sciprint("\n");
    sciprint("  (q)uit                       : %s.\n", _("stop debugging"));
    sciprint("  (w)here or bt                : %s.\n", _("show callstack"));
    sciprint("\n");
    sciprint("  (e)xec cmd                   : %s.\n", _("execute cmd"));
    sciprint("  (r)un cmd                    : %s.\n", _("execute cmd"));
    sciprint("\n");
    sciprint("  (d)isp var                   : %s.\n", _("display variable"));
    sciprint("  (p)rint var                  : %s.\n", _("display variable"));
    sciprint("\n");
    sciprint("  (c)ontinue                   : %s.\n", _("continue execution"));
    sciprint("  (a)bort                      : %s.\n", _("abort execution"));
    sciprint("  step(n)ext or next           : %s.\n", _("continue to next statement"));
    sciprint("  step(i)n or in               : %s.\n", _("step into function"));
    sciprint("  step(o)ut or out             : %s.\n", _("step outside function"));
    sciprint("\n");
    sciprint("  (b)reakpoint or break\n     func [line [\"condition\"]] : %s.\n", _("add a breakpoint"));
    sciprint("  (del)ete                     : %s.\n", _("delete all breakpoints"));
    sciprint("  (del)ete n                   : %s.\n", _("delete a specific breakpoint"));
    sciprint("  enable                       : %s.\n", _("enable all breakpoints"));
    sciprint("  enable n                     : %s.\n", _("enable a specific breakpoint"));
    sciprint("  disable                      : %s.\n", _("disable all breakpoints"));
    sciprint("  disable n                    : %s.\n", _("disable a specific breakpoint"));
    sciprint("  (s)how                       : %s.\n", _("show all breakpoints"));
    sciprint("  (s)how n                     : %s.\n", _("show a specific breakpoint"));
    sciprint("\n");
    sciprint(_("  for more details, show help page.\n"));
}

EnumCommand getCommand(const std::wstring& command)
{
    wchar_t c = command[0];

    switch (c)
    {
        case L'a':
        {
            if (command.size() == 1 || command == L"abort")
            {
                return AbortCommand;
            }
            break;
        }
        case 'b':
        {
            if (command.size() == 1 || command == L"break" || command == L"breakpoint")
            {
                return BreakCommand;
            }

            if (command == L"bt")
            {
                return WhereCommand;
            }
            break;
        }
        case L'c':
        {
            if (command.size() == 1 || command == L"continue")
            {
                return ContinueCommand;
            }
            break;
        }
        case L'd':
        {
            if (command == L"disable")
            {
                return DisableCommand;
            }

            if (command == L"del" || command == L"delete")
            {
                return DeleteCommand;
            }
            break;
        }
        case L'e':
        {
            if (command == L"enable")
            {
                return EnableCommand;
            }
            break;
        }
        case L'h':
        {
            if (command.size() == 1)
            {
                return HelpShortCommand;
            }

            if (command == L"help")
            {
                return HelpCommand;
            }

            break;
        }
        case L'l':
        {
            if (command.size() == 1 || command == L"list")
            {
                return ListCommand;
            }
            break;
        }
        case L'i':
        {
            if (command.size() == 1 || command == L"in")
            {
                return StepInCommand;
            }
            break;
        }
        case L'n':
        {
            if (command.size() == 1 || command == L"next")
            {
                return NextCommand;
            }
            break;
        }
        case L'o':
        {
            if (command.size() == 1 || command == L"out")
            {
                return StepOutCommand;
            }
            break;
        }
        case L'q':
        {
            if (command.size() == 1 || command == L"quit")
            {
                return QuitCommand;
            }
            break;
        }
        case L's':
        {
            if (command.size() == 1 || command == L"show")
            {
                return ShowCommand;
            }
            if (command == L"stepnext")
            {
                return NextCommand;
            }
            if (command == L"stepin")
            {
                return StepInCommand;
            }
            if (command == L"stepout")
            {
                return StepOutCommand;
            }
            break;
        }
        case L'w':
        {
            if (command.size() == 1 || command == L"where")
            {
                return WhereCommand;
            }
            break;
        }
    }

    return UnknowCommand;
}
