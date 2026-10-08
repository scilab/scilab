/*
 *  Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
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

#include <atomic>
#include <functional>
#include <memory>
#include <mutex>

#include "abstractdebugger.hxx"
#include "configvariable.hxx"
#include "breakpoint.hxx"
#include "dynlib_ast.h"

namespace debugger
{
class ExecutionModeGuard;

class EXTERN_AST DebuggerManager
{
    friend class ExecutionModeGuard;

public:
    enum DebugAction
    {
        Continue,
        Aborted,
        StepNext,
        StepIn,
        StepOut
    };

    struct StackRow
    {
        std::string functionName;
        std::string fileName;
        int functionLine;
        int fileLine;
        int scope;
        int column;
        bool hasFile = false;
    };

    // Who can keep Scilab in debug mode while nothing runs.
    enum DebugHolder
    {
        ConsoleHolder = 1, // debug(), from "debug" to "q"
        ClientHolder = 2   // the debug sessions of the DAP endpoint
    };

    typedef std::vector<StackRow> Stack;
    struct CallStack
    {
        std::string exp;
        Stack stack;
    };

private:
    DebuggerManager() :
        breakpoints(),
        callstack(),
        debuggers(),
        pExp(nullptr),
        interrupted(false),
        request_pause(false),
        request_pause_here(false),
        currentBreakPoint(-1),
        action(Continue),
        level(0),
        debugConsole(false) {}

    std::mutex breakpoints_lock;
    Breakpoints breakpoints;
    // Built by the interpreter at each stop, read by the debuggers from
    // their own thread.
    mutable std::mutex callstack_lock;
    CallStack callstack;
    // Debuggers come and go from their own threads while the interpreter
    // notifies them: the table is only read and changed under its lock.
    mutable std::mutex debuggers_lock;
    Debuggers debuggers;

    ast::Exp* pExp;
    // set by the debuggers from their own thread, read by the interpreter one
    std::atomic<bool> interrupted;
    std::atomic<bool> request_pause;
    std::atomic<bool> request_pause_here;
    int currentBreakPoint;
    std::atomic<DebugAction> action;
    std::atomic<int> level;
    std::atomic<bool> debugConsole;
    static std::atomic<int> debugExecutions;
    static std::atomic<int> debugHolders;

    // Set the execution mode of the calling thread, return the one it replaces.
    static bool enterExecutionMode(bool _debug);
    static void leaveExecutionMode(bool _previous);

    // The debuggers to notify. They are called without the lock, which they
    // may need themselves; one removed meanwhile lives until its call returns.
    std::vector<std::shared_ptr<AbstractDebugger>> getDebuggers() const;

    // _announce tells the debuggers of the stop, once its call stack is built.
    void internal_stop(const std::function<void()>& _announce);
    int get_current_level();
    bool callstackAddFile(StackRow* _row, const std::wstring& _fileName);

public:
    ~DebuggerManager()
    {
        for (auto b : breakpoints)
        {
            delete b;
        }

        clearCallStack();
    }

    DebuggerManager(const DebuggerManager&) = delete;
    DebuggerManager& operator=(const DebuggerManager&) = delete;

    static DebuggerManager* getInstance();

    /*
     * Whether the execution going on in the calling thread is debugged. The
     * mode is set by ExecutionModeGuard and belongs to the thread, so code
     * called from another thread than the interpreter one is never debugged.
     */
    static bool isDebugExecution();
    // Whether a debugged execution is going on, running or stopped. Any thread.
    static bool isDebugging()
    {
        return debugExecutions > 0;
    }

    // Whether the debugged execution going on was started inside another one.
    static bool isNestedDebugging()
    {
        return debugExecutions > 1;
    }

    /*
     * Whether Scilab is in debug mode, with or without an execution going on.
     * Any thread. While nothing runs it is in debug mode as long as somebody
     * holds it: what is then started without saying how, a typed line or a
     * callback, is debugged. A debugged execution keeps Scilab in debug mode
     * until it ends, whoever holds it.
     */
    static bool isDebugMode()
    {
        return isDebugModeHeld() || isDebugging();
    }
    static bool isDebugModeHeld()
    {
        return debugHolders != 0;
    }
    // Each holder for itself: debug mode is left when the last one lets go.
    static void holdDebugMode(DebugHolder _holder, bool _hold);

    /*
     * True only when the console itself acts as the debugger interface, that
     * is under the debug() command. A debugger driven from outside, such as
     * the DAP endpoint, does not claim the console: typed lines keep being
     * executed normally.
     */
    bool isDebugConsole()
    {
        return debugConsole;
    }
    void setDebugConsole(bool _enable)
    {
        debugConsole = _enable;
    }

    //debuggers functions
    // The manager owns the debugger: it is destroyed once removed, and no
    // longer being notified.
    void addDebugger(const std::string& _name, AbstractDebugger* _debug);
    void removeDebugger(const std::string& _name);
    bool hasDebugger(const std::string& _name) const;
    int getDebuggerCount() const;

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

    void generateCallStack();
    inline void clearCallStack()
    {
        std::lock_guard<std::mutex> lock(callstack_lock);
        callstack.exp.clear();
        callstack.stack.clear();
    }

    CallStack getCallStack() const
    {
        std::lock_guard<std::mutex> lock(callstack_lock);
        return callstack;
    }

    //send information to debuggers
    void sendStop(int index);
    void sendExecution();
    void sendExecutionReleased();
    void sendPrint(const std::string& variable);
    void sendShow(int bp);
    void sendResume();
    void sendAbort();
    void sendErrorInFile(const std::wstring& filename) const;
    void sendErrorInScript(const std::wstring& funcname) const;
    void sendQuit();
    void sendDebugMode(bool _held);
    void sendUpdate() const;

    void stop(const ast::Exp* pExp, int index);
    void errorInFile(const std::wstring filename, const ast::Exp* pExp);
    void errorInScript(const std::wstring funcname, const ast::Exp* pExp);

    //breakpoints functions
    bool addBreakPoint(Breakpoint* bp);
    bool updateBreakPoint(Breakpoint* bp);
    bool removeBreakPoint(Breakpoint* bp);
    Breakpoints::iterator findBreakPoint(Breakpoint* bp);
    void removeBreakPoint(int _iBreakPoint);
    void setAllBreakPoints(Breakpoints& _bps);
    void removeAllBreakPoints();
    void disableAllBreakPoints();
    void disableBreakPoint(int _iBreakPoint);
    void enableAllBreakPoints();
    void enableBreakPoint(int _iBreakPoint);
    bool isEnableBreakPoint(int _iBreakPoint);
    Breakpoint* getBreakPoint(int _iBreakPoint);
    int getBreakPointCount();
    Breakpoints& getAllBreakPoint();
    void lockBreakpoints()
    {
        breakpoints_lock.lock();
    }
    void unlockBreakpoints()
    {
        breakpoints_lock.unlock();
    }

    //actions called by debuggers
    inline void setStepIn()
    {
        action = StepIn;
        level = get_current_level();
    }

    inline bool isStepIn()
    {
        return action == StepIn && get_current_level() > level;
    }

    inline void resetStepIn()
    {
        if (isStepIn())
        {
            action = Continue;
        }
    }

    inline void setStepOut()
    {
        action = StepOut;
        level = get_current_level();
    }

    inline bool isStepOut()
    {
        // StepIn/StepNext: when stepIn/StepNext at the end of a body, considere it as stepOut
        return (action == StepOut || action == StepNext || action == StepIn) && get_current_level() < level;
    }

    inline void resetStepOut()
    {
        if (isStepOut())
        {
            action = Continue;
        }
    }

    inline void setAborted()
    {
        action = Aborted;
    }

    inline bool isAborted()
    {
        return action == Aborted;
    }

    inline void resetAborted()
    {
        if (isAborted())
        {
            action = Continue;
        }
    }

    inline void setStepNext()
    {
        action = StepNext;
        level = get_current_level();
    }

    inline bool isStepNext()
    {
        // StepIn: when stepIn after stepOut, considere stepIn as StepNext
        return (action == StepNext || action == StepIn) && get_current_level() == level;
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

    char* execute(const std::string& command); //execute a non prioritary command
    char* executeNow(const std::string& command, int iWaitForIt = 1); //execute a prioritary command
    void print(const std::string& variable); //print a variable
    void show(int bp); //print the breakpoint bp or all breakpoints (bp = -1)
    void resume(int iWait = true); //resume execution
    void abort(); //abort execution
    // What the pause function does in debug mode, where it opens no pause
    // level: _fromConsoleLine when it is written in the line typed in the
    // console, not in what that line calls.
    void pause(bool _fromConsoleLine);
    void requestPause(); //pause execution before the next instruction
    bool isPauseRequested(); //get pause request status
    void requestPauseHere(); //pause execution on the instruction being executed
    bool isPauseHereRequested(); //get pause request status
    void resetPauseRequest(); //reset both pause request status
    bool getSourceFile(std::string* filename = nullptr);
};

/*
 * Runs a scope in an execution mode and puts the former one back when the
 * scope is left, by an abort included.
 */
class ExecutionModeGuard
{
public:
    explicit ExecutionModeGuard(bool _debug) :
        previous(DebuggerManager::enterExecutionMode(_debug))
    {
    }

    ~ExecutionModeGuard()
    {
        DebuggerManager::leaveExecutionMode(previous);
    }

    ExecutionModeGuard(const ExecutionModeGuard&) = delete;
    ExecutionModeGuard& operator=(const ExecutionModeGuard&) = delete;

private:
    bool previous;
};

}
#endif /* !__DEBUGGER_MANAGER_HXX__ */
