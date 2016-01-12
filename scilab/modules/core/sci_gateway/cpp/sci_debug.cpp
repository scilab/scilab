/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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

typedef enum EnumCommand
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

void print_help();
EnumCommand getCommand(const std::string& command);

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

    std::string command(((types::String*)in[0])->get(0));
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
            char* pstFunctionName = NULL;
            int iLine = -1;
            char* pstCondition = NULL;
            debugger::Breakpoint* bp = NULL;
            //function name
            if (iRhs > 1)
            {
                //do not check name
                //we can set breakpoint before function declaration
                //for embedded function for example
                pstFunctionName = ((types::String*)in[1])->get(0);
            }

            if (iRhs > 2)
            {
                char* pstLineNumber = ((types::String*)in[2])->get(0);
                char* pstEnd = NULL;
                iLine = strtol(pstLineNumber, &pstEnd, 10);
                if (pstEnd == NULL || *pstEnd != 0)
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d: Scalar positive integer expected.\n"), "breakpoint", 2);
                    return types::Function::Error;
                }
            }

            if (iRhs > 3)
            {
                pstCondition = ((types::String*)in[3])->get(0);
                bp = new debugger::Breakpoint(pstFunctionName, iLine, pstCondition);
            }
            else
            {
                bp = new debugger::Breakpoint(pstFunctionName, iLine);
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
                char* pEnd = NULL;
                int iBp = (int)strtol(((types::String*)in[1])->get(0), &pEnd, 10);
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
                char* pEnd = NULL;
                int iBp = (int)strtol(((types::String*)in[1])->get(0), &pEnd, 10);
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
                char* pEnd = NULL;
                int iBp = (int)strtol(((types::String*)in[1])->get(0), &pEnd, 10);
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
                StorePrioritaryCommand("help debug");
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
                std::ostringstream ostr;
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

            sciprint("% 3s % 7s %24s % 5s %s\n\n", "num", "enable", "function name", "line", "condition");

            if (iRhs > 1)
            {
                char* pEnd = NULL;
                int iBp = (int)strtol(((types::String*)in[1])->get(0), &pEnd, 10);
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
                    std::string condition = bp->getCondition();
                    sciprint("% 3d % 7s %24s % 5d %s\n", iBp, bp->isEnable() ? "true" : "false", bp->getFunctioName().c_str(), bp->getMacroLine(),
                             condition.size() < 30 ? condition.c_str() :
                             (std::string(condition.begin(), condition.begin() + 27) + "...").c_str());
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
                        std::string condition = bp->getCondition();
                        sciprint("% 3d % 7s %24s % 5d %s\n", i, bp->isEnable() ? "true" : "false", bp->getFunctioName().c_str(), bp->getMacroLine(),
                                 condition.size() < 30 ? condition.c_str() :
                                 (std::string(condition.begin(), condition.begin() + 27) + "...").c_str());
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

            std::ostringstream ostr;
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

EnumCommand getCommand(const std::string& command)
{
    wchar_t c = command[0];

    switch (c)
    {
        case 'a':
        {
            if (command.size() == 1 || command == "abort")
            {
                return AbortCommand;
            }
            break;
        }
        case 'b':
        {
            if (command.size() == 1 || command == "break" || command == "breakpoint")
            {
                return BreakCommand;
            }

            if (command == "bt")
            {
                return WhereCommand;
            }
            break;
        }
        case 'c':
        {
            if (command.size() == 1 || command == "continue")
            {
                return ContinueCommand;
            }
            break;
        }
        case 'd':
        {
            if (command == "disable")
            {
                return DisableCommand;
            }

            if (command == "del" || command == "delete")
            {
                return DeleteCommand;
            }
            break;
        }
        case 'e':
        {
            if (command == "enable")
            {
                return EnableCommand;
            }
            break;
        }
        case 'h':
        {
            if (command.size() == 1)
            {
                return HelpShortCommand;
            }

            if (command == "help")
            {
                return HelpCommand;
            }

            break;
        }
        case 'l':
        {
            if (command.size() == 1 || command == "list")
            {
                return ListCommand;
            }
            break;
        }
        case 'i':
        {
            if (command.size() == 1 || command == "in")
            {
                return StepInCommand;
            }
            break;
        }
        case 'n':
        {
            if (command.size() == 1 || command == "next")
            {
                return NextCommand;
            }
            break;
        }
        case 'o':
        {
            if (command.size() == 1 || command == "out")
            {
                return StepOutCommand;
            }
            break;
        }
        case 'q':
        {
            if (command.size() == 1 || command == "quit")
            {
                return QuitCommand;
            }
            break;
        }
        case 's':
        {
            if (command.size() == 1 || command == "show")
            {
                return ShowCommand;
            }
            if (command == "stepnext")
            {
                return NextCommand;
            }
            if (command == "stepin")
            {
                return StepInCommand;
            }
            if (command == "stepout")
            {
                return StepOutCommand;
            }
            break;
        }
        case 'w':
        {
            if (command.size() == 1 || command == "where")
            {
                return WhereCommand;
            }
            break;
        }
    }

    return UnknowCommand;
}
