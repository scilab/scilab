/*
 *  Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *  Copyright (C) 2015 - Scilab Enterprises - Cedric Delamarre
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
#include "configvariable.hxx"
#include "debugmanager.hxx"
#include "runner.hxx"

extern "C"
{
#include "charEncoding.h"
#include "localization.h"
#include "Scierror.h"
#include "sciprint.h"
#include "pause.h"
}

types::Function::ReturnValue sci_pause(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 0)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "pause", 0);
        return types::Function::Error;
    }

    // In debug mode the pause is a stop of the debugger.
    if (debugger::DebuggerManager::isDebugMode())
    {
        debugger::DebuggerManager::getInstance()->pause(StaticRunner::isCalledFromConsoleLine());
        return types::Function::OK;
    }

    // add pause in list of macro called
    // to manage line displayed when error occurred
    // or when the "where()" function is called.
    ConfigVariable::macroFirstLine_begin(2);

    try
    {
        // do pause
        pause_interpreter();
    }
    catch (const ast::InternalAbort& ia)
    {
        ConfigVariable::macroFirstLine_end();
        throw ia;
    }

    ConfigVariable::macroFirstLine_end();

    return types::Function::OK;
}
