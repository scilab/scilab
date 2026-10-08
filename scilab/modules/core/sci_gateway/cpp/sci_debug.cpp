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

#include "core_gw.hxx"
#include "function.hxx"
#include "debuggervisitor.hxx"
#include "configvariable.hxx"
#include "debugmanager.hxx"
#include "consoledebugger.hxx"

extern "C"
{
#include "Scierror.h"
#include "sciprint.h"
}

const char fname[] = "debug";
types::Function::ReturnValue sci_debug(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() > 0)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), fname, 0);
        return types::Function::Error;
    }

    debugger::DebuggerManager* manager = debugger::DebuggerManager::getInstance();

    //debugger already active
    if (manager->isDebugConsole())
    {
        return types::Function::OK;
    }

    // The console can join a debugged execution, running or stopped: there
    // the pause is the stop of the debugger. Otherwise its commands would
    // start a debug run inside a user pause or a function.
    if (manager->isDebugging() == false)
    {
        //debugger cannot be launch inside a pause
        if (ConfigVariable::getPauseLevel())
        {
            Scierror(999, _("%s: Debugger cannot be activated in pause\n"), fname);
            return types::Function::Error;
        }

        //debugger can be lauch only on console scope level
        if (symbol::Context::getInstance()->getScopeLevel() != SCOPE_CONSOLE)
        {
            Scierror(999, _("%s: Debugger can be activated only at console scope level\n"), fname);
            return types::Function::Error;
        }
    }

    if(manager->hasDebugger("console") == false)
    {
        //register console debugger as debugger
        manager->addDebugger("console", new debugger::ConsoleDebugger());
    }

    // debug() is the console debugger: it owns the prompt and the typed lines.
    manager->setDebugConsole(true);
    // and Scilab is in debug mode from now on, with or without a run
    debugger::DebuggerManager::holdDebugMode(debugger::DebuggerManager::ConsoleHolder, true);
    return types::Function::OK;
}
