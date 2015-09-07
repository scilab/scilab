/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *  Copyright (C) 2015 - Scilab Enterprises - Cedric Delamarre
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
#include "threadmanagement.hxx"
#include "configvariable.hxx"
#include "runner.hxx"

extern "C"
{
#include "charEncoding.h"
#include "localization.h"
#include "Scierror.h"
}

types::Function::ReturnValue sci_pause(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 0)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "pause", 0);
        return types::Function::Error;
    }

    ConfigVariable::IncreasePauseLevel();

    // unlock console thread to display prompt again
    ThreadManagement::SendConsoleExecDoneSignal();

    //return to console so change mode to 2
    int iOldMode = ConfigVariable::getPromptMode();
    ConfigVariable::setPromptMode(2);

    int iPauseLevel = ConfigVariable::getPauseLevel();
    while (ConfigVariable::getPauseLevel() == iPauseLevel)
    {
        ThreadManagement::SendAwakeRunnerSignal();
        ThreadManagement::WaitForRunMeSignal();
        StaticRunner_launch();
    }

    //return from console so change mode to initial
    ConfigVariable::setPromptMode(iOldMode);
    return types::Function::OK;
}
