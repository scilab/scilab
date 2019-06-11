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

    //debugger already active
    if (ConfigVariable::getEnableDebug())
    {
        return types::Function::OK;
    }

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

    debugger::DebuggerManager* manager = debugger::DebuggerManager::getInstance();
    if(manager->getDebugger("console") == NULL)
    {
        //register console debugger as debugger
        manager->addDebugger("console", new debugger::ConsoleDebugger());
    }

    ConfigVariable::setEnableDebug(true);
    ConfigVariable::setDefaultVisitor(new ast::DebuggerVisitor());
    return types::Function::OK;
}
