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

#ifndef __DEBUGGER_MANAGER_HXX__
#define __DEBUGGER_MANAGER_HXX__

#include <memory>

#include "abstractdebugger.hxx"
#include "breakpoint.hxx"
#include "dynlib_ast.h"

namespace debugger
{
class EXTERN_AST DebuggerMagager
{
public :
    enum DebugAction
    {
        Continue,
        StepNext,
        StepIn,
        StepOut
    };

private:
    static std::unique_ptr<DebuggerMagager> me;
    DebuggerMagager() :
        breakpoints(),
        debuggers(), pExp(nullptr), interrupted(false), currentBreakPoint(-1), action(Continue), level(0) {}

    Breakpoints breakpoints;
    Debuggers debuggers;

    ast::Exp* pExp;
    bool interrupted;
    int currentBreakPoint;
    DebugAction action;
    int level;

    void internal_stop();
public:
    ~DebuggerMagager()
    {
        for (auto d : debuggers)
        {
            delete d;
        }
        for (auto b : breakpoints)
        {
            delete b;
        }
    }

    static DebuggerMagager* getInstance();

    //debuggers functions
    void addDebugger(AbstractDebugger* _debug);
    void removeDebugger(int _iDebugger);
    AbstractDebugger* getDebugger(int _iDebugger);
    int getDebuggerCount();
    Debuggers& getAllDebugger();

    void setExp(const ast::Exp* _pExp)
    {
        pExp = const_cast<ast::Exp*>(_pExp);
    }
    void clearExp()
    {
        pExp = NULL;
    }
    ast::Exp* getExp()
    {
        return pExp;
    }

    bool isInterrupted()
    {
        return interrupted;
    }

    Breakpoint* getCurrentBreakPoint()
    {
        if (currentBreakPoint != -1)
        {
            return getBreakPoint(currentBreakPoint);
        }

        return nullptr;
    }
    //send information to debuggers
    void sendStop(int index);
    void sendResume();
    void sendAbort();
    void sendErrorInFile(const std::wstring& filename) const;
    void sendErrorInScript(const std::wstring& funcname) const;
    void sendQuit();
    void sendUpdate() const;

    void stop(const ast::Exp* pExp, int index);
    void errorInFile(const std::wstring filename, const ast::Exp* pExp);
    void errorInScript(const std::wstring funcname, const ast::Exp* pExp);
    //breakpoints functions
    void addBreakPoint(Breakpoint* bp);
    void removeBreakPoint(int _iBreakPoint);
    void removeAllBreakPoints();
    void disableAllBreakPoints();
    void disableBreakPoint(int _iBreakPoint);
    void enableAllBreakPoints();
    void enableBreakPoint(int _iBreakPoint);
    bool isEnableBreakPoint(int _iBreakPoint);
    Breakpoint* getBreakPoint(int _iBreakPoint);
    int getBreakPointCount();
    Breakpoints& getAllBreakPoint();


    //actions called by debuggers
    inline void setStepIn() //enter macro
    {
        action = StepIn;
        level = symbol::Context::getInstance()->getScopeLevel();
    }
    inline bool isStepIn()
    {
        int l = symbol::Context::getInstance()->getScopeLevel();
        //if stepIn failed ( not a macro ), stepIn become a stepNext
        return action == StepIn && l >= level;
    }
    inline void resetStepIn()
    {
        if (isStepIn())
        {
            action = Continue;
        }
    }

    inline void setStepOut() //exit macro
    {
        action = StepOut;
        level = symbol::Context::getInstance()->getScopeLevel();
    }
    inline bool isStepOut()
    {
        int l = symbol::Context::getInstance()->getScopeLevel();
        return action == StepOut && l < level;
    }
    inline void resetStepOut()
    {
        if (isStepOut())
        {
            action = Continue;
        }
    }

    inline void setStepNext() //next statement
    {
        action = StepNext;
        level = symbol::Context::getInstance()->getScopeLevel();
    }
    inline bool isStepNext()
    {
        int l = symbol::Context::getInstance()->getScopeLevel();
        //if stepNext failed ( end of macro ), stepNext become a stepOut
        return action == StepNext && l <= level;
    }
    inline void resetStepNext()
    {
        if (isStepNext())
        {
            action = Continue;
        }
    }

    inline void resetStep()
    {
        if (isInterrupted() == false)
        {
            action = Continue;
        }
    }
    void resume(); //resume execution
    void abort(); //abort execution
};

}
#endif /* !__DEBUGGER_MANAGER_HXX__ */
